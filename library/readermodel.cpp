#include "readermodel.h"
#include "functions.h"
#include <QDebug>
//构造函数
readerModel::readerModel(QWidget *parent):QAbstractTableModel(parent)
{

}
//析构函数
readerModel::~readerModel()
{

}
//返回一共多少行数据
int readerModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return pList.count();
}
//返回多少列
int readerModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return headerName.count()+1;
}
//设置数据
bool readerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    int column = index.column();
    class Reader record=pList.at(index.row());
    switch(role)
    {
        case Qt::DisplayRole | Qt::EditRole:
        {
            if(column==1)
            {
                record.nameChange(value.toString());              //以下顾名思义，就是改变对应位置的数据，改变姓名
            }
            else if(column==2)
            {
                record.idChange(value.toString());
            }
            else if(column==3)
            {
                record.setGender(value.toString());
            }
            else if(column==4)
            {
                QString str=value.toString();
                record.bookAuthorityChange(authorityNameToNumber[str]);
            }
            else if(column==5)
            {
                record.setOverdueTime(value.toInt());
            }
            else if(column==6)
            {
                record.setAccount(value.toDouble());
            }
            else if(column==7)
            {
                record.schoolBelongedChange(value.toString());
            }
            else
            {
                return false;
            }
            pList.replace(index.row(),record);                    //直接替换某一个位置的数据
            emit dataChanged(index,index);                        //发射数据
            return true;
        }
        default:
            return false;
    }
}

//返回数据
QVariant readerModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    int column=index.column();
    int row=index.row();
    class Reader record=pList.at(row);
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
                return row+1;                                     //返回行数
            }
            else if(column==1)
            {
                return record.getName();                          //以下函数均为Reader类里返回基本信息的函数
            }
            else if(column==2)
            {
                return record.getId();
            }
            else if(column==3)
            {
                return record.getGender();
            }
            else if(column==4)
            {
                int auth=record.getAuth();
                return authorityNumberToName[auth];
            }
            else if(column==5)
            {
                return record.getOverdueTime();
            }
            else if(column==6)
            {
                return QString::number(record.getFeeAccount(),'f',2);
            }
            else if(column==7)
            {
                return record.getSchool();
            }
        }
        case Qt::EditRole:
        {
            if(column==0)
            {
                return row+1;                                       //返回行数
            }
            else if(column==1)
            {
                return record.getName();                           //返回姓名
            }
            else if(column==2)
            {
                return record.getId();                             //返回学号
            }
            else if(column==3)
            {
                return record.getGender();                         //以下顾名思义，都是reader里面的函数
            }
            else if(column==4)
            {
                int auth=record.getAuth();
                return authorityNumberToName[auth];
            }
            else if(column==5)
            {
                return record.getOverdueTime();
            }
            else if(column==6)
            {
                return record.getFeeAccount();
            }
            else if(column==7)
            {
                return record.getSchool();
            }
        }
        default:
            return QVariant();
    }
}

//更新信息
void readerModel::updateData(QList<class Reader> list)
{
    beginResetModel();
    pList=list;
    endResetModel();
}
//返回某一列的头部信息
QVariant readerModel::headerData(int section, Qt::Orientation orientation, int role) const
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

//返回标志（基本属性）
Qt::ItemFlags readerModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return flags;
}

//设置每一列的头部信息
void readerModel::setHeaderName(const QStringList & list)
{
    headerName=list;
}
