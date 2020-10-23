/******************************************************************
文件名: functions.h
创建人: 顾溢杰
日　期: 2017/5/15
修改人: 杨正浩，姜兴国
日　期: 2017/5/25
描　述: 全局变量，全局函数
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef FUNCTIONS
#define FUNCTIONS

#define NAME_COLUMN 0
#define LOCATION_COLUMN 1
#define ISBN_COLUMN 2

#include <QSettings>
#include <publishertableviewwidget.h>
#include"statisticdata.h"
class publisherTableViewWidget;
class myTime;

class Widget;
QString md5(QString str);                                            //加密函数
void setStyle(const QString &style);
extern QSettings *settings;                                          //设置作为全局变量
extern myTime mytime;                                                //全局时间，比如可以用mytime.getDate()获取当前日期
extern publisherTableViewWidget *pubViewWidget;
extern Widget *mainWindow;                                           //就是主widget的指针
extern statisticsData statisticdata;                                 //统计数据，需要合并包括以下

extern bool sameNumberHash[101];                                     //用户记录哪些序号用过了
extern QMap<QString,int> authorityNameToNumber;
extern QMap<int,QString> authorityNumberToName;
extern bool windowHash[30];                                          //窗口是否open
extern QWidget *windowPointer[30];                                   //窗口指针
extern bool isSimulating;
extern bool cmpDate(QDate a,QDate b);                                //比较前一个日期是否严格早于后一个日期
extern int subtractDate(QDate a,QDate b);                            //后一个日期比前一个日期多了多少天，调用的时候要保证后一个日期晚于前一个日期
extern void addReaderInformation(QString readerID,QStringList info);
QDate myToDate(QString str);                                         //将固定格式的字符串转化为日期
QString myDateToString(QDate d);                                     //将日期转化为固定格式的字符串
QDate addDate(QDate date,int dayAmounts);                            //在某个日期的基础上加上若干天
bool isNormalDate(QString str);                                      //判断日期是否合法

#endif // FUNCTIONS

//窗口信息管理
//0.widget 其他任何一个窗口关闭前，widget都不能关闭
//1.bookview changesamebookdistinction关闭前，该窗口不能关闭
//2.addnewbook 3必须关闭
//3.changesamebookdistinction 无
//4.readerview
//5.addreader 14必须关闭
//6.publisherview
//7.addpublisher 无
//8.borrowandreserve
//9.addborrowandreserve 无
//10.authorityview
//11.addauthority 12必须关闭（由于其特殊性，已经实现）
//12.institutewidget 无
//13.addFeeAcount 无
//14.changePassword 无
//15.administratormanagewidget 16必须关闭
//16.addadministratorbox
//17.bookvarietywidget
//18.showwaitingqueuetablewidget 2关闭时自动关闭18
//19.readerinformationwidget
//20.selectdatewidget
//21.statisticgraph
