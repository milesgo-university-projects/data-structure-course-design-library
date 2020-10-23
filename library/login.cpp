#include "login.h"
#include "functions.h"
#include "widget.h"
#include <QDebug>
#include "logs.h"

class Widget;

//构造函数
loginDialog::loginDialog(QWidget *parent):QDialog(parent)
{
    setWindowIcon(QIcon("../option/titlebarImages/library.png"));
    this->resize(225,130);                                            //设置大小
    this->setWindowFlags(Qt::FramelessWindowHint);
    messageLabel = new QLabel;
    messageLabel->setText(tr("           图书管理系统"));               //登录界面中心文字
    loginButton = new QPushButton(this);                              //登录按钮
    loginButton->setText(tr("登录"));
    closeButton = new QPushButton(this);                              //关闭按钮
    closeButton->setText(tr("关闭"));
    passwordLineEdit = new QPasswordLineEdit(this);                   //密码输入框
    userLineEdit = new QLineEdit(this);                               //用户输入线
    passwordLabel = new QLabel(this);                                 //密码标签
    passwordLabel->setText(tr("密码"));
    userLabel = new QLabel(this);                                     //用户标签
    userLabel->setText(tr("用户名"));
    checkBox = new QCheckBox(this);                                   //勾选框
    checkBox->setText(tr("是否管理员"));

    passwordLayout = new QHBoxLayout;                                 //布局
    passwordLayout->addWidget(passwordLabel,1);
    passwordLayout->addWidget(passwordLineEdit,4);
    userLayout = new QHBoxLayout;
    userLayout->addWidget(userLabel,1);
    userLayout->addWidget(userLineEdit,4);
    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(closeButton);
    vLayout = new QVBoxLayout;
    vLayout->addWidget(messageLabel);
    vLayout->addLayout(userLayout);
    vLayout->addLayout(passwordLayout);
    vLayout->addWidget(checkBox);
    vLayout->setAlignment(checkBox,Qt::AlignHCenter);
    vLayout->addLayout(buttonLayout);
    this->setLayout(vLayout);
    connect(loginButton,SIGNAL(clicked()),this,SLOT(checkPushed()));   //连接信号与槽
    connect(closeButton,SIGNAL(clicked()),this,SLOT(reject()));
    userLineEdit->setFocus();
    QWidget::setTabOrder(userLineEdit,passwordLineEdit);               //设置顺序
    QWidget::setTabOrder(passwordLineEdit,loginButton);
    QWidget::setTabOrder(loginButton,closeButton);
}

//析构函数
loginDialog::~loginDialog()
{
    delete messageLabel;                                               //删除new的类实体
    delete loginButton;
    delete closeButton;
    delete passwordLineEdit;
    delete userLineEdit;
    delete passwordLabel;
    delete userLabel;
    delete passwordLayout;
    delete userLayout;
    delete buttonLayout;
    delete vLayout;
}

//按下按钮后的回调函数
void loginDialog::checkPushed()
{
    if(checkLoginInfo(userLineEdit->text().trimmed(),passwordLineEdit->text().trimmed()))
    {
        this->accept();
    }
    else
    {                                                                   //弹出警告框
        QMessageBox message(QMessageBox::Warning,"警告","用户名或密码错误！",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        passwordLineEdit->clear();
        passwordLineEdit->setFocus();
    }
}
//判断账号和密码是否对应
bool loginDialog::checkLoginInfo(QString user, QString password)
{
    QFile managerPasswordFile("../option/ss.dat");
    managerPasswordFile.open(QIODevice::ReadOnly|QIODevice::Text);
    QTextStream managerPasswordInfoStream(&managerPasswordFile);
    QString line;
    bool flag=false;
    if(checkBox->isChecked())                                            //判断是否管理员
    {
        mainWindow->isManager=true;
        while(1)
        {
            line=managerPasswordInfoStream.readLine();
            if(line.isEmpty())
                break;
            if(line==user+" "+md5(password))
            {
                flag=true;
                mainWindow->currentAccountName=user;
                mainWindow->statusbar->updateLabelInfo();
                Logs::AdministratorLogined(user);                       //登陆了一个管理员账户
                break;
            }
            if(line.isEmpty())
                flag=false;
        }
    }
    if(!checkBox->isChecked())
    {
        mainWindow->isManager=false;
        for(int i=0;i<mainWindow->readerView->recordlister.size();i++)  //登陆了一个普通读者
        {
            if(mainWindow->readerView->recordlister[i].getId()==user&&mainWindow->readerView->recordlister[i].getPassword()==md5(password))
            {                                                           //搜索读者相关信息
                mainWindow->currentAccountName=mainWindow->readerView->recordlister[i].getName();
                mainWindow->currentReaderPos=i;
                mainWindow->statusbar->updateLabelInfo();
                flag=true;
                Logs::ReaderLogined(mainWindow->readerView->recordlister[i].getName());//登陆了一个读者账户
                break;
            }
        }
    }
    managerPasswordFile.close();
    return flag;
}
