/******************************************************************
文件名: addnewbook.h
创建人: 顾溢杰
日　期: 2017/4/19
修改人: 杨正浩，姜兴国
日　期: 2017/5/23
描　述: 添加一本新书
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef ADDNEWBOOK
#define ADDNEWBOOK

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialog>
#include <QPushButton>
#include <QTableWidget>
#include <QFile>
#include <QTextStream>
#include <QKeyEvent>
#include <QModelIndex>
#include <algorithm>
#include <QCloseEvent>
#include <QShowEvent>
#include "searchlineedit.h"
#include "functions.h"
#include "twosizebutton.h"
#include "changesamebookdistinctionitem.h"
#include "friendtablewidget.h"
#include "publisher.h"
#include "book.h"
#include "statusbar.h"

class Book;
struct Publisher;
class friendTableWidget;
struct SameBookDistinction;

//添加一本新书
class addBookBox:public QWidget
{
    Q_OBJECT
public:
    addBookBox(QWidget *parent=0);
    addBookBox(Book rec,QWidget *parent=0);
    ~addBookBox();
                                                                                          //各种label
    QLabel *nameLabel,*authorNameLabel,*ISBNLabel,*publisherNameLabel,*classificationNumberLabel,*classificationNameLabel,*typeNumberLabel,*priceLabel,*introductionLabel;
    QLineEdit *nameLineEdit,*authorNameLineEdit,*ISBNLineEdit,*priceLineEdit,*classificationNameLineEdit;
    QLineEdit *typeNumberLineEdit;                                                        //label对应的输入框
    QTextEdit *introductionEdit;
    SearchLineEdit *publisherLineEdit,*classificationNumberLineEdit;
    friendTableWidget *sameBookDistinctionTableWidget;
    mainStatusBar *statusBar;

    QGridLayout *gridLayout;                                                              //布局
    QHBoxLayout *mainLayout,*buttonLayout;
    QVBoxLayout *rightLayout,*trueMainLayout;                                             //下面为各种按钮
    QPushButton *acceptButton,*rejectButton,*addButton,*changeButton,*deleteButton,*showWaitingQueueButton;
    twoSizeButton *addNewSameBookButton;
    QVector<SameBookDistinction> sbdVector;
    QString classificationType;                                                           //分类种次号
    QQueue<int> waitingQueue;                                                             //等待队列
    int currentRow;
    void init();                                                                          //初始化
    void keyPressEvent(QKeyEvent *e);
    int addOrChangeItem;                                                                  //0表示添加，1表示修改
    bool canOpen;                                                                         //判断是否打开
    bool isChanging;
    QString originalClassificationNumber;
    int originalTypeNumber;
signals:
    void newData(Book);                                                                   //添加了一本新书
public slots:
    void submitData();                                                                    //用于发送上面的newData信号，将所有LineEdit中的信息放在stringList中，然后发送
    void updatePublisherName(Publisher pub);
    void addNewSameBook();
    void updateSameBookDistinctionTableWidget();
    void isChangeItem();
    void isAddItem();
    void addData(QStringList strList);                                                    //增加数据
    void deleteData();                                                                    //删除数据
    void sortByOrder();
    void onClassificationNumberLineEditCurrentTextChanged(QString str);
    void changeClassificationInfo(QString classificationNumber,QString classificationName);
    void changeClassificationNameByClassificationNumber(QString classificationNumber);
    void onShowWintingQueueButtonClicked();
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // ADDNEWBOOK

