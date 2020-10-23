#include "addnewbook.h"
#include "widget.h"
#include <QDebug>
#include "waitingqueuetablewidget.h"
class Book;
class Widget;
struct SameBookDistinction;


//初始化，为各种实例动态申请空间，设置各种属性
void addBookBox::init()
{
    this->setWindowTitle("添加图书");
    canOpen=false;
    isChanging=false;
    this->move(50,50);
    QRegExp regx;
    QValidator *validator;
    this->setObjectName("outsider");
    setAttribute(Qt::WA_DeleteOnClose,true);
    this->resize(1300,500);
    nameLabel = new QLabel(this);
    authorNameLabel = new QLabel(this);
    ISBNLabel = new QLabel(this);
    publisherNameLabel = new QLabel(this);
    classificationNumberLabel = new QLabel(this);
    classificationNameLabel = new QLabel(this);
    typeNumberLabel = new QLabel(this);
    priceLabel = new QLabel(this);
    introductionLabel = new QLabel(this);
    nameLineEdit = new QLineEdit(this);
    nameLineEdit->setFixedHeight(40);
    authorNameLineEdit = new QLineEdit(this);
    authorNameLineEdit->setFixedHeight(40);
    ISBNLineEdit = new QLineEdit(this);
    ISBNLineEdit->setFixedHeight(40);
    regx.setPattern("[0-9]+$");
    validator = new QRegExpValidator(regx,ISBNLineEdit);
    ISBNLineEdit->setValidator(validator);
    publisherLineEdit = new SearchLineEdit(this);
    publisherLineEdit->setFixedHeight(40);
    publisherLineEdit->searchButton->setObjectName("publisherSearchButton");
    classificationNumberLineEdit = new SearchLineEdit(this);
    classificationNumberLineEdit->setFixedHeight(40);
    classificationNumberLineEdit->searchButton->setObjectName("editing");
    classificationNameLineEdit = new QLineEdit(this);
    classificationNameLineEdit->setFocusPolicy(Qt::NoFocus);
    typeNumberLineEdit = new QLineEdit(this);
    typeNumberLineEdit->setFixedHeight(40);
    typeNumberLineEdit->setFocusPolicy(Qt::NoFocus);                                                    //设置为不可编辑
    priceLineEdit = new QLineEdit(this);
    priceLineEdit->setFixedHeight(40);
    regx.setPattern("^(([0-9]+\\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\\.[0-9]+)|([0-9]*[1-9][0-9]*))$");//正浮点数
    validator = new QRegExpValidator(regx,priceLineEdit);
    priceLineEdit->setValidator(validator);
    introductionEdit = new QTextEdit(this);
    introductionEdit->setFixedHeight(150);
    gridLayout = new QGridLayout;
    mainLayout = new QHBoxLayout;
    acceptButton = new QPushButton(this);
    rejectButton = new QPushButton(this);
    sameBookDistinctionTableWidget = new friendTableWidget(this);
    sameBookDistinctionTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    addButton = new QPushButton(this);
    changeButton = new QPushButton(this);
    deleteButton = new QPushButton(this);
    showWaitingQueueButton = new QPushButton(this);
    buttonLayout = new QHBoxLayout;
    rightLayout = new QVBoxLayout;

    addNewSameBookButton = new twoSizeButton("add_16px","add_24px",QSize(30,30),QSize(16,16),QSize(24,24),sameBookDistinctionTableWidget);
    addNewSameBookButton->move(0,0);

    gridLayout->addWidget(nameLabel,0,0);
    gridLayout->addWidget(nameLineEdit,0,1);
    gridLayout->addWidget(authorNameLabel,1,0);
    gridLayout->addWidget(authorNameLineEdit,1,1);
    gridLayout->addWidget(ISBNLabel,2,0);
    gridLayout->addWidget(ISBNLineEdit,2,1);
    gridLayout->addWidget(publisherNameLabel,3,0);
    gridLayout->addWidget(publisherLineEdit,3,1);
    gridLayout->addWidget(classificationNumberLabel,4,0);
    gridLayout->addWidget(classificationNumberLineEdit,4,1);
    gridLayout->addWidget(classificationNameLabel,5,0);
    gridLayout->addWidget(classificationNameLineEdit,5,1);
    gridLayout->addWidget(typeNumberLabel,6,0);
    gridLayout->addWidget(typeNumberLineEdit,6,1);
    gridLayout->addWidget(priceLabel,7,0);
    gridLayout->addWidget(priceLineEdit,7,1);
    gridLayout->addWidget(introductionLabel,8,0);
    gridLayout->addWidget(introductionEdit,8,1);
    gridLayout->addWidget(acceptButton,9,0);
    gridLayout->addWidget(rejectButton,9,1);
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(changeButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(showWaitingQueueButton);
    rightLayout->addWidget(sameBookDistinctionTableWidget);
    rightLayout->addLayout(buttonLayout);
    mainLayout->addLayout(gridLayout);
    mainLayout->setStretchFactor(gridLayout,1);
    mainLayout->addLayout(rightLayout);
    mainLayout->setStretchFactor(rightLayout,2.5);

    statusBar = new mainStatusBar(this);
    trueMainLayout = new QVBoxLayout;
    trueMainLayout->addLayout(mainLayout);
    trueMainLayout->addWidget(statusBar);

    setLayout(trueMainLayout);
    nameLabel->setText(tr("*书名"));
    authorNameLabel->setText(tr("*作者"));
    ISBNLabel->setText(tr("*ISBN"));
    publisherNameLabel->setText(tr("*出版社"));
    classificationNumberLabel->setText(tr("分类号"));
    classificationNameLabel->setText(tr("分类名"));
    typeNumberLabel->setText(tr("种次号"));
    priceLabel->setText(tr("价格"));
    introductionLabel->setText(tr("简介"));

    acceptButton->setText(tr("确定"));
    acceptButton->setFocusPolicy(Qt::NoFocus);
    rejectButton->setText(tr("取消"));
    rejectButton->setFocusPolicy(Qt::NoFocus);
    addButton->setText(tr("添加册"));
    addButton->setFocusPolicy(Qt::NoFocus);
    changeButton->setText(tr("修改册"));
    changeButton->setFocusPolicy(Qt::NoFocus);
    deleteButton->setText(tr("删除册"));
    deleteButton->setFocusPolicy(Qt::NoFocus);
    showWaitingQueueButton->setText(tr("显示等待队列"));
    showWaitingQueueButton->setFocusPolicy(Qt::NoFocus);

    connect(acceptButton,SIGNAL(clicked()),this,SLOT(submitData()));
    connect(rejectButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(publisherLineEdit->searchButton,SIGNAL(clicked()),mainWindow,SLOT(createPublisherViewWidget()));
    connect(classificationNumberLineEdit->searchButton,SIGNAL(clicked()),mainWindow,SLOT(createVarietiesManageViewWidget()));
    connect(mainWindow->publisherView,SIGNAL(publishInfo(Publisher)),this,SLOT(updatePublisherName(Publisher)));
    connect(addNewSameBookButton,SIGNAL(clicked()),this,SLOT(isAddItem()));

    connect(addButton,SIGNAL(clicked()),this,SLOT(isAddItem()));
    connect(changeButton,SIGNAL(clicked()),this,SLOT(isChangeItem()));
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(deleteData()));

    connect(classificationNumberLineEdit,SIGNAL(textChanged(QString)),this,SLOT(onClassificationNumberLineEditCurrentTextChanged(QString)));
    connect(mainWindow->bookVarietiesView,SIGNAL(bookClassificationInfo(QString,QString)),this,SLOT(changeClassificationInfo(QString,QString)));
    connect(classificationNumberLineEdit,SIGNAL(textChanged(QString)),this,SLOT(changeClassificationNameByClassificationNumber(QString)));
    connect(showWaitingQueueButton,SIGNAL(clicked()),this,SLOT(onShowWintingQueueButtonClicked()));
    statusBar->connectLater();
}
//新建时调用的构造函数
addBookBox::addBookBox(QWidget *parent):QWidget(parent)
{
    init();
    showWaitingQueueButton->setHidden(true);
    for(int i=0;i<101;i++)
        sameNumberHash[i]=false;
}

//修改时调用的构造函数
addBookBox::addBookBox(Book rec, QWidget *parent):QWidget(parent)
{
    init();
    waitingQueue=rec.reserveQueue;
    showWaitingQueueButton->setHidden(false);
    isChanging=true;
    classificationType=rec.getClassificationNumber()+"/"+rec.getTypeNumber();
    nameLineEdit->setText(rec.getName());
    authorNameLineEdit->setText(rec.getAuthorName());
    ISBNLineEdit->setText(rec.getISBN());
    publisherLineEdit->setText(rec.getPublisher().name);
    classificationNumberLineEdit->setText(rec.getClassificationNumber());
    originalClassificationNumber=rec.getClassificationNumber();
    typeNumberLineEdit->setText(rec.getTypeNumber());
    originalTypeNumber=rec.getTypeNumber().toInt();
    priceLineEdit->setText(QString("%1").arg(rec.getPrice()));
    introductionEdit->setText(rec.getIntroduction());
    sbdVector=rec.getSameBookDistinctionVector();
    sameBookDistinctionTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格为不可编辑
    sameBookDistinctionTableWidget->verticalHeader()->setMinimumWidth(30);
    sameBookDistinctionTableWidget->horizontalHeader()->setMinimumHeight(30);
    for(int i=0;i<101;i++)
        sameNumberHash[i]=false;
    for(int i=0;i<sbdVector.size();i++)
    {
        int order=sbdVector[i].sameNumber;
        sameNumberHash[order]=true;
    }
    updateSameBookDistinctionTableWidget();
}

//析构函数，删除各种申请的空间
addBookBox::~addBookBox()
{
    delete nameLabel;
    delete authorNameLabel;
    delete ISBNLabel;
    delete publisherNameLabel;
    delete classificationNumberLabel;
    delete classificationNameLabel;
    delete typeNumberLabel;
    delete priceLabel;
    delete nameLineEdit;
    delete authorNameLineEdit;
    delete ISBNLineEdit;
    delete publisherLineEdit;
    delete classificationNumberLineEdit;
    delete classificationNameLineEdit;
    delete typeNumberLineEdit;
    delete priceLineEdit;
    delete acceptButton;
    delete rejectButton;
    delete addNewSameBookButton;
    delete sameBookDistinctionTableWidget;
    delete addButton;
    delete changeButton;
    delete showWaitingQueueButton;
    delete deleteButton;
    delete gridLayout;
    delete mainLayout;
    delete statusBar;
    delete trueMainLayout;
}

//上交数据
void addBookBox::submitData()
{
    Book rec;
    Publisher pub;
    QString str;
    str=nameLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        nameLineEdit->setFocus();
        return;
    }
    rec.setName(str);
    str=authorNameLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        authorNameLineEdit->setFocus();
        return;
    }
    rec.setAuthorName(str);
    str=ISBNLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        ISBNLineEdit->setFocus();
        return;
    }
    rec.setISBN(str);
    str=publisherLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        publisherLineEdit->setFocus();
        return;
    }
    str=classificationNumberLineEdit->text().trimmed();
    if(str.isEmpty())
    {
        classificationNumberLineEdit->setFocus();
        return;
    }
    else
    {
        str=classificationNameLineEdit->text().trimmed();
        if(str=="该分类不存在，请添加！")
        {
            classificationNumberLineEdit->clear();
            classificationNameLineEdit->clear();
            classificationNumberLineEdit->setFocus();
            return;
        }
    }
    pub.name=str;
    rec.setPublisher(pub);
    str=classificationNumberLineEdit->text().trimmed();
    rec.setClassificationNumber(str);
    str=classificationNameLineEdit->text().trimmed();
    rec.setClassificationName(str);
    str=typeNumberLineEdit->text();
    rec.setTypeNumber(str);
    rec.setPrice(priceLineEdit->text().trimmed().toDouble());
    if(sbdVector.isEmpty())
    {
        QMessageBox message(QMessageBox::Warning,"警告","请至少添加一册书",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        sameBookDistinctionTableWidget->setFocus();
        return;
    }
    rec.setSameBookDistinctionVector(sbdVector);
    rec.setIntroduction(introductionEdit->toPlainText());
    rec.setTypeNumber(QString("%1").arg(mainWindow->bookView->classificationToTypeNumberCount[rec.getClassificationNumber()]));
    mainWindow->bookView->classificationToTypeNumberCount[rec.getClassificationNumber()]++;
    canOpen=true;
    emit newData(rec);
    this->close();
}
//更新出版社名字
void addBookBox::updatePublisherName(Publisher pub)
{
    publisherLineEdit->setText(pub.name);
}

