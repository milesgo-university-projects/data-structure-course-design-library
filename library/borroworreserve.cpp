/******************************************************************
文件名: borroworreserve.cpp
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/25
描　述: 预约改成借阅
版　本: v5.00 Copyright(c).
******************************************************************/
#include "borroworreserve.h"

BorrowOrReserve::BorrowOrReserve()
{
    renewTimes=0;
    hasSendOverdueInformation=false;
}

BorrowOrReserve::BorrowOrReserve(int bookPos, int readerPos, int bookSameNumber, QDate addDate)
{//把相应图书借阅改成预约
    this->borrowOrReserveFlag=true;
    this->bookPos=bookPos;
    this->readerPos=readerPos;
    this->bookSameNumber=bookSameNumber;
    this->addDate=addDate;
    renewTimes=0;
    hasSendOverdueInformation=false;
}

BorrowOrReserve::~BorrowOrReserve()
{

}
