#ifndef INITLOG_H
#define INITLOG_H
#include"adminmenu.h"
#include"staffreg.h"
#include"changeps.h"
#include"forgetps.h"

#include <QWidget>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include<QSqlError>
#include <QSqlTableModel>
#include <QTimer>
#include <QTime>
#include<QDebug>
#include <QMessageBox>
#include <QFrame>
QT_BEGIN_NAMESPACE
namespace Ui { class initLog; }
QT_END_NAMESPACE

class initLog : public QWidget
{
    Q_OBJECT

public:
    initLog(QWidget *parent = nullptr);
    ~initLog();
    adminMenu aM; //管理员菜单
    staffReg sR;  //员工签到界面
    changePS cp;
    forgetPS fp;


 //   QString selectPassW(QString user);
private slots:
    void on_btnLog_clicked();
    void on_btnOut_clicked();
    void adminmenuToThis(); //从管理员菜单回到初始界面（本界面）
    void staffregToThis();//从员工登录签到界面回到初始界面（本界面）
    void changePSToThis();//从修改密码界面返回
    void forgetPSToThis();//从忘记密码界面返回
   // void test();
    void on_btnAlter_clicked();

    void on_btnForget_clicked();

private:
    Ui::initLog *ui;
    QSqlDatabase db;
    QFrame * background;
};
#endif // INITLOG_H
