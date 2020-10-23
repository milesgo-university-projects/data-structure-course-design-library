/******************************************************************
文件名: logs.cpp
创建人: 姜兴国
日　期: 2017/5/13
修改人: 姜兴国
日　期: 2017/5/25
描　述: 日志文件的添加
版　本: v5.00 Copyright(c).
******************************************************************/
#include "logs.h"
#include "functions.h"
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>

Logs::Logs()
{

}

Logs::~Logs()
{

}

void Logs::AppendAuthorityLogs(QString username,int borrowBookAmountsLimit,int borrowBookDaysLimit,QString readerType,int renewTimesLimit,int authoritynum)
{//见logs.h文件
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("用户： ")<<username<<QString(" 创建了新的用户权限类型。")<<endl;
    in<<"  "<<QString("读者类型：")<<readerType<<endl;
    in<<"  "<<QString("权限编号：")<<authoritynum<<endl;
    in<<"  "<<QString("借阅、预约册数上限：")<<borrowBookAmountsLimit<<endl;
    in<<"  "<<QString("借阅天数上限：")<<borrowBookDaysLimit<<endl;
    in<<"  "<<QString("续借次数上限：")<<renewTimesLimit<<endl;
    in<<endl;
}

void Logs::DeleAuthorityLogs(QString username,int borrowBookAmountsLimit,int borrowBookDaysLimit,QString readerType,int renewTimesLimit,int authoritynum)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("用户： ")<<username<<QString(" 删除了已有的用户权限类型。")<<endl;
    in<<"  "<<QString("被删除的读者类型：")<<readerType<<endl;
    in<<"  "<<QString("权限编号：")<<authoritynum<<endl;
    in<<"  "<<QString("借阅、预约册数上限：")<<borrowBookAmountsLimit<<endl;
    in<<"  "<<QString("借阅天数上限：")<<borrowBookDaysLimit<<endl;
    in<<"  "<<QString("续借次数上限：")<<renewTimesLimit<<endl;
    in<<endl;
}

void Logs::ReplaceAuthorityLogs(QString username,int lastborrowBookAmountsLimit,int lastborrowBookDaysLimit,QString lastreaderType,int lastrenewTimesLimit,int lastauthoritynum,int borrowBookAmountsLimit,int borrowBookDaysLimit,QString readerType,int renewTimesLimit,int authoritynum)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("用户： ")<<username<<QString(" 修改了新的用户权限类型。")<<endl;
    in<<QString("修改前的信息如下：")<<endl;
    in<<"  "<<QString("读者类型：")<<lastreaderType<<endl;
    in<<"  "<<QString("权限编号：")<<lastauthoritynum<<endl;
    in<<"  "<<QString("借阅、预约册数上限：")<<lastborrowBookAmountsLimit<<endl;
    in<<"  "<<QString("借阅天数上限：")<<lastborrowBookDaysLimit<<endl;
    in<<"  "<<QString("续借次数上限：")<<lastrenewTimesLimit<<endl;
    in<<QString("修改后的信息如下：")<<endl;
    in<<"  "<<QString("读者类型：")<<readerType<<endl;
    in<<"  "<<QString("权限编号：")<<authoritynum<<endl;
    in<<"  "<<QString("借阅、预约册数上限：")<<borrowBookAmountsLimit<<endl;
    in<<"  "<<QString("借阅天数上限：")<<borrowBookDaysLimit<<endl;
    in<<"  "<<QString("续借次数上限：")<<renewTimesLimit<<endl;
    in<<endl;
}

void Logs::AdministratorLogined(QString username)
{
     QString date = myDateToString(mytime.getDate());
     QFile LogsFile("../Logs/"+date+".txt");
     LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

     QTextStream in(&LogsFile);

     QString aaa="管理员账户：“";
     QString bbb="” 已登录。";
     in<<aaa<<username<<bbb<<endl;
     in<<endl;
}

void Logs::ReaderLogined(QString username)
{
     QString date = myDateToString(mytime.getDate());
     QFile LogsFile("../Logs/"+date+".txt");
     LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

     QTextStream in(&LogsFile);
     QString aaa="读者账户：“";
     QString bbb="” 已登录。";
     in<<aaa<<username<<bbb<<endl;
     in<<endl;
}

