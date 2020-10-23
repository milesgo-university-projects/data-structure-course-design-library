/******************************************************************
文件名: mytime.h
创建人: 顾溢杰
日　期: 2017/4/3
修改人: 杨正浩
日　期: 2017/5/22
描　述: 自定义的时间的类
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef MYTIME
#define MYTIME
#include <QLabel>
#include <QTimer>
#include <QDate>
#include <QMouseEvent>

//自定义的关于时间的类
class myTime:public QObject
{
Q_OBJECT
public:
    myTime(QWidget *parent=0);
    void resetDate();                                 //重置日期为当前日期
    void setYear(qint32 y);
    void setMonth(qint32 m);
    void setDay(qint32 d);
    void setUpdateTime(qint32 ut);
    qint32 getYear();                                 //返回年份
    qint32 getMonth();                                //返回月份
    qint32 getDay();                                  //返回天数
    QDate getDate();                                  //返回日期
    qint32 getUpdateTime();
signals:                                              //日期改变的信号
    void dateChanged(const qint32 &year,const qint32 &month,const qint32 &day);
public slots:
    void addDate();                                   //日期增加一天
    void setDate(QDate date);                         //设定日期
private:
    qint32 year,month,day;                            //年日月
    qint32 updateTime;
};

class timeLabel:public QLabel
{
    Q_OBJECT
public:
    timeLabel(QWidget *parent=0);
    ~timeLabel();
    void mouseDoubleClickEvent(QMouseEvent *e);
public slots:
    void showDate(qint32 year,qint32 month,qint32 day);
};
#endif // MYTIME

