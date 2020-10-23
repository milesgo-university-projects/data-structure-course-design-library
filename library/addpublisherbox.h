/******************************************************************
文件名: addpublisherbox.h
创建人: 顾溢杰
日　期: 2017/4/8
修改人: 杨正浩，姜兴国
日　期: 2017/5/21
描　述: 添加出版社以及相关的管理操作
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef ADDPUBLISHERBOX
#define ADDPUBLISHERBOX
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
#include "publisher.h"

struct Publisher;//定义出版社结构体
class addPublisherBox:public QWidget
{
    Q_OBJECT
public:
    addPublisherBox(QWidget *parent=0);
    addPublisherBox(Publisher pub,QWidget *parent=0);
    ~addPublisherBox();
    QLabel *nameLabel,*locationLabel,*ISBNLabel;                  //图出版社涉及三个变量：名称、位置、ISBN
    QLineEdit *nameLineEdit,*locationLineEdit,*ISBNLineEdit;      //对应的文本输入框
    QGridLayout *gridLayout;                                      //构建网格布局
    QPushButton *acceptButton,*rejectButton;
    void init();
    void keyPressEvent(QKeyEvent *e);

    bool canOpen;
signals:
    void newData(QString,QString,QString);                        //新建信号
public slots:
    void submitData();                                            //新建槽
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // ADDPUBLISHERBOX

