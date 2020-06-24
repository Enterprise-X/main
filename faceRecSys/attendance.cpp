//员工考勤查询界面
#include "attendance.h"
#include "ui_attendance.h"

Attendance::Attendance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Attendance)
{
    ui->setupUi(this);
    this->setWindowTitle("员工考勤查询");
    //数据库设置
    db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名
    db.setDatabaseName(".\\src\\faceRS.db");
    //打开数据库
    if(!db.open())
    {
       qDebug()<<"open db error";
    }

}

Attendance::~Attendance()
{
    delete ui;
}
//返回管理员菜单 清空查询信息
void Attendance::on_btnOut_clicked()
{   //判断是否显示了其它信息 清除行
    int row=ui->tableWidget->rowCount();
    for(int i=row;i>=0;i--){
        ui->tableWidget->removeRow(i);
    }
    ui->lineSno->clear();
    ui->combDepart->setCurrentText("全部");
    //2020 5 29 答辩之前进行了新增了以下三条代码
    QString Qstrdate="2020-01-01";
    QDate Qdate=QDate::fromString(Qstrdate,"yyyy-MM-dd");
    ui->dateEdit->setDate(Qdate);

    emit aTTBackToAM();
}
//查询按钮 点击根据情况查询
void Attendance::on_btnQuery_clicked()
{   //获取当前要查询的选择信息
    QDateTime inputdate=ui->dateEdit->dateTime();
    QString theDate=inputdate.toString("yyyy-MM-dd");
    QString depart=ui->combDepart->currentText();
    QString sno=ui->lineSno->text();
    //根据选择进行查询
    //选择日期+部门 日期+员工号 单日期查询
    //单日期查询
    if(depart=="全部"&(sno.isEmpty()||sno=="全部"))
    {
        //进行单日期查询 即查询本日期的签到信息
        //qDebug()<<"riqi";
        //判断是否显示了其它信息 清除行
        int row=ui->tableWidget->rowCount();
        for(int i=row;i>=0;i--){
            ui->tableWidget->removeRow(i);
        }
        //查询输入的员工签到信息
        QString staffReg=QString("select * from Attend where Adate='%1'").arg(theDate);
        QSqlQuery stR(staffReg);
        for (int i=0;stR.next();i++) {
            ui->tableWidget->insertRow(i);
            QTableWidgetItem *date = new QTableWidgetItem(stR.value(0).toString());
            QTableWidgetItem *sno = new QTableWidgetItem(stR.value(1).toString());
            QTableWidgetItem *sname = new QTableWidgetItem(stR.value(2).toString());
            QTableWidgetItem *sdname = new QTableWidgetItem(stR.value(3).toString());
            QTableWidgetItem *amT = new QTableWidgetItem(stR.value(4).toString());
            QTableWidgetItem *amS = new QTableWidgetItem(stR.value(5).toString());
            QTableWidgetItem *pmT = new QTableWidgetItem(stR.value(6).toString());
            QTableWidgetItem *pmS = new QTableWidgetItem(stR.value(7).toString());
            QTableWidgetItem *staffS = new QTableWidgetItem(stR.value(8).toString());
            ui->tableWidget->setItem(i,0,date);
            ui->tableWidget->setItem(i,1,sno);
            ui->tableWidget->setItem(i,2,sname);
            ui->tableWidget->setItem(i,3,sdname);
            ui->tableWidget->setItem(i,4,amT);
            ui->tableWidget->setItem(i,5,amS);
            ui->tableWidget->setItem(i,6,pmT);
            ui->tableWidget->setItem(i,7,pmS);
            ui->tableWidget->setItem(i,8,staffS);
        }
    }
    //日期+部门
    if(depart!="全部"&(sno.isEmpty()||sno=="全部"))
    {
        //进行日期+部门查询
        //qDebug()<<"riqi+bumen";
        //判断是否显示了其它信息 清除行
        int row=ui->tableWidget->rowCount();
        for(int i=row;i>=0;i--){
            ui->tableWidget->removeRow(i);
        }
        //查询输入的员工签到信息
        QString staffReg=QString("select * from Attend where Adate='%1' AND Adname='%2'").arg(theDate).arg(depart);
        QSqlQuery stR(staffReg);
        for (int i=0;stR.next();i++) {
            ui->tableWidget->insertRow(i);
            QTableWidgetItem *date = new QTableWidgetItem(stR.value(0).toString());
            QTableWidgetItem *sno = new QTableWidgetItem(stR.value(1).toString());
            QTableWidgetItem *sname = new QTableWidgetItem(stR.value(2).toString());
            QTableWidgetItem *sdname = new QTableWidgetItem(stR.value(3).toString());
            QTableWidgetItem *amT = new QTableWidgetItem(stR.value(4).toString());
            QTableWidgetItem *amS = new QTableWidgetItem(stR.value(5).toString());
            QTableWidgetItem *pmT = new QTableWidgetItem(stR.value(6).toString());
            QTableWidgetItem *pmS = new QTableWidgetItem(stR.value(7).toString());
            QTableWidgetItem *staffS = new QTableWidgetItem(stR.value(8).toString());
            ui->tableWidget->setItem(i,0,date);
            ui->tableWidget->setItem(i,1,sno);
            ui->tableWidget->setItem(i,2,sname);
            ui->tableWidget->setItem(i,3,sdname);
            ui->tableWidget->setItem(i,4,amT);
            ui->tableWidget->setItem(i,5,amS);
            ui->tableWidget->setItem(i,6,pmT);
            ui->tableWidget->setItem(i,7,pmS);
            ui->tableWidget->setItem(i,8,staffS);
        }
    }
    //日期+工号
    if(!sno.isEmpty()&sno!="全部"&depart=="全部"){

         // qDebug()<<"riqi+yuangong";
        //查询此员工是否存在 存在则查询签到信息
        QString isExist=QString("select * from Staff where Sno='%1'").arg(sno);
        QSqlQuery queryIE(isExist);
        while (!queryIE.next()) {
            QMessageBox::about(this,"提示","该员工不存在，请确认工号再查询");
            return ;
        }
         //进行日期+工号查询
        //判断是否显示了其它信息 清除行
        int row=ui->tableWidget->rowCount();
        //qDebug()<<row;
        for(int i=row;i>=0;i--){
            ui->tableWidget->removeRow(i);
        }
        //查询输入的员工签到信息
        QString staffReg=QString("select * from Attend where Adate='%1' AND Asno='%2'").arg(theDate).arg(sno);
        QSqlQuery stR(staffReg);
        for (int i=0;stR.next();i++) {
            ui->tableWidget->insertRow(i);
            QTableWidgetItem *date = new QTableWidgetItem(stR.value(0).toString());
            QTableWidgetItem *sno = new QTableWidgetItem(stR.value(1).toString());
            QTableWidgetItem *sname = new QTableWidgetItem(stR.value(2).toString());
            QTableWidgetItem *sdname = new QTableWidgetItem(stR.value(3).toString());
            QTableWidgetItem *amT = new QTableWidgetItem(stR.value(4).toString());
            QTableWidgetItem *amS = new QTableWidgetItem(stR.value(5).toString());
            QTableWidgetItem *pmT = new QTableWidgetItem(stR.value(6).toString());
            QTableWidgetItem *pmS = new QTableWidgetItem(stR.value(7).toString());
            QTableWidgetItem *staffS = new QTableWidgetItem(stR.value(8).toString());
            ui->tableWidget->setItem(i,0,date);
            ui->tableWidget->setItem(i,1,sno);
            ui->tableWidget->setItem(i,2,sname);
            ui->tableWidget->setItem(i,3,sdname);
            ui->tableWidget->setItem(i,4,amT);
            ui->tableWidget->setItem(i,5,amS);
            ui->tableWidget->setItem(i,6,pmT);
            ui->tableWidget->setItem(i,7,pmS);
            ui->tableWidget->setItem(i,8,staffS);
        }
    }

}

