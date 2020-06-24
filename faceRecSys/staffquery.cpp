//员工查询个人签到信息界面
#include "staffquery.h"
#include "ui_staffquery.h"

staffQuery::staffQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::staffQuery)
{
    ui->setupUi(this);
    this->setWindowTitle("员工个人签到信息查询");
    //数据库设置
    db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名
    db.setDatabaseName(".\\src\\faceRS.db");
    //打开数据库
    if(!db.open())
    {
       qDebug()<<"open db error";
    }


}

staffQuery::~staffQuery()
{
    delete ui;
}

void staffQuery::querrySnoID(QString ID)
{
    id=ID;
    //qDebug()<<"哪个查询"<<id;
    QDateTime today=QDateTime::currentDateTime();
    QString td=today.toString("yyyy-MM-dd");
    QString stafftodayReg=QString("select * from Attend where Adate='%1' AND Asno='%2'").arg(td).arg(id);
    QSqlQuery stR(stafftodayReg);

    while (stR.next()) {

        QTableWidgetItem *date = new QTableWidgetItem(stR.value(0).toString());
        QTableWidgetItem *sno = new QTableWidgetItem(stR.value(1).toString());
        QTableWidgetItem *sname = new QTableWidgetItem(stR.value(2).toString());
        QTableWidgetItem *sdname = new QTableWidgetItem(stR.value(3).toString());
        QTableWidgetItem *amT = new QTableWidgetItem(stR.value(4).toString());
        QTableWidgetItem *amS = new QTableWidgetItem(stR.value(5).toString());
        QTableWidgetItem *pmT = new QTableWidgetItem(stR.value(6).toString());
        QTableWidgetItem *pmS = new QTableWidgetItem(stR.value(7).toString());

        ui->tableWidget->setItem(1,0,date);
        ui->tableWidget->setItem(1,1,sno);
        ui->tableWidget->setItem(1,2,sname);
        ui->tableWidget->setItem(1,3,sdname);
        ui->tableWidget->setItem(1,4,amT);
        ui->tableWidget->setItem(1,5,amS);
        ui->tableWidget->setItem(1,6,pmT);
        ui->tableWidget->setItem(1,7,pmS);
    }
}

void staffQuery::on_btnOK_clicked()
{
    emit sQbacktosR();
}
