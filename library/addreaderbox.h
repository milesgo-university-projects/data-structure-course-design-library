/******************************************************************
文件名: addreaderbox.h
创建人: 杨正浩
日　期: 2017/4/25
修改人: 顾溢杰
日　期: 2017/5/25
描　述: 添加读者
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef ADDREADERBOX_H
#define ADDREADERBOX_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QPushButton>
#include <QTableWidget>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QShowEvent>
#include "functions.h"
#include "comboboxwithdropdown.h"
#include "Reader.h"
#include "bookborrowedbyreadertablewidget.h"

class Reader;
class addReaderBox:public QWidget
{
    Q_OBJECT
public:
    addReaderBox(QWidget *parent=0);
    addReaderBox(Reader rec,int readerPos,QWidget *parent=0);
    addReaderBox(bool flag,Reader rec,int readerPos,QWidget *parent=0);
    ~addReaderBox();
                                                               //以下为各种输入框，标签，选择框
    QLabel *nameLabel,*IdLabel,*GenderLabel,*AuthLabel,*FeeAccountLable,*SchoolLabel,*overDueTimeLabel;
    QLineEdit *nameLineEdit,*IdLineEdit,*FeeAccountLineEdit,*overDueTimeLineEdit;
    bookBorrowedByReaderWidget*bookBorrowedByReaderTableWidget;
    comboBoxWithDropDown *AuthLineComboBox,*genderLineComboBox,*schoolLineComboBox;

    QGridLayout *gridLayout;                                   //以下为各种布局
    QHBoxLayout *mainLayout;
    QHBoxLayout *rightButtonLayout;
    QVBoxLayout *rightLayout;

    QPushButton *acceptButton,*rejectButton;                   //以下为各种布局
    QPushButton *cancelReserveButton,*renewButton;
    void init();
    void keyPressEvent(QKeyEvent *e);

    bool canOpen;                                              //判断是否打开
    int tmpReaderPos;
    QVector<BookBorrowOrReserve> tmpBBORVector;
signals:
    void newData(QStringList);
public slots:
    void submitData();                                         //用于发送上面的newData信号，将所有LineEdit中的信息放在stringList中，然后发送
    void changePassword();                                     //各种信号
    void onTableWidgetItemClicked();
    void onRenewButtonClicked();
    void onCancelReserveButtonClicked();
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // ADDREADERBOX_H
