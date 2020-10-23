/******************************************************************
文件名: main.cpp
创建人: 顾溢杰
日　期: 2017/4/3
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 主函数 负责各个界面的显示
版　本: v5.00 Copyright(c).
******************************************************************/
#include "widget.h"
#include "login.h"
#include <QCheckBox>
#include <QDir>
#include <QTreeView>
#include <QDebug>
#include "bookviewwidget.h"
#include "readerviewwidget.h"
#include "functions.h"
#include "myapplication.h"

int main(int argc, char *argv[])
{
    int i;
    for(i=0;i<30;i++)
        windowHash[i]=false;
    myApplication a(argc,argv);
    a.setQuitOnLastWindowClosed(true);//最后一个窗口关闭后自动退出程序
    setStyle("../option/style.qss");
    settings = new QSettings("../option/settings.ini",QSettings::IniFormat);

    Widget w;
    loginDialog *logindialog = new loginDialog;
    logindialog->show();//显示各个界面
    if(logindialog->exec()==QDialog::Accepted)
    {
        w.show();
        delete logindialog;
        return a.exec();
    }
    else
    {
        delete logindialog;
        return 0;
    }
   return a.exec();
}
