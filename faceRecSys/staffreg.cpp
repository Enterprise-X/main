//员工登录后的签到界面
#include "staffreg.h"
#include "ui_staffreg.h"

staffReg::staffReg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::staffReg)
{
    ui->setupUi(this);
    this->setWindowTitle("员工签到");
    connect(&sQ,SIGNAL(sQbacktosR()),this,SLOT(sQtoThis()));    
    connect(&timer,&QTimer::timeout,this,&staffReg::run);
    connect(&camTimer,&QTimer::timeout,this,&staffReg::camtimeOut);
    connect(this,SIGNAL(RecOK()),this,SLOT(RecOKSLOT()));
    //摄像头默认关闭
    camFlag=0;
    //数据库设置
    db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名
    db.setDatabaseName(".\\src\\faceRS.db");
    //打开数据库
    if(!db.open())
    {
       qDebug()<<"open db error";
    }

    //显示当前时间
    curTime.start(1000); //每隔1秒更新时间
    connect(&curTime,&QTimer::timeout,this,&staffReg::runtime);

    //签到时间状态 默认不可签到
    recState="不可签到";
    recStateFlag=0;

    //签到识别状态 （是否签到成功）
    isRegFlag=0;

}

staffReg::~staffReg()
{
    delete ui;
}


//哪个员工登录 并把员工信息显示在上面  再查询数据库 看员工是否已经签到 把isRecFlag改变下
//这里要修改 先吃饭
QString staffReg::SnoID(QString sno)
{   id=sno;
    ui->labSno->setText(id);

    QString qID=QString("select * from Staff where Sno='%1'").arg(id);
    QSqlQuery queryID(qID);
    while (queryID.next()) {
       ui->labSname->setText(queryID.value(1).toString());
       ui->labSsex->setText(queryID.value(2).toString());
       ui->labDepart->setText(queryID.value(3).toString());

    }
   // qDebug()<<sno;
     ui->labRegTime->setText("暂未签到");
    disNotice();//显示通知


    //签到识别状态 （是否签到成功）
    isRegFlag=0;//其它员工进来再次置0 等canRec函数去数据库查询状态
    correctNum=0;//识别员工人脸正确次数置0

    QString qcorMax=QString("select corRecMax from Worktime where Wno=1");
    QSqlQuery qM(qcorMax);
    while (qM.next()) {
        corectMax=qM.value(0).toInt();
    }

     canRec();//查询是否考勤时间 和员工在这考勤时间里签到了没有
}
//退出帐号 返回初始界面（同时关掉所有摄像头等东西）待写
void staffReg::on_btnOutUser_clicked()
{
    emit srBackToInit();
    if(camFlag==1){
    on_btnCloseCam_clicked();
    }
    db.close();
}
//接受查询子界面返回的信号
void staffReg::sQtoThis()
{
    sQ.close();

}
//查询个人签到信息（当天） 切换到个人信息查询界面 并把ID传过去
void staffReg::on_btnQuery_clicked()
{   sQ.show();
    sQ.querrySnoID(id);
}

/**********↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓核心代码部分↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓************/


