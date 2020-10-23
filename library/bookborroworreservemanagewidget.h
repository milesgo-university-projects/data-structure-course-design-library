/******************************************************************
文件名: bookborroworreservemanagewidget.h
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/21
描　述: 管理员对借阅以及预约信息的管理
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef BOOKBORROWORRESERVEMANAGEWIDGET
#define BOOKBORROWORRESERVEMANAGEWIDGET
#include <QWidget>
#include <QCloseEvent>
#include <QShowEvent>
#include <QPushButton>
#include "friendtablewidget.h"
#include "bookborrowreservemodel.h"
#include "borroworreserve.h"
#include "statusbar.h"
class BorrowOrReserve;//继承图书借阅以及预约的类
class bookBorrowOrReserveManageWidget:public QWidget
{
    Q_OBJECT
public:
    bookBorrowOrReserveManageWidget(QWidget *parent=0);
    ~bookBorrowOrReserveManageWidget();

    QPushButton *addItem,*deleteItem,*search,*showAll,*change,*exportItems,*renew,*reserveToBorrow;//构件界面所用到的部分
    friendTableView *tableView;//表格
    comboBoxWithDropDown *searchComboBox,*borrowOrReserveComboBox;
    QHBoxLayout *upButtonLayout,*downButtonLayout;//水平布局
    QVBoxLayout *mainLayout;//竖直布局
    QLineEdit *searchEdit;//搜索文本框
    bookBorrowOrReserveTableModel *tablemodel;
    QSortFilterProxyModel *proxymodel;
    QList<class BorrowOrReserve> recordlister;//当前借阅信息的数组
    mainStatusBar *statusBar;

    int columncount;
    QStringList headerName;
    int addOrChangeItem; //0表示添加条目，1表示修改条目,点击修改或添加按钮后修改这个变量
    int currentRow;

    void keyPressEvent(QKeyEvent *e);

    bool isOpen;
    int searchBorrowAndReserve(int bookPos,int bookSameNumber);//返回在recordlister中的下标
    void checkCanReserveTo(BorrowOrReserve rec);
signals:
    void newBorrowAndReserve(BorrowOrReserve);
public slots:
    void updateFileData(QModelIndex index);
    void flushFile();
    void isAddItem();
    void isChangeItem();
    void addData(BorrowOrReserve rec);
    void deleteData();
    void deleteData(int bookPos,int bookSameNumber);
    void deleteData(int row);
    void scrollFocusTo();
    void showAllRows();//遍历输出所有的行
    void filterChanged(QString str);
    void excelExport();
    void onBorrowOrReserveComboBoxCurrentTextChanged(QString str);
    void updateBookAndReaderView(int row);
    void updateButtonEnabledSituation();
    void updateRenewTimes();
    void onDateChanged(int year,int month,int day);//时间变化
    void addRenewTimes(int bookPos,int typeNumber,int readerPos);
    void onReserveOrBorrowButtonClicked();
    void clearAllBookBorrowOrReserveInformation();
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // BOOKBORROWORRESERVEMANAGEWIDGET

