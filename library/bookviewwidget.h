/******************************************************************
文件名: bookviewwidget.h
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/25
描　述: 书籍的添加、更新和删除等管理
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef BOOKVIEWWIDGET_H
#define BOOKVIEWWIDGET_H

#include <QWidget>
#include "bookmodel.h"
#include "addnewbook.h"
#include "comboboxwithdropdown.h"
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
#include <QKeyEvent>
#include <QCloseEvent>
#include <QShowEvent>
#include <QDebug>
#include"ExportExcelObject.h"
#include<QFileDialog>
#include "friendtablewidget.h"
#include "bookborroworreservemanagewidget.h"
#include "statusbar.h"

class BorrowOrReserve;

class bookViewWidget : public QWidget
{
    Q_OBJECT

public:
    bookViewWidget(QWidget *parent = 0);
    ~bookViewWidget();

    QPushButton *addItem,*deleteItem,*search,*showAll,*select,*change,*exportItems;//定义相关按钮
    QPushButton *lookButton,*reserveButton;//定义相关按钮
    friendTableView *tableView;//定义视图
    comboBoxWithDropDown *comboBox;
    QHBoxLayout *upButtonLayout,*downButtonLayout;
    QVBoxLayout *mainLayout;
    QLineEdit *searchEdit;
    tableModel *tablemodel;
    QSortFilterProxyModel *proxymodel;
    mainStatusBar *statusBar;
    QList<class Book> recordlister;

    int columncount;
    QStringList headerName;
    int addOrChangeItem; //0表示添加条目，1表示修改条目,点击修改或添加按钮后修改这个变量
    int currentRow;

    void keyPressEvent(QKeyEvent *e);
    QString getIntroductionContents(QString isbn);
    void saveIntroductionContents(QString info,QString isbn);//保留介绍内容
    bool isBorrowing,isReserving,isReaderLooking;

    QMap<QString,int> classificationToTypeNumberCount;
    void initTypeNumberHash();
    void flushTypeNumberHashFile();

    bool isOpen;
signals:
    void bookInfo(QVector<int>);
    void bookWait(int);
public slots:
    void updateFileData(QModelIndex index);
    void flushFile();
    void isAddItem();
    void isChangeItem();
    void addData(Book booker);
    void deleteData();
    void scrollFocusTo();
    void showAllRows();
    void sendBookInfo();
    void filterChanged(QString str);
    void excelExport();
    void updateBorrowAndReserveInfo(BorrowOrReserve rec);
    void onReserveButtonClicked();
    void onItemClicked();
    void clearAllBookBorrowOrReserveInformation();
    void onDateChanged(int year,int month,int day);
    void clearAllEverBorrowedTimes();
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // BOOKVIEWWIDGET_H
