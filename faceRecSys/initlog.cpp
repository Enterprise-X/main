//初始登录界面
//在此界面登录帐号（可选管理员和普通用户签到）
#include "initlog.h"
#include "ui_initlog.h"

initLog::initLog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::initLog)
{

    ui->setupUi(this);
    this->setWindowTitle("登录");


    //数据库初始化操作
    //本系统采用轻便的sqlite数据库 只为方便修改和测试 如有必要可以换成远程数据库
    //添加数据库驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名
    db.setDatabaseName(".\\src\\faceRS.db");
    //打开数据库
    if(!db.open())
    {
        qDebug()<<"open db error";
    }
/*******************************创建表***************************************/
    //创建员工表：工号 姓名 性别 部门 上班时间 下班时间   状态（正常上班/休假/出差）
    QSqlQuery queryCreateStaff(db);
    if(!queryCreateStaff.exec("create table if not exists Staff("
                              "Sno   vchar primary key,"
                              "Sname text     not NULL,"
                              "Ssex  text     not NULL,"                             
                              "Dname text     not NULL,"
                              "AMtime time not NULL,"
                              "PMtime time   not NULL,"
                              "state text     not NULL)"))
    {
        qDebug()<<"create Staff error"<<queryCreateStaff.lastError();
    }
    //创建用户表：帐号（工号） 密码（默认000000） 问题 答案 是否为管理员
    QSqlQuery queryCreateUser(db);
    if(!queryCreateUser.exec("create table if not exists User("
                             "Uno      vchar primary key,"
                             "Password vchar      not NULL,"
                             "Uque     text      not NULL,"
                             "Uans     text      not NULL,"
                             "IsAdmin  int       not NULL)"))   //" "FOREIGN KEY(Uno) REFERENCES Staff(Sno))
    {
        qDebug()<<"create User error"<<queryCreateUser.lastError();
    }

    //创建考勤表： 日期（data） 工号 姓名 部门 上班打卡时间 上班签到情况（签到/迟到/缺勤） 下班打卡时间 下班签到情况（签到/早退/缺勤） 备注（是否休假/出差）
    QSqlQuery queryCreateAttend(db);
    if(!queryCreateAttend.exec("create table if not exists Attend("                            
                               "Adate    date not NULL,"
                               "Asno     vchar     not NULL,"
                               "Asname   text     not NULL,"
                               "Adname   text     not NULL,"                             
                               "amTime    time,"
                               "amState   text,"
                               "pmTime    time,"
                               "pmState   text,"
                               "staffState   text,"
                               "primary key(Adate,Asno))"))
    {
        qDebug()<<"create Attend error"<<queryCreateAttend.lastError();
    }

//    //添加考勤测试用
//    QString addATT="insert OR REPLACE INTO Attend(Adate,Asno,Asname,Adname) values('2020-05-09','0','测试','技术部')";
//    QSqlQuery sql_addT(db);
//     if(!sql_addT.exec(addATT))
//     {
//         qDebug()<<"insert AddTT error"<<sql_addT.lastError();
//     }


    //创建通知表：日期 通知 （当日消息）
    QSqlQuery queryCreateNotice(db);
    if(!queryCreateNotice.exec("create table if not exists Notice("
                               "Ndata  text primary key,"
                               "todayNotice text)"))
    {
        qDebug()<<"create  Depart error"<< queryCreateNotice.lastError();
    }


    //创建签到时间设置表：主键 上班签到开始时间 上班签到结束时间 下班签到开始时间 下班签到结束时间 人脸识别阈值
    QSqlQuery queryCreateworkTime(db);
    if(!queryCreateworkTime.exec("create table if not exists Worktime("
                               "Wno  int primary key,"
                               "amStart  time not NULL,"
                               "amEnd    time not NULL,"
                               "pmStart  time not NULL,"
                               "pmEnd    time not NULL,"
                               "corRecMax   int not NULL)"))
    {
        qDebug()<<"create worktime error"<< queryCreateworkTime.lastError();
    }
/******************************************************************************/


/******************************给表里填入初始数据******************************************/
    //添加初始管理员帐号和员工表 测试用
    QString addAdmin="insert OR REPLACE INTO User(Uno,Password,Uque,Uans,IsAdmin) values('0','0','问题答案是0','0',1),('1','1','问题答案是1','1',0),('2','2','问题答案是2','2',0),('3','3','问题答案是3','3',0)";
    QSqlQuery sql_add(db);
     if(!sql_add.exec(addAdmin))
     {
         qDebug()<<"insert User error"<<sql_add.lastError();
     }
     QString addStaff="insert OR REPLACE INTO Staff(Sno,Sname,Ssex,Dname,AMtime,PMtime,state) values('0','超级管理员','男','BOSS','12:59','13:00','不用上班'),('1','员工1号','男','技术部','08:00','17:00','正常上班'),('2','员工2号','男','行政部','08:00','17:00','休假'),('3','员工3号','女','销售部','08:00','17:00','出差')";
     QSqlQuery sql_add2(db);
      if(!sql_add2.exec(addStaff))
      {
          qDebug()<<"insert Staff error"<<sql_add2.lastError();
      }


    //查询员工表Staff 将工号 姓名 部门 上班状态和当天时间插入考勤Attend表 默认未签到
    QString selectStaff="select Sno,Sname,Dname,state from Staff";
    QSqlQuery querySelectStaff(selectStaff);
    QDateTime today=QDateTime::currentDateTime();
    QString td=today.toString("yyyy-MM-dd");
    QString Asno,Asname,Adname,Astate;

    while(querySelectStaff.next()){
       Asno=querySelectStaff.value(0).toString();
       Asname=querySelectStaff.value(1).toString();
       Adname=querySelectStaff.value(2).toString();
       Astate=querySelectStaff.value(3).toString();
        //qDebug()<< Asno<<Asname<<Adname;
        QString addATT=QString("insert into Attend(Adate,Asno,Asname,Adname,amState,pmState,staffState) values('%1','%2','%3','%4','未签到','未签到','%5')")
                .arg(td).arg(Asno).arg(Asname).arg(Adname).arg(Astate);
        QSqlQuery queryaddATT(addATT);
        if(!queryaddATT.exec()){
               // qDebug()<<"adderror"<<queryaddATT.lastError();
        }

    }


    //为签到时间设置表添加默认签到和结束时间 上班签到时间：7点到9点 下班签到时间16点到20点 人脸识别阈值默认30
    QString addwT="insert into Worktime(Wno,amStart,amEnd,pmStart,pmEnd,corRecMax) values(1,'07:00','09:00','16:00','20:00',30)";
    QSqlQuery sql_addWT(db);
     if(!sql_addWT.exec(addwT))
     {
         qDebug()<<"insert worktime error"<<sql_addWT.lastError();
     }

/******************************************************************************/
    //连接部分
    connect(&sR,SIGNAL(srBackToInit()),this,SLOT(staffregToThis())); //接受员工签到界面发射的返回信号
    connect(&aM,SIGNAL(amBackToInit()),this,SLOT(adminmenuToThis()));//接受管理员菜单界面发射的返回信号
    connect(&cp,SIGNAL(cpBackToInit()),this,SLOT(changePSToThis()));//接受修改密码界面发射的返回信号
    connect(&fp,SIGNAL(fpBackToInit()),this,SLOT(forgetPSToThis()));//接受忘记密码界面发射的返回信号

   // connect(&aNR,SIGNAL(sendNewToInit()),this,SLOT(test()));//接受管理员菜单→新员工添加→成功添加后发射的信号 生成新考勤表 转到main里编写 在这里信没反应
}

