/******************************************************************
文件名: readerviewwidget.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/25
描　述: 读者信息管理 如添加更新删除等
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef READERVIEWWIDGET_H
#define READERVIEWWIDGET_H

#include <QWidget>
#include "readermodel.h"
#include"addreaderbox.h"
#include "comboboxwithdropdown.h"
#include "functions.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QComboBox>
#include <QPoint>
#include <QScrollBar>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QShowEvent>
#include"ExportExcelObject.h"
#include<QFileDialog>
#include "friendtablewidget.h"
#include "borroworreserve.h"
#include "statusbar.h"

class BorrowOrReserve;

class readerViewWidget:public QWidget
{
public:
    Q_OBJECT

public:
    readerViewWidget(QWidget *parent = 0);
    ~readerViewWidget();

    QPushButton *addItem,*deleteItem,*search,*showAll,*select,*change,*exportItems;//定义界面按钮
    friendTableView *tableView;
    comboBoxWithDropDown *comboBox;//进行界面优化 定义下拉复选框
    QHBoxLayout *upButtonLayout,*downButtonLayout;//定义布局
    QVBoxLayout *mainLayout;
    QLineEdit *searchEdit;//寻找 文本框
    readerModel *tablemodel;
    QSortFilterProxyModel *proxymodel;
    mainStatusBar *statusBar;
    QList<class Reader> recordlister;

    int columncount;
    QStringList headerName;
    int addOrChangeItem; //0表示添加条目，1表示修改条目,点击修改或添加按钮后修改这个变量
    int currentRow;  //暂存当前操作行的下标
    void keyPressEvent(QKeyEvent *e);

    bool isOpen,isAddFeeAcount,isBorrowingOrReserving,isChangePassword;
signals:
    void readerInfo(int);
public slots:
    void updateFileData(QModelIndex index);
    void flushFile();
    void isAddItem();
    void isChangeItem();
    void addData(QStringList strList);
    void deleteData();
    void scrollFocusTo();
    void showAllRows();
    void sendReaderInfo();
    void filterChanged(QString str);
    void excelExport();
    void updateBorrowAndReserveInfo(BorrowOrReserve rec);
    void onDateChanged(int year,int month,int day);
    void clearAllBookBorrowOrReserveInformation();
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // READERVIEWWIDGET_H
