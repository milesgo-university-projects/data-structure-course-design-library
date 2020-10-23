/******************************************************************
文件名: maintabwidget.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 程序主要显示的图标定义
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef MAINTABWIDGET
#define MAINTABWIDGET

#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <timesimulation.h>
#include <QWidget>
#include "widget.h"

class Widget;
class bookManageButtonsWidget;
class readerManageButtonsWidget;
class MainTabWidget;
class bookBorrowOrReserveManageButtonsWidget;

class bookBorrowOrReserveManageButtonsWidget:public QWidget
{
    Q_OBJECT
public:
    bookBorrowOrReserveManageButtonsWidget(QWidget *parent=0);
    ~bookBorrowOrReserveManageButtonsWidget();
    QPushButton *bookBorrowOrReserveManageButton,*addBookBorrowOrReserveButton;
    QVBoxLayout *vLayout;
};

class readerManageButtonsWidget:public QWidget
{
    Q_OBJECT
public:
    readerManageButtonsWidget(QWidget *parent=0);
    ~readerManageButtonsWidget();
    QPushButton *readerManageButton,*addReaderButton,*readerTypeManageButton,*addReaderTypeButton,*addFeeAcountButton;
    QVBoxLayout *vLayout;
};

class bookManageButtonsWidget:public QWidget
{
    Q_OBJECT
public:
    bookManageButtonsWidget(QWidget *parent=0);
    ~bookManageButtonsWidget();
    QPushButton *bookManageButton,*addBookButton,*varietyManageButton;
    QVBoxLayout *vLayout;
};
//统计模块
class statisticWidget:public QWidget
{
     Q_OBJECT
public:
     statisticWidget(QWidget *parent=0);
     ~ statisticWidget();
     QPushButton *bookRentSevenday,*bookRentTopTen,*bookRentPerMonth;
     QVBoxLayout *vLayout;
};
class MainTabWidget:public QTabWidget
{
    Q_OBJECT
public:
    MainTabWidget(Widget *parent=0);
    ~MainTabWidget();

    bookManageButtonsWidget *bookManageWidget;
    readerManageButtonsWidget *readerManageWidget;
    bookBorrowOrReserveManageButtonsWidget *bookBORManageWidget;
    statisticWidget *statistic;
    QLabel *label4;
};

#endif // MAINTABWIDGET

