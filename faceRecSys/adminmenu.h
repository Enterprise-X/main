#ifndef ADMINMENU_H
#define ADMINMENU_H

#include"adminnewreg.h"
#include"noticeset.h"
#include"attendance.h"
#include"adminstaffchange.h"
#include"rectimeset.h"
#include <QWidget>

namespace Ui {
class adminMenu;
}

class adminMenu : public QWidget
{
    Q_OBJECT

public:
    explicit adminMenu(QWidget *parent = nullptr);
    ~adminMenu();
    adminNewReg aNR;
    adminStaffChange aSC;
    Attendance aTT;
    NoticeSet noS;
    recTimeSet rTS;
    void adminID(QString ID);
signals:
    void amBackToInit();//管理员菜单返回初始界面信号

private slots:
    void on_btnBack_clicked();
   //四个子界面返回函数
    void aNRtoThis();
    void aSCtoThis();
    void aTTtoThis();
    void noStoThis();
    void rTStoThis();

    void on_btnNew_clicked();

    void on_btnManage_clicked();

    void on_btnCheck_clicked();

    void on_btnInformSet_clicked();

    void on_btnRecSet_clicked();

private:
    Ui::adminMenu *ui;
    QString id;
};

#endif // ADMINMENU_H