initLog::~initLog()
{


    delete ui;
}


//登录按钮 选择用户类型 输入账号密码→验证→跳转界面（管理员/员工签到）
void initLog::on_btnLog_clicked()
{
    QString user=ui->lineUser->text(); //帐号
    QString pass=ui->linePassword->text(); //密码
    QString typeUser=ui->combSelect->currentText(); //登录模式（管理员/普通用户）
    //查询密码 如果返回空则没有该帐号
    QString queryPS=QString("select Password  from User where Uno='%1'").arg(user);
    QSqlQuery qPS(queryPS);
    QString pwd; //查询得到的密码；
   //查询管理员权限 如果返回空则没有该帐号
    QString queryis=QString("select IsAdmin from User where Uno='%1'").arg(user);
    QSqlQuery qis(queryis);
    int isAdmin=0;//是否为管理员  默认否
    if(!user.isEmpty()&!pass.isEmpty()){
        while (qPS.next()) {
                    pwd = qPS.value(0).toString();
                }
        if(pwd.isNull()){         //没有帐号
             QMessageBox::information(this, tr("错误"), tr("无此帐号"));
             return;
        }
        if(pass!=pwd){          //密码错误
            QMessageBox::information(this, tr("提示"), tr("密码错误"));
            return;
        }
        if(typeUser=="管理员"){
            while (qis.next()) {
                         isAdmin= qis.value(0).toInt();
                      //   qDebug()<<isAdmin;
                    }
            if(isAdmin==0){
                QMessageBox::information(this, tr("警告"), tr("没有管理员权限"));
                return;
            }
              //qDebug()<<isAdmin<<"管理员登录 转到管理员菜单";
              aM.adminID(user);
              this->hide();
              aM.show(); //切换到管理员菜单
        }
        else{
            //qDebug()<<"用户登录 请转到用户签到界面";
            sR.SnoID(user);
            this->hide();
            sR.show(); //切换到员工人脸签到
        }
        return;
    }
    QMessageBox::information(this, tr("错误"), tr("帐号或者密码为空"));
}