//单员工查询
void Attendance::on_btnSno_clicked()
{
    QString sno=ui->lineSno->text();
    if(sno.isEmpty()||sno=="全部"){
        QMessageBox::information(this, tr("提示"), tr("输入工号"));
        return;
    }
    //查询此员工是否存在 存在则查询签到信息
    QString isExist=QString("select * from Staff where Sno='%1'").arg(sno);
    QSqlQuery queryIE(isExist);
    while (!queryIE.next()) {
        QMessageBox::about(this,"提示","该员工不存在，请确认工号再查询");
        return ;
    }
    //判断是否显示了其它信息 清除行
    int row=ui->tableWidget->rowCount();
    //qDebug()<<row;
    for(int i=row;i>=0;i--){
        ui->tableWidget->removeRow(i);
    }
    //查询输入的员工签到信息
    QString staffReg=QString("select * from Attend where Asno='%1'").arg(sno);
    QSqlQuery stR(staffReg);
    for (int i=0;stR.next();i++) {
        ui->tableWidget->insertRow(i);
        QTableWidgetItem *date = new QTableWidgetItem(stR.value(0).toString());
        QTableWidgetItem *sno = new QTableWidgetItem(stR.value(1).toString());
        QTableWidgetItem *sname = new QTableWidgetItem(stR.value(2).toString());
        QTableWidgetItem *sdname = new QTableWidgetItem(stR.value(3).toString());
        QTableWidgetItem *amT = new QTableWidgetItem(stR.value(4).toString());
        QTableWidgetItem *amS = new QTableWidgetItem(stR.value(5).toString());
        QTableWidgetItem *pmT = new QTableWidgetItem(stR.value(6).toString());
        QTableWidgetItem *pmS = new QTableWidgetItem(stR.value(7).toString());
        QTableWidgetItem *staffS = new QTableWidgetItem(stR.value(8).toString());
        ui->tableWidget->setItem(i,0,date);
        ui->tableWidget->setItem(i,1,sno);
        ui->tableWidget->setItem(i,2,sname);
        ui->tableWidget->setItem(i,3,sdname);
        ui->tableWidget->setItem(i,4,amT);
        ui->tableWidget->setItem(i,5,amS);
        ui->tableWidget->setItem(i,6,pmT);
        ui->tableWidget->setItem(i,7,pmS);
         ui->tableWidget->setItem(i,8,staffS);
    }
}
//单部门查询
void Attendance::on_btnDepart_clicked()
{
    QString depart=ui->combDepart->currentText();
    if(depart=="全部"){
        QMessageBox::information(this, tr("提示"), tr("请选择部门"));
        return;
    }

    //判断是否显示了其它信息 清除行
    int row=ui->tableWidget->rowCount();
    //qDebug()<<row;
    for(int i=row;i>=0;i--){
        ui->tableWidget->removeRow(i);
    }
    //查询输入的员工签到信息
    QString queryDepart=QString("select * from Attend where Adname='%1'").arg(depart);
    QSqlQuery qD(queryDepart);
    for (int i=0;qD.next();i++) {
        ui->tableWidget->insertRow(i);
        QTableWidgetItem *date = new QTableWidgetItem(qD.value(0).toString());
        QTableWidgetItem *sno = new QTableWidgetItem(qD.value(1).toString());
        QTableWidgetItem *sname = new QTableWidgetItem(qD.value(2).toString());
        QTableWidgetItem *sdname = new QTableWidgetItem(qD.value(3).toString());
        QTableWidgetItem *amT = new QTableWidgetItem(qD.value(4).toString());
        QTableWidgetItem *amS = new QTableWidgetItem(qD.value(5).toString());
        QTableWidgetItem *pmT = new QTableWidgetItem(qD.value(6).toString());
        QTableWidgetItem *pmS = new QTableWidgetItem(qD.value(7).toString());
        QTableWidgetItem *staffS = new QTableWidgetItem(qD.value(8).toString());
        ui->tableWidget->setItem(i,0,date);
        ui->tableWidget->setItem(i,1,sno);
        ui->tableWidget->setItem(i,2,sname);
        ui->tableWidget->setItem(i,3,sdname);
        ui->tableWidget->setItem(i,4,amT);
        ui->tableWidget->setItem(i,5,amS);
        ui->tableWidget->setItem(i,6,pmT);
        ui->tableWidget->setItem(i,7,pmS);
        ui->tableWidget->setItem(i,8,staffS);
    }


}
//将查询到的签到信息生成excel
void Attendance::on_btnExcel_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(ui->tableWidget, "保存",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),"Excel 文件(*.xls *.xlsx)");
    if (fileName!="")
         {
             QAxObject *excel = new QAxObject;
             if (excel->setControl("Excel.Application")) //连接Excel控件
             {
                 excel->dynamicCall("SetVisible (bool Visible)","false");//不显示窗体
                 excel->setProperty("DisplayAlerts", false);//不显示任何警告信息。如果为true那么在关闭是会出现类似“文件已修改，是否保存”的提示
                 QAxObject *workbooks = excel->querySubObject("WorkBooks");//获取工作簿集合
                 workbooks->dynamicCall("Add");//新建一个工作簿
                 QAxObject *workbook = excel->querySubObject("ActiveWorkBook");//获取当前工作簿
                 QAxObject *worksheet = workbook->querySubObject("Worksheets(int)", 1);

                 int i,j;
                 //QTablewidget 获取数据的列数
                 int colcount=ui->tableWidget->columnCount();
                  //QTablewidget 获取数据的行数
                 int rowscount=ui->tableWidget->rowCount();

                 //QTableView 获取列数
            //int colount=ui->tableview->model->columnCount();
                 //QTableView 获取行数
            //int rowcount=ui->tableview->model->rowCount();

                 QAxObject *cell,*col;
                 //标题行
                 cell=worksheet->querySubObject("Cells(int,int)", 1, 1);
                 cell->dynamicCall("SetValue(const QString&)", "签到信息");
                 cell->querySubObject("Font")->setProperty("Size", 18);
                 //调整行高
                 worksheet->querySubObject("Range(const QString&)", "1:1")->setProperty("RowHeight", 30);
                 //合并标题行
                 QString cellTitle;
                 cellTitle.append("A1:");
                 cellTitle.append(QChar(colcount - 1 + 'A'));
                 cellTitle.append(QString::number(1));
                 QAxObject *range = worksheet->querySubObject("Range(const QString&)", cellTitle);
                 range->setProperty("WrapText", true);
                 range->setProperty("MergeCells", true);
                 range->setProperty("HorizontalAlignment", -4108);//xlCenter
                 range->setProperty("VerticalAlignment", -4108);//xlCenter
                 //列标题
                 for(i=0;i<colcount;i++)
                 {
                     QString columnName;
                     columnName.append(QChar(i  + 'A'));
                     columnName.append(":");
                     columnName.append(QChar(i + 'A'));
                     col = worksheet->querySubObject("Columns(const QString&)", columnName);
                     col->setProperty("ColumnWidth", ui->tableWidget->columnWidth(i)/6);
                     cell=worksheet->querySubObject("Cells(int,int)", 2, i+1);
                     //QTableWidget 获取表格头部文字信息
                     columnName=ui->tableWidget->horizontalHeaderItem(i)->text();
                     //QTableView 获取表格头部文字信息
                     // columnName=ui->tableView_right->model()->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString();
                     cell->dynamicCall("SetValue(const QString&)", columnName);
                     cell->querySubObject("Font")->setProperty("Bold", true);
                     cell->querySubObject("Interior")->setProperty("Color",QColor(191, 191, 191));
                     cell->setProperty("HorizontalAlignment", -4108);//xlCenter
                     cell->setProperty("VerticalAlignment", -4108);//xlCenter
                 }

                //数据区

                //QTableWidget 获取表格数据部分
                 for(i=0;i<rowscount;i++){
                     for (j=0;j<colcount;j++)
                     {
                         worksheet->querySubObject("Cells(int,int)", i+3, j+1)->dynamicCall("SetValue(const QString&)", ui->tableWidget->item(i,j)?ui->tableWidget->item(i,j)->text():"");
                     }
                 }


                //QTableView 获取表格数据部分
                //  for(i=0;i<rowcount;i++) //行数
                //     {
                //         for (j=0;j<colcount;j++)   //列数
                //         {
                //             QModelIndex index = ui->tableView_right->model()->index(i, j);
                //             QString strdata=ui->tableView_right->model()->data(index).toString();
                //             worksheet->querySubObject("Cells(int,int)", i+3, j+1)->dynamicCall("SetValue(const QString&)", strdata);
                //         }
                //     }


                 //画框线
                 QString lrange;
                 lrange.append("A2:");
                 lrange.append(colcount - 1 + 'A');
                 lrange.append(QString::number(ui->tableWidget->rowCount() + 2));
                 range = worksheet->querySubObject("Range(const QString&)", lrange);
                 range->querySubObject("Borders")->setProperty("LineStyle", QString::number(1));
                 range->querySubObject("Borders")->setProperty("Color", QColor(0, 0, 0));
                 //调整数据区行高
                 QString rowsName;
                 rowsName.append("2:");
                 rowsName.append(QString::number(ui->tableWidget->rowCount() + 2));
                 range = worksheet->querySubObject("Range(const QString&)", rowsName);
                 range->setProperty("RowHeight", 20);
                 workbook->dynamicCall("SaveAs(const QString&)",QDir::toNativeSeparators(fileName));//保存至fileName
                 workbook->dynamicCall("Close()");//关闭工作簿
                 excel->dynamicCall("Quit()");//关闭excel
                 delete excel;
                 excel=NULL;
                 if (QMessageBox::question(NULL,"完成","文件已经导出，是否现在打开？",QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
                 {
                     QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(fileName)));
                 }
             }
             else
             {
                 QMessageBox::warning(NULL,"错误","未能创建 Excel 对象，请安装 Microsoft Excel。",QMessageBox::Apply);
             }
         }
}
