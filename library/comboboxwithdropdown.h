/******************************************************************
文件名: comboboxwithdropdown.h
创建人: 顾溢杰
日　期: 2017/4/11
修改人: 杨正浩，姜兴国
日　期: 2017/4/20
描　述: 对combobox进行了一些美化 构建了一个下拉式复选框
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef COMBOBOXWITHDROPDOWN
#define COMBOBOXWITHDROPDOWN
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
class comboBoxWithDropDown:public QComboBox
{
    Q_OBJECT
public:
    comboBoxWithDropDown(QWidget *parent=0);
    ~comboBoxWithDropDown();
    QLabel *comboBoxDropDownPicture;
    QHBoxLayout *mainLayout;
};
#endif // COMBOBOXWITHDROPDOWN

