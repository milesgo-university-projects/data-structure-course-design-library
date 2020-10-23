/******************************************************************
文件名: searchlineedit.cpp
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/25
描　述: 信息检索
版　本: v5.00 Copyright(c).
******************************************************************/
#include "searchlineedit.h"
#include <QDebug>
SearchLineEdit::SearchLineEdit(QWidget *parent):QLineEdit(parent)//生成搜索界面
{
    searchButton = new QPushButton;
    searchButtonLabel = new QLabel(searchButton);
    searchButton->setFixedSize(24,24);

    searchButtonLabel->setAttribute(Qt::WA_TranslucentBackground);
    searchButtonLabel->setPixmap(QPixmap("../images/search.png"));
    searchButton->setFocusPolicy(Qt::NoFocus);

    this->setTextMargins(0,0,searchButton->width()+10,0);
    searchButton->setCursor(Qt::PointingHandCursor);

    layout = new QHBoxLayout;
    layout->addStretch();
    layout->addWidget(searchButton);
    layout->setAlignment(searchButton,Qt::AlignRight|Qt::AlignVCenter);
    layout->setSpacing(0);
    this->setLayout(layout);
    connect(this,SIGNAL(returnPressed()),searchButton,SIGNAL(clicked()));
}

SearchLineEdit::~SearchLineEdit()
{
    delete searchButtonLabel;
    delete searchButton;
    delete layout;
}
