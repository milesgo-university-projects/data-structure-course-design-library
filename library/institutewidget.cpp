/******************************************************************
文件名: institutewidget.cpp
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 学院信息的显示与管理
版　本: v5.00 Copyright(c).
******************************************************************/
#include "institutewidget.h"
#include "widget.h"
class Widget;

instituteManageWidget::instituteManageWidget(QWidget *parent):QWidget(parent)
{//构建界面
    this->setWindowTitle("学院/部门管理");
    this->setFixedSize(300,500);
    instituteNameListWidget = new friendListWidget;
    acceptButton = new QPushButton;
    rejectButton = new QPushButton;
    addButton = new QPushButton;
    deleteButton = new QPushButton;
    changeButton = new QPushButton;
    mainLayout = new QVBoxLayout;
    upButton = new QPushButton;
    downButton = new QPushButton;
    acceptRejectButtonLayout = new QHBoxLayout;
    upDownButtonLayout = new QHBoxLayout;

    acceptButton->setText("确认");
    acceptButton->setFocusPolicy(Qt::NoFocus);
    rejectButton->setText("取消");
    rejectButton->setFocusPolicy(Qt::NoFocus);
    addButton->setText("添加");
    addButton->setFocusPolicy(Qt::NoFocus);
    deleteButton->setText("删除");
    deleteButton->setFocusPolicy(Qt::NoFocus);
    changeButton->setText("修改");
    changeButton->setFocusPolicy(Qt::NoFocus);
    upButton->setText("上移");
    upButton->setFocusPolicy(Qt::NoFocus);
    downButton->setText("下移");
    downButton->setFocusPolicy(Qt::NoFocus);

    mainLayout->addWidget(instituteNameListWidget);
    mainLayout->addWidget(addButton);
    mainLayout->addWidget(deleteButton);
    mainLayout->addWidget(changeButton);
    upDownButtonLayout->addWidget(upButton);
    upDownButtonLayout->addWidget(downButton);
    mainLayout->addLayout(upDownButtonLayout);
    acceptRejectButtonLayout->addWidget(acceptButton);
    acceptRejectButtonLayout->addWidget(rejectButton);
    mainLayout->addLayout(acceptRejectButtonLayout);
    setLayout(mainLayout);
    readFile();
    instituteNameListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    updateListWidget();
    instituteNameListWidget->clearFocus();
    connect(addButton,SIGNAL(clicked()),this,SLOT(isAddItem()));
    connect(changeButton,SIGNAL(clicked()),this,SLOT(isChangeItem()));
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteItem()));
    connect(acceptButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(rejectButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(upButton,SIGNAL(clicked()),this,SLOT(moveUp()));
    connect(downButton,SIGNAL(clicked()),this,SLOT(moveDown()));
}

instituteManageWidget::~instituteManageWidget()
{
    delete instituteNameListWidget;
    delete acceptButton;
    delete rejectButton;
    delete addButton;
    delete deleteButton;
    delete changeButton;
    delete upButton;
    delete downButton;
    delete upDownButtonLayout;
    delete acceptRejectButtonLayout;
    delete mainLayout;
}

void instituteManageWidget::closeEvent(QCloseEvent *e)
{
    windowHash[12]=false;
    isOpen=false;
    flushFile();
    QWidget::closeEvent(e);
}

void instituteManageWidget::showEvent(QShowEvent *e)
{
    windowHash[12]=true;
    windowPointer[12]=qobject_cast<QWidget *>(this);
    isOpen=true;
    QWidget::showEvent(e);
}

void instituteManageWidget::readFile()//读取文件
{
    QFile file("../dat/institute.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error addreaderbox.cpp";
    QTextStream in(&file);
    QString str;
    while(!in.atEnd())
    {
        in>>str;
        instituteNames.push_back(str);
    }
    file.close();
}

void instituteManageWidget::flushFile()//更新文件
{
    int i;
    QFile file("../dat/institute.txt");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        qDebug()<<"open file error institutewidget.cpp";
    QTextStream out(&file);
    for(i=0;i<instituteNames.size()-1;i++)
        out<<instituteNames[i]<<endl;
    if(instituteNames.size())
        out<<instituteNames[i];
    file.close();
}

void instituteManageWidget::updateListWidget()
{
    instituteNameListWidget->clear();
    for(int i=0;i<instituteNames.size();i++)
    {
        instituteNameListWidget->addItem(new QListWidgetItem(instituteNames[i]));
    }
}

void instituteManageWidget::isAddItem()//添加学院以及部门
{
    QInputDialog inputDialog;
    inputDialog.setOkButtonText("确认");
    inputDialog.setCancelButtonText("取消");
    inputDialog.setWindowTitle("学院/部门添加");
    inputDialog.setLabelText("输出学院/部门名称");
    QString str;
    if(inputDialog.exec()==QInputDialog::Accepted)
    {
        str=inputDialog.textValue();
        instituteNames.push_back(str);
        updateListWidget();
        if(!isOpen)
        {
            this->instituteNameListWidget->verticalScrollBar()->setValue(this->instituteNameListWidget->verticalScrollBar()->maximum());
            this->show();
        }
    }
}

void instituteManageWidget::isChangeItem()//改变学院以及部门的相关信息
{
    bool isOk;
    QModelIndexList mIndexList=instituteNameListWidget->selectedIndexes();
    if(mIndexList.isEmpty())
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    int row=mIndexList[0].row();
    QInputDialog inputDialog;
    inputDialog.setOkButtonText("确认");
    inputDialog.setCancelButtonText("取消");
    QString str=inputDialog.getText(NULL,"学院/部门修改","输入学院/部门名称",QLineEdit::Normal,instituteNames[row],&isOk);
    if(isOk)
    {
        instituteNames.replace(row,str);
        updateListWidget();
    }
}

void instituteManageWidget::deleteItem()//删除信息
{
    QModelIndexList mIndexList=instituteNameListWidget->selectedIndexes();
    if(mIndexList.isEmpty())
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    instituteNames.erase(instituteNames.begin()+mIndexList[0].row());
    updateListWidget();
}

void instituteManageWidget::moveUp()
{
    QModelIndexList mIndexList=instituteNameListWidget->selectedIndexes();
    if(mIndexList.isEmpty())
        return;
    int row=mIndexList[0].row();
    if(row==0)
        return;
    QString tmpStr;
    tmpStr=instituteNames[row];
    instituteNames[row]=instituteNames[row-1];
    instituteNames[row-1]=tmpStr;
    updateListWidget();
    instituteNameListWidget->item(row-1)->setSelected(true);
    instituteNameListWidget->verticalScrollBar()->setValue(row-1);
}

void instituteManageWidget::moveDown()
{
    QModelIndexList mIndexList=instituteNameListWidget->selectedIndexes();
    if(mIndexList.isEmpty())
        return;
    int row=mIndexList[0].row();
    if(row==instituteNames.size()-1)
        return;
    QString tmpStr;
    tmpStr=instituteNames[row];
    instituteNames[row]=instituteNames[row+1];
    instituteNames[row+1]=tmpStr;
    updateListWidget();
    instituteNameListWidget->item(row+1)->setSelected(true);
    instituteNameListWidget->verticalScrollBar()->setValue(row+1);
}
