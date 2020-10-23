/******************************************************************
文件名: myapplication.h
创建人: 顾溢杰
日　期: 2017/5/1
修改人: 杨正浩，姜兴国
日　期: 2017/5/25
描　述: 程序主要界面
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef MYAPPLICATION
#define MYAPPLICATION
#include <QApplication>

//对qt自带的application的改善
class myApplication:public QApplication
{
public:
    myApplication(int &argc,char **argv);
    ~myApplication();
    bool notify(QObject *obj, QEvent *e);         //通知
    void setWindowInstance(QWidget *wnd);
private:
    QWidget *widget;
};

#endif // MYAPPLICATION

