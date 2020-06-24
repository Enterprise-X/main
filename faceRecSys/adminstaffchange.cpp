//员工信息修改界面
#include "adminstaffchange.h"
#include "ui_adminstaffchange.h"

adminStaffChange::adminStaffChange(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adminStaffChange)
{
    ui->setupUi(this);
    this->setWindowTitle("员工信息修改");
    isExistStaff=0; //是否有员工信息 默认没有
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

adminStaffChange::~adminStaffChange()
{
    delete ui;
}

//根据工号查询员工信息和帐号信息
void adminStaffChange::on_btnQuery_clicked()
{
    inputSno=ui->lineSno->text();
    if(inputSno.isEmpty()){
         QMessageBox::about(this,"错误","请输入员工工号");
         return;
    }
    //判断员工信息是否存在
    QString isExist=QString("select * from Staff where Sno='%1'").arg(inputSno);
    QSqlQuery queryIE(isExist);
    while (!queryIE.next()) {
        QMessageBox::about(this,"提示","该员工信息不存在，请修改工号");
        return ;
    }
    //员工存在 把输入工号框设置为不可输入 并把标志位置为1
    ui->lineSno->setReadOnly(true);
    isExistStaff=1;
   //显示员工信息
    QString qID=QString("select * from Staff where Sno='%1'").arg(inputSno);
    QSqlQuery queryID(qID);
    while (queryID.next()) {
       ui->lineSname->setText(queryID.value(1).toString());
       ui->combDepart->setCurrentText(queryID.value(3).toString());
       ui->combState->setCurrentText(queryID.value(6).toString());
       ui->amTime->setTime(queryID.value(4).toTime());
       ui->pmTime->setTime(queryID.value(5).toTime());
    }
    //显示员工帐号
     QString UID=QString("select * from User where Uno='%1'").arg(inputSno);
     QSqlQuery queryUID(UID);
     while (queryUID.next()) {
     ui->lineUserPS->setText(queryUID.value(1).toString());
     ui->lineUserQue->setText(queryUID.value(2).toString());
     ui->lineUserAns->setText(queryUID.value(3).toString());
     int is=queryUID.value(4).toInt();
     if(is==1){
     ui->combUserIsAdmin->setCurrentText("是");
     }
     else{
         ui->combUserIsAdmin->setCurrentText("否");
     }
     }


}
//取消按钮 清除所有信息（还原）
void adminStaffChange::on_btnCancel_clicked()
{   ui->lineSno->clear();
    ui->lineSno->setReadOnly(false);
    ui->lineSname->clear();
    ui->combDepart->setCurrentText("请选择部门");
    ui->combState->setCurrentText("请选择工作状态");

    ui->lineUserQue->clear();
    ui->lineUserAns->clear();
    ui->lineUserPS->clear();
    ui->combUserIsAdmin->setCurrentText("否");

    ui->amTime->setTime(QTime(0,0,0));
    ui->pmTime->setTime(QTime(0,0,0));

    isExistStaff=0;

}
//返回管理员菜单
void adminStaffChange::on_btnBack_clicked()
{
    emit aSCBackToAM();
    db.close();
    on_btnCancel_clicked();
}
//修改员工信息
void adminStaffChange::on_btnChange_clicked()
{
    if(isExistStaff==0)
    {
         QMessageBox::about(this,"提示","无员工");
         return;
    }
    QString updateStaff=QString("update Staff set Sname='%1',Dname='%2',AMtime='%3',PMtime='%4',state='%5' where Sno='%6'")
            .arg(ui->lineSname->text()).arg(ui->combDepart->currentText()).arg(ui->amTime->time().toString("hh:mm"))
            .arg(ui->pmTime->time().toString("hh:mm")).arg(ui->combState->currentText())
            .arg(inputSno);
    QSqlQuery sqlUS(updateStaff);
    if(!sqlUS.exec())
    {

     qDebug()<<sqlUS.lastError();
    }
    int isadmin=0;
    if(ui->combUserIsAdmin->currentText()=="是"){
        isadmin=1;
    }
    QString updateUser=QString("update User set Password='%1',Uque='%2',Uans='%3',IsAdmin='%4' where Uno='%5'")
            .arg(ui->lineUserPS->text()).arg(ui->lineUserQue->text()).arg(ui->lineUserAns->text())
            .arg(isadmin)
            .arg(inputSno);
    QSqlQuery sqlUU(updateUser);
    if(!sqlUU.exec())
    {

     qDebug()<<sqlUU.lastError();
    }
     QMessageBox::about(this,"提示","成功修改员工信息和帐号");

}
//删除员工信息
void adminStaffChange::on_pushButton_clicked()
{   //判断员工是否存在
    if(isExistStaff==0)
    {
         QMessageBox::about(this,"提示","无员工");
         return;
    }
   //存在 则删除人脸信息
    dir_str = ".\\src\\faceData\\";//存放人脸数据的目录
    //文件地址
    QDir dir(dir_str+ui->lineSno->text());
     //判断目录是否存在 存在则删除
        if(dir.exists())
        {
            bool reS = dir.removeRecursively();
            qDebug()<<"删除目录是否成功"<<reS;
        }

    QString sqlDeleteS = QString("delete from Staff where Sno='%1'").arg(inputSno);
    QSqlQuery sqlDS(sqlDeleteS);
    QString sqlDeleteU = QString("delete from User where Uno='%1'").arg(inputSno);
    QSqlQuery sqlDU(sqlDeleteU);
    QMessageBox::about(this,"提示","成功删除员工信息和帐号");
    on_btnCancel_clicked();
}
