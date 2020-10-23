/******************************************************************
文件名: publishertablemodel.cpp
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 出版社信息暂存
版　本: v5.00 Copyright(c).
******************************************************************/
#include "publishertablemodel.h"
#include <QDebug>

publisherTableModel::publisherTableModel(QWidget *parent):QAbstractTableModel(parent)
{

}

publisherTableModel::~publisherTableModel()
{

}

int publisherTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return pList.count();
}

int publisherTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return headerName.count()+1;
}

bool publisherTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    int column = index.column();
    Publisher record=pList.at(index.row());
    switch(role)
    {
        case Qt::DisplayRole | Qt::EditRole:
        {
            if(column==1)
            {
                record.name=value.toString();
            }
            else if(column==2)
            {
                record.location=value.toString();
            }
            else if(column==3)
            {
                record.ISBN=value.toString();
            }
            else
            {
                return false;
            }
            pList.replace(index.row(),record);
            emit dataChanged(index,index);
            return true;
        }
        default:
            return false;
    }
}

QVariant publisherTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    int column=index.column();
    int row=index.row();
    Publisher record=pList.at(row);
    switch(role)
    {
        case Qt::TextColorRole:
        {
            return QColor(Qt::white);
        }
        case Qt::TextAlignmentRole:
        {
            return QVariant(Qt::AlignCenter);
        }
        case Qt::DisplayRole:
        {
            if(column==0)
            {
                return row+1;
            }
            else if(column==1)
            {
                return record.name;
            }
            else if(column==2)
            {
                return record.location;
            }
            else if(column==3)
            {
                return record.ISBN;
            }
        }
        case Qt::EditRole:
        {
            if(column==0)
            {
                return row+1;
            }
            else if(column==1)
            {
                return record.name;
            }
            else if(column==2)
            {
                return record.location;
            }
            else if(column==3)
            {
                return record.ISBN;
            }
        }
        default:
            return QVariant();
    }
}

void publisherTableModel::updateData(QList<Publisher> list)
{
    beginResetModel();
    pList=list;
    endResetModel();
}

QVariant publisherTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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
                    return headerName.at(section-1);
            }
        }
        default:
            return QVariant();
    }
}

Qt::ItemFlags publisherTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return flags;
}

void publisherTableModel::setHeaderName(QStringList list)
{
    headerName=list;
}

