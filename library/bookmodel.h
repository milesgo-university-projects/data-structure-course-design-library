/******************************************************************
文件名: bookmodel.h
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/21
描　述: 暂存书籍管理操作的信息
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef BOOKMODEL
#define BOOKMODEL

#include <QAbstractTableModel>
#include <QObject>
#include <QList>
#include <QModelIndex>
#include <QVariant>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QTextStream>
#include <QFile>
#include <QIODevice>
#include "book.h"

class tableModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    tableModel(QWidget *parent=0);
    ~tableModel();
    int rowCount(const QModelIndex &parent) const;//设置表格有几行
    int columnCount(const QModelIndex &parent) const;//设置表格有几列
    QVariant data(const QModelIndex &index, int role) const;
    void updateData(QList<class Book> list);//更新model中的数据
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;//设置表头
    Qt::ItemFlags flags(const QModelIndex &index) const;//表格是否可选中、可编辑等

    void setHeaderName(QStringList list);//外界调用设置表头信息

    QList<class Book> pList;//存储书的容器
    QStringList headerName;//存储表头的容器
    int horizontalHeaderCount;
};
#endif // BOOKMODEL

