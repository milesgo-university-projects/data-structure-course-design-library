/******************************************************************
文件名: borroworreserve.h
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/25
描　述: 预约改成借阅
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef BORROWORRESERVE
#define BORROWORRESERVE
#include <QDate>
#include "book.h"
#include "Reader.h"
class Book;
class Reader;

class BorrowOrReserve
{
public:
    BorrowOrReserve();
    BorrowOrReserve(int bookPos,int readerPos,int bookSameNumber,QDate addDate);
    ~BorrowOrReserve();
    bool borrowOrReserveFlag;//true表示借阅,false表示预约
    int bookPos,readerPos,bookSameNumber;
    QDate addDate;
    int renewTimes;
    bool hasSendOverdueInformation;
};

#endif // BORROWORRESERVE

