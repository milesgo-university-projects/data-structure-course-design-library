/******************************************************************
文件名: authoritywidget.h
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/19
描　述: 新建、更新或删除权限管理
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef AUTHORITYWIDGET
#define AUTHORITYWIDGET

#include <QWidget>
#include "authority.h"
#include "addauthoritybox.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QModelIndex>
#include <QScrollBar>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QShowEvent>
#include <QDebug>
#include "friendtablewidget.h"
#include "addauthoritybox.h"
#include "algorithm"

class addAuthorityBox;//继承addAuthorityBox类

class authorityWidget : public QWidget
{
    Q_OBJECT

public:
    authorityWidget(QWidget *parent = 0);
    ~authorityWidget();

    QPushButton *addItem,*deleteItem,*change,*exportItems;//定义按钮
    friendTableWidget *tableWidget;//继承关系
    QHBoxLayout *downButtonLayout;//竖直布局
    QVBoxLayout *mainLayout;//水平布局
    QList<class Authority> recordlister;//定义权限在内存中的清单，类似数组

    int columncount;
    QStringList headerName;
    int addOrChangeItem; //0表示添加条目，1表示修改条目,点击修改或添加按钮后修改这个变量
    int currentRow;

    void keyPressEvent(QKeyEvent *e);
    int findAuthPos(int auth);

    bool isOpen;
    bool authorityNumberHash[101];
    void sortByAuthorityNumber();
signals:
    void authorityInfo(Authority p);
public slots:
    void updateTableWidget();
    void flushFile();//更新文件
    void isAddItem();//添加内容
    void isChangeItem();//更改内容
    void addData(Authority rec);//添加或更新数据
    void deleteData();//删除数据
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // AUTHORITYWIDGET
