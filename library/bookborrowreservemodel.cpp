/******************************************************************
文件名: bookborrowreservemodel.cpp
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/21
描　述: 暂存借阅和预约信息
版　本: v5.00 Copyright(c).
******************************************************************/
#include "bookborrowreservemodel.h"
#include <QDebug>
#include "bookviewwidget.h"
#include "widget.h"
#include "readerviewwidget.h"
#include "functions.h"
bookBorrowOrReserveTableModel::bookBorrowOrReserveTableModel(QWidget *parent):QAbstractTableModel(parent)
{

}

bookBorrowOrReserveTableModel::~bookBorrowOrReserveTableModel()
{

}

int bookBorrowOrReserveTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return pList.count();
}

int bookBorrowOrReserveTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return headerName.count()+1;
}

QVariant bookBorrowOrReserveTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    int column=index.column();
    int row=index.row();
    class BorrowOrReserve record=pList.at(row);
    class Book tmpBook=mainWindow->bookView->recordlister[record.bookPos];
    class Reader tmpReader=mainWindow->readerView->recordlister[record.readerPos];
    struct SameBookDistinction tmpSBD=tmpBook.getSameBookDistinction(record.bookSameNumber);
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
            {
                int auth=tmpReader.getAuth();
                int authPos=mainWindow->authorityView->findAuthPos(auth);
                int days;
                if(authPos!=-1)
                    days=mainWindow->authorityView->recordlister[authPos].getBorrowBookDaysLimit();
                else
                    days=20;
                QDate date=addDate(record.addDate,days+days*record.renewTimes);
                if(record.borrowOrReserveFlag)
                {
                    if(cmpDate(date,mytime.getDate()))
                        return "借阅已超期";
                    else
                        return "借阅";
                }
                else
                    return "预约";
            }
            else if(column==2)
            {
                return tmpBook.getName();
            }
            else if(column==3)
            {
                QString str;
                str.append(tmpBook.getClassificationNumber());
                str.append("/");
                str.append(tmpBook.getTypeNumber());
                str.append(".");
                str.append(QString("%1").arg(tmpSBD.sameNumber));
                return str;
            }
            else if(column==4)
            {
                return tmpReader.getName();
            }
            else if(column==5)
            {
                return tmpReader.getId();
            }
            else if(column==6)
            {
                return myDateToString(record.addDate);
            }
            else if(column==7)
            {
                if(tmpSBD.isReserved)
                {
                    return myDateToString(addDate(record.addDate,3));
                }
                int auth=tmpReader.getAuth();
                int authPos=mainWindow->authorityView->findAuthPos(auth);
                int days;
                if(authPos!=-1)
                    days=mainWindow->authorityView->recordlister[authPos].getBorrowBookDaysLimit();
                else
                    days=20;
                QDate date=addDate(record.addDate,days+days*record.renewTimes);
                return myDateToString(date);
            }
        }
        default:
            return QVariant();
    }
}

void bookBorrowOrReserveTableModel::updateData(QList<class BorrowOrReserve> list)
{
    beginResetModel();
    pList=list;
    endResetModel();
}

QVariant bookBorrowOrReserveTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags bookBorrowOrReserveTableModel::flags(const QModelIndex &index) const
{//设置各种flag，可编辑？可选中？
    if (!index.isValid())
        return QAbstractItemModel::flags(index);
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return flags;
}

void bookBorrowOrReserveTableModel::setHeaderName(QStringList list)
{
    headerName=list;
}



