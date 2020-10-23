#include "waitingqueuetablewidget.h"
#include "functions.h"
#include "widget.h"

//构造函数
waitingQueueTableWidget::waitingQueueTableWidget(QWidget *parent):QWidget(parent)
{
    this->setWindowTitle("书籍等待队列");
    this->setFixedSize(350,500);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    tableWidget = new QTableWidget(this);                         //new一个实体
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tableWidget);
    setLayout(mainLayout);
}

//析构函数
waitingQueueTableWidget::~waitingQueueTableWidget()
{
    delete tableWidget;
    delete mainLayout;
}

//更新数据
void waitingQueueTableWidget::updateData(QQueue<int> waitingQueue)
{
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList()<<"位序"<<"姓名"<<"学号");
    tableWidget->setRowCount(waitingQueue.size());
    tableWidget->setColumnWidth(0,50);                            //设置每一列的宽度
    tableWidget->setColumnWidth(1,130);
    tableWidget->setColumnWidth(2,130);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    int count=0;
    while(waitingQueue.size())
    {
        int tmp=waitingQueue.front();                             //更新数据
        waitingQueue.pop_front();
        tableWidget->setItem(count,0,new QTableWidgetItem(QString("%1").arg(count+1)));
        tableWidget->setItem(count,1,new QTableWidgetItem(mainWindow->readerView->recordlister[tmp].getName()));
        tableWidget->setItem(count,2,new QTableWidgetItem(mainWindow->readerView->recordlister[tmp].getId()));
        count++;
    }
}

//关闭前需要做的事
void waitingQueueTableWidget::closeEvent(QCloseEvent *e)
{
    windowHash[18]=false;
    e->accept();
    QWidget::closeEvent(e);
}

//显示前需要改变的变量
void waitingQueueTableWidget::showEvent(QShowEvent *e)
{
    windowHash[18]=true;
    windowPointer[18]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}
