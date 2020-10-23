/******************************************************************
文件名: addauthoritybox.cpp
创建人: 顾溢杰
日　期: 2017/4/18
修改人: 杨正浩，姜兴国
日　期: 2017/5/26
描　述: 添加读者权限类型
版　本: v5.00 Copyright(c).
******************************************************************/
#include "addauthoritybox.h"
#include "widget.h"
#include <QDebug>

void addAuthorityBox::init()//新建相应的界面 密码存储均经过加密
{
    this->setWindowTitle("添加权限/读者类型");
    canOpen=false;
    isChanging=false;
    this->setObjectName("outsider");
    this->setAttribute(Qt::WA_DeleteOnClose,true);

    readerTypeLabel = new QLabel(this);
    authorityNumberLabel = new QLabel(this);
    borrowBookAmountsLimitLabel = new QLabel(this);
    borrowBookDaysLimitLabel = new QLabel(this);
    renewTimesLimitLabel = new QLabel(this);

    readerTypeLineEdit = new QLineEdit(this);
    authorityNumberLineEdit = new QLineEdit(this);
    borrowBookAmountsLimitLineEdit = new QLineEdit(this);
    borrowBookDaysLimitLineEdit = new QLineEdit(this);
    renewTimesLimitLineEdit = new QLineEdit(this);

    QRegExp regx("[0-9]+$");//定义正则语言
    QValidator *validator;
    validator = new QRegExpValidator(regx,authorityNumberLineEdit);
    authorityNumberLineEdit->setValidator(validator);
    validator = new QRegExpValidator(regx,borrowBookAmountsLimitLineEdit);
    borrowBookAmountsLimitLineEdit->setValidator(validator);
    validator = new QRegExpValidator(regx,borrowBookDaysLimitLineEdit);
    borrowBookDaysLimitLineEdit->setValidator(validator);
    validator = new QRegExpValidator(regx,renewTimesLimitLineEdit);
    renewTimesLimitLineEdit->setValidator(validator);

    gridLayout = new QGridLayout;
    acceptButton = new QPushButton(this);
    rejectButton = new QPushButton(this);

    readerTypeLabel->setText("*读者类型");
    authorityNumberLabel->setText("*权限编号[0-100]");
    borrowBookAmountsLimitLabel->setText("*借阅、预约册数上限");
    borrowBookDaysLimitLabel->setText("*借阅天数上限");
    renewTimesLimitLabel->setText("*续借次数上限");

    acceptButton->setText(tr("确认"));
    rejectButton->setText(tr("取消"));

    gridLayout->addWidget(readerTypeLabel,0,0);//构建网格布局
    gridLayout->addWidget(readerTypeLineEdit,0,1);
    gridLayout->addWidget(authorityNumberLabel,1,0);
    gridLayout->addWidget(authorityNumberLineEdit,1,1);
    gridLayout->addWidget(borrowBookAmountsLimitLabel,2,0);
    gridLayout->addWidget(borrowBookAmountsLimitLineEdit,2,1);
    gridLayout->addWidget(borrowBookDaysLimitLabel,3,0);
    gridLayout->addWidget(borrowBookDaysLimitLineEdit,3,1);
    gridLayout->addWidget(renewTimesLimitLabel,4,0);
    gridLayout->addWidget(renewTimesLimitLineEdit,4,1);
    gridLayout->addWidget(acceptButton,5,0);
    gridLayout->addWidget(rejectButton,5,1);
    setLayout(gridLayout);

    connect(acceptButton,SIGNAL(clicked(bool)),this,SLOT(submitData()));
    connect(rejectButton,SIGNAL(clicked(bool)),this,SLOT(close()));
}

addAuthorityBox::addAuthorityBox(QWidget *parent):QWidget(parent)//调用该函数则开始新建
{
    init();
}

addAuthorityBox::addAuthorityBox(Authority rec, QWidget *parent):QWidget(parent)
{
    init();
    isChanging=true;
    originalNumber=rec.getAuthorityNumber();
    readerTypeLineEdit->setText(rec.getReaderType());
    authorityNumberLineEdit->setText(QString("%1").arg(rec.getAuthorityNumber()));
    borrowBookAmountsLimitLineEdit->setText(QString("%1").arg(rec.getBorrowBookAmountsLimit()));
    borrowBookDaysLimitLineEdit->setText(QString("%1").arg(rec.getBorrowBookDaysLimit()));
    renewTimesLimitLineEdit->setText(QString("%1").arg(rec.getRenewTimesLimit()));
}

addAuthorityBox::~addAuthorityBox()
{
    delete readerTypeLabel;
    delete authorityNumberLabel;
    delete borrowBookAmountsLimitLabel;
    delete borrowBookDaysLimitLabel;
    delete renewTimesLimitLabel;

    delete readerTypeLineEdit;
    delete authorityNumberLineEdit;
    delete borrowBookAmountsLimitLineEdit;
    delete borrowBookDaysLimitLineEdit;
    delete renewTimesLimitLineEdit;

    delete gridLayout;
    delete acceptButton;
    delete rejectButton;
}

void addAuthorityBox::submitData()//更新数据
{
    Authority rec;
    QString str;
    str=readerTypeLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        readerTypeLineEdit->setFocus();
        return;
    }
    rec.setReaderType(str);
    str=authorityNumberLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        authorityNumberLineEdit->setFocus();
        return;
    }
    if(str.toInt()<0||str.toInt()>100)
    {
        QMessageBox message(QMessageBox::Warning,"警告","编号超出范围",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        authorityNumberLineEdit->setFocus();
        return;
    }
    if((!isChanging||(isChanging&&originalNumber!=str.toInt()))&&mainWindow->authorityView->authorityNumberHash[str.toInt()])
    {
        QMessageBox message(QMessageBox::Warning,"警告","该编号已被使用",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        authorityNumberLineEdit->setFocus();
        return;
    }
    rec.setAuthorityNumber(str.toInt());
    str=borrowBookAmountsLimitLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        borrowBookAmountsLimitLineEdit->setFocus();
        return;
    }
    rec.setBorrowBookAmoutsLimit(str.toInt());
    str=borrowBookDaysLimitLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        borrowBookDaysLimitLineEdit->setFocus();
        return;
    }
    rec.setBorrowBookDaysLimit(str.toInt());
    str=renewTimesLimitLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        renewTimesLimitLineEdit->setFocus();
        return;
    }
    rec.setRenewTimesLimit(str.toInt());
    canOpen=true;
    emit newData(rec);
    this->close();
}

void addAuthorityBox::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers()==Qt::ControlModifier&&e->key()==Qt::Key_S)
        emit acceptButton->clicked(true);
    QWidget::keyPressEvent(e);
}

#include <QDebug>

void addAuthorityBox::closeEvent(QCloseEvent *e)
{
    windowHash[11]=false;
   if(!mainWindow->authorityView->isOpen&&canOpen)
   {
       canOpen=false;
       mainWindow->authorityView->show();
       mainWindow->authorityView->tableWidget->verticalScrollBar()->setValue(mainWindow->authorityView->tableWidget->verticalScrollBar()->maximum());
   }
    QWidget::closeEvent(e);
}

void addAuthorityBox::showEvent(QShowEvent *e)
{
    windowHash[11]=true;
    windowPointer[11]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}
