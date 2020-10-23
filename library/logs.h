/******************************************************************
文件名: logs.h
创建人: 姜兴国
日　期: 2017/5/13
修改人: 姜兴国
日　期: 2017/5/25
描　述: 日志文件的添加
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef LOGS
#define LOGS
#include <QString>

class Logs
{
public:
    Logs();
    ~Logs();
    static void AppendAuthorityLogs(QString username,int borrowBookAmountsLimit,int borrowBookDaysLimit,QString readerType,int renewTimesLimit,int authoritynum);
    static void ReplaceAuthorityLogs(QString username,int lastborrowBookAmountsLimit,int lastborrowBookDaysLimit,QString lastreaderType,int lastrenewTimesLimit,int lastauthoritynum,int borrowBookAmountsLimit,int borrowBookDaysLimit,QString readerType,int renewTimesLimit,int authoritynum);
    static void DeleAuthorityLogs(QString username,int borrowBookAmountsLimit,int borrowBookDaysLimit,QString readerType,int renewTimesLimit,int authoritynum);
    //添加权限
    static void AdministratorLogined(QString username);
    static void ReaderLogined(QString username);
    //管理员或者读者登录
    static void AddBorrowOrReserveLogs(bool boroworreverse,QString readername,QString readernum,QString bookname,QString classificationNumber,QString typeNumber,QString classificationName,QString booksamenum);
    static void DeleBorrowOrReserveLogs(bool boroworreverse,QString readername,QString readernum,QString bookname,QString classificationNumber,QString typeNumber,QString classificationName,QString booksamenum);
    //添加以及删除借阅信息
    static void AddBook(QString name,QString authorName,QString ISBN,QString classificationNumber,QString classificationName,QString typeNumber,double price,QString publishername,QString publisherISBN,QString introduction);
    static void ReplaceBook(QString name,QString authorName,QString ISBN,QString classificationNumber,QString classificationName,QString typeNumber,double price,QString publishername,QString publisherISBN,QString introduction);
    static void DeleBook(QString name,QString authorName,QString ISBN,QString classificationNumber,QString classificationName,QString typeNumber,double price,QString publishername,QString publisherISBN,QString introduction);
    //添加更改、删除图书信息
    static void AddReader(QString name,QString id,QString schoolBelonged,QString password,QString bookAuthorityname,int bookauthoritynum,double feeAccount);
    static void ReplaceReader(QString lastname,QString lastid,QString lastschoolBelonged,QString lastpassword,QString lastbookAuthorityname,int lastbookauthoritynum,double lastfeeAccount,QString name,QString id,QString schoolBelonged,QString password,QString bookAuthorityname,int bookauthoritynum,double feeAccount);
    static void DeleReader(QString name,QString id,QString schoolBelonged,QString password,QString bookAuthorityname,int bookauthoritynum,double feeAccount);
    //添加、更改和删除读者信息
    static void AddAdministrator(QString currentad,QString adname,QString adpassword);
    static void ReplaceAdministrator(QString currentad,QString lastadname,QString lastadpassword,QString adname,QString adpassword);
    static void DeleAdministrator(QString currentad,QString adname,QString adpassword);
    //添加更改或删除管理员账户
    static void AddFeeAcount(QString username,QString id,double lastfee,double fee);
    //添加费用
    static void AddBooksVariety(QString username,QString classname,QString classnum);
    //static void ReplaceBooksVariety(QString username,QString lastclassname,QString lastclassnum,QString classname,QString classnum);
    static void DeleBooksVariety(QString username,QString classname,QString classnum);
    //添加删除图书类别
    static void AddWaitingQueue(QString username,QString usernum,QString bookname);
    //加入等待队列
    static void ReverseToBorrow(QString username,QString usernum,QString bookname,QString booksamenum);
    //预约改借阅
    static void ReaderHistoryAdd(bool boroworreverse,QString readername,QString readernum,QString bookname,QString classificationNumber,QString typeNumber,QString classificationName);
    static void ReaderHistoryDele(bool boroworreverse,QString readername,QString readernum,QString bookname,QString classificationNumber,QString typeNumber,QString classificationName);
    //某位读者添加或者归还过一本书
    static void ManagerGoOnBorrow(bool type,QString username,QString managername,QString bookname,int time);
    //该书被借阅情况
    static void DeleteAllLogsFile();
};

#endif // LOGS

