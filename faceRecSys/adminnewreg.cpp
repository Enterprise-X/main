//新员工注册和人脸采集界面
#include "adminnewreg.h"
#include "ui_adminnewreg.h"
#include<QDebug>
adminNewReg::adminNewReg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adminNewReg)
{
    ui->setupUi(this);
    this->setWindowTitle("新员工注册和人脸采集");
    //数据库设置
    db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名
    db.setDatabaseName(".\\src\\faceRS.db");
    //打开数据库
    if(!db.open())
    {
       qDebug()<<"open db error";
    }
    //使用opencv官方训练好的人脸识别分类器做人脸检测
    cascada.load(".\\src\\haarcascade_frontalface_alt2.xml");
    picNum=1;//采集到的的人脸照片数
    connect(&mtimer,&QTimer::timeout,this,&adminNewReg::run); //打开摄像头按钮连接的定时器

    train = new faceDataTrain();
    connect(train,SIGNAL(finshed()),this,SLOT(trainOK()));
}

adminNewReg::~adminNewReg()
{

    delete ui;
}
//返回管理员菜单 清空输入信息
void adminNewReg::on_btnOut_clicked()
{
    emit aNRBackToAM();
    ui->lineSno->clear();
    ui->lineSname->clear();
    ui->combSex->setCurrentText("请选择性别");
    ui->combDepart->setCurrentText("请选择部门");
    QString temp="00:00";
    QTime a=QTime::fromString(temp,"hh:mm");
    QTime p=QTime::fromString(temp,"hh:mm");
    ui->amTime->setTime(a);
    ui->pmTime->setTime(p);
    ui->lineUserAns->clear();
    ui->lineUserQue->clear();
    ui->labCam->clear();
    ui->labCamPic->clear();
    ui->lineFile->clear();
    on_btnCloseCam_clicked();
    db.close();
}
//添加员工信息
void adminNewReg::on_btnAdd_clicked()
{
    QString inputSno=ui->lineSno->text();
    QString inputSname=ui->lineSname->text();
    QString inputSex=ui->combSex->currentText();
    QString inputDepart=ui->combDepart->currentText();
    QTime inputAMtime=ui->amTime->time();
    QString inputAMtoStr=inputAMtime.toString("hh:mm");
    QTime inputPMtime=ui->pmTime->time();    
    QString inputPMtoStr=inputPMtime.toString("hh:mm");
    QString inputQue=ui->lineUserQue->text();
    QString inputAns=ui->lineUserAns->text();
 //   qDebug()<<inputSno<<inputSname<<inputSex<<inputDepart<<inputAMtoStr<<inputPMtoStr<<inputQue<<inputAns;
   /* 从字符串转成时间QTime
    QTime amtimeT;
    amtimeT=QTime::fromString(inputAMtoStr,"hh:mm");
    qDebug()<<"at"<<amtimeT;*/

    if(inputSno.isEmpty()||inputSname.isEmpty()||inputSex=="请选择性别"||inputDepart=="请选择部门"||inputQue.isEmpty()||inputAns.isEmpty()){
        QMessageBox::about(this,"提示","请将信息填写完整");
        return ;
    }
    if(inputAMtoStr==inputPMtoStr)
    {   QMessageBox::about(this,"提示","请将上班时间和下班时间设置好");
        return;
    }
    //判断员工信息是否存在
    QString isExist=QString("select * from Staff where Sno='%1'").arg(inputSno);
    QSqlQuery queryIE(isExist);
    while (queryIE.next()) {
        QMessageBox::about(this,"提示","该员工信息已存在，添加失败");
        return ;
    }


    //如果不存在 添加员工信息到Staff表 并将帐号=员工工号 密码=000000 问题 答案 不是管理员添加到User表
    QString addtoStaff=QString("insert into Staff(Sno,Sname,Ssex,Dname,AMtime,PMtime,state) values('%1','%2','%3','%4','%5','%6','正常上班')")
            .arg(inputSno).arg(inputSname).arg(inputSex).arg(inputDepart).arg(inputAMtoStr).arg(inputPMtoStr);

    QString addtoUser=QString("insert into User(Uno,Password,Uque,Uans,IsAdmin) values('%1','000000','%2','%3',0)")
            .arg(inputSno).arg(inputQue).arg(inputAns);
    QSqlQuery queryAS;
    QSqlQuery queryAU;


    if(queryAS.exec(addtoStaff)){
        qDebug()<<"add to Staff error"<<queryAS.lastError();
         // QMessageBox::about(this,"提示","员工信息添加成功");
   }
    if(queryAU.exec(addtoUser)){
       // qDebug()<<"add to User error"<<queryAU.lastError();
          QMessageBox::about(this,"提示","员工信息添加成功,请采集员工人脸信息");
    }
    //添加到考勤表
    QString addATT=QString("insert into Attend(Adate,Asno,Asname,Adname,amState,pmState,staffState) values('%1','%2','%3','%4','未签到','未签到','正常上班')")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")).arg(inputSno).arg(inputSname).arg(inputDepart);
    QSqlQuery queryaddATT(addATT);
    if(!queryaddATT.exec()){
            qDebug()<<"New adderror"<<queryaddATT.lastError();
    }
//    QString querySno=QString("select * from Staff where Sno='%1'").arg(inputSno);
//    QSqlQuery qSno(querySno);
//    while (qSno.next()) {
//        qDebug()<<qSno.value(0)<<qSno.value(1)<<qSno.value(2)<<qSno.value(3)<<qSno.value(4)<<qSno.value(5)<<qSno.value(6)<<qSno.value(7);
//    }
//        QString querySno=QString("select * from User where Uno='%1'").arg(inputSno);
//        QSqlQuery qSno(querySno);
//        while (qSno.next()) {
//            qDebug()<<qSno.value(0)<<qSno.value(1)<<qSno.value(2)<<qSno.value(3)<<qSno.value(4);
//        }

}
/************************************采集人脸模块代码***********************************************/

