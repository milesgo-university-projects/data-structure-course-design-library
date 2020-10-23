/******************************************************************
文件名: ExportExcelObject.h
创建人: 杨正浩
日　期: 2017/4/20
修改人: 顾溢杰
日　期: 2017/5/17
描　述: 导出Excel，借用了网上的demo
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef EXPORTEXCELOBJECT_H
#define EXPORTEXCELOBJECT_H

#include <QObject>
#include <QTableView>
#include <QStringList>
#include <QSqlDatabase>

//Excel每一列的头
class EEO_Field
{
public:
    EEO_Field(const int ic, const QString &sf, const QString &st):
            iCol(ic),sFieldName(sf),sFieldType(st){}

    int     iCol;                              //列
    QString sFieldName;                        //列头
    QString sFieldType;                        //类型
};

//导出Excel
class ExportExcelObject : public QObject
{
    Q_OBJECT
public:
    ExportExcelObject(const QString &filepath, const QString &sheettitle,
                      QTableView *tableview):excelFilePath(filepath),
                      sheetName(sheettitle), tableView(tableview){}

    ~ExportExcelObject() {QSqlDatabase::removeDatabase("excelexport");}

public:
    void setOutputFilePath(const QString &spath) {excelFilePath = spath;}  //改变导出路径
    void setOutputSheetTitle(const QString &ssheet) {sheetName = ssheet;}  //设置工作单的名字
    void setTableView(QTableView *tableview) {tableView = tableview;}      //需要导出的tableview

    void addField(const int iCol, const QString &fieldname, const QString &fieldtype)
         {fieldList << new EEO_Field(iCol, fieldname, fieldtype);}         //添加列

    void removeAllFields()                                                 //移除所有列
         {while (!fieldList.isEmpty()) delete fieldList.takeFirst();}

    int export2Excel();                                                    //导出Excel

signals:
    void exportedRowCount(int row);

private:
    QString excelFilePath;                                                  //工作路径
    QString sheetName;                                                      //表单名字
    QTableView *tableView;                                                  //导出源
    QList<EEO_Field *> fieldList;                                           //列头
};

#endif // EXPORTEXCELOBJECT_H
