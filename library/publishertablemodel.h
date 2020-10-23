/******************************************************************
文件名: publishertablemodel.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 出版社信息暂存
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef PUBLISHERTABLEMODEL
#define PUBLISHERTABLEMODEL
#include "functions.h"
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
#include <QDebug>

struct Publisher;

class publisherTableModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    publisherTableModel(QWidget *parent=0);
    ~publisherTableModel();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    void updateData(QList<Publisher> list);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void setHeaderName(QStringList list);
    QList<Publisher> pList;
    QStringList headerName;
    int horizontalHeaderCount;
};
#endif // PUBLISHERTABLEMODEL