//核心函数 检测和识别人脸 并根据ID匹配人脸数据库
void staffReg::run()
{   Mat temp;
    capture>>cap;
    flip(cap,cap,1);
    cascade.detectMultiScale(cap, faces, 1.1, 4, CV_HAAR_DO_ROUGH_SEARCH, Size(60, 60), Size(400, 400)); //检测人脸
        //匹配人脸列表中的人脸
        for (int i = 0; i < faces.size(); i++) {
            Mat roi=cap(faces[i]); //框选人脸 目的是减少运算
            cvtColor(roi,gray,COLOR_BGR2GRAY);//转为灰度 同减少运算
            ::resize(gray,tempp,Size(100,100)); //将图像重新改变尺寸 默认使用双线性插值（INTER_LINEAR）这种方法
            IDlabel=model->predict(tempp); //将修改后的图像传送给模型做预测（即识别出是CSV文件里的哪个label）
            if(IDlabel==-1){
                qDebug()<<"识别失败";
            }
             qDebug()<<IDlabel;
            rectangle(cap, faces[i], Scalar(255, 0, 0), 2, 8, 0); //框出人脸

            putText(cap, to_string(IDlabel), faces[0].tl(), 3, 1.2, (0, 0, 225), 2, 0); //受到技术限制 opencv目前只能显示英文 此处显示的是预言出来的label（即工号）

            //识别成功（识别到的label等于登录员工的工号IDlabel==id） & 在上班时间签到or下班时间签到 没有识别成功签到过的(isRegFlag=0)
                //增加识别到的次数 当识别员工正确N次再确认签到
                   if(IDlabel==id.toInt()&isRegFlag==0){
                      // 进入签到登记函数RecOKSLOT（） 并让RecOKSLOT把isRegFlag置为1
                          correctNum++;
                          qDebug()<<"正确识别人脸次数"<<correctNum;                        
                          if(correctNum==corectMax){ //识别员工人脸正确N次 跳转  可以修改增加准确性（数字越大 识别越慢 正确性越高） 经过测试中位数50 识别率(10秒内)和正确性达到调和
                          emit RecOK();return;}

                   }
        }
    cvtColor(cap,temp,CV_BGR2RGB);
    QImage image(temp.data, temp.cols, temp.rows, temp.step1() , QImage::Format_RGB888);
    QPixmap mmp = QPixmap::fromImage(image);
    mmp = mmp.scaled(ui->labCam->size());
    ui->labCam->setPixmap(mmp);
}


