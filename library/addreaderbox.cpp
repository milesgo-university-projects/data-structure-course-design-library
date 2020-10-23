#include "addreaderbox.h"
#include "widget.h"
#include <QDebug>
#include "functions.h"
#include "changepasswordwidget.h"
#include <QVBoxLayout>
#include "logs.h"

//初始化函数，结合构造函数使用（复用）
void addReaderBox::init()
{
    this->setWindowTitle("添加读者");                                     //初始化各种信息
    canOpen=false;
    this->move(50,50);
    QRegExp regx;
    QValidator *validator;
    this->setObjectName("outsider");
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    nameLabel = new QLabel(this);
    IdLabel = new QLabel(this);                                          //初始化各种标签
    GenderLabel = new QLabel(this);
    AuthLabel = new QLabel(this);
    overDueTimeLabel = new QLabel(this);
    FeeAccountLable= new QLabel(this);
    SchoolLabel = new QLabel(this);
    nameLineEdit = new QLineEdit(this);
    IdLineEdit = new QLineEdit(this);
    regx.setPattern("[0-9]+$");
    validator = new QRegExpValidator(regx,IdLineEdit);
    IdLineEdit->setValidator(validator);
    genderLineComboBox = new comboBoxWithDropDown(this);
    AuthLineComboBox = new comboBoxWithDropDown(this);
    AuthLineComboBox->setFocusPolicy(Qt::NoFocus);
    overDueTimeLineEdit = new QLineEdit(this);
    overDueTimeLineEdit->setFocusPolicy(Qt::NoFocus);
    FeeAccountLineEdit = new QLineEdit(this);
    regx.setPattern("^(?!0(\\d|\\.0+$|$))\\d+(\\.\\d{1,2})?$");         //两位小数
    validator = new QRegExpValidator(regx,FeeAccountLineEdit);
    FeeAccountLineEdit->setValidator(validator);
    schoolLineComboBox = new comboBoxWithDropDown(this);
    gridLayout = new QGridLayout;                                       //初始化布局
    mainLayout = new QHBoxLayout;
    acceptButton = new QPushButton(this);
    acceptButton->setFocusPolicy(Qt::NoFocus);
    rejectButton = new QPushButton(this);
    rejectButton->setFocusPolicy(Qt::NoFocus);

    cancelReserveButton = new QPushButton(this);                        //各种按钮
    cancelReserveButton->setFocusPolicy(Qt::NoFocus);
    cancelReserveButton->setText("取消预约");
    renewButton = new QPushButton(this);
    renewButton->setFocusPolicy(Qt::NoFocus);
    renewButton->setText("续借");

    rightLayout = new QVBoxLayout;
    rightButtonLayout = new QHBoxLayout;
    rightButtonLayout->addWidget(cancelReserveButton);
    rightButtonLayout->addWidget(renewButton);
    rightLayout->addLayout(rightButtonLayout);

    QFile file("../dat/institute.txt");                                //从文件中读入相关信息
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error addreaderbox.cpp";
    QTextStream in(&file);
    QString str;
    while(!in.atEnd())
    {
        in>>str;
        schoolLineComboBox->addItem(str);
    }
    genderLineComboBox->addItem("男");
    genderLineComboBox->addItem("女");
    for(int i=0;i<mainWindow->authorityView->recordlister.size();i++)
        AuthLineComboBox->addItem(mainWindow->authorityView->recordlister[i].getReaderType());

    gridLayout->addWidget(nameLabel,0,0);                                 //设置布局
    gridLayout->addWidget(nameLineEdit,0,1);
    gridLayout->addWidget(IdLabel,1,0);
    gridLayout->addWidget(IdLineEdit,1,1);
    gridLayout->addWidget(GenderLabel,2,0);
    gridLayout->addWidget(genderLineComboBox,2,1);
    gridLayout->addWidget(AuthLabel,3,0);
    gridLayout->addWidget(AuthLineComboBox,3,1);
    gridLayout->addWidget(overDueTimeLabel,4,0);
    gridLayout->addWidget(overDueTimeLineEdit,4,1);
    gridLayout->addWidget(FeeAccountLable,5,0);
    gridLayout->addWidget(FeeAccountLineEdit,5,1);
    gridLayout->addWidget(SchoolLabel,6,0);
    gridLayout->addWidget(schoolLineComboBox,6,1);
    gridLayout->addWidget(acceptButton,7,0);
    gridLayout->addWidget(rejectButton,7,1);
    mainLayout->addLayout(gridLayout);
    mainLayout->setStretchFactor(gridLayout,1);

    setLayout(mainLayout);

    nameLabel->setText(tr("*姓名"));                                      //设置各种显示的文本
    IdLabel->setText(tr("*学/工号"));
    GenderLabel->setText(tr("性别"));
    AuthLabel->setText(tr("权限"));
    overDueTimeLabel->setText(tr("逾期天数"));
    FeeAccountLable->setText(tr("账户余额"));
    SchoolLabel->setText(tr("所在院系"));

    acceptButton->setText(tr("确定"));
    rejectButton->setText(tr("取消"));

    connect(acceptButton,SIGNAL(clicked()),this,SLOT(submitData()));
    connect(rejectButton,SIGNAL(clicked()),this,SLOT(close()));
}

