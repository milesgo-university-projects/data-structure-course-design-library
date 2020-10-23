/******************************************************************
文件名: changesamebookdistinctionitem.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 修改册信息的对话框 修改该册的出版日期和册序号
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef CHANGESAMEBOOKDISTINCTIONITEM
#define CHANGESAMEBOOKDISTINCTIONITEM
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QShowEvent>
#include <QDateEdit>
#include <QCalendarWidget>
#include "functions.h"

class changeSameBookDistinctionItem:public QWidget
{
    Q_OBJECT
public:
    changeSameBookDistinctionItem(QWidget *parent=0);
    changeSameBookDistinctionItem(QStringList tmpList,QWidget *parent=0);
    ~changeSameBookDistinctionItem();
    QLabel *sameNumberLabel,*publishDateLabel;
    QLineEdit *sameNumberLineEdit;
    QDateEdit *publishDateLineEdit;
    QCalendarWidget *calendar;

    QGridLayout *gridLayout;
    QPushButton *acceptButton,*rejectButton;
    void init();
    void keyPressEvent(QKeyEvent *e);
signals:
    void newData(QStringList);
public slots:
    void submitData();//用于发送上面的newData信号，将所有LineEdit中的信息放在stringList中，然后发送
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};
#endif // CHANGESAMEBOOKDISTINCTIONITEM

