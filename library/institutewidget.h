/******************************************************************
文件名: institutewidget.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 学院信息的显示与管理
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef INSTITUTEWIDGET
#define INSTITUTEWIDGET
#include <QWidget>
#include <QPushButton>
#include <QCloseEvent>
#include <QShowEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QModelIndex>
#include <QModelIndexList>

#include "friendtablewidget.h"
#include "functions.h"

class friendListWidget;
class instituteManageWidget:public QWidget
{
    Q_OBJECT
public:
    instituteManageWidget(QWidget *parent=0);
    ~instituteManageWidget();
    friendListWidget *instituteNameListWidget;
    QPushButton *acceptButton,*rejectButton,*deleteButton,*addButton,*changeButton;
    QPushButton *upButton,*downButton;
    QVBoxLayout *mainLayout;
    QHBoxLayout *acceptRejectButtonLayout,*upDownButtonLayout;
    QVector<QString> instituteNames;
    void readFile();
    void flushFile();
    void updateListWidget();

    bool isOpen;
public slots:
    void isAddItem();
    void isChangeItem();
    void deleteItem();
    void moveUp();
    void moveDown();
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // INSTITUEWIDGET
