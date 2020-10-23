/******************************************************************
文件名: Reader.h
创建人: 杨正浩
日　期: 2017/3/20
修改人: 顾溢杰
日　期: 2017/4/25
描　述: 读者的信息
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef READERS_H
#define READERS_H
#include<QString>
#include<algorithm>
#include <QTableWidget>
#include <QTableWidgetItem>
#include<QMap>
#include<QModelIndex>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QVector>
#include <QDebug>
using namespace std;

//表示预约或借阅信息相关的结构体
struct BookBorrowOrReserve
{
    bool borrowOrReserveFlag;//true表示借阅,false表示预约
    int bookPos;
    int sameNumber;
    QDate startDate;
    int renewTimes;
};

class Reader
{
private:
    QString name;                                                                       //姓名
    QString id;                                                                         //学/工号
    QString schoolBelonged;                                                             //学校
    QString password;                                                                   //密码
    enum Gender{Male,Female};
    Gender gender;                                                                      //性别，0男1女
    int overdueTime;                                                                    //标识超期天数
    int bookAuthority;                                                                  //借书权限
    double feeAccount;                                                                  //账户余额

    QVector<BookBorrowOrReserve> bookBorrowOrReserve;                                                           //已借书籍清单

public:
    Reader(QString Name,QString Id,QString Password,int GEnder,double Fee,QString School="SCS");
    Reader(QString Name,QString Id,QString Password,int GEnder,int Overdue,double Fee,QString School="SCS");
    Reader(){;}
    bool hasSentAcountNotEnoughInformation;
    QString getId(){return id;}                                                        //返回id
    QString getName(){return name;}
    QString getPassword(){return password;}                                            //返回加密后密码
    QString getSchool(){return schoolBelonged;}                                        //返回所在院系
    QString getGender(){return (gender==Male)?"男":"女";}                               //返回字符串形式性别
    QString getGender1(){return (gender==Male)?"0":"1";}
    QVector<BookBorrowOrReserve> getBookBorrowOrReserveVector();
    int getOverdueTime(){return overdueTime;}                                          //返回逾期次数
    double getFeeAccount(){return feeAccount;}                                         //返回目前账户余额
    int getAuth(){return bookAuthority;}                                               //返回权限，int型

    void chargeOverdueFee(double Fee){feeAccount -= Fee;}                              //预期扣费
    void Deposit(double Fee){feeAccount += Fee;}                                       //充值功能
    void setAccount(double Fee){feeAccount=Fee;}                                       //修改账户余额
    void setAccount(const QString &Fee){setAccount(Fee.toDouble());}
    void bookAuthorityChange(int Auth);                                                //权限修改，比如逾期次数太多，或者账户余额为空等
    void bookAuthorityChange(const QString & Auth);
    void idChange(QString ID);                                                         //修改id
    void nameChange(QString Name){name = Name;}                                        //修改姓名
    void operator =(const Reader& a);                                                  //重载赋值运算符，直接替换某个reader
    void passwordChange(QString newPass);                                              //改变密码
    void setMd5PassWord(QString md5Pass){password=md5Pass;}
    void schoolBelongedChange(const QString &School){schoolBelonged = School;}
    void setGender(const QString &GEnder){gender=(GEnder=="男"||GEnder=="0")?Male:Female;}
    void setOverdueTime(int times){overdueTime=(times>=0)?times:0;}                    //修改逾期次数
    void setOverdueTime(const QString &times){setOverdueTime(times.toInt());}
    void addOverdueTime(){overdueTime++;}
    void addBookBorrowOrReserveVector(BookBorrowOrReserve rec);
    void addRenewTimes(int bookPos);
    int getRenewTimeByRow(int row);
    int getSameNumberByRow(int row);
    int getBookPosByRow(int row);                                                      //获取为第几行
    bool operator<(const Reader& a);                                                   //排序所用
    void deleteBookAt(int bookPos,int sameNumber);
    void updateBookPosDueToBookDelete(int row);
    void deleteBBORItem(int bookPos,int bookSameNumber);
    void reserveToBorrow(int bookPos,int bookSameNumber);
    bool isBorrowOrReserveThisBook(int bookPos,bool &flag);
    void updateBookReserveInfo(int bookPos,int sameNumber);
    void clearBBOR();
};

#endif // READERS_H
