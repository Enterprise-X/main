#ifndef TRAINTHREAD_H
#define TRAINTHREAD_H

#include <QObject>
#include <QThread>
#include <opencv2/face.hpp>
#include<opencv2\core.hpp>
#include<opencv2\highgui.hpp>
#include<opencv2\imgproc.hpp>
#include <fstream>
#include <sstream>
#include <math.h>
#include <QDebug>

using namespace std;
using namespace cv;
class faceDataTrain : public QThread
{
    Q_OBJECT
public:

    explicit faceDataTrain(QObject *parent = nullptr);
    void SetFilePath(QString path);
protected:
    void run();

    void readCsv(const string& csvFileName, vector<Mat>& imgs, vector<int>& labels);

private:
    QString path;
signals:
    void finshed();
};

#endif // TRAINTHREAD_H
