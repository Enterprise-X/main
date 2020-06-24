//消息通知界面
#include "noticeset.h"
#include "ui_noticeset.h"
#include<QDebug>
NoticeSet::NoticeSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NoticeSet)
{
    ui->setupUi(this);
    this->setWindowTitle("通知设置");
    //数据库设置
    db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名
    db.setDatabaseName(".\\src\\faceRS.db");
    //打开数据库
    if(!db.open())
    {
       qDebug()<<"open db error";
    }
    QDateTime today=QDateTime::currentDateTime();

    todayDate=today.toString("MM-dd");
    notice="今日无消息";
    queryTodayNotice();


}

NoticeSet::~NoticeSet()
{
    delete ui;
}
//返回管理员菜单
void NoticeSet::on_btnCancel_clicked()
{   ui->textEdit->setText("请输入消息");
    emit  noSBackToAM();
    qDebug()<<"noS";
}

void NoticeSet::on_btnOK_clicked()
{
       if(ui->textEdit->toPlainText()=="请输入消息")
       {
           QMessageBox::about(this,"提示","消息设置失败");
           return ;
       }
       //如果输入界面更改 添加信息到数据库
       notice=ui->textEdit->toPlainText();
       QString addNotice=QString("insert OR REPLACE INTO Notice(Ndata,todayNotice) values('%1','%2')").arg(todayDate).arg(notice);
       QSqlQuery sql_add(addNotice);
       if(!sql_add.exec()){
            QMessageBox::about(this,"提示","通知添加失败");
            qDebug()<<sql_add.lastError();
       }
       QMessageBox::about(this,"提示","通知添加成功");
       queryTodayNotice();

}

void NoticeSet::queryTodayNotice()
{
    QString queryNotice=QString("select todayNotice from Notice where Ndata='%1'").arg(todayDate);
    QSqlQuery qN(queryNotice);
    QString qnotice="今日无消息";
    while(qN.next()){
        qnotice=qN.value(0).toString();
    }
    ui->labInfrom->setText(qnotice);
}

