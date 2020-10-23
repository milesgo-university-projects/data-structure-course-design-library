#include "statisticdata.h"
#include<QFile>
#include<QTextStream>
#include<QDebug>

//构造函数，还需要增加网文件里的读操作
statisticsData::statisticsData()
{
    
    QFile file("../dat/bookcount.txt");                          //打开对应的储存信息的文件
    QFile file1("../statistic/bookRentTimes.txt");
    QFile file2("../statistic/ReaderBorrowTimes.txt");
    QFile file3("../statistic/borrowRate.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error statisticdata.cpp 2";
    if(!file1.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error statisticdata.cpp 2";
    if(!file2.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error statisticdata.cpp 2";
    if(!file3.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error statisticdata.cpp 2";
    QTextStream in(&file);                                        //绑定输入流
    QTextStream in1(&file1);
    QTextStream in2(&file2);
    QTextStream in3(&file3);
    in>>this->today.bookNums;                                     //读取读书总数
    while(!in1.atEnd())
    {//将文件中的信息读到容器中
        booktemp temp;int nums;                                   //临时对象，存取部分书籍信息
        QString str;
        in1>>temp.name;
        in1>>temp.id;
        in1>>str;
        nums=str.toInt();
        bookRecord[temp]=nums;
    }
    while(!in2.atEnd())
    {//将文件中的信息读到容器中
        person temp;int nums;                                     //临时对象，存取部分读者信息
        QString str;
        in2>>temp.name;
        in2>>temp.id;
        in2>>str;
        nums=str.toInt();
        readerRecord[temp]=nums;
    }
    while(!in3.atEnd())                                           //读取历史统计信息（统计率）
    {//将文件中的信息读到容器中
        bookRentRate temp;
        QString str;
        in3>>temp.date;
        in3>>str;
        temp.bookCountPerDay=str.toInt();
        in3>>str;
        temp.bookNums=str.toInt();
        allRates.push_back(temp);
    }
    if(allRates.size())                                           //只有一条信息，则为今天的信息
    {
        today=allRates[allRates.size()-1];
        allRates.pop_back();
    }
    if(!allRates.size())                                          //文件为空
    {
        today.date=mytime.getDate().toString("dd.MM.yy");
        today.bookCountPerDay=0;
    }
    file.close();                                                 //关闭文件
    file1.close();
    file2.close();
    file3.close();
}

//析构函数，处理一部分数据保存的工作,保存图书信息
statisticsData::~statisticsData()
{
    flushStatistic();                                             //析构前先刷新保存一次
}

//重置日期时，所有统计信息也清除
void statisticsData::clear()
{
    this->bookRecord.clear();
    this->readerRecord.clear();
    this->allRates.clear();
    today.bookCountPerDay=0;
    QDir dir("../statistic");                          //清除所有信息，包括文件里的
    dir.setFilter(QDir::Files);
    int fileCount = dir.count();
    for (int i = 0; i < fileCount; i++)
        dir.remove(dir[i]);
}

//每次修改就刷新文件,一个一个的增加
void statisticsData::flushStatistic()
{
   allRates.push_back(today);                          //临时把今天的借阅统计信息放进去
   QFile file("../statistic/bookRentTimes.txt");       //绑定文件
   QFile file1("../statistic/ReaderBorrowTimes.txt");
   QFile file2("../statistic/borrowRate.txt");
   if(!file.open(QIODevice::WriteOnly|QIODevice::Text))//打开文件操作
       qDebug()<<"open file error statisticdata.cpp 2";
   if(!file1.open(QIODevice::WriteOnly|QIODevice::Text))
       qDebug()<<"open file error statisticdata.cpp 2";
   if(!file2.open(QIODevice::WriteOnly|QIODevice::Text))
       qDebug()<<"open file error statisticdata.cpp 2";
   QTextStream out(&file);                             //绑定输出流
   QTextStream out1(&file1);
   QTextStream out2(&file2);
   QMap<booktemp,int>::iterator i;QMap<person,int>::iterator i1;
   int j;                                              //保存所有书籍信息
   for(i=bookRecord.begin(),j=0;j<bookRecord.count()-1;i++,j++)
       out<<i.key().name<<" "<<i.key().id<<" "<<QString::number(i.value(),10)<<endl;
   if(bookRecord.count())
       out<<i.key().name<<" "<<i.key().id<<" "<<QString::number(i.value(),10);
   for(i1=readerRecord.begin(),j=0;j<readerRecord.count()-1;i1++,j++)
       out1<<i1.key().name<<" "<<i1.key().id<<" "<<QString::number(i1.value(),10)<<endl;
   if(readerRecord.count())                            //保存所有统计信息
       out1<<i1.key().name<<" "<<i1.key().id<<" "<<QString::number(i1.value(),10);
   if(today.date!="")
   {
       for(j=0;j<allRates.size()-1;j++)
          out2<<allRates[j].date<<" "<<QString::number(allRates[j].bookCountPerDay,10)<<" "<<QString::number(allRates[j].bookNums,10)<<endl;
       if(allRates.size())
          out2<<allRates[j].date<<" "<<QString::number(allRates[j].bookCountPerDay,10)<<" "<<QString::number(allRates[j].bookNums,10);
   }
   file.close();                                        //保存文件
   file1.close();
   file2.close();
   if(allRates.size())
        allRates.pop_back();                            //把今天的统计日期弹出
}

//修改所有的借阅率
void statisticsData::addRate()
{
    allRates.push_back(today);                          //时间每过一天就把前一天的信息记录进去
}

//修改总的书籍统计信息
void statisticsData::statisticAddBookTotal(QString bookname,QString bookid)
{
     booktemp temp;
     temp.id=bookid;
     temp.name=bookname;
     if(bookRecord.contains(temp))                      //判断此书籍是否存在于记录中
           bookRecord[temp]++;
     else
     {
         bookRecord[temp]=1;                            //不存在就记录进去
     }
}

//建立一个hash表记录被借书籍情况
void statisticsData::statisticAddReaderTotal(QString readername, QString readerid)
{
    person temp;
    temp.id=readerid;
    temp.name=readername;
    if(readerRecord.contains(temp))                      //原理同上，只是这里统计的是读者
        readerRecord[temp]++;
    else
        readerRecord[temp]=1;
}

//返回借阅率
const QVector<bookRentRate> statisticsData::getRate()
{
    QVector<bookRentRate> temp;
    if(allRates.size()>=6)                               //容器的记录大于6，返回最近七天的统计率
    {
        for(int i=allRates.size()-6;i<allRates.size();i++)
            temp.push_back(allRates[i]);
        temp.push_back(today);

    }
    else
    {
        for(int i=0;i<allRates.size();i++)              //否则，全部返回
            temp.push_back(allRates[i]);
        temp.push_back(today);
    }
    return temp;
}

//比较函数，用于书籍和读者的排序函数
bool bookCompare(QPair<QString,int>a ,QPair<QString,int> b)
{
    return a.second<b.second||(a.second==b.second&&a.first<b.first);
}

//返回top10被借的书
const QVector<QPair<QString,int> > statisticsData::getToptenBook()
{
    QVector<QPair<QString,int> >a,b;
    QMap<booktemp,int>::iterator it;                   //将hash表中的数据转存到容器中，便于排序
    for(it=bookRecord.begin();it!=bookRecord.end();it++)
    {
        a.push_back(QPair<QString,int>(it.key().name,it.value()));
    }
    std::sort(a.begin(),a.end(),bookCompare);          //排序
    for(int i=a.size()-1;i>=0&&a.size()-i<=9;i--)
        b.push_back(a[i]);
    return b;
}

//返回top10借书的人
const QVector<QPair<QString,int> > statisticsData::getToptenReader()
{
    QVector<QPair<QString,int> >a,b;
    QMap<person,int>::iterator it;                     //原理同上
    for(it=readerRecord.begin();it!=readerRecord.end();it++)
    {
        a.push_back(QPair<QString,int>(it.key().name,it.value()));
    }
    std::sort(a.begin(),a.end(),bookCompare);
    for(int i=a.size()-1;i>=0&&a.size()-i<=9;i--)
        b.push_back(a[i]);
    return b;
}

