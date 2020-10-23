#include "selectdatewidget.h"
#include "functions.h"

//构造函数
SelectDateWidget::SelectDateWidget(QWidget *parent):QWidget(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("选择日期");                                      //设置标题
    this->resize(300,200);
    dateEdit = new QDateEdit;
    dateEdit->setCalendarPopup(true);
    dateEdit->setStyleSheet("QDateEdit::down-arrow{image: url(../images/down_arrow.png)}");
    calendar = new QCalendarWidget;
    dateEdit->setCalendarWidget(calendar);
    acceptButton = new QPushButton;
    acceptButton->setText("确认");
    acceptButton->setFocusPolicy(Qt::NoFocus);
    rejectButton = new QPushButton;
    rejectButton->setText("取消");
    rejectButton->setFocusPolicy(Qt::NoFocus);
    buttonLayout = new QHBoxLayout;                                       //布局
    buttonLayout->addWidget(acceptButton);
    buttonLayout->addWidget(rejectButton);
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(dateEdit);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);                                                //设置布局

    connect(rejectButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(acceptButton,SIGNAL(clicked()),this,SLOT(onAcceptButtonClicked()));
}

//析构函数
SelectDateWidget::~SelectDateWidget()
{
    delete calendar;
    delete dateEdit;
    delete acceptButton;
    delete rejectButton;
    delete buttonLayout;
    delete mainLayout;
}
//关闭前需要做的事
void SelectDateWidget::closeEvent(QCloseEvent *e)
{
    windowHash[20]=false;
    QWidget::closeEvent(e);
}
//显示前需要做的事
void SelectDateWidget::showEvent(QShowEvent *e)
{
    windowHash[20]=true;
    windowPointer[20]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}

//确定，发送选择的日期
void SelectDateWidget::onAcceptButtonClicked()
{
    emit dateInfo(dateEdit->date());
    this->close();
}

