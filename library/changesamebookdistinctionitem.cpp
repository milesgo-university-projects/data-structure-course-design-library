/******************************************************************
文件名: changesamebookdistinctionitem.cpp
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 修改册信息的对话框 修改该册的出版日期和册序号
版　本: v5.00 Copyright(c).
******************************************************************/
#include "changesamebookdistinctionitem.h"
#include "widget.h"
#include <QDebug>

void changeSameBookDistinctionItem::init()
{
    this->setWindowTitle("添加/修改册");
    QRegExp regx;
    QValidator *validator;
    this->setObjectName("outsider");
    setAttribute(Qt::WA_DeleteOnClose,true);
    sameNumberLabel = new QLabel;
    publishDateLabel = new QLabel;
    sameNumberLineEdit = new QLineEdit;
    regx.setPattern("[0-9]+$");
    validator = new QRegExpValidator(regx,sameNumberLineEdit);
    sameNumberLineEdit->setValidator(validator);
    publishDateLineEdit = new QDateEdit;
    publishDateLineEdit->setStyleSheet("QDateEdit::down-arrow{image: url(../images/down_arrow.png)}");
    publishDateLineEdit->setCalendarPopup(true);
    calendar = new QCalendarWidget;
    publishDateLineEdit->setCalendarWidget(calendar);
    acceptButton = new QPushButton;
    rejectButton = new QPushButton;
    gridLayout = new QGridLayout;

    sameNumberLabel->setText("*序号[0-100]");
    publishDateLabel->setText("*出版日期");
    acceptButton->setText("确认");
    rejectButton->setText("取消");
    acceptButton->setFocusPolicy(Qt::NoFocus);
    rejectButton->setFocusPolicy(Qt::NoFocus);

    gridLayout->addWidget(sameNumberLabel,0,0);
    gridLayout->addWidget(sameNumberLineEdit,0,1);
    gridLayout->addWidget(publishDateLabel,1,0);
    gridLayout->addWidget(publishDateLineEdit,1,1);
    gridLayout->addWidget(acceptButton,2,0);
    gridLayout->addWidget(rejectButton,2,1);
    setLayout(gridLayout);

    connect(acceptButton,SIGNAL(clicked()),this,SLOT(submitData()));
    connect(rejectButton,SIGNAL(clicked()),this,SLOT(close()));
}

changeSameBookDistinctionItem::changeSameBookDistinctionItem(QWidget *parent):QWidget(parent)
{
    init();
}

changeSameBookDistinctionItem::changeSameBookDistinctionItem(QStringList tmpList, QWidget *parent):QWidget(parent)
{
    init();
    sameNumberLineEdit->setText(tmpList.at(0));
    publishDateLineEdit->setDate(myToDate(tmpList.at(1)));
}

changeSameBookDistinctionItem::~changeSameBookDistinctionItem()
{
    delete sameNumberLabel;
    delete publishDateLabel;
    delete sameNumberLineEdit;
    delete publishDateLineEdit;
    delete acceptButton;
    delete rejectButton;
    delete gridLayout;
}

void changeSameBookDistinctionItem::submitData()//添加数据
{
    QStringList strList;
    QString str;
    strList<<sameNumberLineEdit->text().trimmed()<<myDateToString(publishDateLineEdit->date());
    str=strList.at(0);
    int order=str.toInt();
    if(sameNumberHash[order]||order<0||order>100)
    {
        sameNumberLineEdit->setFocus();
        return;
    }
    emit newData(strList);
    this->close();
}

void changeSameBookDistinctionItem::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers()==Qt::ControlModifier&&e->key()==Qt::Key_S)
        emit acceptButton->clicked(true);
    QWidget::keyPressEvent(e);
}

void changeSameBookDistinctionItem::closeEvent(QCloseEvent *e)
{
    windowHash[3]=false;
    QWidget::closeEvent(e);
}

void changeSameBookDistinctionItem::showEvent(QShowEvent *e)
{
    windowHash[3]=true;
    windowPointer[3]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}
