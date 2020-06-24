#ifndef NOTICESET_H
#define NOTICESET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include<QSqlError>
#include <QSqlTableModel>
#include <QTime>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class NoticeSet;
}

class NoticeSet : public QWidget
{
    Q_OBJECT

public:
    explicit NoticeSet(QWidget *parent = nullptr);
    ~NoticeSet();

signals:
    void noSBackToAM();

private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();
    void queryTodayNotice();
private:
    Ui::NoticeSet *ui;
    QSqlDatabase db;
    QString notice;
    QString todayDate;
};

#endif // NOTICESET_H
