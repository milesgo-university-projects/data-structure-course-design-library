/******************************************************************
文件名: statusbar.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/21
描　述: 状态栏
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef STATUSBAR
#define STATUSBAR

#include <QStatusBar>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include "mytime.h"
#include "twosizebutton.h"
#include "readerinformationwidget.h"

//状态栏
class mainStatusBar:public QStatusBar
{
    Q_OBJECT
public:
    mainStatusBar(QWidget *parent=0);
    ~mainStatusBar();
    void updateLabelInfo();
                                                             //如事件模拟，读者，图书有关的按钮
    twoSizeButton *startOrStopSimulationButton,*readerInformationButton,*readerInformButton;

    QLabel *currentAccountLabel;                             //标签
    timeLabel *timelabel;
    InformationWidget *informationWidgetPointer;
public slots:
    void onStartOrStopSimulationButtonClicked();              //槽函数，执行任务
    void onReaderInformationButtonClicked();
    void onReaderInformButtonClicked();
    void updateStartOrStopSimulationButton();
    void connectLater();
};

#endif // STATUSBAR

