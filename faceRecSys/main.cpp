#include "initlog.h"
#include "adminmenu.h"
#include"staffquery.h"
#include"staffreg.h"
#include"adminnewreg.h"
#include"noticeset.h"
#include"attendance.h"
#include"adminstaffchange.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initLog w;
    w.show();

//    adminMenu d;
//    d.show();
//    staffReg s;
//    s.show();
//    staffQuery s1;
//    s1.show();

//    adminNewReg s3;
//    s3.show();

//    NoticeSet s2;
//    s2.show();
//    Attendance s4;
//    s4.show();
//    adminStaffChange s5;
//    s5.show();

    return a.exec();
}

//判断时间大小测试
//    QTime timeam=ui->amTime->time();
//    QTime timepm=ui->pmTime->time();
//    QTime curT=QTime::currentTime();
//    int s=timeam.operator<=(timepm);
//    qDebug()<<timeam.toString("hh:mm");
//    qDebug()<<timepm.toString("hh:mm");
//    qDebug()<<s;
