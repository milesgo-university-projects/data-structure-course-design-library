/******************************************************************
文件名: addbookborroworreservewidget.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 添加图书借阅、预约、归还等信息
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef ADDBOOKBORROWORRESERVEWIDGET
#define ADDBOOKBORROWORRESERVEWIDGET
#include <QWidget>
#include <QCloseEvent>
#include <QShowEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "borroworreserve.h"
#include "searchlineedit.h"
#include "bookborrowedbyreadertablewidget.h"
#include "comboboxwithdropdown.h"
#include "statusbar.h"

class addBookBorrowOrReserveWidget:public QWidget
{
    Q_OBJECT
public:
    addBookBorrowOrReserveWidget(QWidget *parent=0);
    addBookBorrowOrReserveWidget(BorrowOrReserve rec,QWidget *parent=0);
    ~addBookBorrowOrReserveWidget();
    //管理员先搜读者，再搜书
    QLabel *searchReaderLabel,*searchBookLabel;                                       //创建寻找读者和寻找书籍的标签
    QLabel *bookAlreadyBorrowedNumberLabel,*bookAvailableBorrowedNumberLabel;         //有效借阅序列标签
    QLineEdit *bookAlreadyBorrowedNumberLineEdit,*bookAvailableBorrowedNumberLineEdit;//有效借阅序列输入文本框
    QLabel *readerNameLabel;                                                          //读者姓名序列
    QLineEdit *readerNameLineEdit;                                                    //与上一项匹配，对应输入
    QLabel *bookClassificationTypeNumberLabel;                                        //书籍标识显示
    QLineEdit *bookClassificationTypeNumberLineEdit;                                  //书籍识别符输入
    SearchLineEdit *searchReaderLineEdit,*searchBookLineEdit;                         //检索读者和图书
    bookBorrowedByReaderWidget *bookBorrowedByThisReader;                             //创建一个负责显示以及管理借阅信息的类
    QHBoxLayout *searchBookLayout,*searchReaderLayout,*readerInfoLayout,*bookInfoLayout,*buttonLayout;//创建水平布局
    QVBoxLayout *mainLayout;                                                          //垂直创建布局
    comboBoxWithDropDown *addBorrowOrReserveComboBox;
    mainStatusBar *statusBar;
    int tmpReaderPos,tmpBookPos,tmpSameNumber;//暂时存储当前被操作的读者和图书的下标
    QPushButton *acceptButton,*rejectButton;//创建按钮
    void init();
    bool canOpen;
    bool isLooking;
signals:
    void newData(BorrowOrReserve);//新建借阅或者预约信息，向其中添加数据或者修改某行值
    void searchBookInfo(QString);//寻找图书信息
    void searchReaderInfo(QString);//寻找读者信息
protected:
    void closeEvent(QCloseEvent *e);//在关闭当前界面需要完成的工作
    void showEvent(QShowEvent *e);//显示界面
public slots:
    void completeBookInfo(QVector<int> tmpVector);//构建完整的图书借阅信息
    void completeBookInfo();
    void completeReaderInfo(int readerPos);
    void onAddBorrowOrReserveComboBoxCurrentTextChanged(QString str);
    void onSearchBookButtonClicked();//寻找图书，弹出对话框，显示当前已有的图书
    void onSearchReaderButtonClicked();//寻找读者，弹出对话框，显示当前的读者信息
    void submitData();//加入相应的数据
    void onBookWaitSignal(int bookPos);//加入等待队列
};

#endif // ADDBOOKBORROWORRESERVEWIDGET

