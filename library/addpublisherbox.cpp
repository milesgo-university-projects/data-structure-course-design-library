/******************************************************************
文件名: addpublisherbox.cpp
创建人: 顾溢杰
日　期: 2017/4/8
修改人: 杨正浩，姜兴国
日　期: 2017/5/21
描　述: 添加出版社以及相关的管理操作
版　本: v5.00 Copyright(c).
******************************************************************/
#include "addpublisherbox.h"
#include "publishertableviewwidget.h"
#include "widget.h"
#include <QDebug>
void addPublisherBox::init()                              //新建界面操作 生成布局和按钮
{
    this->setWindowTitle("添加出版社");
    canOpen=false;
    QRegExp regx;
    QValidator *validator;
    this->setObjectName("outsider");
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    nameLabel = new QLabel(this);
    locationLabel = new QLabel(this);
    ISBNLabel = new QLabel(this);
    nameLineEdit = new QLineEdit(this);
    locationLineEdit = new QLineEdit(this);
    ISBNLineEdit = new QLineEdit(this);
    regx.setPattern("[0-9]+$");
    validator = new QRegExpValidator(regx,ISBNLineEdit);
    ISBNLineEdit->setValidator(validator);
    gridLayout = new QGridLayout;
    acceptButton = new QPushButton(this);
    rejectButton = new QPushButton(this);

    nameLabel->setText(tr("*名称"));
    locationLabel->setText(tr("*地点"));
    ISBNLabel->setText(tr("*ISBN"));
    acceptButton->setText(tr("确认"));
    rejectButton->setText(tr("取消"));

    gridLayout->addWidget(nameLabel,0,0);                    //设计网格布局
    gridLayout->addWidget(nameLineEdit,0,1);
    gridLayout->addWidget(locationLabel,1,0);
    gridLayout->addWidget(locationLineEdit,1,1);
    gridLayout->addWidget(ISBNLabel,2,0);
    gridLayout->addWidget(ISBNLineEdit,2,1);
    gridLayout->addWidget(acceptButton,3,0);
    gridLayout->addWidget(rejectButton,3,1);
    setLayout(gridLayout);

    connect(acceptButton,SIGNAL(clicked(bool)),this,SLOT(submitData()));//点击新建按钮
    connect(rejectButton,SIGNAL(clicked(bool)),this,SLOT(close()));//点击关闭按钮
}

addPublisherBox::addPublisherBox(QWidget *parent):QWidget(parent)//点击进到这个界面就调用init函数
{
    init();
}

addPublisherBox::addPublisherBox(Publisher pub, QWidget *parent):QWidget(parent)
{
    init();
    nameLineEdit->setText(pub.name);
    locationLineEdit->setText(pub.location);
    ISBNLineEdit->setText(pub.ISBN);
}

addPublisherBox::~addPublisherBox()//按顺序释放指针
{
    delete nameLabel;
    delete nameLineEdit;
    delete locationLabel;
    delete locationLineEdit;
    delete ISBNLabel;
    delete ISBNLineEdit;
    delete gridLayout;
    delete acceptButton;
    delete rejectButton;
}

void addPublisherBox::submitData()//更新布局
{
    QString str1,str2,str3;
    str1=nameLineEdit->text().trimmed();
    if(str1.isEmpty())
    {
        nameLineEdit->setFocus();
        return;
    }
    str2=locationLineEdit->text().trimmed();
    if(str2.isEmpty())
    {
        locationLineEdit->setFocus();
        return;
    }
    str3=ISBNLineEdit->text().trimmed();
    if(str3.isEmpty())
    {
        ISBNLineEdit->setFocus();
        return;
    }
    canOpen=true;
    emit newData(str1,str2,str3);
    this->close();
}

void addPublisherBox::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers()==Qt::ControlModifier&&e->key()==Qt::Key_S)
        emit acceptButton->clicked(true);
    QWidget::keyPressEvent(e);
}

#include <QDebug>
void addPublisherBox::closeEvent(QCloseEvent *e)
{
    windowHash[7]=false;
    if(!mainWindow->publisherView->isOpen&&canOpen)
    {
        canOpen=false;
        mainWindow->publisherView->show();
        mainWindow->publisherView->tableView->verticalScrollBar()->setValue(mainWindow->publisherView->tableView->verticalScrollBar()->maximum());
    }
    QWidget::closeEvent(e);
}

void addPublisherBox::showEvent(QShowEvent *e)
{
    windowHash[7]=true;
    windowPointer[7]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}
