#ifndef RECTIMESET_H
#define RECTIMESET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include<QSqlError>
#include <QSqlTableModel>
#include <QTime>
#include <QMessageBox>
namespace Ui {
class recTimeSet;
}

class recTimeSet : public QWidget
{
    Q_OBJECT

public:
    explicit recTimeSet(QWidget *parent = nullptr);
    ~recTimeSet();
signals:
    void rTSBackToAM();
private slots:
    void on_btnOK_clicked();

    void on_btnBACK_clicked();

private:
    Ui::recTimeSet *ui;
    QSqlDatabase db;
};

#endif // RECTIMESET_H
