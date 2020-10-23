/******************************************************************
文件名: friendtablewidget.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 添加图书借阅、预约、归还等信息
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef FRIENDTABLEWIDGET
#define FRIENDTABLEWIDGET
#include <QTableWidget>
#include <QListWidget>
#include <QTableView>
#include <QKeyEvent>
#include "addnewbook.h"
class friendTableWidget:public QTableWidget
{
    Q_OBJECT
public:
    friendTableWidget(QWidget *parent=0);
    ~friendTableWidget();
    friend class addBookBox;
    friend class authorityWidget;
    friend class administratorManageWidget;
    void keyPressEvent(QKeyEvent *e);
};

class friendListWidget:public QListWidget
{
    Q_OBJECT
public:
    friendListWidget(QWidget *parent=0);
    ~friendListWidget();
    friend class instituteManageWidget;
};

class friendTableView:public QTableView
{
    Q_OBJECT
public:
    friendTableView(QWidget *parent=0);
    ~friendTableView();
    friend class publisherTableViewWidget;
    friend class bookViewWidget;
    friend class readerViewWidget;
    friend class bookBorrowOrReserveManageWidget;
};

#endif // FRIENDTABLEWIDGET

