/******************************************************************
文件名: titlebar.h
创建人: 顾溢杰
日　期: 2017/4/6
修改人: 杨正浩，姜兴国
日　期: 2017/5/18
描　述: 标题栏
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef TITLEBAR
#define TITLEBAR

#include <QWidget>
#include <QObject>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QPoint>
#include <QIcon>
#include <QApplication>
#include <QDebug>

//标题栏
class titleBar:public QWidget
{
    Q_OBJECT
public:
    titleBar(QWidget *parent=0);
    ~titleBar();
    void mouseMoveEvent(QMouseEvent *e);              //鼠标移动事件
    void mousePressEvent(QMouseEvent *e);             //鼠标点击事件
    void mouseReleaseEvent(QMouseEvent *e);           //鼠标松开
    bool eventFilter(QObject *obj, QEvent *e);
public slots:
    void onClicked();
private:
    QPushButton *closeButton;                         //各种按钮
    QPushButton *minSizeButton;
    QLabel *iconLabel;                                //各种标签
    QLabel *titleLabel;
    QLabel *closeButtonLabel;
    QLabel *minSizeButtonLabel;
    QHBoxLayout *titleLayout;                         //布局
    bool isPressed;                                   //判断是否点击
    QPoint point;
};

#endif // TITLEBAR

