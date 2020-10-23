/******************************************************************
文件名: qpasswordlineedit.h
创建人: 顾溢杰
日　期: 2017/3/18
修改人: 杨正浩，姜兴国
日　期: 2017/4/17
描　述: 密码输入框
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef QPASSWORDLINEEDIT
#define QPASSWORDLINEEDIT
#include <QLineEdit>
#include <QObject>
#include <QKeyEvent>
#include <QMouseEvent>

//密码输入
class QPasswordLineEdit:public QLineEdit
{
    Q_OBJECT
public:
    QPasswordLineEdit(QWidget *parent=0);
    void keyPressEvent(QKeyEvent *e);               //键盘事件
    void mouseMoveEvent(QMouseEvent *e);            //鼠标事件
    void mouseDoubleClickEvent(QMouseEvent *e);
};

#endif // QPASSWORDLINEEDIT

