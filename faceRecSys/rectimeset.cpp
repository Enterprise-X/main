//签到时间设置界面
#include "rectimeset.h"
#include "ui_rectimeset.h"
#include<QDebug>
recTimeSet::recTimeSet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recTimeSet)
{
    ui->setupUi(this);
    this->setWindowTitle("签到时间设置");

}

recTimeSet::~recTimeSet()
{
    delete ui;
}

void recTimeSet::on_btnOK_clicked()
{
    QTime amS=ui->amS->time();
    QTime amE=ui->amE->time();
    QTime pmS=ui->pmS->time();
    QTime pmE=ui->pmE->time();
    //判断上班开始和结束时间是否有冲突 1为不冲突
    int am=amS.operator<(amE);
    int pm=pmS.operator<(pmE);
    if(am!=1||pm!=1)
    {
           QMessageBox::about(this,"错误","时间冲突 请重新设置");
           return;
    }
    //不冲突则重新设置时间
    //判断人脸识别正确阈值是否过低（10最低）
    if(ui->lineFaceMax->text().toInt()<20){
        QMessageBox::about(this,"错误","人脸识别正确次数过低(次数小于20)");
        return;
    }
    //添加数据库驱动
    db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名
    db.setDatabaseName(".\\src\\faceRS.db");
    //打开数据库
    if(!db.open())
    {
        qDebug()<<"open db error";
    }

    QString updateTime=QString("update Worktime set amStart='%1',amEnd='%2',pmStart='%3',pmEnd='%4',corRecMax='%5' where Wno=1")
            .arg(amS.toString("hh:mm")).arg(amE.toString("hh:mm"))
            .arg(pmS.toString("hh:mm")).arg(pmE.toString("hh:mm"))
            .arg(ui->lineFaceMax->text().toInt());
    QSqlQuery sqlUT(updateTime);
    if(!sqlUT.exec())
    {

     qDebug()<<sqlUT.lastError();
    }
     QMessageBox::about(this,"提示","设置签到时间和人脸识别次数成功");

}

void recTimeSet::on_btnBACK_clicked()
{
    emit rTSBackToAM();
}
