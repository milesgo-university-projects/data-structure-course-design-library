#include "statusbar.h"
#include "functions.h"
#include "widget.h"
#include "addreaderbox.h"
#include <QDebug>

class Widget;

//构造函数
mainStatusBar::mainStatusBar(QWidget *parent):QStatusBar(parent)
{
    currentAccountLabel = new QLabel(this);
    timelabel = new timeLabel(this);

    startOrStopSimulationButton = new twoSizeButton("start_16px","start_24px",QSize(30,30),QSize(16,16),QSize(24,24));
//读者信息
    readerInformationButton = new twoSizeButton("reader_information_16px","reader_information_24px",QSize(30,30),QSize(16,16),QSize(24,24));
//读者通知
    readerInformButton = new twoSizeButton("envelope_16px","envelope_24px",QSize(30,30),QSize(16,16),QSize(24,24));

    addWidget(currentAccountLabel);
    readerInformationButton->setStyleSheet("border: none");
    readerInformationButton->setFixedHeight(30);
    addWidget(readerInformationButton);
    readerInformButton->setStyleSheet("border: none");
    readerInformButton->setFixedHeight(30);
    addWidget(readerInformButton);
    startOrStopSimulationButton->setStyleSheet("border: none");
    startOrStopSimulationButton->setFixedHeight(30);
    addPermanentWidget(startOrStopSimulationButton);
    addPermanentWidget(timelabel);

    connect(startOrStopSimulationButton,SIGNAL(clicked()),this,SLOT(onStartOrStopSimulationButtonClicked()));
    connect(readerInformationButton,SIGNAL(clicked()),this,SLOT(onReaderInformationButtonClicked()));
    connect(readerInformButton,SIGNAL(clicked()),this,SLOT(onReaderInformButtonClicked()));
}

//析构函数
mainStatusBar::~mainStatusBar()
{
    delete readerInformationButton;
    delete readerInformButton;
    delete startOrStopSimulationButton;
    delete currentAccountLabel;
    delete timelabel;
}

//更新标签信息
void mainStatusBar::updateLabelInfo()
{
    currentAccountLabel->setText(QString("当前用户:%1").arg(mainWindow->currentAccountName));
}

void mainStatusBar::onStartOrStopSimulationButtonClicked()
{
    if(mainWindow->timeSimulationWidget->timer->isActive())
    {
        mainWindow->timeSimulationWidget->stopsimulation();
    }
    else
    {
        mainWindow->timeSimulationWidget->startsimulation();
    }
}

//双击读者后显示的窗格
void mainStatusBar::onReaderInformationButtonClicked()
{
    class Reader rec=mainWindow->readerView->recordlister.at(mainWindow->currentReaderPos);
    addReaderBox *inputDialog = new addReaderBox(true,rec,mainWindow->currentReaderPos);//调用另外一个构造函数，以初始化输入框信息
    inputDialog->genderLineComboBox->setFocusPolicy(Qt::NoFocus);
    inputDialog->schoolLineComboBox->setFocusPolicy(Qt::NoFocus);
    inputDialog->acceptButton->setHidden(true);
    inputDialog->rejectButton->setHidden(true);
    inputDialog->setWindowTitle("读者信息");
    inputDialog->show();
}


//点击后显示读者信息
void mainStatusBar::onReaderInformButtonClicked()
{
    informationWidgetPointer = new InformationWidget(mainWindow->readerView->recordlister[mainWindow->currentReaderPos].getId());
    informationWidgetPointer->show();
}

//开始或者暂停模拟的图片
void mainStatusBar::updateStartOrStopSimulationButton()
{
    if(mainWindow->timeSimulationWidget->timer->isActive())
    {
        startOrStopSimulationButton->setPicture("pause_16px","pause_24px");
    }
    else
    {
        startOrStopSimulationButton->setPicture("start_16px","start_24px");
    }
}

//时间模拟
void mainStatusBar::connectLater()
{
    connect(mainWindow->timeSimulationWidget,SIGNAL(simulationChanged()),this,SLOT(updateStartOrStopSimulationButton()));
}