//检测快捷键
void addBookBox::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers()==Qt::ControlModifier&&e->key()==Qt::Key_S)
        emit acceptButton->clicked(true);
    QWidget::keyPressEvent(e);
}
//增加同类的书
void addBookBox::addNewSameBook()
{
    int i;
    bool hash[101];
    SameBookDistinction tmpSBD;
    for(i=0;i<101;i++)
        hash[i]=false;
    for(i=0;i<sbdVector.size();i++)
        hash[sbdVector[i].sameNumber]=true;
    for(i=0;hash[i];i++);
    tmpSBD.sameNumber=i;
    tmpSBD.addDate=mytime.getDate();
    sbdVector.push_back(tmpSBD);
    updateSameBookDistinctionTableWidget();
}

//更改同一类书不同书的借阅情况
void addBookBox::updateSameBookDistinctionTableWidget()
{
    sameBookDistinctionTableWidget->clear();
    QStringList tmpList;
    tmpList<<"序号"<<"是否被预约"<<"预约者"<<"是否被外借"<<"借阅者"<<"出版日期"<<"添置日期"<<"索书号";
    sameBookDistinctionTableWidget->setColumnCount(8);
    sameBookDistinctionTableWidget->setHorizontalHeaderLabels(tmpList);
    sameBookDistinctionTableWidget->setRowCount(sbdVector.size());
    QTableWidgetItem *tmpItem;
    for(int i=0;i<sbdVector.size();i++)
    {
        int order=sbdVector[i].sameNumber;
        tmpItem = new QTableWidgetItem(QString("%1").arg(order));
        sameBookDistinctionTableWidget->setItem(i,0,tmpItem);
        if(sbdVector[i].isReserved)
        {
            tmpItem = new QTableWidgetItem("是");
            sameBookDistinctionTableWidget->setItem(i,1,tmpItem);
            tmpItem = new QTableWidgetItem(sbdVector[i].reserverID);
            sameBookDistinctionTableWidget->setItem(i,2,tmpItem);
        }
        else
        {
            tmpItem = new QTableWidgetItem("否");
            sameBookDistinctionTableWidget->setItem(i,1,tmpItem);
            tmpItem = new QTableWidgetItem("略");
            sameBookDistinctionTableWidget->setItem(i,2,tmpItem);
        }
        if(sbdVector[i].isBorrowed)
        {
            tmpItem = new QTableWidgetItem("是");
            sameBookDistinctionTableWidget->setItem(i,3,tmpItem);
            tmpItem = new QTableWidgetItem(sbdVector[i].borrowerID);
            sameBookDistinctionTableWidget->setItem(i,4,tmpItem);
        }
        else
        {
            tmpItem = new QTableWidgetItem("否");
            sameBookDistinctionTableWidget->setItem(i,3,tmpItem);
            tmpItem = new QTableWidgetItem("略");
            sameBookDistinctionTableWidget->setItem(i,4,tmpItem);
        }
        QDate tmpDate;
        tmpDate=sbdVector[i].publishDate;
        tmpItem = new QTableWidgetItem(myDateToString(tmpDate));
        sameBookDistinctionTableWidget->setItem(i,5,tmpItem);
        tmpDate=sbdVector[i].addDate;
        tmpItem = new QTableWidgetItem(myDateToString(tmpDate));
        sameBookDistinctionTableWidget->setItem(i,6,tmpItem);
        classificationType=classificationNumberLineEdit->text().trimmed()+"/"+typeNumberLineEdit->text().trimmed();
        QString str=classificationType+"."+QString("%1").arg(sbdVector[i].sameNumber);
        tmpItem = new QTableWidgetItem(str);
        sameBookDistinctionTableWidget->setItem(i,7,tmpItem);
    }
}

