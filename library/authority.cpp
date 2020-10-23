/******************************************************************
文件名: administratormanagewidget.cpp
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/19
描　述: 权限定义与管理
版　本: v5.00 Copyright(c).
******************************************************************/
#include "authority.h"

void Authority::setReaderType(QString readerType)
{
    this->readerType=readerType;
}
void Authority::setAuthorityNumber(int authorityNumber)
{
    this->authorityNumber=authorityNumber;
}
void Authority::setBorrowBookAmoutsLimit(int borrowBookAmountsLimit)
{
    this->borrowBookAmountsLimit=borrowBookAmountsLimit;
}
void Authority::setBorrowBookDaysLimit(int borrowBookDaysLimit)
{
    this->borrowBookDaysLimit=borrowBookDaysLimit;
}
void Authority::setRenewTimesLimit(int renewTimesLimit)
{
    this->renewTimesLimit=renewTimesLimit;
}

QString Authority::getReaderType()
{
    return readerType;
}
int Authority::getAuthorityNumber()
{
    return authorityNumber;
}
int Authority::getBorrowBookAmountsLimit()
{
    return borrowBookAmountsLimit;
}
int Authority::getBorrowBookDaysLimit()
{
    return borrowBookDaysLimit;
}
int Authority::getRenewTimesLimit()
{
    return renewTimesLimit;
}
