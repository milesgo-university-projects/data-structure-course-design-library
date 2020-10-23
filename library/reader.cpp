#include "Reader.h"
#include <QDebug>
#include "functions.h"
#include "mytime.h"

//构造函数，注册时用

Reader::Reader(QString Name,QString Id,QString Password,int GEnder,double Fee,QString School)
{
    name=Name;                                                                            //对照参数表修改相应参数
    id=Id;
    password=Password;
    gender=(GEnder)?Female:Male;                                                          //判断性别
    feeAccount=Fee;
    schoolBelonged=School;
    switch(id[4].unicode())                                                               //通过id判断身份，分配相应的默认权限，11,21,01都是学生
    {
        case '0':case '1':case '2':
            bookAuthority=1;
            break;
        case '3':                                                                         //31是教职工
            bookAuthority=2;
            break;
    }
    overdueTime=0;                                             //初始借的书预约的书都为0
    hasSentAcountNotEnoughInformation=false;
}

//构造函数，读取所有信息
Reader::Reader(QString Name,QString Id,QString Password,int GEnder,int Overdue,double Fee,QString School)
{
    name=Name;                                                                            //对照参数表修改相应参数
    id=Id;
    password=Password;
    gender=(GEnder)?Female:Male;
    feeAccount=Fee;
    schoolBelonged=School;
    switch(id[4].unicode())                                                                //同上一个构造函数
    {
        case '0':case '1':case '2':
            bookAuthority=1;
            break;
        case '3':
            bookAuthority=2;
            break;
    }
    overdueTime=Overdue;
    hasSentAcountNotEnoughInformation=false;
}

//重载赋值运算符，简化修改操作时使用
void Reader::operator =(const Reader& a)
{
    this->name=a.name;                                                                      //将所有信息修改为赋值运算符右侧的值
    this->id=a.id;
    this->schoolBelonged=a.schoolBelonged;
    this->password=a.password;
    this->gender=a.gender;
    this->overdueTime=a.overdueTime;
    this->bookAuthority=a.bookAuthority;
    this->feeAccount=a.feeAccount;
}

//修改读者权限,不修改身份，只修改为身份对应的权限,
void Reader::bookAuthorityChange(int Auth)
{
    bookAuthority=Auth;
}

//重载小于运算符
bool Reader::operator <(const Reader& a)
{
    return id<a.id;
}

//改变ID
void Reader::idChange(QString ID)
{
    id=ID;
    switch(id[4].unicode())
    {
        case '0':case '1':case '2':
            bookAuthority=1;
            break;
        case '3':
            bookAuthority=2;
            break;
    }
}

//改变权限
void Reader::bookAuthorityChange(const QString & Auth)
{
    if(Auth=="1"||Auth=="职工")
        bookAuthorityChange(1);
    else if(Auth=="2"||Auth=="学生")
        bookAuthorityChange(2);
    else
        bookAuthorityChange(3);
}

//返回借阅或者预约队列
QVector<BookBorrowOrReserve> Reader::getBookBorrowOrReserveVector()
{
    return bookBorrowOrReserve;
}

void Reader::addBookBorrowOrReserveVector(BookBorrowOrReserve rec)
{
    bookBorrowOrReserve.push_back(rec);
}

void Reader::deleteBookAt(int bookPos, int sameNumber)
{
    int i;
    for(i=0;i<bookBorrowOrReserve.size();i++)
    {
        if(bookBorrowOrReserve[i].bookPos==bookPos&&bookBorrowOrReserve[i].sameNumber==sameNumber)
        {
            bookBorrowOrReserve.erase(bookBorrowOrReserve.begin()+i);
            break;
        }
    }
}

//更新由于删除书籍而造成的影响
void Reader::updateBookPosDueToBookDelete(int row)
{
    int i;
    for(i=0;i<bookBorrowOrReserve.size();i++)
    {
        if(bookBorrowOrReserve[i].bookPos>row)
            bookBorrowOrReserve[i].bookPos--;
    }
}

//改变密码，输入加密前的密码
void Reader::passwordChange(QString newPass)
{
    password=md5(newPass);
}

//同一本书被多次预约
void Reader::addRenewTimes(int bookPos)
{
    for(int i=0;i<bookBorrowOrReserve.size();i++)
    {
        if(bookBorrowOrReserve[i].bookPos==bookPos)
        {
            bookBorrowOrReserve[i].renewTimes++;
            break;
        }
    }
}

int Reader::getRenewTimeByRow(int row)
{
    if(row>=0&&row<bookBorrowOrReserve.size())
        return bookBorrowOrReserve[row].renewTimes;
    qDebug()<<"reader.cpp getRenewTimeByRow 越界";
    return -1;
}

int Reader::getSameNumberByRow(int row)
{
    if(row>=0&&row<bookBorrowOrReserve.size())
        return bookBorrowOrReserve[row].sameNumber;
    qDebug()<<"reader.cpp getSameNumberByRow 越界";
    return -1;
}

int Reader::getBookPosByRow(int row)
{
    if(row>=0&&row<bookBorrowOrReserve.size())
        return bookBorrowOrReserve[row].bookPos;
    qDebug()<<"reader.cpp getBookPosByRow 越界";
    return -1;
}

//取消借阅或者预约
void Reader::deleteBBORItem(int bookPos, int bookSameNumber)
{
    for(int i=0;i<bookBorrowOrReserve.size();i++)
    {
        if(bookBorrowOrReserve[i].bookPos==bookPos&&bookBorrowOrReserve[i].sameNumber==bookSameNumber)
        {
            bookBorrowOrReserve.erase(bookBorrowOrReserve.begin()+i);
            return;
        }
    }
}

//判断某本书是被预约或者被借阅
bool Reader::isBorrowOrReserveThisBook(int bookPos, bool &flag)
{
    for(int i=0;i<bookBorrowOrReserve.size();i++)
    {
        if(bookBorrowOrReserve[i].bookPos==bookPos)
        {
            flag=bookBorrowOrReserve[i].borrowOrReserveFlag;
            return true;
        }
    }
    return false;
}

//将预留书籍变为被借书籍
void Reader::reserveToBorrow(int bookPos, int bookSameNumber)
{
    for(int i=0;i<bookBorrowOrReserve.size();i++)
    {
        if(bookBorrowOrReserve[i].bookPos==bookPos&&bookBorrowOrReserve[i].sameNumber==bookSameNumber)
        {
            bookBorrowOrReserve[i].borrowOrReserveFlag=true;
            bookBorrowOrReserve[i].renewTimes=0;
            bookBorrowOrReserve[i].startDate=mytime.getDate();
        }
    }
}

//更新书籍预留信息
void Reader::updateBookReserveInfo(int bookPos, int sameNumber)
{
    BookBorrowOrReserve rec;
    rec.bookPos=bookPos;
    rec.borrowOrReserveFlag=false;
    rec.renewTimes=0;
    rec.sameNumber=sameNumber;
    rec.startDate=mytime.getDate();
    addBookBorrowOrReserveVector(rec);
}

//清楚书籍借阅或者预约的队列
void Reader::clearBBOR()
{
    bookBorrowOrReserve.clear();
}
