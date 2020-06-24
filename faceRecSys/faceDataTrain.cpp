//训练人脸数据模块
#include "faceDataTrain.h"
#include<QDebug>
faceDataTrain::faceDataTrain(QObject *parent):QThread (parent)
{

}

void faceDataTrain::SetFilePath(QString path)
{
    this->path = path;
}

//读取文件的图像和标签
void faceDataTrain::readCsv(const string& csvFileName, vector<Mat>& imgs, vector<int>& labels) {
    char sep = ';';
    ifstream file(csvFileName.c_str(), ifstream::in);
    if (!file) //如果文件不存在
    {
        string error_message = "Invalid input";
        CV_Error(CV_StsBadArg, error_message);
    }
    string path, classlab,line;
    while (getline(file, line)) //从文本文件中读取一行字符，未指定限定符默认限定符为“/n”
    {
        stringstream lines(line);//这里采用stringstream主要作用是做字符串的分割
        getline(lines, path, sep);//读入图片文件路径以分好作为限定符
        getline(lines, classlab);//读入图片标签，默认限定符
        if (!path.empty() && !classlab.empty()) //如果读取成功，则将图片和对应标签压入对应容器中
        {
            imgs.push_back(imread(path, 0));
            labels.push_back(atoi(classlab.c_str()));
        }
    }
}


//开始训练
void faceDataTrain::run()
{
    string fnCsv = path.toLocal8Bit().toStdString();
    // 2个容器来存放图像数据和对应的标签
    vector<Mat> imgs; vector<int> labels;
    // 读取数据. 如果文件不合法就会出错
    // 输入的文件名已经有了
    try
    {readCsv(fnCsv, imgs, labels);}//从csv文件中批量读取训练数据
    catch (cv::Exception& e)
    {exit(1);}
    // 如果没有读取到足够图片，退出.
    if (imgs.size() <= 1) {
        string error_message = "Img too less";
        CV_Error(CV_StsError, error_message);}
    for (int i = 0; i < imgs.size(); i++)
        {//判断图片是否大小是不等于（100,100）
        if (imgs[i].size() != Size(100, 100))
        { qDebug()<<i<<imgs[i].size;}}


    // 下面的几行代码仅仅是从你的数据集中移除最后一张图片，作为测试图片
    //[gm:自然这里需要根据自己的需要修改，他这里简化了很多问题]
    Mat testSample = imgs[imgs.size() - 1];
    int testLabel = labels[labels.size() - 1];
    imgs.pop_back();//删除最后一张照片，此照片作为测试图片
    labels.pop_back();//删除最有一张照片的labels
    //特征脸识别
    Ptr<face::BasicFaceRecognizer> model = face::EigenFaceRecognizer::create();
    model->train(imgs, labels);
    model->save(".\\src\\EnterpriseEigenFaceModel.xml");
    //Fisher
    Ptr<face::BasicFaceRecognizer> model1 = face::FisherFaceRecognizer::create();
    model1->train(imgs, labels);
    model1->save(".\\src\\EnterpriseFaceFisherModel.xml");
    //LBPH(局部二值识别）
    Ptr<face::LBPHFaceRecognizer> model2 = face::LBPHFaceRecognizer::create();
    model2->train(imgs, labels);
    model2->save(".\\src\\EnterpriseLBPHModel.xml");

    // 下面对测试图像进行预测，predictedLabel是预测标签结果
    //注意predict()入口参数必须为单通道灰度图像，如果图像类型不符，需要先进行转换
    //predict()函数返回一个整形变量作为识别标签
    int predictedLabel = model->predict(testSample);//加载分类器
    int predictedLabel1 = model1->predict(testSample);
    int predictedLabel2 = model2->predict(testSample);

    // 还有一种调用方式，可以获取结果同时得到阈值:
    // int predictedLabel = -1;
    // double confidence = 0.0;
    //  model->predict(testSample, predictedLabel, confidence);
    emit finshed();
    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
    string result_message1 = format("Predicted class = %d / Actual class = %d.", predictedLabel1, testLabel);
    string result_message2 = format("Predicted class = %d / Actual class = %d.", predictedLabel2, testLabel);
    qDebug()<<QString::fromLocal8Bit(result_message.c_str());
    qDebug()<<QString::fromLocal8Bit(result_message1.c_str());
    qDebug()<<QString::fromLocal8Bit(result_message2.c_str());

}