//构造函数
addReaderBox::addReaderBox(QWidget *parent):QWidget(parent)
{
    bookBorrowedByReaderTableWidget=NULL;
    this->resize(300,500);
    init();
    cancelReserveButton->setHidden(true);
    renewButton->setHidden(true);
    overDueTimeLineEdit->setText("0");
    FeeAccountLineEdit->setText("0");
}

//构造函数，重载
addReaderBox::addReaderBox(Reader rec,int readerPos,QWidget *parent):QWidget(parent)
{
    tmpReaderPos=readerPos;
    tmpBBORVector=rec.getBookBorrowOrReserveVector();
    bookBorrowedByReaderTableWidget=NULL;
    this->resize(1200,500);
    bookBorrowedByReaderTableWidget = new bookBorrowedByReaderWidget(rec.getBookBorrowOrReserveVector(),readerPos);
    bookBorrowedByReaderTableWidget->verticalHeader()->setHidden(true);
    bookBorrowedByReaderTableWidget->setColumnWidth(0,144);
    bookBorrowedByReaderTableWidget->setColumnWidth(1,299);
    bookBorrowedByReaderTableWidget->setColumnWidth(2,144);
    bookBorrowedByReaderTableWidget->setColumnWidth(3,144);
    bookBorrowedByReaderTableWidget->setColumnWidth(4,144);
    init();
    rightLayout->insertWidget(0,bookBorrowedByReaderTableWidget);
    mainLayout->insertLayout(1,rightLayout);
    mainLayout->setStretchFactor(rightLayout,3.5);
    nameLineEdit->setText(rec.getName());
    IdLineEdit->setText(rec.getId());
    genderLineComboBox->setCurrentText(rec.getGender());
    int auth=rec.getAuth();
    AuthLineComboBox->setCurrentText(authorityNumberToName[auth]);
    overDueTimeLineEdit->setText(QString("%1").arg(rec.getOverdueTime()));
    FeeAccountLineEdit->setText(QString::number(rec.getFeeAccount(),'f',2));
    FeeAccountLineEdit->setFocusPolicy(Qt::NoFocus);
    schoolLineComboBox->setCurrentText(rec.getSchool());
    renewButton->setEnabled(false);
    cancelReserveButton->setEnabled(false);
    connect(bookBorrowedByReaderTableWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(onTableWidgetItemClicked()));
    connect(renewButton,SIGNAL(clicked()),this,SLOT(onRenewButtonClicked()));
}

