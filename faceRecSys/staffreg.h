#ifndef STAFFREG_H
#define STAFFREG_H
#include"staffquery.h"
#include <QWidget>
#include <QDebug>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include<QSqlError>
#include <QSqlTableModel>
#include <QTimer>
#include <QMessageBox>
#include <vector>
#include<QTime>
#include<QTimer>
#include <QDir>
#include <QThread>
#include <QTextCodec>//转码
#include <QProcess>
#include <QFileDialog>
#include <QFile>
/*
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
*/
using namespace cv;
using namespace std;
namespace Ui {
class staffReg;
}

class staffReg : public QWidget
{
    Q_OBJECT

public:
    explicit staffReg(QWidget *parent = nullptr);
    ~staffReg();
    staffQuery sQ;
    QString SnoID(QString);

signals:
      void srBackToInit();//员工签到界面返回初始界面信号
private slots:

      void on_btnOutUser_clicked();
      void sQtoThis();
      void run();
      void camtimeOut();
      void on_btnQuery_clicked();

      void on_btnStart_clicked();

      void on_btnCloseCam_clicked();
      void RecOKSLOT();
      void runtime();
      void disNotice();
      void canRec();
signals:
      void RecOK();//识别成功
private:
    Ui::staffReg *ui;
    QString id;//员工ID
    int logFlag=0;//登录标志 表示是否有员工登录 用于数据库通知查询
    int correctNum=0; //识别员工人脸正确次数 目的是减少识别误差
    int corectMax=30; //识别人脸正确的阈值
    QString todayNotice;
    QSqlDatabase db;
     int camFlag;//摄像头是否打开标志
     QTimer curTime,timer,camTimer;//timer定时器 效果同adminnewreg里的
      QDateTime td;//今天日期的QdateTime形式
     Mat cap,gray,dst,tempp;
     VideoCapture capture;
    vector<Rect> faces;//用于存放人脸
     Ptr<face::FaceRecognizer> model;
     CascadeClassifier cascade;
     //检测的标签
     int IDlabel;//将识别到的人脸送进model里面进行识别 如果识别到人脸数据库里的人脸则返回这个label
     int isRegFlag;//登录员工是否已经识别完成(即签到成功) 默认否（0） 如果经过run函数里的model识别成功 则将此标志置为1

     //查询签到时间表 得到的开始和结束时间
     QTime amS;//上班开始签到时间
     QTime amE;//上班签到结束时间
     QTime pmS;//下班开始签到时间
     QTime pmE;//下班签到结束时间
     //转换的字符串
     QString str_amS,str_amE,str_pmS,str_pmE;
    //签到时间状态 显示：上班签到 下班签到 不可签到  签到时间标志:0不可以签到 1上班签到 2下班签到
     QString recState;
     int recStateFlag;

};

#endif // STAFFREG_H
