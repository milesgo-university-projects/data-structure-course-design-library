/******************************************************************
文件名: administratormanagewidget.cpp
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/19
描　述: 添加管理员账户
版　本: v5.00 Copyright(c).
******************************************************************/
#include "administratormanagewidget.h"
#include "widget.h"
#include "functions.h"
#include "logs.h"
class Widget;

administratorManageWidget::administratorManageWidget(QWidget *parent):QWidget(parent)//构建布局
{
    this->setWindowTitle("管理员账户管理");
    this->setFixedSize(300,200);
    tableWidget = new friendTableWidget(this);

    addButton = new QPushButton(this);
    addButton->setFocusPolicy(Qt::NoFocus);
    addButton->setText("添加管理员账户");
    deleteButton = new QPushButton(this);
    deleteButton->setFocusPolicy(Qt::NoFocus);
    deleteButton->setText("删除管理员账户");
    changeButton = new QPushButton(this);
    changeButton->setFocusPolicy(Qt::NoFocus);
    changeButton->setText("修改密码");

    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(changeButton);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tableWidget);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    QFile managerPasswordFile("../option/ss.dat");
    if(!managerPasswordFile.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error administratorManageWidget.cpp";
    QTextStream in(&managerPasswordFile);
    QString str1,str2;
    AdministratorAccount rec;
    while(!in.atEnd())
    {
        in>>str1>>str2;
        rec.account=str1;
        rec.md5Password=str2;
        recordlister.push_back(rec);
    }
    managerPasswordFile.close();

    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(addButton,SIGNAL(clicked()),this,SLOT(onAddButtonClicked()));
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(onDeleteButtonClicked()));
    connect(changeButton,SIGNAL(clicked()),this,SLOT(onChangedButtonClicked()));

    updateTableWidget();
}

administratorManageWidget::~administratorManageWidget()
{
    delete tableWidget;
    delete addButton;
    delete deleteButton;
    delete changeButton;
    delete buttonLayout;
    delete mainLayout;
}

void administratorManageWidget::updateTableWidget()//更新当前的文档表格，将刚更新的值刷新到界面
{
    tableWidget->clear();
    tableWidget->setColumnCount(1);
    tableWidget->setColumnWidth(0,280);
    tableWidget->setRowCount(recordlister.size());
    QStringList tmpList;
    tmpList<<"管理员账号名";
    tableWidget->setHorizontalHeaderLabels(tmpList);
    int i;
    for(i=0;i<recordlister.size();i++)
        tableWidget->setItem(i,0,new QTableWidgetItem(recordlister[i].account));
}

void administratorManageWidget::onAddButtonClicked()//对应按钮被点击
{
    isAddOrChange=true;
    addAdministratorBox *inputDialog = new addAdministratorBox;
    connect(inputDialog,SIGNAL(newData(QString,QString)),this,SLOT(addData(QString,QString)));
    inputDialog->currentPasswordLabel->setHidden(true);
    inputDialog->currentPasswordLineEdit->setHidden(true);
    inputDialog->show();
}

void administratorManageWidget::onChangedButtonClicked()
{
    isAddOrChange=false;
    QModelIndexList mIndexList=tableWidget->selectedIndexes();
    if(mIndexList.size())
    {
        currentRow=mIndexList[0].row();
        addAdministratorBox *inputDialog = new addAdministratorBox(currentRow,false);
        connect(inputDialog,SIGNAL(newData(QString,QString)),this,SLOT(addData(QString,QString)));
        inputDialog->administratorNameLineEdit->setFocusPolicy(Qt::NoFocus);
        inputDialog->administratorNameLineEdit->setText(recordlister[currentRow].account);
        inputDialog->currentPasswordLabel->setHidden(false);
        inputDialog->currentPasswordLineEdit->setHidden(false);
        inputDialog->show();
    }
    else
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
}

