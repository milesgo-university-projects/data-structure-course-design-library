/******************************************************************
文件名: readermodel.h
创建人: 杨正浩
日　期: 2017/4/25
修改人: 顾溢杰
日　期: 2017/5/25
描　述: 显示数据的一种模型
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef READERMODEL_H
#define READERMODEL_H

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
#include "reader.h"

class readerModel:public QAbstractTableModel
{
    Q_OBJECT
public:
    readerModel(QWidget *parent=0);
    ~readerModel();
    int rowCount(const QModelIndex &parent) const;                        //返回由多少行
    int columnCount(const QModelIndex &parent) const;                     //返回多少列
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;              //返回数据
    void updateData(QList<class Reader> list);                            //更新数据
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void setHeaderName(const QStringList & list);

    QList<class Reader> pList;                                            //列表
    QStringList headerName;                                               //每一列的头
    int horizontalHeaderCount;
};

#endif // READERMODEL_H
