#ifndef STAFFQUERY_H
#define STAFFQUERY_H

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

namespace Ui {
class staffQuery;
}

class staffQuery : public QWidget
{
    Q_OBJECT

public:
    explicit staffQuery(QWidget *parent = nullptr);
    ~staffQuery();
    void querrySnoID(QString);
private slots:
    void on_btnOK_clicked();

signals:
    void sQbacktosR();
private:
    Ui::staffQuery *ui;
    QString id;//员工ID
    QSqlDatabase db;
    QTimer qTimer;
};

#endif // STAFFQUERY_H
