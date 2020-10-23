#include "addfeeacount.h"
#include "functions.h"
#include "widget.h"
#include "logs.h"
class Widget;

//构造函数
addFeeAcount::addFeeAcount(QWidget *parent):QWidget(parent)
{
    setWindowTitle("充值");
    QRegExp regx;
    QValidator *validator;
    this->setAttribute(Qt::WA_DeleteOnClose);
    searchReaderLabel = new QLabel(this);                //实例化一个label，下面相同
    searchReaderLabel->setText("*选择读者");              //设置label显示的文字，下面相同
    currentFeeAcountLabel = new QLabel(this);
    currentFeeAcountLabel->setText("当前余额");
    addFeeAcountNumberLabel = new QLabel(this);
    addFeeAcountNumberLabel->setText("*请输入充值金额[2位小数]");

    searchReaderLineEdit = new SearchLineEdit(this);     //搜索框
    searchReaderLineEdit->setFixedHeight(40);            //设置大小
    searchReaderLineEdit->searchButton->setObjectName("addFeeAcount");
    currentFeeAcountLineEdit = new QLineEdit(this);
    currentFeeAcountLineEdit->setFocusPolicy(Qt::NoFocus);
    addFeeAcountNumberLineEdit = new QLineEdit(this);

    regx.setPattern("^(?!0(\\d|\\.0+$|$))\\d+(\\.\\d{1,2})?$");//正则表达式，两位小数
    validator = new QRegExpValidator(regx,addFeeAcountNumberLineEdit);
    addFeeAcountNumberLineEdit->setValidator(validator);

    acceptButton = new QPushButton(this);                //确定按钮，设置文字
    acceptButton->setFocusPolicy(Qt::NoFocus);
    acceptButton->setText("确认");                        //设置文字，设置文字
    rejectButton = new QPushButton(this);
    rejectButton->setFocusPolicy(Qt::NoFocus);
    rejectButton->setText("取消");

    mainLayout = new QGridLayout;                        //主要的布局
    mainLayout->addWidget(searchReaderLabel,0,0);
    mainLayout->addWidget(searchReaderLineEdit,0,1);
    mainLayout->addWidget(currentFeeAcountLabel,1,0);
    mainLayout->addWidget(currentFeeAcountLineEdit,1,1);
    mainLayout->addWidget(addFeeAcountNumberLabel,2,0);
    mainLayout->addWidget(addFeeAcountNumberLineEdit,2,1);
    mainLayout->addWidget(acceptButton,3,0);
    mainLayout->addWidget(rejectButton,3,1);

    setLayout(mainLayout);                               //设置布局
                                                         //主要的槽函数与信号
    connect(acceptButton,SIGNAL(clicked()),this,SLOT(submitData()));
    connect(rejectButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(searchReaderLineEdit->searchButton,SIGNAL(clicked()),this,SLOT(onSearchReaderButtonClicked()));
    connect(mainWindow->readerView,SIGNAL(readerInfo(int)),this,SLOT(completeReaderInfo(int)));
    connect(searchReaderLineEdit,SIGNAL(textChanged(QString)),currentFeeAcountLineEdit,SLOT(clear()));
}

//析构函数
addFeeAcount::~addFeeAcount()
{
    delete searchReaderLabel;
    delete currentFeeAcountLabel;
    delete addFeeAcountNumberLabel;
    delete searchReaderLineEdit;
    delete currentFeeAcountLineEdit;
    delete addFeeAcountNumberLineEdit;
    delete acceptButton;
    delete rejectButton;
    delete mainLayout;
}

//搜索button
void addFeeAcount::onSearchReaderButtonClicked()
{
    QString str=searchReaderLineEdit->text().trimmed();
    mainWindow->readerView->comboBox->setCurrentText("学/工号");
    mainWindow->readerView->searchEdit->setText(str);
    emit mainWindow->readerView->search->clicked(true);
    mainWindow->readerView->select->setEnabled(true);
    mainWindow->readerView->isAddFeeAcount=true;
    mainWindow->createReaderViewWidget();
}

//得到读者的信息
void addFeeAcount::completeReaderInfo(int readerPos)
{
    tmpReaderPos=readerPos;
    double feeAcount=mainWindow->readerView->recordlister[readerPos].getFeeAccount();
    QString id=mainWindow->readerView->recordlister[readerPos].getId();
    searchReaderLineEdit->setText(id);
    currentFeeAcountLineEdit->setText(QString("%1").arg(feeAcount));
}

//选择读者后，输入重置金额，再更新信息
void addFeeAcount::submitData()
{
    if(searchReaderLineEdit->text().trimmed()=="")   //判断是否选择读者
    {
        QMessageBox message(QMessageBox::Warning,"警告","请在读者界面选择读者",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        searchReaderLineEdit->setFocus();
        return;
    }
    if(currentFeeAcountLineEdit->text()=="")
    {
        QMessageBox message(QMessageBox::Warning,"警告","请在读者界面选择读者",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        searchReaderLineEdit->setFocus();
        return;
    }
    if(addFeeAcountNumberLineEdit->text()=="")     //输入金额
    {
        QMessageBox message(QMessageBox::Warning,"警告","请输入充值金额",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        searchReaderLineEdit->setFocus();
        return;
    }
    double feeAcount=addFeeAcountNumberLineEdit->text().toDouble();
    mainWindow->readerView->recordlister[tmpReaderPos].Deposit(feeAcount);
    mainWindow->readerView->tablemodel->updateData(mainWindow->readerView->recordlister);
    mainWindow->readerView->flushFile();          //刷新进文件
    Logs::AddFeeAcount(mainWindow->readerView->recordlister[tmpReaderPos].getName(),mainWindow->readerView->recordlister[tmpReaderPos].getId(),mainWindow->readerView->recordlister[tmpReaderPos].getFeeAccount()-feeAcount,mainWindow->readerView->recordlister[tmpReaderPos].getFeeAccount());
    if(mainWindow->readerView->recordlister[tmpReaderPos].getFeeAccount()>0)
        mainWindow->readerView->recordlister[tmpReaderPos].hasSentAcountNotEnoughInformation=false;
    this->close();
}

//关闭前需要做的事
void addFeeAcount::closeEvent(QCloseEvent *e)
{
    windowHash[13]=false;
    QWidget::closeEvent(e);
}

//显示前需要做的事
void addFeeAcount::showEvent(QShowEvent *e)
{
    windowHash[13]=true;
    windowPointer[13]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}
