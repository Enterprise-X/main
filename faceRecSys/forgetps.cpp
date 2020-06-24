//忘记密码界面
#include "forgetps.h"
#include "ui_forgetps.h"

forgetPS::forgetPS(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::forgetPS)
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


forgetPS::~forgetPS()
{
    delete ui;
}

void forgetPS::forgetID(QString id)
{
    ID=id;

    ui->linePS->clear();
    ui->lineAns->clear();
    ui->labUser->setText("用户:"+ID);
    //查询问题
    QString queryQue=QString("select Uque from User where Uno='%1'").arg(ID); //查询密码 如果返回空则没有该帐号
    QSqlQuery qQue(queryQue);
    QString que; //查询得到的密码；
    while (qQue.next()) {
                que = qQue.value(0).toString();
            }
    ui->labQue->setText(que);
}

void forgetPS::on_btnOK_clicked()
{
    if(ui->linePS->text().isEmpty())
    {   QMessageBox::about(this,"错误","请输入新密码");
        return;
    }
    QString inputAns=ui->lineAns->text();
    //查询答案
    QString queryAns=QString("select Uans from User where Uno='%1'").arg(ID); //查询密码 如果返回空则没有该帐号
    QSqlQuery qAns(queryAns);
    QString ans; //查询得到的密码；
    while (qAns.next()) {
                ans = qAns.value(0).toString();
            }
    if(ans!=inputAns){
        QMessageBox::about(this,"错误","答案错误 请重新输入");
        return;
    }
    //如果答案正确 则修改密码
         QString updateUserPS=QString("update User set Password='%1' where Uno='%2'").arg(ui->linePS->text()).arg(ID);
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
                 emit fpBackToInit();
}

void forgetPS::on_btnBack_clicked()
{
    emit fpBackToInit();
}
