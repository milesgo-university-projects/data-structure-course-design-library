/******************************************************************
文件名: timesimulation
创建人: 顾溢杰
日　期: 2017/5/15
修改人: 杨正浩
日　期: 2017/5/25
描　述: 时间模拟的模块
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef TIMESIMULATION
#define TIMESIMULATION
#include "mytime.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDate>
#include <QTimer>
#include <QInputDialog>
#include <QLineEdit>
#include <QLabel>

//时间模拟模块
class timeSimulation:public QWidget
{
    Q_OBJECT
public:
    timeSimulation(QWidget *parent=0);
    ~timeSimulation();
    QPushButton *changeUpdateTime;                   //改变刷新timer的按钮
    QPushButton *startSimulation;                    //开始模拟的按钮
    QPushButton *stopSimulation;                     //停止模拟的按钮
    QPushButton *resetSimulation;                    //重置时间的按钮
    QVBoxLayout *vlayout;                            //布局
    QTimer *timer;                                   //定时器
    bool isRunning;                                  //是否正在模拟
signals:
    void simulationChanged();                        //模拟状态被改变的信号
public slots:                                        //槽函数（类似于回调函数，QT的特性）
    void restartsimulation();                        //重新模拟
    void startsimulation();                          //开始模拟
    void stopsimulation();                           //停止模拟
    void resetsimulation();                          //重置包括清空所有信息
    void changeupdatetime();                         //更改日期
    void onResetDate(QDate date);                    //是否更改日记，以及更改的什么时候
};

#endif // TIMESIMULATION