void Logs::AddBorrowOrReserveLogs(bool boroworreverse,QString readername,QString readernum,QString bookname,QString classificationNumber,QString typeNumber,QString classificationName,QString booksamenum)
{
     QString date = myDateToString(mytime.getDate());
     QFile LogsFile("../Logs/"+date+".txt");
     LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

     QTextStream in(&LogsFile);

     if(boroworreverse==1)
     {
         in<<QString("图书已借阅：")<<endl;
         in<<"  "<<QString("姓名为：")<<readername<<QString("  学号为：")<<readernum<<QString("的读者借阅了一本书。")<<endl;
         in<<"  "<<QString("该书的书名为：《")<<bookname<<QString("》  类名为：“")<<classificationName<<QString("”  索书号为：")<<classificationNumber<<"/"<<typeNumber<<endl;
     }
     else
     {
         in<<QString("图书已预约：")<<endl;
         in<<"  "<<QString("姓名为：")<<readername<<QString("  学号为：")<<readernum<<QString("的读者预约了一本书。")<<endl;
         in<<"  "<<QString("该书的书名为：《")<<bookname<<QString("》  类名为：“")<<classificationName<<QString("”  索书号为：")<<classificationNumber<<"/"<<typeNumber<<endl;
     }
     in<<endl;

     QFile BookFile("../Logs/book/"+bookname+".txt");
     BookFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

     QTextStream bookin(&BookFile);

     if(boroworreverse==1)
     {
         bookin<<QString("该图书已被借阅：")<<endl;
         bookin<<"  "<<QString("借阅者姓名为：")<<readername<<QString("  学号为：")<<readernum<<QString("  书的册号为：")<<booksamenum<<endl;
     }
     else
     {
         bookin<<QString("该图书已被预约：")<<endl;
         bookin<<"  "<<QString("预约者姓名为：")<<readername<<QString("  学号为：")<<readernum<<QString("  书的册号为：")<<booksamenum<<endl;
     }
     bookin<<endl;

}

void Logs::DeleBorrowOrReserveLogs(bool boroworreverse,QString readername,QString readernum,QString bookname,QString classificationNumber,QString typeNumber,QString classificationName,QString booksamenum)
{
     QString date = myDateToString(mytime.getDate());
     QFile LogsFile("../Logs/"+date+".txt");
     LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

     QTextStream in(&LogsFile);

     if(boroworreverse==1)
     {
         in<<QString("图书已归还：")<<endl;
         in<<"  "<<QString("姓名为：")<<readername<<QString("  学号为：")<<readernum<<QString("的读者归还了一本书。")<<endl;
         in<<"  "<<QString("该书的书名为：《")<<bookname<<QString("》  类名为：“")<<classificationName<<QString("”  索书号为：")<<classificationNumber<<"/"<<typeNumber<<endl;
     }
     else
     {
         in<<QString("图书已取消预约：")<<endl;
         in<<"  "<<QString("姓名为：")<<readername<<QString("  学号为：")<<readernum<<QString("的读者取消预约了一本书。")<<endl;
         in<<"  "<<QString("该书的书名为：《")<<bookname<<QString("》  类名为：“")<<classificationName<<QString("”  索书号为：")<<classificationNumber<<"/"<<typeNumber<<endl;
     }
     in<<endl;

     QFile BookFile("../Logs/book/"+bookname+".txt");
     BookFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

     QTextStream bookin(&BookFile);

     if(boroworreverse==1)
     {
         bookin<<QString("该图书已被归还：")<<endl;
         bookin<<"  "<<QString("归还者者姓名为：")<<readername<<QString("  学号为：")<<readernum<<QString("  书的册号为：")<<booksamenum<<endl;
     }
     else
     {
         bookin<<QString("该图书已取消预约：")<<endl;
         bookin<<"  "<<QString("取消预约者姓名为：")<<readername<<QString("  学号为：")<<readernum<<QString("  书的册号为：")<<booksamenum<<endl;
     }
     bookin<<endl;
}