//点击按钮→如果摄像头没打开&并且现在是签到时间→打开摄像头并加载人脸识别分类器
//并启动两个定时器 一个做刷新的帧率 一个做记时 在这段时间内没完成人脸识别就关闭摄像头(10秒)
void staffReg::on_btnStart_clicked()
{   //qDebug()<<"isReg:"<<isRegFlag;
    if(camFlag==0&recStateFlag!=0){
    camFlag=1;
    capture.open(0);


    //1.加载训练好的分类器  运用LBPH进行人脸检测识别
    cascade.load(".\\src\\haarcascade_frontalface_alt2.xml");
    model =face::LBPHFaceRecognizer::create();   model->read(".\\src\\EnterpriseLBPHModel.xml");
   // model =face::FisherFaceRecognizer::create(); model->read(".\\src\\EnterpriseFaceFisherModel.xml");
    //model =face::EigenFaceRecognizer::create();   model->read(".\\src\\EnterpriseEigenFaceModel.xml");
    timer.start(20); //帧率
    camTimer.start(1000*10);
    correctNum=0;//每次打开摄像头 正确识别人脸次数置0
    return;
    }
    if(camFlag==1){
     QMessageBox::information(this,"提示","摄像头已经打开");}
    if(recStateFlag==0){
        QMessageBox::information(this,"提示","非签到时间 不可签到");
    }
}
//时间到 关闭摄像头 如果没有完成签到则提示签到失败
void staffReg::camtimeOut()
{
    on_btnCloseCam_clicked();
    if(ui->labRegTime->text()=="暂未签到"){
        QMessageBox::information(this,"提示","签到失败");
        return;
    }
}
//关闭摄像头按钮 释放capture 标志位置为0
void staffReg::on_btnCloseCam_clicked()
{
    if(camFlag==1){
        capture.release();
        ui->labCam->clear();
        timer.stop();
        camTimer.stop();
        camFlag=0;
        return;
    }
    QMessageBox::information(this,"提示","摄像头没有打开");
}
//识别成功 提示签到成功 登记到数据库
void staffReg::RecOKSLOT()
{    QTime Regtime=QTime::currentTime(); //获取当前时间 作为签到时间
     QString strRegtime=Regtime.toString("hh:mm"); //转成字符串
    // qDebug()<<"regtime:"<<Regtime;
     QString name=ui->labSname->text();
     //签到标志位置为1
     isRegFlag=1;
     //判断上or下班签到 共四种状态 并登记把签到时间和状态到数据库签到表Attend
/**************************************************************************************************************************/
     //上班签到 在上班时间之前签到
     QString queryRegAM=QString("update Attend set amTime='%1',amState='签到' where Adate='%2' AND Asno='%3'")
             .arg(strRegtime).arg(td.toString("yyyy-MM-dd")).arg(id);
   //  QSqlQuery qRAM(queryRegAM);
     //上班迟到 在上班时间之后签到 (如果不打卡 按缺勤(未签到)算）
     QString queryLateAM=QString("update Attend set amTime='%1',amState='迟到' where Adate='%2' AND Asno='%3'")
             .arg(strRegtime).arg(td.toString("yyyy-MM-dd")).arg(id);
     // QSqlQuery qLAM(queryLateAM);


     //下班签到 在下班时间之后签到
     QString queryRegPM=QString("update Attend set pmTime='%1',pmState='签到' where Adate='%2' AND Asno='%3'")
             .arg(strRegtime).arg(td.toString("yyyy-MM-dd")).arg(id);
     //QSqlQuery qRPM(queryRegPM);


     //下班早退 在下班时间之前签到（早早溜走 如果不打卡 按缺(未签到)勤算）
     QString queryEarlyPM=QString("update Attend set pmTime='%1',pmState='早退' where Adate='%2' AND Asno='%3'")
             .arg(strRegtime).arg(td.toString("yyyy-MM-dd")).arg(id);
    // QSqlQuery qEPM(queryEarlyPM);
/**************************************************************************************************************************/
     //查询员工上班时间
     QString queryStaffAM=QString("select AMtime from Staff where Sno='%1'").arg(id);
     QSqlQuery qSAM(queryStaffAM);
     int isLate=0; //是否迟到 默认否 之后用签到时间和上班时间比较 然后把结果装在这

     //查询员工下班时间
     QString queryStaffPM=QString("select PMtime from Staff where Sno='%1'").arg(id);
     QSqlQuery qSPM(queryStaffPM);
     int isEarly=0; //是否早退 默认否 之后用签到时间和下班时间比较 然后把结果装在这

     //判断是上班还是下班
     //现在是上班 那么查询员工数据库上班时间 然后再和签到时间比较
     if(recStateFlag==1){
        QTime amTemp;   //从字符串转过来的QTime
        QString str_amTemp; //从数据库转换过来的时间字符串

         if(qSAM.next()){
             str_amTemp=qSAM.value(0).toString();
         }
        // qDebug()<<"amtempSTR OUT:"<<str_amTemp;
         amTemp=QTime::fromString(str_amTemp,"hh:ss");
         isLate=amTemp.operator<(Regtime); //员工上班时间是否早于签到时间 早则isLate=1 员工迟到
       //  qDebug()<<isLate;
        if(isLate){ //员工迟到 登记数据库
             QSqlQuery qLAM(queryLateAM);
             qLAM.exec();
             ui->labRegTime->setText(strRegtime);
        }
        else{
         QSqlQuery qRAM(queryRegAM);
         qRAM.exec();
         ui->labRegTime->setText(strRegtime);
        }
     }

     //现在是下班 那么查询员工数据库下班时间 然后再和签到时间比较
     if(recStateFlag==2){
        QTime pmTemp;   //从字符串转过来的QTime
        QString str_pmTemp; //从数据库转换过来的时间字符串

         if(qSPM.next()){
             str_pmTemp=qSPM.value(0).toString();

         }

         pmTemp=QTime::fromString(str_pmTemp,"hh:ss");
         isEarly=pmTemp.operator>(Regtime); //员工下班时间是否晚于签到时间 晚则isEarly=1 员工早退

        if(isEarly){ //员工早退 登记数据库
             QSqlQuery qEPM(queryEarlyPM);
             qEPM.exec();
             ui->labRegTime->setText(strRegtime);
        }
        else{
         QSqlQuery qRPM(queryRegPM);
         qRPM.exec();
         ui->labRegTime->setText(strRegtime);
        }
     }
     QMessageBox::information(this,"签到成功",name+"签到成功");


}


/***********↑↑↑↑↑↑↑↑↑↑↑↑↑↑核心代码部分↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑*************/