//打开摄像头
void adminNewReg::on_btnOpenCam_clicked()
{
    dir_str = ".\\src\\faceData\\";//存放人脸数据的目录
    //判断员工工号是否存在
    if(!ui->lineSno->text().isEmpty())
    {   //输入工号存在则新建员工人脸目录
        QDir dir;
        //判断目录是否存在
        if(!dir.exists(dir_str+ui->lineSno->text()))
        {
            bool reS = dir.mkpath(dir_str+ui->lineSno->text());
            qDebug()<<"新建目录是否成功"<<reS;
        }
    }
    else {
        QMessageBox::about(this,"提示","请输入员工工号");
        return ;
    }
   cap.open(0);
    //利用视频捕捉人脸 测试用
     //cap.open("1.mp4");
     mtimer.start(20);
}
//关闭摄像头
void adminNewReg::on_btnCloseCam_clicked()
{
    mtimer.stop();
    cap.release();
    ui->labCam->clear();
    ui->labCamPic->clear();
    picNum = 1;
    ui->labCollNum->setText("采集0张");
}
//采集人脸（只有一张时采集）
void adminNewReg::on_btnFaceColl_clicked()
{
    //当只有一个人脸的时候才进行采集
    if (faces.size() == 1)
    {
       // Mat faceROI = frameGray(faces[0]);//在灰度图中将圈出的脸所在区域裁剪出
         Mat faceROI = tempColl(faces[0]);//在彩色图中将圈出的脸所在区域裁剪出
        //cout << faces[0].x << endl;//测试下face[0].x
         flip(tempColl,tempColl,1); //左右旋转图像 因为摄像头是反过来的
        ::resize(faceROI, myFace, Size(100, 100));//将兴趣域size为100*100 并使用默认的插值法
        putText(tempColl, to_string(picNum), faces[0].tl(), 3, 1.2, (0, 0, 225), 2, 0);//在 faces[0].tl()的左上角上面写序号
        //先保存图片
        //将图片保存的路径为 工号
        QString dir = dir_str+ui->lineSno->text()+"\\"+QString::number(picNum)+".jpg";
        //进行转码在转字符串
        string filename = dir.toLocal8Bit().toStdString();
        imwrite(filename, myFace);//存在dir路径下    
        //再进行框选人脸
        cvtColor(tempColl,tempColl,CV_BGR2RGB);
        rectangle(tempColl, faces[0], Scalar(0, 255, 0), 2, 8, 0);
        //转换成QT可以接受的QImage格式 并显示
        QImage image(tempColl.data, tempColl.cols, tempColl.rows, tempColl.step1() , QImage::Format_RGB888);
        QPixmap mmp = QPixmap::fromImage(image);
        ui->labCamPic->setScaledContents(true);
        ui->labCamPic->setPixmap(mmp);
        ui->labCamPic->show();
        ui->labCollNum->setText("采集"+QString::number(picNum)+"张");
        picNum++;
    }
}
//摄像头运行
void adminNewReg::run()
{
    Mat Temp;
    //摄像头读图像

    cap >> frame;
    cap >>tempColl;    //放进员工采集mat里面 因为下步操作框出人脸 采集时会把框也采集进去
    //cvtColor(frame, frameGray, COLOR_BGR2GRAY);//转灰度化 减少运算和内存
    cascada.detectMultiScale(frame, faces, 1.1, 4, CV_HAAR_DO_ROUGH_SEARCH, Size(60, 60), Size(800, 800));
    //人脸检测函数介绍
    /*
    frame:Mat类_输入的要做人脸检测的图像
    faces:vecto<Rect>类_存放检测出的人脸序列
    1.1:缩放因子 默认是1.1
    4:最小邻居 默认是3
    CV_HAAR_DO_ROUGH_SEARCH:只做粗略检测
    Size(60,60):最小尺寸
    Size(800,800):最大尺寸
    */
    flip(frame,frame,1);//旋转图片 左右互换（因为摄像头拍出的是反的）

    //识别到的脸用矩形圈出
    for (int i = 0; i < faces.size(); i++)
    {
        rectangle(frame, faces[i], Scalar(0, 255, 0), 2, 8, 0);
    }

    //转换成QT可以接受的QImage显示在label上面

    cvtColor(frame,Temp,CV_BGR2RGB);
    QImage image(Temp.data, Temp.cols, Temp.rows, Temp.step1() , QImage::Format_RGB888);
    QPixmap mmp = QPixmap::fromImage(image);
    ui->labCam->setScaledContents(true);
    ui->labCam->setPixmap(mmp);
    ui->labCam->show();

}
/************************************采集人脸模块代码***********************************************/