void administratorManageWidget::onDeleteButtonClicked()
{
    QModelIndexList mIndexList=tableWidget->selectedIndexes();
    if(mIndexList.size())
    {
        int row=mIndexList[0].row();
        if(recordlister[row].account=="admin")
        {
            QMessageBox message(QMessageBox::Warning,"警告","admin账号无法删除",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        else
        {
            Logs::DeleAdministrator(mainWindow->currentAccountName,mainWindow->administratorManageView->recordlister[row].account,mainWindow->administratorManageView->recordlister[row].md5Password);
            recordlister.erase(recordlister.begin()+row);
            updateTableWidget();
        }
    }
    else
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
}

void administratorManageWidget::addData(QString str1, QString str2)//添加或修改对应的账户信息
{
    AdministratorAccount rec;
    if(isAddOrChange)
    {
        rec.account=str1;
        rec.md5Password=md5(str2);
        Logs::AddAdministrator(mainWindow->currentAccountName,rec.account,rec.md5Password);
        recordlister.push_back(rec);
        updateTableWidget();
    }
    else
    {
        rec.account=str1;
        rec.md5Password=md5(str2);
        //qDebug()<<str2;
        Logs::ReplaceAdministrator(mainWindow->currentAccountName,mainWindow->administratorManageView->recordlister[currentRow].account,mainWindow->administratorManageView->recordlister[currentRow].md5Password,rec.account,rec.md5Password);
        recordlister.replace(currentRow,rec);
    }
}

void administratorManageWidget::flushFile()//写入到文件中
{
    QFile managerPasswordFile("../option/ss.dat");
    if(!managerPasswordFile.open(QIODevice::WriteOnly|QIODevice::Text))
        qDebug()<<"open file error administratorManageWidget.cpp";
    QTextStream out(&managerPasswordFile);
    if(recordlister.size()==1)
        out<<recordlister[0].account<<" "<<recordlister[0].md5Password;
    if(recordlister.size()>1)
    {
        int i;
        for(i=0;i<recordlister.size()-1;i++)
            out<<recordlister[i].account<<" "<<recordlister[i].md5Password<<endl;
        out<<recordlister[i].account<<" "<<recordlister[i].md5Password;
    }
    managerPasswordFile.close();
}

void administratorManageWidget::closeEvent(QCloseEvent *e)
{
    if(windowHash[16])
    {
        windowPointer[16]->close();
    }
    flushFile();
    windowHash[15]=false;
    e->accept();
    QWidget::closeEvent(e);
}

void administratorManageWidget::showEvent(QShowEvent *e)
{
    windowHash[15]=true;
    QWidget::showEvent(e);
}

addAdministratorBox::addAdministratorBox(int administratorPos,bool addOrChange,QWidget *parent):QWidget(parent)
{
    tmpAdministratorPos=administratorPos;
    isAddOrChange=addOrChange;
    this->setAttribute(Qt::WA_DeleteOnClose);
    administratorNameLabel = new QLabel(this);
    administratorNameLabel->setText("*请输入管理员账号名");
    currentPasswordLabel = new QLabel(this);
    currentPasswordLabel->setText("*请输入原密码");
    passwordLabel = new QLabel(this);
    passwordLabel->setText("*请输入密码");
    passwordAgainLabel = new QLabel(this);
    passwordAgainLabel->setText("*请再次输入密码");

    administratorNameLineEdit = new QLineEdit(this);
    currentPasswordLineEdit = new QLineEdit(this);
    currentPasswordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordAgainLineEdit = new QLineEdit(this);
    passwordAgainLineEdit->setEchoMode(QLineEdit::Password);

    acceptButton = new QPushButton(this);
    acceptButton->setFocusPolicy(Qt::NoFocus);
    acceptButton->setText("确认");
    rejectButton = new QPushButton(this);
    rejectButton->setFocusPolicy(Qt::NoFocus);
    rejectButton->setText("取消");

    mainLayout = new QGridLayout;
    mainLayout->addWidget(administratorNameLabel,0,0);
    mainLayout->addWidget(administratorNameLineEdit,0,1);
    mainLayout->addWidget(currentPasswordLabel,1,0);
    mainLayout->addWidget(currentPasswordLineEdit,1,1);
    mainLayout->addWidget(passwordLabel,2,0);
    mainLayout->addWidget(passwordLineEdit,2,1);
    mainLayout->addWidget(passwordAgainLabel,3,0);
    mainLayout->addWidget(passwordAgainLineEdit,3,1);
    mainLayout->addWidget(acceptButton,4,0);
    mainLayout->addWidget(rejectButton,4,1);

    setLayout(mainLayout);

    connect(acceptButton,SIGNAL(clicked()),this,SLOT(submitData()));
    connect(rejectButton,SIGNAL(clicked()),this,SLOT(close()));
}

addAdministratorBox::~addAdministratorBox()
{
    delete administratorNameLabel;
    delete currentPasswordLabel;
    delete passwordLabel;
    delete passwordAgainLabel;
    delete administratorNameLineEdit;
    delete currentPasswordLineEdit;
    delete passwordLineEdit;
    delete passwordAgainLineEdit;
    delete acceptButton;
    delete rejectButton;
    delete mainLayout;
}

void addAdministratorBox::submitData()//更新数据
{
    if(isAddOrChange)
    {
        if(administratorNameLineEdit->text().trimmed()=="")
        {
            administratorNameLineEdit->setFocus();
            return;
        }
        for(int i=0;i<mainWindow->administratorManageView->recordlister.size();i++)
        {
            if(mainWindow->administratorManageView->recordlister[i].account==administratorNameLineEdit->text().trimmed())
            {
                QMessageBox message(QMessageBox::Warning,"警告","该账户名已存在",QMessageBox::Yes,NULL);
                message.setButtonText(QMessageBox::Yes,"好的");
                message.exec();
                return;
            }
        }
        if(passwordLineEdit->text().trimmed()=="")
        {
            passwordLineEdit->setFocus();
            return;
        }
        if(passwordAgainLineEdit->text().trimmed()=="")
        {
            passwordAgainLineEdit->setFocus();
            return;
        }
        if(passwordLineEdit->text().trimmed()!=passwordAgainLineEdit->text().trimmed())
        {
            QMessageBox message(QMessageBox::Warning,"警告","两次输入的密码不一致",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        emit newData(administratorNameLineEdit->text(),passwordLineEdit->text().trimmed());
    }
    else
    {
        if(currentPasswordLineEdit->text().trimmed()=="")
        {
            currentPasswordLineEdit->setFocus();
            return;
        }
        if(passwordLineEdit->text().trimmed()=="")
        {
            passwordLineEdit->setFocus();
            return;
        }
        if(passwordAgainLineEdit->text().trimmed()=="")
        {
            passwordAgainLineEdit->setFocus();
            return;
        }
        if(passwordLineEdit->text().trimmed()!=passwordAgainLineEdit->text().trimmed())
        {
            QMessageBox message(QMessageBox::Warning,"警告","两次输入的密码不一致",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        if(md5(currentPasswordLineEdit->text().trimmed())!=mainWindow->administratorManageView->recordlister[tmpAdministratorPos].md5Password)
        {
            QMessageBox message(QMessageBox::Warning,"警告","原密码错误",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        QMessageBox message(QMessageBox::Information,"恭喜","修改密码成功",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        emit newData(administratorNameLineEdit->text(),passwordLineEdit->text().trimmed());
    }
    this->close();
}

void addAdministratorBox::closeEvent(QCloseEvent *e)
{
    windowHash[16]=false;
    e->accept();
    QWidget::closeEvent(e);
}

void addAdministratorBox::showEvent(QShowEvent *e)
{
    windowHash[16]=true;
    windowPointer[16]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}
