/******************************************************************
文件名: borroworreserve.h
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/25
描　述: 密码变更
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef CHANGEPASSWORDWIDGET
#define CHANGEPASSWORDWIDGET

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QCloseEvent>
#include <QShowEvent>
#include "searchlineedit.h"

class changePasswordWidget:public QWidget
{
    Q_OBJECT
public:
    changePasswordWidget(QWidget *parent=0);
    ~changePasswordWidget();
    QLabel *readerIdLabel,*currentPasswordLabel,*newPasswordLabel,*newPasswordAgainLabel;//新旧密码标签
    QLineEdit *readerIdLineEdit,*currentPasswordLineEdit,*newPasswordLineEdit,*newPasswordAgainLineEdit;//对应文本框
    QPushButton *acceptButton,*rejectButton;//确认和取消按钮
    QGridLayout *mainLayout;

    int tmpReaderPos;
public slots:
    void submitData();
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // CHANGEPASSWORDWIDGET