//构造函数，重载
addReaderBox::addReaderBox(bool flag,Reader rec,int readerPos,QWidget *parent):QWidget(parent)
{
    Q_UNUSED(flag);
    tmpReaderPos=readerPos;
    tmpBBORVector=rec.getBookBorrowOrReserveVector();
    bookBorrowedByReaderTableWidget=NULL;
    this->resize(1200,500);
    bookBorrowedByReaderTableWidget = new bookBorrowedByReaderWidget(rec.getBookBorrowOrReserveVector(),readerPos);
    bookBorrowedByReaderTableWidget->verticalHeader()->setHidden(true);
    bookBorrowedByReaderTableWidget->setColumnWidth(0,144);
    bookBorrowedByReaderTableWidget->setColumnWidth(1,299);
    bookBorrowedByReaderTableWidget->setColumnWidth(2,144);
    bookBorrowedByReaderTableWidget->setColumnWidth(3,144);
    bookBorrowedByReaderTableWidget->setColumnWidth(4,144);

    this->setWindowTitle("添加读者");                                               //一下部分和init函数相同，未进行复用
    canOpen=false;
    this->move(50,50);
    this->setObjectName("outsider");
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    nameLabel = new QLabel(this);
    IdLabel = new QLabel(this);                                                    //各种控件
    GenderLabel = new QLabel(this);
    AuthLabel = new QLabel(this);
    overDueTimeLabel = new QLabel(this);
    FeeAccountLable= new QLabel(this);
    SchoolLabel = new QLabel(this);
    nameLineEdit = new QLineEdit(this);
    nameLineEdit->setFocusPolicy(Qt::NoFocus);
    IdLineEdit = new QLineEdit(this);
    IdLineEdit->setFocusPolicy(Qt::NoFocus);
    genderLineComboBox = new comboBoxWithDropDown(this);
    AuthLineComboBox = new comboBoxWithDropDown(this);
    overDueTimeLineEdit = new QLineEdit(this);
    overDueTimeLineEdit->setFocusPolicy(Qt::NoFocus);
    FeeAccountLineEdit = new QLineEdit(this);
    FeeAccountLineEdit->setFocusPolicy(Qt::NoFocus);
    schoolLineComboBox = new comboBoxWithDropDown(this);
    gridLayout = new QGridLayout;                                                  //各种布局
    mainLayout = new QHBoxLayout;
    acceptButton = new QPushButton(this);
    rejectButton = new QPushButton(this);

    cancelReserveButton = new QPushButton(this);
    cancelReserveButton->setFocusPolicy(Qt::NoFocus);
    cancelReserveButton->setText("取消预约");
    renewButton = new QPushButton(this);
    renewButton->setFocusPolicy(Qt::NoFocus);
    renewButton->setText("续借");

    rightLayout = new QVBoxLayout;
    rightButtonLayout = new QHBoxLayout;
    rightButtonLayout->addWidget(cancelReserveButton);
    rightButtonLayout->addWidget(renewButton);
    rightLayout->addLayout(rightButtonLayout);

    QPushButton *changePasswordButton = new QPushButton(this);                     //没有delete应该没有什么影响吧
    changePasswordButton->setFocusPolicy(Qt::NoFocus);
    changePasswordButton->setText("修改密码");


    schoolLineComboBox->addItem(rec.getSchool());

    genderLineComboBox->addItem(rec.getGender());

    gridLayout->addWidget(nameLabel,0,0);
    gridLayout->addWidget(nameLineEdit,0,1);
    gridLayout->addWidget(IdLabel,1,0);
    gridLayout->addWidget(IdLineEdit,1,1);
    gridLayout->addWidget(GenderLabel,2,0);
    gridLayout->addWidget(genderLineComboBox,2,1);
    gridLayout->addWidget(AuthLabel,3,0);
    gridLayout->addWidget(AuthLineComboBox,3,1);
    gridLayout->addWidget(overDueTimeLabel,4,0);
    gridLayout->addWidget(overDueTimeLineEdit,4,1);
    gridLayout->addWidget(FeeAccountLable,5,0);
    gridLayout->addWidget(FeeAccountLineEdit,5,1);
    gridLayout->addWidget(SchoolLabel,6,0);
    gridLayout->addWidget(schoolLineComboBox,6,1);
    gridLayout->addWidget(changePasswordButton,7,1);

    mainLayout->addLayout(gridLayout);
    mainLayout->setStretchFactor(gridLayout,1);

    setLayout(mainLayout);

    nameLabel->setText(tr("*姓名"));
    IdLabel->setText(tr("*学/工号"));
    GenderLabel->setText(tr("性别"));
    AuthLabel->setText(tr("权限"));
    overDueTimeLabel->setText(tr("逾期天数"));
    FeeAccountLable->setText(tr("账户余额"));
    SchoolLabel->setText(tr("所在院系"));

    acceptButton->setText(tr("确定"));
    rejectButton->setText(tr("取消"));

    rightLayout->insertWidget(0,bookBorrowedByReaderTableWidget);
    mainLayout->insertLayout(1,rightLayout);
    mainLayout->setStretchFactor(rightLayout,3.5);
    nameLineEdit->setText(rec.getName());
    IdLineEdit->setText(rec.getId());
    genderLineComboBox->setCurrentText(rec.getGender());
    int auth=rec.getAuth();
    AuthLineComboBox->addItem(authorityNumberToName[auth]);
    AuthLineComboBox->setCurrentText(authorityNumberToName[auth]);
    overDueTimeLineEdit->setText(QString("%1").arg(rec.getOverdueTime()));
    FeeAccountLineEdit->setText(QString::number(rec.getFeeAccount(),'f',2));
    schoolLineComboBox->setCurrentText(rec.getSchool());

    renewButton->setEnabled(false);
    cancelReserveButton->setEnabled(false);

    connect(changePasswordButton,SIGNAL(clicked()),this,SLOT(changePassword()));                //连接槽与信号
    connect(bookBorrowedByReaderTableWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(onTableWidgetItemClicked()));
    connect(renewButton,SIGNAL(clicked()),this,SLOT(onRenewButtonClicked()));
    connect(cancelReserveButton,SIGNAL(clicked()),this,SLOT(onCancelReserveButtonClicked()));
}