void Logs::AddBook(QString name,QString authorName,QString ISBN,QString classificationNumber,QString classificationName,QString typeNumber,double price,QString publishername,QString publisherISBN,QString introduction)
{
     QString date = myDateToString(mytime.getDate());
     QFile LogsFile("../Logs/"+date+".txt");
     LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

     QTextStream in(&LogsFile);

     in<<QString("管理员已添加一本图书：")<<endl;
     in<<"  "<<QString("该书的书名为：《")<<name<<QString("》  作者姓名为：")<<authorName<<QString("  ISBN：")<<ISBN<<endl;
     in<<"  "<<QString("该书的类名为：“")<<classificationName<<QString("”  索书号为：")<<classificationNumber<<"/"<<typeNumber<<QString("  价格为：")<<price<<QString("  出版社名称为：")<<publishername<<QString("  出版社ISBN为：")<<publisherISBN<<endl;
     if(introduction!="")
         in<<"  "<<QString("该书的简介为：")<<introduction<<endl;
     in<<endl;
}

void Logs::ReplaceBook(QString name,QString authorName,QString ISBN,QString classificationNumber,QString classificationName,QString typeNumber,double price,QString publishername,QString publisherISBN,QString introduction)
{
     QString date = myDateToString(mytime.getDate());
     QFile LogsFile("../Logs/"+date+".txt");
     LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

     QTextStream in(&LogsFile);

     in<<QString("管理员已更改一本图书的信息：")<<endl;
     in<<"  "<<QString("更改图书的书名为：《")<<name<<QString("》")<<endl;
     in<<QString("更新后的信息如下：")<<endl;
     in<<QString("  作者姓名为：")<<authorName<<QString("  ISBN：")<<ISBN<<endl;
     in<<"  "<<QString("该书的类名为：“")<<classificationName<<QString("”  索书号为：")<<classificationNumber<<"/"<<typeNumber<<QString("  价格为：")<<price<<QString("  出版社名称为：")<<publishername<<QString("  出版社ISBN为：")<<publisherISBN<<endl;
     if(introduction!="")
         in<<"  "<<QString("该书的简介为：")<<introduction<<endl;
     in<<endl;
}

void Logs::DeleBook(QString name,QString authorName,QString ISBN,QString classificationNumber,QString classificationName,QString typeNumber,double price,QString publishername,QString publisherISBN,QString introduction)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("管理员已删除一本图书：")<<endl;
    in<<"  "<<QString("该书的书名为：《")<<name<<QString("》  作者姓名为：")<<authorName<<QString("  ISBN：")<<ISBN<<endl;
    in<<"  "<<QString("该书的类名为：“")<<classificationName<<QString("”  索书号为：")<<classificationNumber<<"/"<<typeNumber<<QString("  价格为：")<<price<<QString("  出版社名称为：")<<publishername<<QString("  出版社ISBN为：")<<publisherISBN<<endl;
    if(introduction!="")
        in<<"  "<<QString("该书的简介为：")<<introduction<<endl;
    in<<endl;
}

void Logs::AddReader(QString name,QString id,QString schoolBelonged,QString password,QString bookAuthorityname,int bookauthoritynum,double feeAccount)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("管理员已录入一位读者。")<<endl;
    in<<QString("  该读者的姓名为：")<<name<<QString("  学号：")<<id<<QString("  密码：")<<password<<QString("  所在学院：")<<schoolBelonged<<endl;
    in<<QString("  该读者的借阅权限名称为：")<<bookAuthorityname<<QString("  借阅权限编号：")<<bookauthoritynum<<QString("  用户初始充值为：")<<feeAccount<<endl;
    in<<endl;
}

void Logs::ReplaceReader(QString lastname,QString lastid,QString lastschoolBelonged,QString lastpassword,QString lastbookAuthorityname,int lastbookauthoritynum,double lastfeeAccount,QString name,QString id,QString schoolBelonged,QString password,QString bookAuthorityname,int bookauthoritynum,double feeAccount)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("管理员已修改名为 ")<<lastname<<QString(" 的读者信息。")<<endl;
    in<<QString("  修改前的学号：")<<lastid<<QString("  修改前的密码：")<<lastpassword<<QString("  修改前的所在学院：")<<lastschoolBelonged<<endl;
    in<<QString("  该读者修改前的借阅权限名称为：")<<lastbookAuthorityname<<QString("  修改前的借阅权限编号：")<<lastbookauthoritynum<<QString("  修改前的用户余额为：")<<lastfeeAccount<<endl;
    in<<QString("该读者修改后的信息如下：")<<endl;
    in<<QString("  该读者的修改后的姓名为：")<<name<<QString("  学号：")<<id<<QString("  密码：")<<password<<QString("  所在学院：")<<schoolBelonged<<endl;
    in<<QString("  该读者的借阅权限名称为：")<<bookAuthorityname<<QString("  借阅权限编号：")<<bookauthoritynum<<QString("  用户余额为：")<<feeAccount<<endl;
    in<<endl;
}

