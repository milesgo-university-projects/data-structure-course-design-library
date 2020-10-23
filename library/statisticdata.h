/******************************************************************
文件名: statisticdata.h
创建人: 杨正浩
日　期: 2017/5/15
修改人: 杨正浩
日　期: 2017/5/25
描　述: 记录统计信息
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef STATISTICSDATA_H
#define STATISTICSDATA_H
#include<QString>
#include<QVector>
#include<QMap>
#include<QDate>
#include<QPair>
#include<algorithm>
#include<QFile>
#include<QTextStream>
#include<Qdir>
#include<mytime.h>
extern myTime mytime;
//临时存储读者部分信息，姓名，学/工号
class person{
public:
    QString name;
    QString id;
    friend bool operator<(const person&a,const person &b)               //重载小于运算符
    {
        return (a.id<b.id||(a.id==b.id&&a.name<b.name));
    }
};

//临时储存书籍部分信息，书名，书籍编号
class booktemp
{
public:
    QString name;
    QString id;
    friend bool operator<(const booktemp &a,const booktemp & b)         //重载小于运算符
    {
        return (a.id<b.id||(a.id==b.id&&a.name<b.name));
    }
};
class bookRentRate{
public:
    int bookCountPerDay;
    int bookNums;
    QString date;
    bookRentRate()                                                      //默认构造函数
    {
        date="";
        bookCountPerDay=0;
        bookNums=1;
    }
}; 

//统计数据
class statisticsData
{
public:
    statisticsData();                                                    //构造函数
    ~statisticsData();                                                   //析构函数
    void addRate();                                                      //增加一天的统计信息
    void statisticAddBookTotal(QString bookname,QString bookid);
    void statisticAddReaderTotal(QString readername,QString readerid);
    const QVector<bookRentRate> getRate();                               //返回七天借阅率
    const QVector<QPair<QString,int> > getToptenBook();                  //返回借阅次数最多的前十本书（未到10本按照实际算）
    const QVector<QPair<QString,int> > getToptenReader();                //返回借阅书籍数量最多的读者
    void flushStatistic();                                               //刷新写入文件
    void clear();                                                        //需要增加
    bookRentRate today;
    
private:
    QMap<booktemp,int> bookRecord;                                       //图书即借阅次数
    QMap<person,int> readerRecord;                                       //读者借阅次数
    QVector<bookRentRate> allRates;                                      //借阅数据
};

#endif // STATISTICSDATA_H
