#ifndef CHANGEPS_H
#define CHANGEPS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include<QSqlError>

namespace Ui {
class changePS;
}

class changePS : public QWidget
{
    Q_OBJECT

public:
    explicit changePS(QWidget *parent = nullptr);
    ~changePS();
    void changeID(QString id);
signals:
    void cpBackToInit();
private slots:
    void on_btnChange_clicked();

    void on_btnCancel_clicked();

private:
    Ui::changePS *ui;    
    QString ID;
    QSqlDatabase db;
};

#endif // CHANGEPS_H