//判断是否增加
void addBookBox::isChangeItem()
{
    QModelIndex mIndex=sameBookDistinctionTableWidget->currentIndex();
    if(!mIndex.isValid()||mIndex.row()>=sbdVector.size()||mIndex.row()<0)
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        sameBookDistinctionTableWidget->clearFocus();
        return;
    }
    currentRow=mIndex.row();
    QStringList strList;
    addOrChangeItem=1;
    strList<<QString("%1").arg(sbdVector[currentRow].sameNumber)<<myDateToString(sbdVector[currentRow].publishDate);
    changeSameBookDistinctionItem *inputDialog = new changeSameBookDistinctionItem(strList);
    connect(inputDialog,SIGNAL(newData(QStringList)),this,SLOT(addData(QStringList)));
    inputDialog->show();
}

//增加一本书
void addBookBox::isAddItem()
{
    int i;
    QStringList strList;
    for(i=0;sameNumberHash[i];i++);
    strList<<QString("%1").arg(i)<<"";
    addOrChangeItem=0;
    changeSameBookDistinctionItem *inputDialog = new changeSameBookDistinctionItem(strList);
    connect(inputDialog,SIGNAL(newData(QStringList)),this,SLOT(addData(QStringList)));
    inputDialog->show();
}

//增加数据
void addBookBox::addData(QStringList strList)
{
    if(addOrChangeItem==1)
    {
        QString str=strList.at(0);
        int order=str.toInt();
        sameNumberHash[order]=true;
        sbdVector[currentRow].sameNumber=order;
        str=strList.at(1);
        sbdVector[currentRow].publishDate=myToDate(str);
    }
    else
    {
        QString str;
        SameBookDistinction tmpSBD;
        tmpSBD.addDate=mytime.getDate();
        str=strList.at(0);
        int order=str.toInt();
        sameNumberHash[order]=true;
        tmpSBD.sameNumber=order;
        tmpSBD.publishDate=myToDate(strList.at(1));
        sbdVector.push_back(tmpSBD);
    }
    sortByOrder();
    updateSameBookDistinctionTableWidget();
}

