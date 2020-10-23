/******************************************************************
文件名: book.h
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/21
描　述: 定义书籍涉及参数
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef BOOK
#define BOOK
#include <QString>
#include <vector>
#include <algorithm>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMap>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDate>
#include <QVector>
#include <QDebug>
#include <QQueue>
#include "functions.h"
#include "publisher.h"

struct SameBookDistinction{
    int sameNumber; //同一本书中的编号
    bool isReserved; //是否被预定
    QString reserverID; //预订者ID,如果没有被预定,设置为空
    bool isBorrowed; //是否被借出
    QString borrowerID; //借阅者ID
    QDate addDate; //添加/购置时间
    QDate publishDate;
    int everBorrowedTimes;
    SameBookDistinction()
    {
        isReserved=false;
        reserverID="";
        isBorrowed=false;
        borrowerID="";
        everBorrowedTimes=0;
    }
};

/*分类规则
索书号=分类号/种次号/同一本书册号//索书号不用你来管

分类规则
分类号=用户所选的分类
种次号=书序号（按照书的添加顺序编号）
有一个全局变量 map<QString,int> addOrder;

addOrder[分类号] 表示该分类当前应该给书编序号为addOrder[分类号]
你写一个函数返回QString“分类号/种次号”
例如返回
K825.6/13

K852.6表示分类（用户在treewidget里选了什么就是什么），为人物传记.文学
/后面为种次号
13表示这本书为第13本添加的此种书（所以说当前addOrder["K825.6"]==13）
（即使同名也要不同编号，严格按照添加顺序编号）
然后别忘了addOrder["K825.6"]++;
 */

class Book
{
private:
    QString name; //图书名称 1
    QString authorName; //作者姓名
    QString ISBN; //ISBN编号 1

    QString classificationNumber;//分类号 管理员指定(兴国)
    QString classificationName;
    QString typeNumber; //种次号
    //分类号+/+种次号+.+sameNumber=索书号

    QVector<SameBookDistinction> sameBookDistinction;

    double price; //图书价格 1
    Publisher publisher; //出版社信息 1
    QString introduction;
public:
    Book();
    ~Book();

    void setName(QString name);
    void setAuthorName(QString authorName);
    void setISBN(QString ISBN);
    void setClassificationNumber(QString classificationNumber);
    void setClassificationName(QString classificationName);
    void setTypeNumber(QString typeNumber);

    void setIsReserved(bool isReserved,int num);
    void setAllNoReserved();
    void setIsBorrowed(bool isBorrowed,int num);
    void setAllNoBorrowed();
    void setReserverID(QString reserverID,int num);
    void setBorrowerID(QString borrowerID,int num);
    void setSameNumber(int sameNumber,int num);
    void setAddDate(QDate addDate,int num);
    void setPublishDate(QDate publishDate,int num);
    void setSameBookDistinction(SameBookDistinction sbd,int num);
    void addSameBookDistinction(SameBookDistinction sbd);
    void setSameBookDistinctionVector(QVector<SameBookDistinction> sbdVector);
    void replaceSameBookDistinction(SameBookDistinction sbd,int sameNumber);
    void clearSameBookDistinction();

    void setPrice(double price);
    void setPublisher(Publisher publisher);
    void setIntroduction(QString info);

    QString getName();
    QString getAuthorName();
    QString getISBN();
    QString getClassificationNumber();
    QString getClassificationName();
    QString getTypeNumber();
    QString getCallNumber();

    bool getIsReserved(int num);
    bool getAllIsReserved();
    bool getIsBorrowed(int num);
    bool getAllIsBorrowed();
    bool checkThisReaderHasBorrowedOrReserved(QString readerID);
    bool isThereBookAvailable(QString readerID,int &sameNumber);
    QString getReserverID(int num);
    QString getBorrowerID(int num);
    int getSameNumber(int num);
    QDate getAddDate(int num);
    QDate getPublishDate(int num);
    SameBookDistinction getSameBookDistinction(int num);
    QVector<SameBookDistinction> getSameBookDistinctionVector();

    double getPrice();
    Publisher getPublisher();
    QString getIntroduction();

    QQueue<int> reserveQueue;
    int checkReserveQueue();
    bool checkWhetherThisReaderInReserveQueue(int readerPos,int &pos);

    int getAllEverBorrowedTimes();
    int getAllInLibraryDays();
    double getBorrowedRatio();
    void clearAllEverBorrowedTimes();
    void addEverBorrowedTimes(int sameNumber,int delta);
};
#endif // BOOK
