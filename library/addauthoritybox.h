/******************************************************************
文件名: addauthoritybox.h
创建人: 顾溢杰
日　期: 2017/4/18
修改人: 杨正浩，姜兴国
日　期: 2017/5/26
描　述: 添加读者权限类型
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef ADDAUTHORITYBOX
#define ADDAUTHORITYBOX
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QDialog>
#include <QPushButton>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QShowEvent>
#include "functions.h"
#include "authority.h"

class addAuthorityBox:public QWidget
{
    Q_OBJECT
public:
    addAuthorityBox(QWidget *parent=0);
    addAuthorityBox(Authority rec,QWidget *parent=0);
    ~addAuthorityBox();//定义相关的标签以及文本框
    QLabel *readerTypeLabel,*authorityNumberLabel,*borrowBookAmountsLimitLabel,*borrowBookDaysLimitLabel,*renewTimesLimitLabel;
    QLineEdit *readerTypeLineEdit,*authorityNumberLineEdit,*borrowBookAmountsLimitLineEdit,*borrowBookDaysLimitLineEdit,*renewTimesLimitLineEdit;
    QGridLayout *gridLayout;
    QPushButton *acceptButton,*rejectButton;
    void init();
    void keyPressEvent(QKeyEvent *e);
    bool canOpen;
    bool isChanging;//是否被修改过
    int originalNumber;
signals:
    void newData(Authority rec);
public slots:
    void submitData();//更新数据
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // ADDAUTHORITYBOX
