#ifndef ADMINSTAFFCHANGE_H
#define ADMINSTAFFCHANGE_H

#include <QWidget>
#include<QDebug>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include<QSqlError>
#include <QSqlTableModel>
#include <QTime>
#include <QMessageBox>
#include<QTimer>
#include <QDir>
#include <QFileDialog>
#include <QFile>
namespace Ui {
class adminStaffChange;
}

class adminStaffChange : public QWidget
{
    Q_OBJECT

public:
    explicit adminStaffChange(QWidget *parent = nullptr);
    ~adminStaffChange();

signals:
    void aSCBackToAM();


private slots:
    void on_btnCancel_clicked();

    void on_btnQuery_clicked();

    void on_btnBack_clicked();

    void on_btnChange_clicked();

    void on_pushButton_clicked();

private:
    Ui::adminStaffChange *ui;
    QSqlDatabase db;
    QString inputSno;
    int isExistStaff;
    QString dir_str;//存放采集员工人脸的文件目录 删除用
};

#endif // ADMINSTAFFCHANGE_H
