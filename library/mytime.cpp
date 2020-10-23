class myTime;
#include "functions.h"
#include "widget.h"
class Widget;
class LogsManageWidget;

//构造函数
myTime::myTime(QWidget *parent):QObject(parent)
{

}

//设置年份
void myTime::setYear(qint32 y)
{
    year=y;
    emit dateChanged(year,month,day);                      //发送日期被修改的信号
}

//设置月份
void myTime::setMonth(qint32 m)
{
    month=m;
    emit dateChanged(year,month,day);
}

//设置几号
void myTime::setDay(qint32 d)
{
    day=d;
    emit dateChanged(year,month,day);
}

//设置日期
void myTime::setDate(QDate date)
{
    year=date.year();//改了
    month=date.month();
    day=date.day();                                        //设置统计变量的今天的日期
    statisticdata.today.date=getDate().toString("dd.MM.yy");
    emit dateChanged(year,month,day);
}

//返回年份
qint32 myTime::getYear()
{
    return year;
}

//返回月份
qint32 myTime::getMonth()
{
    return month;
}

//返回是几号
qint32 myTime::getDay()
{
    return day;
}

//返回日期
QDate myTime::getDate()
{
    return QDate(year,month,day);
}

//重置时间
void myTime::resetDate()
{
    QDate date(QDate::currentDate());
    year=date.year();
    month=date.month();
    day=date.day();
    emit dateChanged(year,month,day);
}


void myTime::setUpdateTime(qint32 ut)
{
    updateTime=ut;
}

qint32 myTime::getUpdateTime()
{
    return updateTime;
}

void myTime::addDate()
{
    statisticdata.today.date=getDate().toString("dd.MM.yy"); //将前一天的借阅率压进去
    statisticdata.addRate();                                 //将今天的统计率压进去
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
    statisticdata.today.bookCountPerDay=0;                   //借阅书籍数归零
    statisticdata.today.date=getDate().toString("dd.MM.yy"); //改变后今天的日期
    statisticdata.flushStatistic();
    emit dateChanged(year,month,day);
}

//窗体下显示的日期
timeLabel::timeLabel(QWidget *parent):QLabel(parent)
{
    this->setFont(QFont("Microsoft YaHei",10,75));
    connect(&mytime,SIGNAL(dateChanged(qint32,qint32,qint32)),this,SLOT(showDate(qint32,qint32,qint32)));
    setText(QString("%1年%2月%3日").arg(mytime.getYear()).arg(mytime.getMonth()).arg(mytime.getDay()));
}

//析构函数
timeLabel::~timeLabel()
{

}

//显示日期
void timeLabel::showDate(qint32 year, qint32 month, qint32 day)
{
    setText(QString("%1年%2月%3日").arg(year,4).arg(month,2).arg(day,2));
}

//判断是否有鼠标事件，如果正在模拟，鼠标一有动作就停止
void timeLabel::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(mainWindow->timeSimulationWidget->timer->isActive())
        mainWindow->timeSimulationWidget->stopsimulation();
    else
        mainWindow->timeSimulationWidget->startsimulation();
    QLabel::mouseDoubleClickEvent(e);
}
