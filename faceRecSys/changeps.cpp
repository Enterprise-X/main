//修改密码界面
#include "changeps.h"
#include "ui_changeps.h"
#include<QDebug>
#include<QMessageBox>
changePS::changePS(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::changePS)
{
    ui->setupUi(this);
        this->setWindowTitle("修改密码");
        //添加数据库驱动
        db = QSqlDatabase::addDatabase("QSQLITE");
        //设置数据库名
        db.setDatabaseName(".\\src\\faceRS.db");
        //打开数据库
        if(!db.open())
        {
            qDebug()<<"open db error";
        }
    }

changePS::~changePS()
{
    delete ui;
}

void changePS::changeID(QString id)
{
    ID=id;
    //qDebug()<<ID;
    ui->labUser->setText("用户:"+ID);
    ui->lineNew->clear();
    ui->lineOld->clear();


}

void changePS::on_btnChange_clicked()
{
    QString oldPS=ui->lineOld->text();//旧密码

    if(ui->lineNew->text().isEmpty())
    {   QMessageBox::about(this,"错误","请输入新密码");
        return;
    }
      QString newPS=ui->lineNew->text();//新密码
    //查询旧密码是否正确
    QString queryPS=QString("select Password from User where Uno='%1'").arg(ID); //查询密码 如果返回空则没有该帐号
    QSqlQuery qPS(queryPS);
    QString pwd; //查询得到的密码；
    while (qPS.next()) {
                pwd = qPS.value(0).toString();

            }

    if(pwd!=oldPS)
    {
        QMessageBox::about(this,"错误","旧密码错误 请重新输入");
        return;
    }
    //如果密码正确 则修改密码
     QString updateUserPS=QString("update User set Password='%1' where Uno='%2'").arg(newPS).arg(ID);
     QSqlQuery sqlUU(updateUserPS);
     if(!sqlUU.exec())
     {

      qDebug()<<sqlUU.lastError();
     }
    //  QMessageBox::about(this,"提示","成功修改帐号密码,是否返回登录？");

         QMessageBox mess(QMessageBox::Information, tr("提示"), tr("成功修改帐号密码,是否返回登录"));
         QPushButton *buttonBack = (mess.addButton(tr("返回"), QMessageBox::AcceptRole));
         QPushButton *buttonCancel = (mess.addButton(tr("否"), QMessageBox::RejectRole));
         mess.exec();
         if (mess.clickedButton() == buttonBack)
             emit cpBackToInit();


}

void changePS::on_btnCancel_clicked()
{
    emit cpBackToInit();
}
