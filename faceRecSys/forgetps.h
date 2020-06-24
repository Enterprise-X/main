#ifndef FORGETPS_H
#define FORGETPS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include<QSqlError>
#include<QDebug>
#include<QMessageBox>

namespace Ui {
class forgetPS;
}

class forgetPS : public QWidget
{
    Q_OBJECT

public:
    explicit forgetPS(QWidget *parent = nullptr);
    ~forgetPS();
    void forgetID(QString id);
signals:
    void fpBackToInit();
private slots:
    void on_btnOK_clicked();

    void on_btnBack_clicked();

private:
    Ui::forgetPS *ui;
    QString ID;
    QSqlDatabase db;
};

#endif // FORGETPS_H
