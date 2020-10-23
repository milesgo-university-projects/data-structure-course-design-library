/******************************************************************
文件名: publishertableviewwidget.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 出版社信息显示以及相应的管理
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef TABLEVIEW
#define TABLEVIEW
#include <QTableView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QComboBox>
#include <QPoint>
#include <QScrollBar>
#include <QHeaderView>
#include <QRegExp>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QShowEvent>
#include "publishertablemodel.h"
#include "addpublisherbox.h"
#include"ExportExcelObject.h"
#include "comboboxwithdropdown.h"
#include<QFileDialog>
#include "functions.h"
#include "friendtablewidget.h"
#include "statusbar.h"
class publisherTableModel;
struct Publisher;
class friendTableView;

class publisherTableViewWidget:public QWidget
{
    Q_OBJECT
public:
    publisherTableViewWidget(QWidget *parent=0);
    ~publisherTableViewWidget();

    QPushButton *addItemButton,*deleteItem,*search,*showAll,*select,*change,*exportExcel;//定义按钮
    friendTableView *tableView;
    comboBoxWithDropDown *comboBox;
    QHBoxLayout *upButtonLayout,*downButtonLayout;//定义布局
    QVBoxLayout *mainLayout;
    QLineEdit *searchEdit;
    publisherTableModel *tablemodel;
    QSortFilterProxyModel *proxymodel;
    mainStatusBar *statusBar;
    QList<Publisher> recordlister;

    int columncount;
    QStringList headerName;

    int addOrChangeItem;//0表示添加条目，1表示修改条目,点击修改或添加按钮后修改这个变量
    int currentRow;

    void keyPressEvent(QKeyEvent *e);

    bool isOpen;
signals:
    void publishInfo(Publisher);
public slots:
    void updateFileData(QModelIndex index);//实时显示
    void flushFile();//更新文件
    void isAddItem();//添加出版社
    void isChangeItem();//改变信息
    void addData(QString a,QString b,QString c);//添加或更新数据
    void deleteData();//删除数据
    void scrollFocusTo();
    void showAllRows();
    void sendPublishInfo();
    void filterChanged(QString str);
    void excelExport();//统计
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};
#endif // TABLEVIEW