/**************显示消息*****************/
//显示时间
void staffReg::runtime()
{
    QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd dddd hh:mm:ss");   
    ui->labTime->setText(time+" "+recState);

}
//显示通知 因为要做一次查询 所以每次登录更新一遍通知 并判断现在的签到状态
void staffReg::disNotice()
{

    //显示当日通知
    //判断今天是否有通知
    td=QDateTime::currentDateTime();
    QString today=td.toString("MM-dd");       
   // todayNotice="今日暂无通知";
    QString isExistNotice=QString("select todayNotice from Notice where Ndata='%1'").arg(today);
    QSqlQuery queryIE(isExistNotice);
    while (queryIE.next()) {
        todayNotice=queryIE.value(0).toString();
       // qDebug()<<"tdn："<<todayNotice;
        ui->labInform->setText(todayNotice);
       // qDebug()<<queryIE.value(0).toString();
        return;
    }
    ui->labInform->setText("今日暂无通知");

}
//查询签到时间表找到相应的签到开始和结束时间 做判断能否签到 并提示是上班签到还是下班签到 或者不能签到 并且查询登录员工签到了没有
void staffReg::canRec()
{
    //查询签到时间表 判断签到时间
    QString queryTime=QString("select * from Worktime where Wno=1");
    QSqlQuery qT(queryTime);
    while(qT.next()){
        str_amS=qT.value(1).toString();
        str_amE=qT.value(2).toString();
        str_pmS=qT.value(3).toString();
        str_pmE=qT.value(4).toString();
    }
    //str_amS=str_amS+":00";

    amS=QTime::fromString(str_amS,"hh:mm");
    amE=QTime::fromString(str_amE,"hh:mm");
    pmS=QTime::fromString(str_pmS,"hh:mm");
    pmE=QTime::fromString(str_pmE,"hh:mm");
   // qDebug()<<amS<<amE<<pmS<<pmE;
    QTime now=QTime::currentTime();

    int isAM=(now.operator>=(amS) &now.operator<=(amE)); //现在是否是上班签到时间
    int isPM=(now.operator>=(pmS)&now.operator<=(pmE));  //现在是否是上班签到时间
   // qDebug()<<isAM<<isPM; //当两个状态全0时不可以签到

    //设置签到状态 recState和recStateFlag 并查询员工当前签到状态  isRegFlag（0，1)

    //查询员工今日签到情况
    QString today=td.toString("yyyy-MM-dd");
    //qDebug()<<"canRec today"<<today;
   QString queryIsRegFlag=QString("select * from Attend where Adate='%1' AND Asno='%2'").arg(today).arg(id);
   QSqlQuery qIRF(queryIsRegFlag);

   QString regTime;//如果员工签到了 把签到时间放进regTime 并把isRegFlag置为1
   //上班签到时间
   if(isAM&!isPM){
        recState="上班签到";
        recStateFlag=1;
        while(qIRF.next()){
            regTime=qIRF.value(4).toString();            
        }
        if(!regTime.isEmpty()){
             ui->labRegTime->setText(regTime);
             isRegFlag=1;
           //  qDebug()<<isRegFlag<<"if里面";
             return;
        }
         ui->labRegTime->setText("暂未签到");
         isRegFlag=0;
      //   qDebug()<<isRegFlag<<"if外面";
    }

   //下班签到时间
    else if(isPM&!isAM){
         recState="下班签到";
         recStateFlag=2;
         while(qIRF.next()){
             regTime=qIRF.value(6).toString();
             //qDebug()<<"下班："<<regTime;
         }
         if(!regTime.isEmpty()){
              ui->labRegTime->setText(regTime);
              isRegFlag=1;
             // qDebug()<<isRegFlag<<"if里面";
              return;
         }
          ui->labRegTime->setText("暂未签到");
          isRegFlag=0;
       //   qDebug()<<isRegFlag<<"if外面";
    }
    else{
        recState="不可签到";
        recStateFlag=0;
        isRegFlag=0;
    }
//    int state1=now.operator<(amS); //上班开始签到时间前 不可签到
//    int state2= (now.operator>(amE) & now.operator<(pmS)); //上班签到时间结束且没有到下班签到时间 不可签到
//    int state3=now.operator>(pmE);//下班签到时间结束 不可签到
//    qDebug()<<state1<<state2<<state3; //单全0时候才可以签到 进入下一步判断是上班还是下班
//    if(state1||state2||state3) //当三个状态判断有1时 不可签到 设置签到状态recState
//    {
//        recState="不可签到";
//    }

}