void Logs::DeleReader(QString name,QString id,QString schoolBelonged,QString password,QString bookAuthorityname,int bookauthoritynum,double feeAccount)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("管理员已注销一位读者，被注销的读者信息如下：")<<endl;
    in<<QString("  姓名：")<<name<<QString("  学号：")<<id<<QString("  密码：")<<password<<QString("  所在学院：")<<schoolBelonged<<endl;
    in<<QString("  该读者的借阅权限名称：")<<bookAuthorityname<<QString("  借阅权限编号：")<<bookauthoritynum<<QString("  用户初始充值为：")<<feeAccount<<endl;
    in<<endl;
}

void Logs::AddAdministrator(QString currentad,QString adname, QString adpassword)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("当前管理员“")<<currentad<<QString("”创建了一个管理员账户。")<<endl;
    in<<QString("  创建的管理员账户名：")<<adname<<QString("  密码：")<<adpassword<<endl;
    in<<endl;
}

void Logs::ReplaceAdministrator(QString currentad,QString lastadname,QString lastadpassword,QString adname,QString adpassword)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("当前管理员“")<<currentad<<QString("”修改了一个管理员账户。")<<endl;
    in<<QString("  修改前的管理员账户名：")<<lastadname<<QString("  密码：")<<lastadpassword<<endl;
    in<<QString("  修改后的管理员账户名：")<<adname<<QString("  密码：")<<adpassword<<endl;
    in<<endl;
}

void Logs::DeleAdministrator(QString currentad,QString adname, QString adpassword)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("当前管理员“")<<currentad<<QString("”注销了一个管理员账户。")<<endl;
    in<<QString("  注销的管理员账户名：")<<adname<<QString("  密码：")<<adpassword<<endl;
    in<<endl;
}

void Logs::AddFeeAcount(QString username, QString id, double lastfee, double fee)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("用户已充值。")<<endl;
    in<<QString("  被充值的用户名：")<<username<<QString("  ID：")<<id<<QString("  充值前金额：")<<lastfee<<QString("  充值后金额")<<fee<<endl;
    in<<endl;
}

void Logs::AddBooksVariety(QString username,QString classname,QString classnum)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("管理员：“")<<username<<QString("”已添加一个书籍类别。")<<endl;
    in<<"  "<<QString("类名：")<<classname<<QString("  类编号：")<<classnum<<endl;
    in<<endl;
}

void Logs::DeleBooksVariety(QString username,QString classname,QString classnum)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("管理员：“")<<username<<QString("”已删除一个书籍类别。")<<endl;
    in<<"  "<<QString("被删除的类名：")<<classname<<QString("  被删除的类编号：")<<classnum<<endl;
    in<<endl;
}

void Logs::AddWaitingQueue(QString username,QString usernum, QString bookname)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("用户：")<<username<<QString("  当前预约的图书: ")<<bookname<<QString(" 没有多余册，已加入等待队列。")<<endl;
    in<<endl;

    QFile AddFile("../Logs/reader/"+usernum+".txt");
    AddFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream aaa(&AddFile);

    aaa<<QString("用户：")<<username<<QString("  当前预约的图书: ")<<bookname<<QString(" 没有多余册，已加入等待队列。")<<endl;
    aaa<<endl;
}

