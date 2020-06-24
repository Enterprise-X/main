#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include<QSqlError>
#include <QSqlTableModel>
#include <QTimer>
#include <QTime>
#include<QDebug>
#include <QMessageBox>
#include <QTableWidget>
#include <QFileDialog>
#include <QDesktopServices>
#include <QMessageBox>
#include <QAxObject>

namespace Ui {
class Attendance;
}

class Attendance : public QWidget
{
    Q_OBJECT

public:
    explicit Attendance(QWidget *parent = nullptr);
    ~Attendance();

signals:
    void aTTBackToAM();

private slots:
    void on_btnOut_clicked();

    void on_btnQuery_clicked();

    void on_btnSno_clicked();

    void on_btnDepart_clicked();

    void on_btnExcel_clicked();

private:
    Ui::Attendance *ui;
    QSqlDatabase db;
};

#endif // ATTENDANCE_H
