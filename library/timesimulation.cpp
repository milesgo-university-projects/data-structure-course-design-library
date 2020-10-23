#include "timesimulation.h"
#include "functions.h"
#include "selectdatewidget.h"
#include "logs.h"
#include "widget.h"
#include <QMessageBox>
#include <QDebug>
timeSimulation::timeSimulation(QWidget *parent):QWidget(parent)
{
    isRunning=false;                                             //初始化为没有运行
    mytime.setUpdateTime(settings->value("updateTime",200).toInt());

    changeUpdateTime = new QPushButton(this);                    //实例化依附于这个窗口的button
    changeUpdateTime->setFixedHeight(50);                        //设置大小
    startSimulation = new QPushButton(this);
    startSimulation->setFixedHeight(50);
    stopSimulation = new QPushButton(this);
    stopSimulation->setFixedHeight(50);
    resetSimulation = new QPushButton(this);
    resetSimulation->setFixedHeight(50);
    vlayout = new QVBoxLayout;
    timer = new QTimer(this);

    vlayout->addWidget(changeUpdateTime);
    vlayout->addWidget(startSimulation);
    vlayout->addWidget(stopSimulation);
    vlayout->addWidget(resetSimulation);

    changeUpdateTime->setText(tr("变速(秒/天)"));                 //设置标签
    changeUpdateTime->setFocusPolicy(Qt::NoFocus);
    startSimulation->setText(tr("开始模拟"));
    startSimulation->setObjectName("insider");
    stopSimulation->setText(tr("停止模拟"));
    stopSimulation->setObjectName("insider");
    stopSimulation->setFocusPolicy(Qt::NoFocus);                 //设置焦点
    resetSimulation->setText(tr("日期重置"));
    resetSimulation->setFocusPolicy(Qt::NoFocus);

    setLayout(vlayout);                                          //设置布局

    connect(timer,SIGNAL(timeout()),&mytime,SLOT(addDate()));    //连接信号与槽函数（信号与动作）
    connect(startSimulation,SIGNAL(clicked()),this,SLOT(startsimulation()));
    connect(stopSimulation,SIGNAL(clicked()),this,SLOT(stopsimulation()));
    connect(resetSimulation,SIGNAL(clicked()),this,SLOT(resetsimulation()));
    connect(changeUpdateTime,SIGNAL(clicked()),this,SLOT(changeupdatetime()));

    mytime.setDate(myToDate(settings->value("date").toString()));
}

//析构函数
timeSimulation::~timeSimulation()
{
    delete changeUpdateTime;                                     //删除new的试题，避免内存泄漏
    delete startSimulation;
    delete stopSimulation;
    delete resetSimulation;
    delete vlayout;
    delete timer;
}

//重新启动模拟
void timeSimulation::restartsimulation()
{
   if(!isRunning)
       return;
   if(timer->isActive())
       timer->stop();
   timer->start(mytime.getUpdateTime());                        //开启定时器
   emit simulationChanged();
}

//开始模拟
void timeSimulation::startsimulation()
{
//    QObject *object=qobject_cast<QObject*>(sender());
//    if(object!=NULL&&object->objectName()=="insider")
//        wasRunning=true;
    isRunning=true;
    if(!timer->isActive())
    {
        timer->start(mytime.getUpdateTime());
        emit simulationChanged();                               //发送信号
    }
}

//停止模拟
void timeSimulation::stopsimulation()
{
    isRunning=false;
    if(timer->isActive())
    {
        timer->stop();
        startSimulation->clearFocus();
        emit simulationChanged();
    }
}

//重置时间
void timeSimulation::resetsimulation()
{
    SelectDateWidget *inputDialog = new SelectDateWidget;
    inputDialog->show();
    connect(inputDialog,SIGNAL(dateInfo(QDate)),this,SLOT(onResetDate(QDate)));
}

//更改刷新timer
void timeSimulation::changeupdatetime()
{
    QInputDialog inputDialog;                                  //输入框
    inputDialog.setOkButtonText("确认");
    inputDialog.setCancelButtonText("取消");
    inputDialog.setWindowTitle("变速（秒/天）");
    inputDialog.setLabelText("请输入一个数字");
    inputDialog.setTextValue(QString("%1").arg(mytime.getUpdateTime()*1.0/1000));
    if(inputDialog.exec()==QInputDialog::Accepted)             //判断是否选中确定
    {
        mytime.setUpdateTime((qint32)(inputDialog.textValue().toDouble()*1000));
        settings->setValue("updateTime",mytime.getUpdateTime());
        if(timer->isActive())
        {
            timer->stop();
            timer->start(mytime.getUpdateTime());
        }
    }
}

//判断是否重置日期，及重置日期
void timeSimulation::onResetDate(QDate date)
{//也要清空读者历史借阅信息，书籍历史借阅信息，书籍历史借阅次数(影响到借阅率)
    QMessageBox message(QMessageBox::Warning,"警告","日期重置将清空所有借阅信息、日志信息，且其他相关日期可能变得不正确，确认继续？",QMessageBox::Yes|QMessageBox::No,NULL);
    message.setButtonText(QMessageBox::Yes,"继续");
    message.setButtonText(QMessageBox::No,"取消");
    if(message.exec()==QMessageBox::No)
    {
        return;
    }
    mytime.setDate(date);
    isRunning=false;
    if(timer->isActive())
    {
        timer->stop();
        startSimulation->clearFocus();
        emit simulationChanged();
    }

    Logs::DeleteAllLogsFile();                           //清空所有借阅信息及日志信息
    statisticdata.clear();                                 //清空所有统计信息，下一步更改今天的时间
    statisticdata.today.date=mytime.getDate().toString("dd.MM.yy");
    mainWindow->bookBorrowOrReserveView->clearAllBookBorrowOrReserveInformation();
    mainWindow->bookView->clearAllEverBorrowedTimes();
}