//析构函数
addReaderBox::~addReaderBox()
{
    delete nameLabel;
    delete IdLabel;
    delete GenderLabel;
    delete AuthLabel;
    delete overDueTimeLabel;
    delete FeeAccountLable;
    delete SchoolLabel;
    delete nameLineEdit;
    delete IdLineEdit;
    delete genderLineComboBox;
    delete AuthLineComboBox;
    delete overDueTimeLineEdit;
    delete FeeAccountLineEdit;
    delete schoolLineComboBox;
    delete acceptButton;
    delete rejectButton;
    if(bookBorrowedByReaderTableWidget!=NULL)
        delete bookBorrowedByReaderTableWidget;
    delete gridLayout;
}

//槽函数，提交新加读者的信息
void addReaderBox::submitData()
{
    QString str;
    QStringList tmpList;
    str=nameLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        nameLineEdit->setFocus();
        return;
    }
    tmpList.append(str);
    str=IdLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        IdLineEdit->setFocus();
        return;
    }
    tmpList.append(str);
    tmpList.append(genderLineComboBox->currentText());
    tmpList.append(AuthLineComboBox->currentText());
    tmpList.append(overDueTimeLineEdit->text().trimmed());
    tmpList.append(FeeAccountLineEdit->text().trimmed());
    tmpList.append(schoolLineComboBox->currentText());
    canOpen=true;
    emit newData(tmpList);
    this->close();
}

//改变面貌
void addReaderBox::changePassword()
{
    changePasswordWidget *inputDialog = new changePasswordWidget;
    inputDialog->show();
}


void addReaderBox::onTableWidgetItemClicked()
{
    QModelIndexList mIndexList=bookBorrowedByReaderTableWidget->selectedIndexes();
    if(mIndexList.size())
    {
        if(mIndexList[0].data().toString()=="借阅")
        {
            renewButton->setEnabled(true);
            cancelReserveButton->setEnabled(false);
        }
        else
        {
            renewButton->setEnabled(false);
            cancelReserveButton->setEnabled(true);
        }
    }
}