void Logs::ReverseToBorrow(QString username,QString usernum, QString bookname,QString booksamenum)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    in<<QString("用户：")<<username<<QString("  当前预约的图书: ")<<bookname<<QString(" 已转为借阅状态。")<<endl;
    in<<endl;

    QFile AddFile("../Logs/reader/"+usernum+".txt");
    AddFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream aaa(&AddFile);

    aaa<<QString("用户：")<<username<<QString("  当前预约的图书: ")<<bookname<<QString(" 已转为借阅状态。")<<endl;
    aaa<<endl;

    QFile BookFile("../Logs/book/"+bookname+".txt");
    BookFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream bookin(&BookFile);

    bookin<<QString("该图书已被借阅：")<<endl;
    bookin<<"  "<<QString("借阅者姓名为：")<<username<<QString("  学号为：")<<usernum<<QString("  书的册号为：")<<booksamenum<<endl;
    bookin<<endl;

}

void Logs::ReaderHistoryAdd(bool boroworreverse, QString readername, QString readernum, QString bookname, QString classificationNumber, QString typeNumber, QString classificationName)
{
    QFile LogsFile("../Logs/reader/"+readername+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    if(boroworreverse==1)
    {
        in<<QString("用户")<<readername<<QString("  学号为：")<<readernum<<QString("已借阅一本图书：")<<endl;
        in<<"  "<<QString("该书的书名为：《")<<bookname<<QString("》  类名为：“")<<classificationName<<QString("”  索书号为：")<<classificationNumber<<"/"<<typeNumber<<endl;
    }
    else
    {
        in<<QString("用户")<<readername<<QString("  学号为：")<<readernum<<QString("已预约一本图书：")<<endl;
        in<<"  "<<QString("该书的书名为：《")<<bookname<<QString("》  类名为：“")<<classificationName<<QString("”  索书号为：")<<classificationNumber<<"/"<<typeNumber<<endl;
    }
    in<<endl;

}

void Logs::ReaderHistoryDele(bool boroworreverse, QString readername, QString readernum, QString bookname, QString classificationNumber, QString typeNumber, QString classificationName)
{
    QFile LogsFile("../Logs/reader/"+readername+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    if(boroworreverse==1)
    {
        in<<QString("用户： ")<<readername<<QString("  学号为：")<<readernum<<QString(" 已归还了一本图书：")<<endl;
        in<<"  "<<QString("该书的书名为：《")<<bookname<<QString("》  类名为：“")<<classificationName<<QString("”  索书号为：")<<classificationNumber<<"/"<<typeNumber<<endl;
    }
    else
    {
        in<<QString("用户")<<readername<<QString("  学号为：")<<readernum<<QString("已取消预约了一本图书：")<<endl;
        in<<"  "<<QString("该书的书名为：《")<<bookname<<QString("》  类名为：“")<<classificationName<<QString("”  索书号为：")<<classificationNumber<<"/"<<typeNumber<<endl;
    }
    in<<endl;
}

void Logs::ManagerGoOnBorrow(bool type, QString username, QString managername,QString bookname, int time)
{
    QString date = myDateToString(mytime.getDate());
    QFile LogsFile("../Logs/"+date+".txt");
    LogsFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream in(&LogsFile);

    if(type==1)
    {
        in<<QString("管理员：“")<<managername<<QString("”为读者“")<<username<<QString("”完成续借。")<<endl;
        in<<QString("续借书目为：《")<<bookname<<QString("》  当前续借次数为：")<<time<<endl;
        in<<endl;
    }
    else
    {
        in<<QString("读者“")<<username<<QString("”自行完成续借。")<<endl;
        in<<QString("续借书目为：《")<<bookname<<QString("》  当前续借次数为：")<<time<<endl;
        in<<endl;
    }
}

void Logs::DeleteAllLogsFile()
{
    QDir dir("../Logs");
    dir.setFilter(QDir::Files);
    int fileCount = dir.count();
    for (int i = 0; i < fileCount; i++)
    dir.remove(dir[i]);

    dir.setPath("../Logs/book");
    dir.setFilter(QDir::Files);
    fileCount = dir.count();
    for (int i = 0; i < fileCount; i++)
    dir.remove(dir[i]);

    dir.setPath("../Logs/reader");
    dir.setFilter(QDir::Files);
    fileCount = dir.count();
    for (int i = 0; i < fileCount; i++)
    dir.remove(dir[i]);

    dir.setPath("../dat/readerinformation");
    dir.setFilter(QDir::Files);
    fileCount = dir.count();
    for (int i = 0; i < fileCount; i++)
    dir.remove(dir[i]);
}
