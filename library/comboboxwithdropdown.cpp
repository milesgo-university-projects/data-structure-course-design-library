/******************************************************************
文件名: comboboxwithdropdown.cpp
创建人: 顾溢杰
日　期: 2017/4/11
修改人: 杨正浩，姜兴国
日　期: 2017/4/20
描　述: 对combobox进行了一些美化 构建了一个下拉式复选框
版　本: v5.00 Copyright(c).
******************************************************************/
#include "comboboxwithdropdown.h"
#include <QDebug>
comboBoxWithDropDown::comboBoxWithDropDown(QWidget *parent):QComboBox(parent)
{
    comboBoxDropDownPicture = new QLabel(this);
    mainLayout = new QHBoxLayout;
    comboBoxDropDownPicture->setAttribute(Qt::WA_TranslucentBackground);
    comboBoxDropDownPicture->resize(16,10);
    comboBoxDropDownPicture->setPixmap(QPixmap("../images/drop_down.png"));
    mainLayout->addWidget(comboBoxDropDownPicture);
    mainLayout->setAlignment(comboBoxDropDownPicture,Qt::AlignRight);
    setLayout(mainLayout);
}

comboBoxWithDropDown::~comboBoxWithDropDown()
{
    delete comboBoxDropDownPicture;
}
