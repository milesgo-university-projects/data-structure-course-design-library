/******************************************************************
文件名: bookborrowedbyreadertablewidget.h
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/21
描　述: 读者借阅以及预约信息
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef BOOKBORROWEDBYREADERTABLEWIDGET//读者可以查看但不能修改
#define BOOKBORROWEDBYREADERTABLEWIDGET

#include "friendtablewidget.h"
#include <QPushButton>
#include <QLabel>
#include "Reader.h"
class BookBorrowOrReserve;//继承图书借阅类
class addReaderBox;//继承读者添加类

class bookBorrowedByReaderWidget:public friendTableWidget
{
    Q_OBJECT
public:
    bookBorrowedByReaderWidget(QWidget *parent=0);
    bookBorrowedByReaderWidget(QVector<BookBorrowOrReserve> recVector,int readerPos,QWidget *parent=0);//定义存储借阅信息的容器
    ~bookBorrowedByReaderWidget();
    friend class addReaderBox;
    void init();
    void updateData(QVector<BookBorrowOrReserve> recVector,int readerPos);//定义更新数据的函数
};

#endif // BOOKBORROWEDBYREADERTABLEWIDGET