//删除数据
void addBookBox::deleteData()
{
    QModelIndex mIndex=sameBookDistinctionTableWidget->currentIndex();
    if(!mIndex.isValid()||mIndex.row()>=sbdVector.size()||mIndex.row()<0)
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        sameBookDistinctionTableWidget->clearFocus();
        return;
    }
    QModelIndexList mIndexList=sameBookDistinctionTableWidget->selectedIndexes();
        int order=mIndexList[0].data().toInt();
        QString reserveStr,borrowStr;
        reserveStr=mIndexList[1].data().toString();
        borrowStr=mIndexList[3].data().toString();
        if(reserveStr=="是")
        {
            QMessageBox message(QMessageBox::Warning,"警告","该册已被预约，无法删除",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        else if(borrowStr=="是")
        {
            QMessageBox message(QMessageBox::Warning,"警告","该册已被借阅，无法删除",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        sbdVector.erase(sbdVector.begin()+mIndexList[0].row());
        sameNumberHash[order]=false;
        updateSameBookDistinctionTableWidget();
}

//比较函数
bool sameBookDistinctionCompare(SameBookDistinction a,SameBookDistinction b)
{
    return a.sameNumber<b.sameNumber;
}

//排序
void addBookBox::sortByOrder()
{
    sort(sbdVector.begin(),sbdVector.end(),sameBookDistinctionCompare);
    updateSameBookDistinctionTableWidget();
}

//关闭前需要做的事
void addBookBox::closeEvent(QCloseEvent *e)
{
    if(windowHash[3])
        windowPointer[3]->close();
    if(windowHash[18])
        windowPointer[18]->close();
    windowHash[2]=false;
    isChanging=false;
    if(!mainWindow->bookView->isOpen&&this->canOpen)
    {
        canOpen=false;
        mainWindow->bookView->show();
        mainWindow->bookView->tableView->verticalScrollBar()->setValue(mainWindow->bookView->tableView->verticalScrollBar()->maximum());
    }
    QWidget::closeEvent(e);
}

//显示前需要做的事
void addBookBox::showEvent(QShowEvent *e)
{
    statusBar->readerInformationButton->setHidden(true);
    statusBar->readerInformButton->setHidden(true);
    windowHash[2]=true;
    windowPointer[2]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}

//
void addBookBox::onClassificationNumberLineEditCurrentTextChanged(QString str)
{
    if(str=="")
    {
        typeNumberLineEdit->clear();
        return;
    }
    if(isChanging&&str==originalClassificationNumber)
    {
        typeNumberLineEdit->setText(QString("%1").arg(originalTypeNumber));
        updateSameBookDistinctionTableWidget();
    }
    else
    {
        int num=mainWindow->bookView->classificationToTypeNumberCount[str];
        typeNumberLineEdit->setText(QString("%1").arg(num));
        updateSameBookDistinctionTableWidget();
    }
}

//改变分类信息
void addBookBox::changeClassificationInfo(QString classificationNumber, QString classificationName)
{
    Q_UNUSED(classificationName)
    classificationNumberLineEdit->setText(classificationNumber);
}

//
void addBookBox::onShowWintingQueueButtonClicked()
{
    waitingQueueTableWidget *waitingQueuetableWidgetOfThisReader = new waitingQueueTableWidget;
    waitingQueuetableWidgetOfThisReader->updateData(waitingQueue);
    waitingQueuetableWidgetOfThisReader->show();
}

//打开分类文件并搜索
void addBookBox::changeClassificationNameByClassificationNumber(QString classificationNumber)
{
    if(classificationNumber=="")
    {
        classificationNameLineEdit->clear();
        return;
    }
    QFile file("../dat/library classify.txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug()<<"open file error addnewbook.cpp"<<endl;
    }
    QTextStream in(&file);
    in.seek(0);
    QString str1,str2;
    bool flag=false;
    while(!in.atEnd())
    {
        in>>str1>>str2;
        if(str1.size()==1&&str1==classificationNumber)
        {
            flag=true;
            classificationNameLineEdit->setText(str2);
            break;
        }
        else
        {
            str1.remove(0,1);
            if(str1==classificationNumber)
            {
                flag=true;
                classificationNameLineEdit->setText(str2);
                break;
            }
        }
    }
    if(!flag)
    {
        classificationNameLineEdit->setText("该分类不存在，请添加！");
    }
    file.close();
}
