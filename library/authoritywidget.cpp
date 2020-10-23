/******************************************************************
文件名: authoritywidget.cpp
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/19
描　述: 新建、更新或删除权限管理
版　本: v5.00 Copyright(c).
******************************************************************/
#include "authorityWidget.h"
#include "widget.h"
#include <QApplication>
#include <QVector>
#include <QModelIndexList>
#include <QDebug>
#include "logs.h"

authorityWidget::authorityWidget(QWidget *parent)//构件界面
    : QWidget(parent)
{
    int i;
    for(i=0;i<101;i++)
        authorityNumberHash[i]=false;
    this->setWindowTitle("权限/读者类型管理");
    this->setObjectName("outsider");
    this->setFixedSize(630,300);
    addItem = new QPushButton(this);
    deleteItem = new QPushButton(this);
    change = new QPushButton(this);
    tableWidget = new friendTableWidget(this);
    downButtonLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;

    addItem->setText(tr("新建"));
    addItem->setFocusPolicy(Qt::NoFocus);
    deleteItem->setText(tr("删除"));
    deleteItem->setFocusPolicy(Qt::NoFocus);

    change->setText(tr("修改"));
    change->setFocusPolicy(Qt::NoFocus);

    downButtonLayout->addWidget(addItem);//按钮布局
    downButtonLayout->addWidget(deleteItem);
    downButtonLayout->addWidget(change);
    mainLayout->addWidget(tableWidget);
    mainLayout->addLayout(downButtonLayout);
    setLayout(mainLayout);

    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QFile file("../dat/authority.txt");//创建以及打开文件
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error authorityWidget.cpp";
    QTextStream in(&file);
    QString str;
    Authority rec;
    while(!in.atEnd())
    {//将文件中的信息读到容器中
        in>>str;
        rec.setReaderType(str);
        in>>str;
        rec.setAuthorityNumber(str.toInt());
        authorityNumberHash[str.toInt()]=true;
        in>>str;
        rec.setBorrowBookAmoutsLimit(str.toInt());
        in>>str;
        rec.setBorrowBookDaysLimit(str.toInt());
        in>>str;
        rec.setRenewTimesLimit(str.toInt());
        recordlister.push_back(rec);
    }
    file.close();

    connect(addItem,SIGNAL(clicked()),this,SLOT(isAddItem()));//点击新建按钮
    connect(deleteItem,SIGNAL(clicked()),this,SLOT(deleteData()));//点击删除按钮
    connect(change,SIGNAL(clicked()),this,SLOT(isChangeItem()));//点击修改按钮
    connect(tableWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(isChangeItem()));//双击信号

    updateTableWidget();
}

authorityWidget::~authorityWidget()
{
    delete addItem;
    delete deleteItem;
    delete tableWidget;
    delete change;
    delete downButtonLayout;
    delete mainLayout;
}

void authorityWidget::updateTableWidget()
{
    sortByAuthorityNumber();
    tableWidget->clear();
    tableWidget->setColumnCount(5);
    tableWidget->setRowCount(recordlister.size());

    tableWidget->setColumnWidth(0,150);
    tableWidget->setColumnWidth(1,105);
    tableWidget->setColumnWidth(2,121);
    tableWidget->setColumnWidth(3,102);
    tableWidget->setColumnWidth(4,105);

    authorityNameToNumber.clear();
    authorityNumberToName.clear();
    QTableWidgetItem *tmpItem;
    for(int i=0;i<recordlister.size();i++)
    {
        tmpItem = new QTableWidgetItem(recordlister[i].getReaderType());
        tableWidget->setItem(i,0,tmpItem);
        tmpItem = new QTableWidgetItem(QString("%1").arg(recordlister[i].getAuthorityNumber()));
        tableWidget->setItem(i,1,tmpItem);
        tmpItem = new QTableWidgetItem(QString("%1").arg(recordlister[i].getBorrowBookAmountsLimit()));
        tableWidget->setItem(i,2,tmpItem);
        tmpItem = new QTableWidgetItem(QString("%1").arg(recordlister[i].getBorrowBookDaysLimit()));
        tableWidget->setItem(i,3,tmpItem);
        tmpItem = new QTableWidgetItem(QString("%1").arg(recordlister[i].getRenewTimesLimit()));
        tableWidget->setItem(i,4,tmpItem);
        authorityNameToNumber[recordlister[i].getReaderType()]=recordlister[i].getAuthorityNumber();
        authorityNumberToName[recordlister[i].getAuthorityNumber()]=recordlister[i].getReaderType();
    }
    QStringList list;
    list<<"读者类型"<<"权限编号"<<"借阅、预约册数上限"<<"借阅天数上限"<<"续借次数上限";
    tableWidget->setHorizontalHeaderLabels(list);
}

