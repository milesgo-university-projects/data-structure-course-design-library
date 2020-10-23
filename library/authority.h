/******************************************************************
文件名: authority.h
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/19
描　述: 权限定义与管理
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef AUTHORITY
#define AUTHORITY
#include <QString>

class Authority
{
private:
    QString readerType;
    int authorityNumber;          //权限类型
    int borrowBookAmountsLimit;   //借阅数量限制
    int borrowBookDaysLimit;      //借阅天数限制
    int renewTimesLimit;          //借阅次数限制
public:
    void setReaderType(QString readerType);
    void setAuthorityNumber(int authorityNumber);
    void setBorrowBookAmoutsLimit(int borrowBookAmountsLimit);
    void setBorrowBookDaysLimit(int borrowBookDaysLimit);
    void setRenewTimesLimit(int renewTimesLimit);

    QString getReaderType();
    int getAuthorityNumber();
    int getBorrowBookAmountsLimit();
    int getBorrowBookDaysLimit();
    int getRenewTimesLimit();
};

#endif // AUTHORITY

