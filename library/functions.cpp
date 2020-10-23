#include <string.h>
#include <QString>
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QMap>
#include "mytime.h"
#include "readerinformationwidget.h"
#include "widget.h"
#include"statisticdata.h"
class readerInformation;
class myTime;
class Widget;
class publisherTableViewWidget;
using namespace std;

//定义初始化变量
QSettings *settings;
myTime mytime;
publisherTableViewWidget *pubViewWidget;
Widget *mainWindow;
statisticsData statisticdata;
bool sameNumberHash[101];                                        //用户记录哪些序号用过了
QMap<QString,int> authorityNameToNumber;
QMap<int,QString> authorityNumberToName;
bool windowHash[30];
QWidget *windowPointer[30];
bool isSimulating;

//加密函数
QString md5(QString str)
{
    QByteArray bb;
    bb = QCryptographicHash::hash (str.toLatin1(), QCryptographicHash::Md5);
    return bb.toHex();
}

//设置风格
void setStyle(const QString &style)
{
        QFile qss(style);
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
}

QDate myToDate(QString str)
{
    string s=str.toStdString();
    int year=0,month=0,day=0;
    int i;
    for(i=0;s[i]!='-';i++)
    {
        year=year*10+(int)(s[i]-'0');
    }
    for(i++;s[i]!='-';i++)
        month=month*10+(int)(s[i]-'0');
    for(i++;s[i]!='\0';i++)
        day=day*10+(int)(s[i]-'0');
    return QDate(year,month,day);
}

//将时间转化为字符串
QString myDateToString(QDate d)
{
    QString tmp;
    tmp.append(QString("%1").arg(d.year()));
    tmp.append("-");
    if(d.month()<10)
        tmp.append("0");
    tmp.append(QString("%1").arg(d.month()));
    tmp.append("-");
    if(d.day()<10)
        tmp.append("0");
    tmp.append(QString("%1").arg(d.day()));
    return tmp;
}

//判断是否是闰年
bool isLeapYear(int year)
{
    if(year%4==0&&year%200!=0)
        return true;
    return false;
}

//判断是否是合法日期
bool isNormalDate(QString str)
{
    QDate date=myToDate(str);
    int year=date.year(),month=date.month(),day=date.day();
    bool flag=true;
    if(month>12||month<1||day>31||day<1)
        flag=false;
    if(flag&&(month==4||month==6||month==9||month==11))
    {
        if(day>30)
            flag=false;
    }
    if(flag&&month==2)
    {
        if(isLeapYear(year))
        {
            if(day>29)
                flag=false;
        }
        else
        {
            if(day>28)
                flag=false;
        }
    }
    return flag;
}

//日期增加
QDate addDate(QDate date,int dayAmounts)
{
    int year=date.year(),month=date.month(),day=date.day();
    for(int i=0;i<dayAmounts;i++)
    {
        if(month==1||month==3||month==5||month==7||month==8||month==10||month==12)
        {
            if(day==31)
            {
                if(month==12)
                {
                    year++;
                    month=1;
                }
                else
                {
                    month++;
                }
                day=1;
            }
            else
            {
                day++;
            }
        }
        else if(month==4||month==6||month==9||month==11)
        {
            if(day==30)
            {
                month++;
                day=1;
            }
            else
            {
                day++;
            }
        }
        else if(month==2)
        {
            if(year%4==0&&year%200!=0)
            {
                if(day==29)
                {
                    month++;
                    day=1;
                }
                else
                {
                    day++;
                }
            }
            else
            {
                if(day==28)
                {
                    month++;
                    day=1;
                }
                else
                {
                    day++;
                }
            }
        }
    }
    date.setDate(year,month,day);
    return date;
}

//比较两个日期，严格小于才返回true
bool cmpDate(QDate a,QDate b)
{
    if(a.year()!=b.year())
        return a.year()<b.year();
    else if(a.month()!=b.month())
        return a.month()<b.month();
    else if(a.day()!=b.day())
        return a.day()<b.day();
    else
        return false;
}

//看两个日期差距多少
int subtractDate(QDate a,QDate b)
{
    int i;
    for(i=1;cmpDate(addDate(a,i),b);i++);
    return i;
}

//添加读者消息
void addReaderInformation(QString readerID,QStringList info)
{
    QList<readerInformation> recordlister;
    QFile file("../dat/readerinformation/"+readerID+".txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))                        //打开文件
    {
        qDebug()<<"open file error";
        return;
    }
    QTextStream stream(&file);                                  //关联输入流
    QString str;
    stream>>str;
    int bound;
    if(str.isEmpty())
        bound=0;
    else
        bound=str.toInt();                                      //读取记录数
    QList<int> informationLineSize;
    QList<int> isRead;
    QStringList dateList;
    for(int i=0;i<bound;i++)                                    //读取信息
    {
        stream>>str;
        informationLineSize.push_back(str.toInt());
        stream>>str;
        isRead.push_back(str.toInt());
        stream>>str;
        dateList.push_back(str);
    }
    stream.readLine();
    for(int i=0;i<bound;i++)                                    //读取其他信息
    {
        readerInformation rec;
        if(isRead[i]==0)
            rec.hasRead=false;
        else
            rec.hasRead=true;
        rec.date=myToDate(dateList[i]);
        for(int j=0;j<informationLineSize[i];j++)
        {
            str=stream.readLine();
            rec.informationDivideByLine<<str;
        }
        recordlister.push_front(rec);
    }

    readerInformation tmpRec;
    tmpRec.hasRead=false;
    tmpRec.informationDivideByLine=info;
    tmpRec.date=mytime.getDate();
    //qDebug()<<recordlister.size()<<"before";
    //for(int i=0;i<info.size();i++)
        //qDebug()<<info[i];
    recordlister.push_front(tmpRec);
    //qDebug()<<recordlister.size()<<"after";
    file.close();

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))                        //打开文件
    {
        qDebug()<<"open file error";
        return;
    }
    QTextStream stream1(&file);                                  //关联输入流

    stream1<<recordlister.size()<<" ";
    for(int i=recordlister.size()-1;i>=0;i--)
    {
        stream1<<recordlister[i].informationDivideByLine.size()<<" ";
        if(recordlister[i].hasRead)
            stream1<<"1 ";
        else
            stream1<<"0 ";
        stream1<<myDateToString(recordlister[i].date)<<" ";
    }
    stream1<<endl;
    for(int i=recordlister.size()-1;i>=0;i--)
    {
        for(int j=0;j<recordlister[i].informationDivideByLine.size();j++)
            stream1<<recordlister[i].informationDivideByLine[j]<<endl;
    }
    file.close();
}
