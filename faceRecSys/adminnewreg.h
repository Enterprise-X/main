#ifndef ADMINNEWREG_H
#define ADMINNEWREG_H

#include <QWidget>
#include<QDebug>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include<QSqlError>
#include <QSqlTableModel>
#include <QTime>
#include <QMessageBox>
#include <vector>
#include<QTimer>
#include <QDir>
#include <QThread>
#include <QTextCodec>//转码
#include <QProcess>
#include <QFileDialog>
#include <QFile>
#include "faceDataTrain.h"

using namespace cv;
using namespace std;
namespace Ui {
class adminNewReg;
}

class adminNewReg : public QWidget
{
    Q_OBJECT

public:
    explicit adminNewReg(QWidget *parent = nullptr);
    ~adminNewReg();

signals:
    void aNRBackToAM();


private slots:
    void on_btnOut_clicked();

    void on_btnAdd_clicked();

    void on_btnOpenCam_clicked();

    void on_btnCloseCam_clicked();

    void on_btnFaceColl_clicked();

    void run();
    void trainOK();
    void on_btnStartTarin_clicked();

    void on_btNCreateDate_clicked();

    void on_btnSelect_clicked();

private:
    Ui::adminNewReg *ui;
    QSqlDatabase db;
    CascadeClassifier cascada;
    int picNum;
    QString dir_str;//存放采集员工人脸的文件目录
    Mat frame, myFace,tempColl;
    Mat frameGray;//转为灰度图
    Mat tc;
    VideoCapture cap;
    vector<Rect> faces;//std::vector容器存检测到的faces
    QTimer mtimer;
    faceDataTrain *train;
};

#endif // ADMINNEWREG_H
