/******************************************************************
文件名: twoSizeButton.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/21
描　述: 自定义两种尺寸的按钮
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef TWOSIZEBUTTON
#define TWOSIZEBUTTON
#include <QPushButton>
#include <QSize>
#include <QPixmap>
#include <QLabel>
#include <QEvent>

//2种尺寸的按钮
class twoSizeButton:public QPushButton
{//按钮的大小其实是固定的，改变大小的是其上面的label的大小，或者图片的大小
    Q_OBJECT
public:
    twoSizeButton(QString smallButtonName="",QString bigButtonName="",QSize buttonSize=QSize(30,30),QSize smallSize=QSize(16,16),QSize bigSize=QSize(24,24),QWidget *parent=0);
    ~twoSizeButton();
    QSize smallSize,bigSize;                   //尺寸
    QLabel *smallLabel,*bigLabel;              //标签
    void setSmallSize(QSize size);             //设置尺寸
    void setSmallSize(int w,int h);
    void setBigSize(QSize size);
    void setBigSize(int w,int h);
    void setPicture(QString smallButtonName,QString bigButtonName);
    void enterEvent(QEvent *e);               //进入
    void leaveEvent(QEvent *e);               //变比
};

#endif // TWOSIZEBUTTON

