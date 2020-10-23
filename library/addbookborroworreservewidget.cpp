/******************************************************************
文件名: addbookborroworreservewidget.cpp
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 添加图书借阅、预约、归还等信息
版　本: v5.00 Copyright(c).
******************************************************************/
#include "addbookborroworreservewidget.h"
#include "functions.h"
#include "widget.h"
#include "logs.h"
void addBookBorrowOrReserveWidget::init()//定义各种布局和按钮，进行界面的显示
{
    bookBorrowedByThisReader=NULL;
    canOpen=false;
    isLooking=false;
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("新建借阅/预约");
    this->resize(700,200);
    this->move(300,50);

    searchReaderLabel = new QLabel(this);//寻找读者
    searchBookLabel = new QLabel(this);
    bookAlreadyBorrowedNumberLabel = new QLabel(this);
    bookAvailableBorrowedNumberLabel = new QLabel(this);
    readerNameLabel = new QLabel(this);
    bookClassificationTypeNumberLabel= new QLabel(this);

    searchReaderLabel->setText("*加入读者信息[学/工号]");
    searchBookLabel->setText("*加入图书信息[书名]");
    bookAlreadyBorrowedNumberLabel->setText("当前已借阅/预约");
    bookAvailableBorrowedNumberLabel->setText("当前可借阅/预约");
    readerNameLabel->setText("读者姓名");
    bookClassificationTypeNumberLabel->setText("索书号");

    bookAlreadyBorrowedNumberLineEdit = new QLineEdit(this);//当前已借阅
    bookAlreadyBorrowedNumberLineEdit->setFocusPolicy(Qt::NoFocus);
    bookAvailableBorrowedNumberLineEdit = new QLineEdit(this);//当前未借阅
    bookAvailableBorrowedNumberLineEdit->setFocusPolicy(Qt::NoFocus);
    readerNameLineEdit = new QLineEdit(this);//读者姓名
    readerNameLineEdit->setFocusPolicy(Qt::NoFocus);
    bookClassificationTypeNumberLineEdit = new QLineEdit(this);//创建索书号
    bookClassificationTypeNumberLineEdit->setFocusPolicy(Qt::NoFocus);//设置无焦点窗体

    searchReaderLineEdit = new SearchLineEdit(this);
    searchBookLineEdit = new SearchLineEdit(this);

    addBorrowOrReserveComboBox = new comboBoxWithDropDown(this);//添加节约或预约ComboBox
    addBorrowOrReserveComboBox->addItem("借阅");
    addBorrowOrReserveComboBox->addItem("预约");
    addBorrowOrReserveComboBox->setCurrentText("借阅");

    acceptButton = new QPushButton(this);//完成按钮
    acceptButton->setText("确认");
    acceptButton->setFocusPolicy(Qt::NoFocus);
    rejectButton = new QPushButton(this);
    rejectButton->setText("取消");
    rejectButton->setFocusPolicy(Qt::NoFocus);

    searchBookLayout = new QHBoxLayout;//索书布局
    searchReaderLayout = new QHBoxLayout;
    readerInfoLayout = new QHBoxLayout;
    bookInfoLayout = new QHBoxLayout;
    buttonLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;

    searchReaderLayout->addWidget(addBorrowOrReserveComboBox);//寻找读者布局
    searchReaderLayout->addWidget(searchReaderLabel);
    searchReaderLineEdit->setFixedHeight(40);
    searchReaderLayout->addWidget(searchReaderLineEdit);
    mainLayout->addLayout(searchReaderLayout);
    mainLayout->setStretchFactor(searchReaderLayout,2);

    readerInfoLayout->addWidget(readerNameLabel);//读者信息布局
    readerInfoLayout->addWidget(readerNameLineEdit);
    readerInfoLayout->addWidget(bookAlreadyBorrowedNumberLabel);
    readerInfoLayout->addWidget(bookAlreadyBorrowedNumberLineEdit);
    readerInfoLayout->addWidget(bookAvailableBorrowedNumberLabel);
    readerInfoLayout->addWidget(bookAvailableBorrowedNumberLineEdit);
    mainLayout->addLayout(readerInfoLayout);
    mainLayout->setStretchFactor(readerInfoLayout,1);

    searchBookLayout->addWidget(searchBookLabel);//寻找图书布局
    searchBookLineEdit->setFixedHeight(40);
    searchBookLayout->addWidget(searchBookLineEdit);
    mainLayout->addLayout(searchBookLayout);
    mainLayout->setStretchFactor(searchBookLayout,2);

    bookInfoLayout->addWidget(bookClassificationTypeNumberLabel);//书籍信息布局
    bookInfoLayout->addWidget(bookClassificationTypeNumberLineEdit);
    mainLayout->addLayout(bookInfoLayout);
    mainLayout->setStretchFactor(bookInfoLayout,1);

    buttonLayout->addWidget(acceptButton);//按钮布局方案
    buttonLayout->addWidget(rejectButton);
    mainLayout->addLayout(buttonLayout);

    statusBar = new mainStatusBar(this);
    mainLayout->addWidget(statusBar);

    setLayout(mainLayout);

    //若各个按钮被点击 触发槽函数
    connect(searchBookLineEdit->searchButton,SIGNAL(clicked()),this,SLOT(onSearchBookButtonClicked()));
    connect(searchReaderLineEdit->searchButton,SIGNAL(clicked()),this,SLOT(onSearchReaderButtonClicked()));
    connect(mainWindow->bookView,SIGNAL(bookInfo(QVector<int>)),this,SLOT(completeBookInfo(QVector<int>)));
    connect(mainWindow->bookView,SIGNAL(bookWait(int)),this,SLOT(onBookWaitSignal(int)));
    connect(mainWindow->readerView,SIGNAL(readerInfo(int)),this,SLOT(completeReaderInfo(int)));
    connect(rejectButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(acceptButton,SIGNAL(clicked()),this,SLOT(submitData()));
    statusBar->connectLater();
}

addBookBorrowOrReserveWidget::addBookBorrowOrReserveWidget(QWidget *parent):QWidget(parent)//当点击新建借阅信息 弹出对话框
{
    init();
    connect(addBorrowOrReserveComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(onAddBorrowOrReserveComboBoxCurrentTextChanged(QString)));
}

addBookBorrowOrReserveWidget::addBookBorrowOrReserveWidget(BorrowOrReserve rec, QWidget *parent):QWidget(parent)
{
    init();
    isLooking=true;
    readerNameLineEdit->setText(mainWindow->readerView->recordlister[rec.readerPos].getName());
    searchReaderLineEdit->setText(mainWindow->readerView->recordlister[rec.readerPos].getId());
    searchBookLineEdit->setText(mainWindow->bookView->recordlister[rec.bookPos].getName());
    QString str;
    str.append(mainWindow->bookView->recordlister[rec.bookPos].getClassificationNumber());
    str.append("/");
    str.append(mainWindow->bookView->recordlister[rec.bookPos].getTypeNumber());
    str.append(".");
    str.append(QString("%1").arg(rec.bookSameNumber));
    bookClassificationTypeNumberLineEdit->setText(str);
    int auth=mainWindow->readerView->recordlister[rec.readerPos].getAuth();
    int authPos=mainWindow->authorityView->findAuthPos(auth);
    int bookAmountsLimit;
    if(authPos!=-1)
        bookAmountsLimit=mainWindow->authorityView->recordlister[authPos].getBorrowBookAmountsLimit();
    else
        bookAmountsLimit=5;
    int bookAlreadyBorrowedAmounts=mainWindow->readerView->recordlister[rec.readerPos].getBookBorrowOrReserveVector().size();
    bookAlreadyBorrowedNumberLineEdit->setText(QString("%1").arg(bookAlreadyBorrowedAmounts));
    bookAvailableBorrowedNumberLineEdit->setText(QString("%1").arg(bookAmountsLimit-bookAlreadyBorrowedAmounts));
    if(rec.borrowOrReserveFlag)
        addBorrowOrReserveComboBox->setCurrentText("借阅");
    else
        addBorrowOrReserveComboBox->setCurrentText("预约");
    bookBorrowedByThisReader = new bookBorrowedByReaderWidget(mainWindow->readerView->recordlister[rec.readerPos].getBookBorrowOrReserveVector(),rec.readerPos,this);
    bookBorrowedByThisReader->setColumnWidth(0,100);
    bookBorrowedByThisReader->setColumnWidth(1,255);
    bookBorrowedByThisReader->setColumnWidth(2,100);
    bookBorrowedByThisReader->setColumnWidth(3,100);
    bookBorrowedByThisReader->setColumnWidth(4,100);
    mainLayout->insertWidget(2,bookBorrowedByThisReader,5);
    this->resize(700,600);
    connect(addBorrowOrReserveComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(onAddBorrowOrReserveComboBoxCurrentTextChanged(QString)));
}

addBookBorrowOrReserveWidget::~addBookBorrowOrReserveWidget()
{
    delete searchReaderLabel;
    delete searchBookLabel;
    delete bookAlreadyBorrowedNumberLabel;
    delete bookAvailableBorrowedNumberLabel;
    delete readerNameLabel;
    delete bookClassificationTypeNumberLabel;

    delete bookAlreadyBorrowedNumberLineEdit;
    delete bookAvailableBorrowedNumberLineEdit;
    delete readerNameLineEdit;
    delete bookClassificationTypeNumberLineEdit;

    delete searchReaderLineEdit;
    delete searchBookLineEdit;

    delete addBorrowOrReserveComboBox;

    delete acceptButton;
    delete rejectButton;

    if(bookBorrowedByThisReader!=NULL)
        delete bookBorrowedByThisReader;

    delete searchBookLayout;
    delete searchReaderLayout;
    delete readerInfoLayout;
    delete bookInfoLayout;
    delete buttonLayout;
    delete statusBar;
    delete mainLayout;
}

void addBookBorrowOrReserveWidget::closeEvent(QCloseEvent *e)
{
    windowHash[9]=false;
    if(!mainWindow->bookBorrowOrReserveView->isOpen&&canOpen)
    {
        canOpen=false;
        mainWindow->bookBorrowOrReserveView->show();
        mainWindow->bookBorrowOrReserveView->tableView->verticalScrollBar()->setValue(mainWindow->bookBorrowOrReserveView->tableView->verticalScrollBar()->maximum());
    }
    e->accept();
    QWidget::closeEvent(e);
}

void addBookBorrowOrReserveWidget::showEvent(QShowEvent *e)
{
    statusBar->readerInformationButton->setHidden(true);
    statusBar->readerInformButton->setHidden(true);
    windowHash[9]=true;
    windowPointer[9]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}

void addBookBorrowOrReserveWidget::completeBookInfo(QVector<int> tmpVector)
{
    tmpBookPos=tmpVector[0];
    class Book rec=mainWindow->bookView->recordlister[tmpVector[0]];
    searchBookLineEdit->setText(rec.getName());
    QString str;
    str.append(rec.getClassificationNumber());
    str.append("/");
    str.append(rec.getTypeNumber());
    str.append(".");
    //tmpSameNumber=rec.getSameBookDistinctionVector()[tmpVector[1]].sameNumber;
    tmpSameNumber=tmpVector[1];
    str.append(QString("%1").arg(tmpSameNumber));
    bookClassificationTypeNumberLineEdit->setText(str);
}

void addBookBorrowOrReserveWidget::completeBookInfo()
{//由于是加入等待队列，索书号只显示分类号和种次号
    class Book rec=mainWindow->bookView->recordlister[tmpBookPos];
    searchBookLineEdit->setText(rec.getName());
    QString str;
    str.append(rec.getClassificationNumber());
    str.append("/");
    str.append(rec.getTypeNumber());
    bookClassificationTypeNumberLineEdit->setText(str);
}

void addBookBorrowOrReserveWidget::completeReaderInfo(int readerPos)
{
    tmpReaderPos=readerPos;
    int auth=mainWindow->readerView->recordlister[readerPos].getAuth();
    int authPos=mainWindow->authorityView->findAuthPos(auth);
    int bookAmountsLimit;
    if(authPos!=-1)
        bookAmountsLimit=mainWindow->authorityView->recordlister[authPos].getBorrowBookAmountsLimit();
    else
        bookAmountsLimit=5;
    int bookAlreadyBorrowedAmounts=mainWindow->readerView->recordlister[readerPos].getBookBorrowOrReserveVector().size();
    if(bookAlreadyBorrowedAmounts>=bookAmountsLimit)
    {
        QMessageBox message(QMessageBox::Warning,"警告","该读者借阅/预约量已达上限",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    readerNameLineEdit->setText(mainWindow->readerView->recordlister[readerPos].getName());
    searchReaderLineEdit->setText(mainWindow->readerView->recordlister[readerPos].getId());
    bookAlreadyBorrowedNumberLineEdit->setText(QString("%1").arg(bookAlreadyBorrowedAmounts));
    bookAvailableBorrowedNumberLineEdit->setText(QString("%1").arg(bookAmountsLimit-bookAlreadyBorrowedAmounts));
    if(bookBorrowedByThisReader!=NULL)
        delete bookBorrowedByThisReader;
    bookBorrowedByThisReader = new bookBorrowedByReaderWidget(mainWindow->readerView->recordlister[readerPos].getBookBorrowOrReserveVector(),readerPos,this);
    mainLayout->insertWidget(2,bookBorrowedByThisReader,5);
    this->resize(700,600);
}

void addBookBorrowOrReserveWidget::onAddBorrowOrReserveComboBoxCurrentTextChanged(QString str)
{
    if(isLooking)
        return;
    if(str!="加入等待队列"&&addBorrowOrReserveComboBox->itemText(2)!="")
    {
        searchBookLineEdit->clear();
        bookClassificationTypeNumberLineEdit->clear();
        addBorrowOrReserveComboBox->removeItem(2);
    }
}

void addBookBorrowOrReserveWidget::onSearchBookButtonClicked()
{
    QString str=searchBookLineEdit->text().trimmed();
    mainWindow->bookView->comboBox->setCurrentText("书名");
    mainWindow->bookView->searchEdit->setText(str);
    emit mainWindow->bookView->search->clicked(true);
    mainWindow->bookView->select->setEnabled(true);
    if(addBorrowOrReserveComboBox->currentText()=="借阅")
        mainWindow->bookView->isBorrowing=true;
    else
        mainWindow->bookView->isReserving=true;
    mainWindow->createBookViewWidget();
}

void addBookBorrowOrReserveWidget::onSearchReaderButtonClicked()
{
    QString str=searchReaderLineEdit->text().trimmed();
    mainWindow->readerView->comboBox->setCurrentText("学/工号");
    mainWindow->readerView->searchEdit->setText(str);
    emit mainWindow->readerView->search->clicked(true);
    mainWindow->readerView->select->setEnabled(true);
    mainWindow->readerView->isBorrowingOrReserving=true;
    mainWindow->createReaderViewWidget();
}

void addBookBorrowOrReserveWidget::onBookWaitSignal(int bookPos)
{
    addBorrowOrReserveComboBox->addItem("加入等待队列");
    addBorrowOrReserveComboBox->setCurrentText("加入等待队列");
    tmpBookPos=bookPos;
    completeBookInfo();
}

void addBookBorrowOrReserveWidget::submitData()
{//不管是借阅还是预约，先遍历一遍，看是否已经借阅过该册书
    BorrowOrReserve rec;
    if(searchReaderLineEdit->text().trimmed()==""||bookAlreadyBorrowedNumberLineEdit->text().trimmed()==""
     ||bookAvailableBorrowedNumberLineEdit->text().trimmed()==""||readerNameLineEdit->text().trimmed()=="")
    {
        searchReaderLineEdit->setFocus();
        return;
    }
    if(bookClassificationTypeNumberLineEdit->text().trimmed()==""||searchBookLineEdit->text().trimmed()=="")
    {
        searchBookLineEdit->setFocus();
        return;
    }
    Reader reader=mainWindow->readerView->recordlister[tmpReaderPos];
    bool flag;
    if(reader.getFeeAccount()<0)
    {
        QMessageBox message(QMessageBox::Warning,"警告","该读者余额不足，请尽快充值，否则将无法借阅或预约",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    if(reader.isBorrowOrReserveThisBook(tmpBookPos,flag))
    {
        if(flag)
        {
            QMessageBox message(QMessageBox::Warning,"警告","该读者已借阅该书",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
        }
        else
        {
            QMessageBox message(QMessageBox::Warning,"警告","该读者已预约该书",QMessageBox::No|QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"是");
            message.setButtonText(QMessageBox::No,"否");
        }
        return;
    }
    if(addBorrowOrReserveComboBox->currentText()=="加入等待队列")
    {
        QQueue<int> tmpQueue=mainWindow->bookView->recordlister[tmpBookPos].reserveQueue;
        bool inQueue=false;
        while(!tmpQueue.isEmpty())
        {
            int tmp=tmpQueue.front();
            tmpQueue.pop_front();
            if(tmp==tmpReaderPos)
            {
                QMessageBox message(QMessageBox::Warning,"警告","该读者已在等待队列中",QMessageBox::Yes,NULL);
                message.setButtonText(QMessageBox::Yes,"好的");
                message.exec();
                inQueue=true;
                break;
            }
        }
        if(!inQueue)
        {
            mainWindow->bookView->recordlister[tmpBookPos].reserveQueue.push_back(tmpReaderPos);
            mainWindow->bookView->tablemodel->updateData(mainWindow->bookView->recordlister);
            mainWindow->bookView->flushFile();
            Logs::AddWaitingQueue(reader.getName(),reader.getId(),mainWindow->bookView->recordlister[tmpBookPos].getName());
            QMessageBox message(QMessageBox::Information,"恭喜",QString("提前预约成功，位序%1").arg(mainWindow->bookView->recordlister[tmpBookPos].reserveQueue.size()),QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
        }
        this->close();
        return;
    }
    if(addBorrowOrReserveComboBox->currentText()=="借阅")
        rec.borrowOrReserveFlag=true;
    else
        rec.borrowOrReserveFlag=false;
    rec.readerPos=tmpReaderPos;
    rec.bookPos=tmpBookPos;
    rec.bookSameNumber=tmpSameNumber;
    rec.renewTimes=0;
    rec.addDate=mytime.getDate();
    canOpen=true;
    emit newData(rec);
    this->close();
}
