#include "ExportExcelObject.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QVariant>
#include <QProgressDialog>
#include <QDebug>

//导出数据，借用数据库原理
int ExportExcelObject::export2Excel()
{
    if(fieldList.size() <= 0)                         //没有列头
    {
        qDebug() << "ExportExcelObject::export2Excel failed: No fields defined.";
        return -1;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "excelexport");
    if(!db.isValid())                                 //数据库无效
    {
        qDebug() << "ExportExcelObject::export2Excel failed: QODBC not supported.";
        return -2;
    }
    // set the dsn string
    QString dsn = QString("DRIVER={Microsoft Excel Driver (*.xls)};DSN='';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%1\";DBQ=%2").
                  arg(excelFilePath).arg(excelFilePath);
    db.setDatabaseName(dsn);
    if(!db.open())                                     //数据库为打开
    {
        qDebug() << "ExportExcelObject::export2Excel failed: Create Excel file failed by DRIVER={Microsoft Excel Driver (*.xls)}.";
        //QSqlDatabase::removeDatabase("excelexport");
        return -3;
    }

    QSqlQuery query(db);

    //drop the table if it's already exists            //删除重复表单
    QString s, sSql = QString("DROP TABLE [%1] (").arg(sheetName);
    query.exec(sSql);

    //create the table (sheet in Excel file)           //创造table
    sSql = QString("CREATE TABLE [%1] (").arg(sheetName);
    for (int i = 0; i < fieldList.size(); i++)
    {
        s = QString("[%1] %2").arg(fieldList.at(i)->sFieldName).arg(fieldList.at(i)->sFieldType);
        sSql += s;
        if(i < fieldList.size() - 1)
            sSql += " , ";
    }

    sSql += ")";
    query.prepare(sSql);

    if(!query.exec())
    {
        qDebug() << "ExportExcelObject::export2Excel failed: Create Excel sheet failed.";
        //db.close();
        //QSqlDatabase::removeDatabase("excelexport");
        return -4;
    }

    //add all rows                                    //开始插入
    sSql = QString("INSERT INTO [%1] (").arg(sheetName);
    for (int i = 0; i < fieldList.size(); i++)
    {
        sSql += "["+QString(fieldList.at(i)->sFieldName)+"]";
        if(i < fieldList.size() - 1)
            sSql += " , ";
    }
    sSql += ") VALUES (";
    for (int i = 0; i < fieldList.size(); i++)
    {
        sSql += QString(":data%1").arg(i);
        if(i < fieldList.size() - 1)
            sSql += " , ";
    }
    sSql += ")";

    QProgressDialog progressDialog;
    progressDialog.setRange(0,tableView->model()->rowCount());
    progressDialog.setValue(0);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setCancelButton(NULL);
    progressDialog.setWindowFlags(Qt::FramelessWindowHint);
    progressDialog.setMinimumDuration(0);
    progressDialog.show();

    int r, iRet = 0;                                 //写入所有数据
    for(r = 0 ; r < tableView->model()->rowCount() ; r++)
    {
        query.prepare(sSql);
        for (int c = 0; c < fieldList.size(); c++)
        {
            query.bindValue(QString(":data%1").arg(c), tableView->model()->data(tableView->model()->index(r, fieldList.at(c)->iCol)));
        }
        if(query.exec())
            iRet++;
        progressDialog.setValue(r+1);
    }
    return iRet;
}