//借阅书籍
void addReaderBox::onRenewButtonClicked()
{
    QModelIndexList mIndexList=bookBorrowedByReaderTableWidget->selectedIndexes();
    if(mIndexList.size())
    {
        int row=mIndexList[0].row();
        int auth=mainWindow->readerView->recordlister[tmpReaderPos].getAuth();
        int authPos=mainWindow->authorityView->findAuthPos(auth);
        int bookRenewTimesLimit;
        if(authPos!=-1)
            bookRenewTimesLimit=mainWindow->authorityView->recordlister[authPos].getRenewTimesLimit();
        else
            bookRenewTimesLimit=3;
        if(tmpBBORVector[row].renewTimes>=bookRenewTimesLimit)                  //续借达到上限
        {
            QMessageBox message(QMessageBox::Warning,"警告","已达续借上限",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        mainWindow->readerView->recordlister[tmpReaderPos].addRenewTimes(tmpBBORVector[row].bookPos);
        mainWindow->readerView->tablemodel->updateData(mainWindow->readerView->recordlister);
        mainWindow->readerView->flushFile();
        mainWindow->bookBorrowOrReserveView->addRenewTimes(tmpBBORVector[row].bookPos,tmpBBORVector[row].sameNumber,tmpReaderPos);
        tmpBBORVector[row].renewTimes++;
        Logs::ManagerGoOnBorrow(0,mainWindow->readerView->recordlister[tmpReaderPos].getName(),"",mainWindow->bookView->recordlister[tmpBBORVector[row].bookPos].getName(),tmpBBORVector[row].renewTimes);
        bookBorrowedByReaderTableWidget->updateData(tmpBBORVector,tmpReaderPos);
    }
}

//取消借阅
void addReaderBox::onCancelReserveButtonClicked()
{
    QModelIndexList mIndexList=bookBorrowedByReaderTableWidget->selectedIndexes();
    if(mIndexList.size())
    {
        int row=mIndexList[0].row();
        Logs::DeleBorrowOrReserveLogs(0,mainWindow->readerView->recordlister[mainWindow->currentReaderPos].getName(),mainWindow->readerView->recordlister[mainWindow->currentReaderPos].getId(),mainWindow->bookView->recordlister[tmpBBORVector[row].bookPos].getName(),mainWindow->bookView->recordlister[tmpBBORVector[row].bookPos].getClassificationNumber(),mainWindow->bookView->recordlister[tmpBBORVector[row].bookPos].getTypeNumber(),mainWindow->bookView->recordlister[tmpBBORVector[row].bookPos].getClassificationName(),QString("%1").arg(tmpBBORVector[row].sameNumber));
        mainWindow->bookBorrowOrReserveView->deleteData(tmpBBORVector[row].bookPos,tmpBBORVector[row].sameNumber);
        mainWindow->readerView->recordlister[tmpReaderPos].deleteBBORItem(tmpBBORVector[row].bookPos,tmpBBORVector[row].sameNumber);
        mainWindow->readerView->tablemodel->updateData(mainWindow->readerView->recordlister);
        mainWindow->readerView->flushFile();
        mainWindow->bookView->recordlister[tmpBBORVector[row].bookPos].setIsReserved(false,tmpBBORVector[row].sameNumber);
        mainWindow->bookView->tablemodel->updateData(mainWindow->bookView->recordlister);
        mainWindow->bookView->flushFile();                                          //刷新
        tmpBBORVector.erase(tmpBBORVector.begin()+row);
        bookBorrowedByReaderTableWidget->updateData(tmpBBORVector,tmpReaderPos);
    }
}

//鼠标按下事件
void addReaderBox::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers()==Qt::ControlModifier&&e->key()==Qt::Key_S)
        emit acceptButton->clicked(true);
    QWidget::keyPressEvent(e);
}

//关闭事件
void addReaderBox::closeEvent(QCloseEvent *e)
{
    if(windowHash[14])
    {
        windowPointer[14]->close();
    }
    windowHash[5]=false;
    if(!mainWindow->readerView->isOpen&&canOpen)
    {
        canOpen=false;
        mainWindow->readerView->show();
        mainWindow->readerView->tableView->verticalScrollBar()->setValue(mainWindow->readerView->tableView->verticalScrollBar()->maximum());
    }
    QWidget::closeEvent(e);
}
//显示事件
void addReaderBox::showEvent(QShowEvent *e)
{
    windowHash[5]=true;
    windowPointer[5]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}

