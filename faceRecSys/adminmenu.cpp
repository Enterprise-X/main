//管理员菜单界面
//四个模块：新员工注册和人脸采集 员工信息管理 员工考勤 通知设置 签到时间设置（可选）
#include "adminmenu.h"
#include "ui_adminmenu.h"
#include<QDebug>
adminMenu::adminMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adminMenu)
{
    ui->setupUi(this);
    this->setWindowTitle("管理员菜单");
    //连接5个子界面发出的返回信号

    connect(&aNR,SIGNAL(aNRBackToAM()),this,SLOT(aNRtoThis()));

    connect(&aSC,SIGNAL(aSCBackToAM()),this,SLOT(aSCtoThis()));

    connect(&aTT,SIGNAL(aTTBackToAM()),this,SLOT(aTTtoThis()));

    connect(&noS,SIGNAL(noSBackToAM()),this,SLOT(noStoThis()));

    connect(&rTS,SIGNAL(rTSBackToAM()),this,SLOT(rTStoThis()));


}

adminMenu::~adminMenu()
{
    delete ui;
}

void adminMenu::adminID(QString ID)
{
    id=ID;
    ui->label->setText("当前管理员为:"+id);
}
//返回登录界面 退出帐号
void adminMenu::on_btnBack_clicked()
{
    emit amBackToInit();
}

//5个接受子界面发射的信号槽函数 作用 把本界面显示出来
void adminMenu::aNRtoThis()
{
       this->show();
       aNR.close();
       qDebug()<<"aNRT";
}

void adminMenu::aSCtoThis()
{
    this->show();
    aSC.close();
}

void adminMenu::aTTtoThis()
{
     this->show();
    aTT.close();
}

void adminMenu::noStoThis()
{
    this->show();
    noS.close();
}

void adminMenu::rTStoThis()
{
    this->show();
    rTS.close();
}



//5个切换子界面函数
void adminMenu::on_btnNew_clicked()
{
    this->hide();
    aNR.show();
}

void adminMenu::on_btnManage_clicked()
{
    this->hide();
    aSC.show();
}

void adminMenu::on_btnCheck_clicked()
{
    this->hide();
    aTT.show();
}

void adminMenu::on_btnInformSet_clicked()
{
    this->hide();
    noS.show();
}

void adminMenu::on_btnRecSet_clicked()
{
    this->hide();
    rTS.show();
}
