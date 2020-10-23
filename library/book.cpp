/******************************************************************
文件名: book.cpp
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/21
描　述: 定义书籍涉及参数
版　本: v5.00 Copyright(c).
******************************************************************/
#include "book.h"
#include <QDebug>
#include "functions.h"
#include "widget.h"
class Widget;
//构造、析构函数
Book::Book()
{

}

Book::~Book()
{

}

void Book::setName(QString name)
{
    this->name=name;
}
void Book::setAuthorName(QString authorName)
{
    this->authorName=authorName;
}
void Book::setISBN(QString ISBN)
{
    this->ISBN=ISBN;
}
void Book::setClassificationNumber(QString classificationNumber)
{
    this->classificationNumber=classificationNumber;
}
void Book::setTypeNumber(QString typeNumber)
{
    this->typeNumber=typeNumber;
}
void Book::setIsReserved(bool isReserved,int num)
{
    int i;
    for(i=0;i<sameBookDistinction.size();i++)
    {
        if(sameBookDistinction[i].sameNumber==num)
        {
            sameBookDistinction[i].isReserved=isReserved;
            break;
        }
    }
}
void Book::setIsBorrowed(bool isBorrowed,int num)
{
    int i;
    for(i=0;i<sameBookDistinction.size();i++)
    {
        if(sameBookDistinction[i].sameNumber==num)
        {
            sameBookDistinction[i].isBorrowed=isBorrowed;
            break;
        }
    }
}
void Book::setReserverID(QString reserverID,int num)
{
    int i;
    for(i=0;i<sameBookDistinction.size();i++)
    {
        if(sameBookDistinction[i].sameNumber==num)
        {
            sameBookDistinction[i].reserverID=reserverID;
            break;
        }
    }
}
void Book::setBorrowerID(QString borrowerID,int num)
{
    int i;
    for(i=0;i<sameBookDistinction.size();i++)
    {
        if(sameBookDistinction[i].sameNumber==num)
        {
            sameBookDistinction[i].borrowerID=borrowerID;
            break;
        }
    }
}
void Book::setPrice(double price)
{
    this->price=price;
}
void Book::setAddDate(QDate addDate,int num)
{
    if(num<0||num>=sameBookDistinction.size())
    {
        qDebug()<<"越界";
        return;
    }
    this->sameBookDistinction[num].addDate=addDate;
}
void Book::setPublisher(Publisher publisher)
{
    this->publisher=publisher;
}
void Book::setSameNumber(int sameNumber,int num)
{
    if(num<0||num>=sameBookDistinction.size())
    {
        qDebug()<<"越界";
        return;
    }
    this->sameBookDistinction[num].sameNumber=sameNumber;
}
void Book::setPublishDate(QDate publishDate,int num)
{
    if(num<0||num>=sameBookDistinction.size())
    {
        qDebug()<<"越界";
        return;
    }
    this->sameBookDistinction[num].publishDate=publishDate;
}

void Book::setSameBookDistinction(SameBookDistinction sbd,int num)
{
    if(num<0||num>=sameBookDistinction.size())
    {
        qDebug()<<"越界";
        return;
    }
    this->sameBookDistinction[num]=sbd;
}

void Book::addSameBookDistinction(SameBookDistinction sbd)
{
    this->sameBookDistinction.push_back(sbd);
}

QString Book::getName()
{
    return name;
}
QString Book::getAuthorName()
{
    return authorName;
}
QString Book::getISBN()
{
    return ISBN;
}
QString Book::getClassificationNumber()
{
    return classificationNumber;
}
QString Book::getTypeNumber()
{
    return typeNumber;
}
QString Book::getCallNumber()
{
    return classificationNumber+"/"+typeNumber;
}
bool Book::getIsReserved(int num)
{
    return sameBookDistinction[num].isReserved;
}
bool Book::getIsBorrowed(int num)
{
    return sameBookDistinction[num].isBorrowed;
}
QString Book::getReserverID(int num)
{
    return sameBookDistinction[num].reserverID;
}
QString Book::getBorrowerID(int num)
{
    return sameBookDistinction[num].borrowerID;
}
double Book::getPrice()
{
    return price;
}
QDate Book::getAddDate(int num)
{
    return sameBookDistinction[num].addDate;
}
Publisher Book::getPublisher()
{
    return publisher;
}
int Book::getSameNumber(int num)
{
    return sameBookDistinction[num].sameNumber;
}
QDate Book::getPublishDate(int num)
{
    return this->sameBookDistinction[num].publishDate;
}