void authorityWidget::flushFile()
{//保存到文件
    QFile file("../dat/authority.txt");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        qDebug()<<"open file error authorityWidget.cpp";
    QTextStream out(&file);
    int i;
    for(i=0;i<recordlister.count()-1;i++)
    {
        out<<recordlister[i].getReaderType()<<" "<<recordlister[i].getAuthorityNumber()<<"  "
              <<recordlister[i].getBorrowBookAmountsLimit()<<"  "<<recordlister[i].getBorrowBookDaysLimit()<<"   "
              <<recordlister[i].getRenewTimesLimit()<<endl;
    }
    out<<recordlister[i].getReaderType()<<" "<<recordlister[i].getAuthorityNumber()<<"  "
          <<recordlister[i].getBorrowBookAmountsLimit()<<"  "<<recordlister[i].getBorrowBookDaysLimit()<<"   "
          <<recordlister[i].getRenewTimesLimit();
    file.close();
}

void authorityWidget::isAddItem()
{//点击新建按钮后，创建输入框
    addOrChangeItem=0;
    addAuthorityBox *inputDialog = new addAuthorityBox;
    connect(inputDialog,SIGNAL(newData(Authority)),this,SLOT(addData(Authority)));
    inputDialog->show();
}

void authorityWidget::isChangeItem()
{
    QModelIndexList mIndexList;
    mIndexList=tableWidget->selectedIndexes();
    if(mIndexList.isEmpty())
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    currentRow=mIndexList[0].row();
    addOrChangeItem=1;
    class Authority rec=recordlister.at(currentRow);
    addAuthorityBox *inputDialog = new addAuthorityBox(rec);//调用另外一个构造函数，以初始化输入框信息
    connect(inputDialog,SIGNAL(newData(Authority)),this,SLOT(addData(Authority)));
    inputDialog->show();
}

void authorityWidget::addData(Authority rec)
{
        if(addOrChangeItem==0)//如果是新建，就直接append增加
        {
            recordlister.append(rec);
            Logs::AppendAuthorityLogs(mainWindow->currentAccountName,rec.getBorrowBookAmountsLimit(),rec.getBorrowBookDaysLimit(),rec.getReaderType(),rec.getRenewTimesLimit(),rec.getAuthorityNumber());
            authorityNumberHash[rec.getAuthorityNumber()]=true;
        }
        else//如果是修改，就replace替换
        {
            authorityNumberHash[recordlister[currentRow].getAuthorityNumber()]=false;
            authorityNumberHash[rec.getAuthorityNumber()]=true;
            Logs::ReplaceAuthorityLogs(mainWindow->currentAccountName,recordlister[currentRow].getBorrowBookAmountsLimit(),recordlister[currentRow].getBorrowBookDaysLimit(),recordlister[currentRow].getReaderType(),recordlister[currentRow].getRenewTimesLimit(),recordlister[currentRow].getAuthorityNumber(),rec.getBorrowBookAmountsLimit(),rec.getBorrowBookDaysLimit(),rec.getReaderType(),rec.getRenewTimesLimit(),rec.getAuthorityNumber());
            recordlister.replace(currentRow,rec);
        }
        updateTableWidget();
}
#include <QDebug>
void authorityWidget::deleteData()
{//删除
    QModelIndexList mIndexList=tableWidget->selectedIndexes();
    if(mIndexList.isEmpty())
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    int bound=mIndexList.size()/5;
    for(int i=0;i<bound;i++)
    {
        int row=mIndexList[i*5].row();
        authorityNumberHash[recordlister[row-i].getAuthorityNumber()]=false;
        Logs::DeleAuthorityLogs(mainWindow->currentAccountName,recordlister[row-i].getBorrowBookAmountsLimit(),recordlister[row-i].getBorrowBookDaysLimit(),recordlister[row-i].getReaderType(),recordlister[row-i].getRenewTimesLimit(),recordlister[row-i].getAuthorityNumber());
        recordlister.erase(recordlister.begin()+row-i);
    }
    updateTableWidget();
}

void authorityWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers()==Qt::ControlModifier&&e->key()==Qt::Key_S)
    {
        flushFile();
        return;
    }
    switch(e->key())
    {
        case    Qt::Key_Delete:
            deleteData();
        break;
        default:
            QWidget::keyPressEvent(e);
        break;
    }
}

void authorityWidget::closeEvent(QCloseEvent *e)
{
    windowHash[10]=false;
    isOpen=false;
    flushFile();
    e->accept();
    QWidget::closeEvent(e);
}

void authorityWidget::showEvent(QShowEvent *e)
{
    windowHash[10]=true;
    windowPointer[10]=qobject_cast<QWidget *>(this);
    isOpen=true;
    QWidget::showEvent(e);
}

int authorityWidget::findAuthPos(int auth)
{
    int i;
    for(i=0;i<recordlister.size();i++)
    {
        if(recordlister[i].getAuthorityNumber()==auth)
            break;
    }
    if(i!=recordlister.size())
    {
        return i;
    }
    else
    {
        return -1;
    }
}

bool myCMP(Authority a,Authority b)
{
    return a.getAuthorityNumber()<b.getAuthorityNumber();
}

void authorityWidget::sortByAuthorityNumber()
{
    sort(recordlister.begin(),recordlister.end(),myCMP);
}