void initLog::on_btnOut_clicked()
{
    this->close();
}
//从管理员菜单回到初始界面（本界面） 槽函数
void initLog::adminmenuToThis()
{
    aM.close();
    this->show();
    ui->lineUser->clear();
    ui->linePassword->clear();
}
//从员工登录签到界面回到初始界面（本界面）槽函数
void initLog::staffregToThis()
{
    sR.close();
    this->show();
    ui->lineUser->clear();
    ui->linePassword->clear();
}


//修改密码
void initLog::on_btnAlter_clicked()
{   //判断有无帐号
    QString user=ui->lineUser->text();

    //判断用户信息是否存在
    QString isExist=QString("select * from User where Uno='%1'").arg(user);
    QSqlQuery queryIE(isExist);
    while (!queryIE.next()) {
        QMessageBox::about(this,"提示","该员工帐号信息不存在，请修改工号");
        return ;
    }
    cp.changeID(user);
    cp.show();
    this->hide();

}
void initLog::changePSToThis()
{

    cp.close();
    this->show();
}

//忘记密码
void initLog::on_btnForget_clicked()
{
       //判断有无帐号
        QString user=ui->lineUser->text();

        //判断用户信息是否存在
        QString isExist=QString("select * from User where Uno='%1'").arg(user);
        QSqlQuery queryIE(isExist);
        while (!queryIE.next()) {
            QMessageBox::about(this,"提示","该员工帐号信息不存在，请修改工号");
            return ;
        }
        fp.forgetID(user);
        fp.show();
        this->hide();

}
void initLog::forgetPSToThis()
{
    fp.close();
    this->show();
}
//接受adminnewreg发射的创建新员工函数 激活添加到签到表
//void initLog::creatNewAttend()
//{   qDebug()<<"dadwa daw dwas";
//    QString selectStaff="select Sno,Sname,Dname,state from Staff";
//    QSqlQuery querySelectStaff(selectStaff);
//    QDateTime today=QDateTime::currentDateTime();
//    QString td=today.toString("yyyy-MM-dd");
//    QString Asno,Asname,Adname,Astate;

//    while(querySelectStaff.next()){
//       Asno=querySelectStaff.value(0).toString();
//       Asname=querySelectStaff.value(1).toString();
//       Adname=querySelectStaff.value(2).toString();
//       Astate=querySelectStaff.value(3).toString();
//        //qDebug()<< Asno<<Asname<<Adname;
//        QString addATT=QString("insert into Attend(Adate,Asno,Asname,Adname,amState,pmState,staffState) values('%1','%2','%3','%4','未签到','未签到','%5')")
//                .arg(td).arg(Asno).arg(Asname).arg(Adname).arg(Astate);
//        QSqlQuery queryaddATT(addATT);
//        if(!queryaddATT.exec()){
//               // qDebug()<<"adderror"<<queryaddATT.lastError();
//        }

//    }
//}

//void initLog::test()
//{
//     qDebug()<<"test test test";
//}