SameBookDistinction Book::getSameBookDistinction(int num)
{
    SameBookDistinction tmp;
    for(int i=0;i<sameBookDistinction.size();i++)
    {
        if(sameBookDistinction[i].sameNumber==num)
        {
            tmp=sameBookDistinction[i];
            break;
        }
    }
    return tmp;
}

QVector<SameBookDistinction> Book::getSameBookDistinctionVector()
{
    return sameBookDistinction;
}

void Book::clearSameBookDistinction()
{
    sameBookDistinction.clear();
}

void Book::setSameBookDistinctionVector(QVector<SameBookDistinction> sbdVector)
{
    sameBookDistinction=sbdVector;
}

void Book::setIntroduction(QString info)
{
    introduction=info;
}

QString Book::getIntroduction()
{
    return introduction;
}

void Book::replaceSameBookDistinction(SameBookDistinction sbd,int sameNumber)
{
    int i;
    for(i=0;i<sameBookDistinction.size();i++)
    {
        if(sameBookDistinction[i].sameNumber==sameNumber)
            sameBookDistinction.replace(i,sbd);
    }
}

bool Book::getAllIsBorrowed()
{
    int i;
    for(i=0;i<sameBookDistinction.size();i++)
    {
        if(sameBookDistinction[i].isBorrowed)
            return true;
    }
    return false;
}

bool Book::getAllIsReserved()
{
    int i;
    for(i=0;i<sameBookDistinction.size();i++)
    {
        if(sameBookDistinction[i].isReserved)
            return true;
    }
    return false;
}

void Book::setClassificationName(QString classificationName)
{
    this->classificationName=classificationName;
}

QString Book::getClassificationName()
{
    return this->classificationName;
}

int Book::checkReserveQueue()
{
    while(reserveQueue.size())
    {
        int tmp=reserveQueue.front();
        reserveQueue.pop_front();
        if(mainWindow->readerView->recordlister[tmp].getFeeAccount()>=0)
            return tmp;
    }
    return -1;
}

bool Book::checkThisReaderHasBorrowedOrReserved(QString readerID)
{
    for(int i=0;i<sameBookDistinction.size();i++)
    {
        if(sameBookDistinction[i].isBorrowed&&sameBookDistinction[i].borrowerID==readerID)
            return true;
        if(sameBookDistinction[i].isReserved&&sameBookDistinction[i].reserverID==readerID)
            return true;
    }
    return false;
}

bool Book::checkWhetherThisReaderInReserveQueue(int readerPos,int &pos)
{
    QQueue<int> tmpQue=reserveQueue;
    int count=0;
    while(tmpQue.size())
    {
        count++;
        if(tmpQue.front()==readerPos)
        {
            pos=count;
            return true;
        }
        tmpQue.pop_front();
    }
    return false;
}

bool Book::isThereBookAvailable(QString readerID,int &sameNumber)
{
    Q_UNUSED(readerID)
    for(int i=0;i<sameBookDistinction.size();i++)
    {
        if(!sameBookDistinction[i].isBorrowed&&!sameBookDistinction[i].isReserved)
        {
            sameNumber=sameBookDistinction[i].sameNumber;
            return true;
        }
    }
    return false;
}

void Book::setAllNoReserved()
{
    for(int i=0;i<sameBookDistinction.size();i++)
    {
        sameBookDistinction[i].isReserved=false;
    }
}

void Book::setAllNoBorrowed()
{
    for(int i=0;i<sameBookDistinction.size();i++)
    {
        sameBookDistinction[i].isBorrowed=false;
    }
}

int Book::getAllEverBorrowedTimes()
{
    int total=0;
    for(int i=0;i<sameBookDistinction.size();i++)
    {
        total+=sameBookDistinction[i].everBorrowedTimes;
    }
    return total;
}

int Book::getAllInLibraryDays()
{
    QDate date=mytime.getDate();
    int total=0;
    for(int i=0;i<sameBookDistinction.size();i++)
    {
        total+=subtractDate(sameBookDistinction[i].addDate,date);
    }
    return total;
}

double Book::getBorrowedRatio()
{
    int allEverBorrowedTimes=getAllEverBorrowedTimes();
    int allInLibraryDays=getAllInLibraryDays();
    return 1.0*allEverBorrowedTimes/allInLibraryDays;
}

void Book::clearAllEverBorrowedTimes()
{
    for(int i=0;i<sameBookDistinction.size();i++)
    {
        sameBookDistinction[i].everBorrowedTimes=0;
    }
}

void Book::addEverBorrowedTimes(int sameNumber,int delta)
{
    for(int i=0;i<sameBookDistinction.size();i++)
    {
        if(sameBookDistinction[i].sameNumber==sameNumber)
        {
            sameBookDistinction[i].everBorrowedTimes+=delta;
            break;
        }
    }
}
