/******************************************************************
文件名: changepasswordwidget.cpp
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/25
描　述: 密码变更
版　本: v5.00 Copyright(c).
******************************************************************/
#include "changepasswordwidget.h"
#include "functions.h"
#include "widget.h"
class Widget;
changePasswordWidget::changePasswordWidget(QWidget *parent):QWidget(parent)
{//构建密码修改界面
    setFixedSize(350,200);
    setWindowTitle("修改密码");
    this->setAttribute(Qt::WA_DeleteOnClose);
    readerIdLabel = new QLabel(this);
    readerIdLabel->setText("读者ID");
    currentPasswordLabel = new QLabel(this);
    currentPasswordLabel->setText("*当前密码");
    newPasswordLabel = new QLabel(this);
    newPasswordLabel->setText("*新密码");
    newPasswordAgainLabel = new QLabel(this);
    newPasswordAgainLabel->setText("*确认新密码");

    readerIdLineEdit = new QLineEdit(this);
    readerIdLineEdit->setFocusPolicy(Qt::NoFocus);
    readerIdLineEdit->setText(mainWindow->readerView->recordlister[mainWindow->currentReaderPos].getId());
    currentPasswordLineEdit = new QLineEdit(this);
    currentPasswordLineEdit->setEchoMode(QLineEdit::Password);
    newPasswordLineEdit = new QLineEdit(this);
    newPasswordLineEdit->setEchoMode(QLineEdit::Password);
    newPasswordAgainLineEdit = new QLineEdit(this);
    newPasswordAgainLineEdit->setEchoMode(QLineEdit::Password);

    acceptButton = new QPushButton(this);
    acceptButton->setFocusPolicy(Qt::NoFocus);
    acceptButton->setText("确认");
    rejectButton = new QPushButton(this);
    rejectButton->setFocusPolicy(Qt::NoFocus);
    rejectButton->setText("取消");

    mainLayout = new QGridLayout;
    mainLayout->addWidget(readerIdLabel,0,0);
    mainLayout->addWidget(readerIdLineEdit,0,1);
    mainLayout->addWidget(currentPasswordLabel,1,0);
    mainLayout->addWidget(currentPasswordLineEdit,1,1);
    mainLayout->addWidget(newPasswordLabel,2,0);
    mainLayout->addWidget(newPasswordLineEdit,2,1);
    mainLayout->addWidget(newPasswordAgainLabel,3,0);
    mainLayout->addWidget(newPasswordAgainLineEdit,3,1);
    mainLayout->addWidget(acceptButton,4,0);
    mainLayout->addWidget(rejectButton,4,1);

    setLayout(mainLayout);

    connect(acceptButton,SIGNAL(clicked()),this,SLOT(submitData()));
    connect(rejectButton,SIGNAL(clicked()),this,SLOT(close()));
}

changePasswordWidget::~changePasswordWidget()
{//按顺序释放指针
    delete readerIdLabel;
    delete currentPasswordLabel;
    delete newPasswordLabel;
    delete newPasswordAgainLabel;
    delete readerIdLineEdit;
    delete currentPasswordLineEdit;
    delete newPasswordLineEdit;
    delete newPasswordAgainLineEdit;
    delete acceptButton;
    delete rejectButton;
    delete mainLayout;
}

void changePasswordWidget::submitData()
{//获取新输入的密码
    if(currentPasswordLineEdit->text()=="")//判错
    {
        currentPasswordLineEdit->setFocus();
        return;
    }
    if(newPasswordLineEdit->text().trimmed()=="")
    {
        newPasswordLineEdit->setFocus();
        return;
    }
    if(newPasswordAgainLineEdit->text().trimmed()=="")
    {
        newPasswordAgainLineEdit->setFocus();
        return;
    }
    if(newPasswordAgainLineEdit->text().trimmed()!=newPasswordLineEdit->text().trimmed())
    {
        QMessageBox message(QMessageBox::Warning,"警告","两次输入的密码不一致",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        newPasswordAgainLineEdit->setFocus();
        return;
    }
    if(md5(currentPasswordLineEdit->text().trimmed())!=mainWindow->readerView->recordlister[mainWindow->currentReaderPos].getPassword())
    {
        QMessageBox message(QMessageBox::Warning,"警告","原密码错误",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        currentPasswordLineEdit->setFocus();
        return;
    }//成功创建
    mainWindow->readerView->recordlister[mainWindow->currentReaderPos].passwordChange(newPasswordLineEdit->text().trimmed());
    mainWindow->readerView->tablemodel->updateData(mainWindow->readerView->recordlister);
    mainWindow->readerView->flushFile();
    QMessageBox message(QMessageBox::Information,"恭喜","修改密码成功",QMessageBox::Yes,NULL);
    message.setButtonText(QMessageBox::Yes,"好的");
    message.exec();
    this->close();
}

void changePasswordWidget::closeEvent(QCloseEvent *e)
{
    windowHash[14]=false;
    QWidget::closeEvent(e);
}

void changePasswordWidget::showEvent(QShowEvent *e)
{
    windowHash[14]=true;
    windowPointer[14]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}