/************************************训练模块代码***********************************************/
//生成CSV文件（供Opencv识别）
void adminNewReg::on_btNCreateDate_clicked()
{
    QProcess *mprocess = new QProcess();
    try {
         mprocess->start("python createCSV.py");


    } catch (cv::Exception& e) {
        QMessageBox::about(this,"提示","文件生成失败");
        return ;
    }
   QMessageBox::about(this,"提示","训练文件准备好了，可以开始训练");
}
//选择要训练的CSV
void adminNewReg::on_btnSelect_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,"选择训练文本",".\\src\\faceData");
    ui->lineFile->setText(filepath);
}
//开始训练 生成三种model数据（特征脸 Fisher LBPH）
void adminNewReg::on_btnStartTarin_clicked()
{
    QString filepath = ui->lineFile->text();
    if(filepath.isNull()){
         QMessageBox::about(this,"提示","请选择训练数据");
        return;
    }
    try {
        //传训练文件的路径
        train->SetFilePath(filepath);
        train->start();
        QMessageBox::about(this,"提示","正在训练, 请稍后...");
    } catch (exception e) {
        QMessageBox::about(this,"提示","训练失败！");
    }
}
//训练完成
void adminNewReg::trainOK()
{
    QMessageBox::about(this,"提示","训练成功！");
    train->quit();
}

/************************************训练模块代码**********************************************/
