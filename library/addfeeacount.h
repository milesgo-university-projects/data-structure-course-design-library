/******************************************************************
文件名: addfeeaccount.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/23
描　述: 改变账户余额
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef ADDFEEACOUNT
#define ADDFEEACOUNT
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QCloseEvent>
#include <QShowEvent>
#include "searchlineedit.h"

//改变账户余额
class addFeeAcount:public QWidget
{
    Q_OBJECT
public:
    addFeeAcount(QWidget *parent=0);
    ~addFeeAcount();                            //各种显示用标签
    QLabel *searchReaderLabel,*currentFeeAcountLabel,*addFeeAcountNumberLabel;
    SearchLineEdit *searchReaderLineEdit;       //搜索框
    QLineEdit *currentFeeAcountLineEdit,*addFeeAcountNumberLineEdit;
    QPushButton *acceptButton,*rejectButton;    //按钮
    QGridLayout *mainLayout;                    //布局

    int tmpReaderPos;
public slots:
    void onSearchReaderButtonClicked();          //搜索
    void completeReaderInfo(int readerPos);      //更改读者信息
    void submitData();                           //提交数据
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // ADDFEEACOUNT

