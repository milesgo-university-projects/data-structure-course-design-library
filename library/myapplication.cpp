#include "myapplication.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMetaObject>
#include <QDebug>
#include "functions.h"
#include "mytime.h"
#include "timesimulation.h"
#include "widget.h"

myApplication::myApplication(int &argc, char **argv):QApplication(argc,argv)
{

}

myApplication::~myApplication()
{

}

//识别各种鼠标、键盘动作
bool myApplication::notify(QObject *obj, QEvent *e)
{
    if(e->type()==QEvent::KeyPress||e->type()==QEvent::KeyRelease||e->type()==QEvent::MouseButtonDblClick
       ||e->type()==QEvent::MouseButtonPress||e->type()==QEvent::MouseButtonRelease||e->type()==QEvent::MouseMove)
    {//这里重置定时器
        mainWindow->timeSimulationWidget->restartsimulation();
    }
    return QApplication::notify(obj,e);
}

//设置窗体
void myApplication::setWindowInstance(QWidget *wnd)
{
    widget=wnd;
}
