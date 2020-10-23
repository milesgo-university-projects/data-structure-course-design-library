/******************************************************************
文件名: bookmodel.cpp
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/21
描　述: 暂存书籍管理操作的信息
版　本: v5.00 Copyright(c).
******************************************************************/
#include "bookmodel.h"
#include <QDebug>

tableModel::tableModel(QWidget *parent):QAbstractTableModel(parent)
{

}

tableModel::~tableModel()
{

}

int tableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return pList.count();
}

int tableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return headerName.count()+1;
}

QVariant tableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    int column=index.column();
    int row=index.row();
    class Book record=pList.at(row);
    switch(role)
    {
        case Qt::TextColorRole:
        {
            return QColor(Qt::white);//字体颜色
        }
        case Qt::TextAlignmentRole:
        {
            return QVariant(Qt::AlignCenter);//信息居中显示
        }
        case Qt::DisplayRole:
        {
            if(column==0)
            {//如果是第0列，显示行数
                return row+1;
            }
            else if(column==1)
            {//如果是第1列，显示书名
                return record.getName();
            }
            else if(column==2)
            {
                return record.getAuthorName();
            }
            else if(column==3)
            {
                return record.getISBN();
            }
            else if(column==4)
            {
                return record.getPublisher().name;
            }
            else if(column==5)
            {
                return record.getClassificationNumber();
            }
            else if(column==6)
            {
                return record.getClassificationName();
            }
            else if(column==7)
            {
                return record.getTypeNumber();
            }
            else if(column==8)
            {
                return record.getPrice();
            }
            else if(column==9)
            {
                return QString::number(record.getBorrowedRatio()/100.0,'f',3);
            }
        }
        default:
            return QVariant();
    }
}

void tableModel::updateData(QList<class Book> list)
{
    beginResetModel();
    pList=list;
    endResetModel();
}

QVariant tableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(role)
    {
        case Qt::TextAlignmentRole:
        {
            return QVariant(Qt::AlignCenter);
        }
        case Qt::DisplayRole:
        {
            if(orientation==Qt::Horizontal)
            {
                if(section==0)
                    return "行数";
                else
                    return headerName.at(section-1);//设置表头
            }
        }
        default:
            return QVariant();
    }
}

Qt::ItemFlags tableModel::flags(const QModelIndex &index) const
{//设置各种flag，可编辑？可选中？
    if (!index.isValid())
        return QAbstractItemModel::flags(index);
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return flags;
}

void tableModel::setHeaderName(QStringList list)
{
    headerName=list;
}


