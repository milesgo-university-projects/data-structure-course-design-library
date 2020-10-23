/******************************************************************
文件名: publisher.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/21
描　述: 等待队列窗口
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef WAITINGQUEUETABLEWIDGET
#define WAITINGQUEUETABLEWIDGET
#include <QWidget>
#include <QTableWidget>
#include <QQueue>
#include <QCloseEvent>
#include <QShowEvent>
#include <QVBoxLayout>

//等待队列窗口
class waitingQueueTableWidget:public QWidget
{
  Q_OBJECT
public:
    waitingQueueTableWidget(QWidget *parent=0);           //构造函数
    ~waitingQueueTableWidget();
    QTableWidget *tableWidget;                            //界面
    QVBoxLayout *mainLayout;                              //布局
    void updateData(QQueue<int> waitingQueue);            //数据更新
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // WAITINGQUEUETABLEWIDGET

