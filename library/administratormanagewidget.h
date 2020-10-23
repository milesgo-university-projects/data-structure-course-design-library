/******************************************************************
文件名: administratormanagewidget.h
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/19
描　述: 添加管理员账户
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef ADMINISTRATORMANAGEWIDGET
#define ADMINISTRATORMANAGEWIDGET

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCloseEvent>
#include <QShowEvent>
#include <QLabel>
#include <QList>
#include "friendtablewidget.h"

struct AdministratorAccount
{
    QString account;
    QString md5Password;
};

class addAdministratorBox:public QWidget
{
    Q_OBJECT
public:
    addAdministratorBox(int administratorPos=-1,bool addOrChange=true,QWidget *parent=0);
    ~addAdministratorBox();
    QLabel *administratorNameLabel,*currentPasswordLabel,*passwordLabel,*passwordAgainLabel;//新建标签
    QLineEdit *administratorNameLineEdit,*currentPasswordLineEdit,*passwordLineEdit,*passwordAgainLineEdit;//对应的文本框
    QPushButton *acceptButton,*rejectButton;        //确认和取消按钮
    QGridLayout *mainLayout;                        //创建布局
    bool isAddOrChange;                             //添加还是修改片段
    int tmpAdministratorPos;
signals:
    void newData(QString,QString);//新建或修改新号
public slots:
     void submitData();//新建或修改具体的槽
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

class administratorManageWidget:public QWidget//界面设计
{
    Q_OBJECT
public:
    administratorManageWidget(QWidget *parent=0);
    ~administratorManageWidget();
    friendTableWidget *tableWidget;
    QPushButton *addButton,*deleteButton,*changeButton;//相应的按钮
    QHBoxLayout *buttonLayout;
    QVBoxLayout *mainLayout;
    QList<AdministratorAccount> recordlister;
    void updateTableWidget();
    bool isAddOrChange;
    int currentRow;
public slots:
    void onAddButtonClicked();//被点击信号接受的槽
    void onDeleteButtonClicked();
    void onChangedButtonClicked();
    void flushFile();
    void addData(QString str1,QString str2);//添加或者修改的函数，传入变量分别为新的姓名和密码
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // ADMINISTRATORMANAGEWIDGET

