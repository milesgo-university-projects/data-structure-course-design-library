/******************************************************************
文件名: bookviewwidget.cpp
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/25
描　述: 书籍的添加、更新和删除等管理
版　本: v5.00 Copyright(c).
******************************************************************/
#include "bookviewwidget.h"
#include "widget.h"
#include <QApplication>
#include <QVector>
#include <QModelIndexList>
#include <QDebug>
#include <logs.h>
struct SameBookDistinction;
bookViewWidget::bookViewWidget(QWidget *parent)
    : QWidget(parent)
{//布局的定义以及相关显示 以及进行信号与槽的关联
    initTypeNumberHash();
    this->setWindowTitle("图书管理");
    this->setObjectName("outsider");
    this->setFixedSize(1400,600);
    isBorrowing=false;
    isReserving=false;
    isReaderLooking=false;
    addItem = new QPushButton(this);
    deleteItem = new QPushButton(this);
    search = new QPushButton(this);
    showAll = new QPushButton(this);
    select = new QPushButton(this);
    change = new QPushButton(this);
    exportItems = new QPushButton(this);
    lookButton = new QPushButton(this);
    reserveButton = new QPushButton(this);

    tableView = new friendTableView(this);
    searchEdit = new QLineEdit(this);
    upButtonLayout = new QHBoxLayout;
    downButtonLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;
    tablemodel = new tableModel;
    proxymodel = new QSortFilterProxyModel;
    comboBox = new comboBoxWithDropDown(this);

    statusBar = new mainStatusBar(this);

    tableView->setObjectName("tableView");

    comboBox->addItem("行数");
    comboBox->addItem("书名");
    comboBox->addItem("作者");
    comboBox->addItem("ISBN");
    comboBox->addItem("出版社");
    comboBox->addItem("分类号");
    comboBox->addItem("分类名");
    comboBox->addItem("种次号");
    comboBox->addItem("价格");
    comboBox->addItem("借阅率%");
    comboBox->setCurrentText("书名");

    QStringList list;
    list<<"书名"<<"作者"<<"ISBN"<<"出版社"<<"分类号"<<"分类名"<<"种次号"<<"价格"<<"借阅率%";
    tablemodel->setHeaderName(list);
    headerName=list;

    addItem->setText(tr("新建"));
    addItem->setFocusPolicy(Qt::NoFocus);
    deleteItem->setText(tr("删除"));
    deleteItem->setFocusPolicy(Qt::NoFocus);
    search->setText(tr("搜索"));
    search->setFocusPolicy(Qt::NoFocus);

    showAll->setText(tr("显示全部"));
    showAll->setFocusPolicy(Qt::NoFocus);
    select->setText(tr("选择"));
    select->setFocusPolicy(Qt::NoFocus);
    change->setText(tr("修改"));
    change->setFocusPolicy(Qt::NoFocus);
    exportItems->setText(tr("导出"));
    exportItems->setFocusPolicy(Qt::NoFocus);

    lookButton->setText("查看");
    lookButton->setFocusPolicy(Qt::NoFocus);
    reserveButton->setText("预约");
    reserveButton->setFocusPolicy(Qt::NoFocus);

    upButtonLayout->addWidget(searchEdit);
    upButtonLayout->addWidget(search);
    upButtonLayout->addWidget(comboBox);
    upButtonLayout->addWidget(showAll);
    downButtonLayout->addWidget(lookButton);
    downButtonLayout->addWidget(reserveButton);
    downButtonLayout->addWidget(addItem);
    downButtonLayout->addWidget(deleteItem);
    downButtonLayout->addWidget(select);
    downButtonLayout->addWidget(change);
    downButtonLayout->addWidget(exportItems);
    mainLayout->addLayout(upButtonLayout);
    mainLayout->addWidget(tableView);
    mainLayout->addLayout(downButtonLayout);
    mainLayout->addWidget(statusBar);
    setLayout(mainLayout);

    proxymodel->setSourceModel(tablemodel);
    tableView->setModel(proxymodel);
    tableView->setSortingEnabled(true);
    tableView->sortByColumn(0,Qt::AscendingOrder);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setColumnWidth(0,50);
    tableView->setColumnWidth(1,320);
    tableView->setColumnWidth(2,150);
    tableView->setColumnWidth(3,130);
    tableView->setColumnWidth(4,140);
    tableView->setColumnWidth(5,150);
    tableView->setColumnWidth(6,103);
    tableView->setColumnWidth(7,100);
    tableView->setColumnWidth(8,100);

    QFile file("../dat/book.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error bookviewwidget.cpp 1";
    QTextStream in(&file);
    QStringList tmpList;
    QString str;
    class Book rec;
    SameBookDistinction sbd;
    Publisher pub;
    pub.location="";
    pub.ISBN="";
    while(!in.atEnd())
    {//将文件中的信息读到容器中
        rec.clearSameBookDistinction();
        while(!rec.reserveQueue.isEmpty())
            rec.reserveQueue.pop_front();
        for(int i=0;i<8;i++)
        {
            in>>str;
            tmpList.append(str);
        }
        in>>str;
        int sameBookNum=str.toInt();
        for(int i=0;i<sameBookNum;i++)
        {
            in>>str;
            sbd.sameNumber=str.toInt();

            sbd.isReserved=false;
            sbd.isBorrowed=false;

            in>>str;
            sbd.addDate=myToDate(str);

            in>>str;
            sbd.publishDate=myToDate(str);

            in>>str;
            sbd.everBorrowedTimes=str.toInt();

            rec.addSameBookDistinction(sbd);
        }
        rec.setName(tmpList.at(0));
        rec.setAuthorName(tmpList.at(1));
        rec.setISBN(tmpList.at(2));
        pub.name=tmpList.at(3);
        rec.setPublisher(pub);
        rec.setClassificationNumber(tmpList.at(4));
        rec.setClassificationName(tmpList.at(5));
        rec.setTypeNumber(tmpList.at(6));
        str=tmpList.at(7);
        rec.setPrice(str.toDouble());
        tmpList.clear();
        in>>str;
        int bound=str.toInt();
        for(int i=0;i<bound;i++)
        {
            in>>str;
            rec.reserveQueue.push_back(str.toInt());
        }
        rec.setIntroduction(getIntroductionContents(rec.getISBN()));
        recordlister.append(rec);
    }
    file.close();
    tablemodel->updateData(recordlister);//别忘了update data of model
    connect(tablemodel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(updateFileData(QModelIndex)));
    connect(addItem,SIGNAL(clicked()),this,SLOT(isAddItem()));//点击新建按钮
    connect(deleteItem,SIGNAL(clicked()),this,SLOT(deleteData()));//点击删除按钮
    connect(search,SIGNAL(clicked()),this,SLOT(scrollFocusTo()));//点击搜索按钮
    connect(showAll,SIGNAL(clicked()),this,SLOT(showAllRows()));//点击显示全部按钮
    connect(select,SIGNAL(clicked()),this,SLOT(sendBookInfo()));
    connect(change,SIGNAL(clicked()),this,SLOT(isChangeItem()));//点击修改按钮
    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(isChangeItem()));//双击信号
    connect(exportItems,SIGNAL(clicked()),this,SLOT(excelExport()));
    connect(searchEdit,SIGNAL(returnPressed()),search,SIGNAL(clicked()));//在搜索框内按下回车，自动按下搜索键
    connect(lookButton,SIGNAL(clicked()),this,SLOT(isChangeItem()));
    connect(reserveButton,SIGNAL(clicked()),this,SLOT(onReserveButtonClicked()));
    connect(tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(onItemClicked()));
    connect(&mytime,SIGNAL(dateChanged(int,int,int)),this,SLOT(onDateChanged(int,int,int)));

    statusBar->connectLater();
}

bookViewWidget::~bookViewWidget()
{
    delete addItem;
    delete deleteItem;
    delete tableView;
    delete tablemodel;
    delete proxymodel;
    delete searchEdit;
    delete search;
    delete showAll;
    delete comboBox;
    delete select;
    delete change;
    delete exportItems;
    delete lookButton;
    delete reserveButton;
    delete downButtonLayout;
    delete statusBar;
    delete mainLayout;
}

void bookViewWidget::updateFileData(QModelIndex index)
{
    class Book record;
    record=tablemodel->pList.at(index.row());
    recordlister.replace(index.row(),record);
}

void bookViewWidget::flushFile()
{//保存到文件
    int allbookscount=0;
    QFile file("../dat/book.txt");
    QFile file1("../dat/bookcount.txt");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        qDebug()<<"open file error bookviewwidget.cpp 2";
    if(!file1.open(QIODevice::WriteOnly|QIODevice::Text))
        qDebug()<<"open file error bookviewwidget.cpp 2";
    QTextStream out(&file);
    QTextStream out1(&file1);
    int i,j;
    for(i=0;i<recordlister.count()-1;i++)
    {
        QVector<SameBookDistinction> sbdVector=recordlister[i].getSameBookDistinctionVector();
        out<<recordlister[i].getName()<<"   "<<recordlister[i].getAuthorName()<<"   "<<recordlister[i].getISBN()
        <<" "<<recordlister[i].getPublisher().name<<"   "<<recordlister[i].getClassificationNumber()<<" "<<recordlister[i].getClassificationName()
        <<" "<<recordlister[i].getTypeNumber()<<"   "<<recordlister[i].getPrice()<<"    "<<sbdVector.size()<<endl;
        allbookscount+=sbdVector.size();
        for(j=0;j<sbdVector.size();j++)
        {
            out<<sbdVector[j].sameNumber<<" ";
            out<<myDateToString(sbdVector[j].addDate)<<"    ";
            out<<myDateToString(sbdVector[j].publishDate)<<"    "<<sbdVector[j].everBorrowedTimes<<endl;
        }
        QQueue<int> tmpQueue=recordlister[i].reserveQueue;
        if(tmpQueue.isEmpty())
            out<<"0"<<endl;
        else
        {
            out<<tmpQueue.size()<<endl;
            while(tmpQueue.size())
            {
                int tmp=tmpQueue.front();
                tmpQueue.pop_front();
                out<<tmp<<" ";
            }
            out<<endl;
        }
        saveIntroductionContents(recordlister[i].getIntroduction(),recordlister[i].getISBN());
    }
    if(recordlister.size())
    {
        QVector<SameBookDistinction> sbdVector=recordlister[i].getSameBookDistinctionVector();
        out<<recordlister[i].getName()<<" "<<recordlister[i].getAuthorName()<<" "<<recordlister[i].getISBN()
        <<" "<<recordlister[i].getPublisher().name<<" "<<recordlister[i].getClassificationNumber()<<" "<<recordlister[i].getClassificationName()
        <<" "<<recordlister[i].getTypeNumber()<<" "<<recordlister[i].getPrice()<<" "<<sbdVector.size()<<endl;
        allbookscount+=sbdVector.size();
        for(j=0;j<sbdVector.size();j++)
        {
            out<<sbdVector[j].sameNumber<<" ";
            out<<myDateToString(sbdVector[j].addDate)<<"    ";
            out<<myDateToString(sbdVector[j].publishDate)<<"    "<<sbdVector[j].everBorrowedTimes<<endl;
        }
        QQueue<int> tmpQueue=recordlister[i].reserveQueue;
        if(tmpQueue.isEmpty())
            out<<"0";
        else
        {
            out<<tmpQueue.size()<<endl;
            while(tmpQueue.size())
            {
                int tmp=tmpQueue.front();
                tmpQueue.pop_front();
                out<<tmp<<" ";
            }
        }
        saveIntroductionContents(recordlister[i].getIntroduction(),recordlister[i].getISBN());
    }
    out1<<allbookscount<<endl;
    statisticdata.today.bookNums=allbookscount;
    file.close();
    file1.close();
}


void bookViewWidget::isAddItem()
{//点击新建按钮后，创建输入框
    addOrChangeItem=0;
    addBookBox *inputDialog = new addBookBox;
    connect(inputDialog,SIGNAL(newData(Book)),this,SLOT(addData(Book)));
    inputDialog->show();
}

void bookViewWidget::isChangeItem()
{
    //点击修改按钮后，创建输入框
    int row;
    QModelIndexList mIndexList=tableView->selectedIndexes();
    if(mIndexList.isEmpty())
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    currentRow=row=mIndexList[0].data().toInt()-1;
    addOrChangeItem=1;
    class Book rec=recordlister.at(row);
    addBookBox *inputDialog = new addBookBox(rec);//调用另外一个构造函数，以初始化输入框信息
    if(!isReaderLooking)
    {
        connect(inputDialog,SIGNAL(newData(Book)),this,SLOT(addData(Book)));
    }
    else
    {
        inputDialog->addNewSameBookButton->setHidden(true);
        inputDialog->addButton->setHidden(true);
        inputDialog->changeButton->setHidden(true);
        inputDialog->deleteButton->setHidden(true);
        inputDialog->acceptButton->setHidden(true);
        inputDialog->rejectButton->setHidden(true);
        inputDialog->publisherLineEdit->searchButton->setHidden(true);
        inputDialog->classificationNumberLineEdit->searchButton->setHidden(true);
        inputDialog->showWaitingQueueButton->setHidden(true);
        inputDialog->sameBookDistinctionTableWidget->setColumnHidden(2,true);
        inputDialog->sameBookDistinctionTableWidget->setColumnHidden(4,true);
        inputDialog->nameLineEdit->setFocusPolicy(Qt::NoFocus);
        inputDialog->authorNameLineEdit->setFocusPolicy(Qt::NoFocus);
        inputDialog->ISBNLineEdit->setFocusPolicy(Qt::NoFocus);
        inputDialog->classificationNumberLineEdit->setFocusPolicy(Qt::NoFocus);
        inputDialog->priceLineEdit->setFocusPolicy(Qt::NoFocus);
        inputDialog->introductionEdit->setFocusPolicy(Qt::NoFocus);
    }
    inputDialog->show();
}

void bookViewWidget::addData(Book booker)
{
        if(addOrChangeItem==0)//如果是新建，就直接append增加
        {
            Logs::AddBook(booker.getName(),booker.getAuthorName(),booker.getISBN(),booker.getClassificationNumber(),booker.getClassificationName(),booker.getTypeNumber(),booker.getPrice(),booker.getPublisher().name,booker.getPublisher().ISBN,booker.getIntroduction());
            recordlister.append(booker);
            currentRow=recordlister.size();
        }
        else//如果是修改，就replace替换
        {
            Logs::ReplaceBook(booker.getName(),booker.getAuthorName(),booker.getISBN(),booker.getClassificationNumber(),booker.getClassificationName(),booker.getTypeNumber(),booker.getPrice(),booker.getPublisher().name,booker.getPublisher().ISBN,booker.getIntroduction());
            recordlister.replace(currentRow,booker);
        }
        tablemodel->updateData(recordlister);//更新数据
        flushFile();
}
#include <QDebug>
void bookViewWidget::deleteData()
{//删除
        QModelIndexList mIndexList=tableView->selectedIndexes();
        if(mIndexList.isEmpty())
        {
            QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        int row=mIndexList[0].data().toInt()-1;
        if(recordlister[row].getAllIsBorrowed()||recordlister[row].getAllIsReserved())
        {
            QMessageBox message(QMessageBox::Warning,"警告","该书被借阅/预约，无法删除",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        for(int i=0;i<mainWindow->bookBorrowOrReserveView->recordlister.size();i++)
        {
            BorrowOrReserve rec=mainWindow->bookBorrowOrReserveView->recordlister[i];
            if(rec.bookPos>row)
            {
                mainWindow->readerView->recordlister[rec.readerPos].updateBookPosDueToBookDelete(row);
                mainWindow->bookBorrowOrReserveView->recordlister[i].bookPos--;
            }
        }
        Logs::DeleBook(mainWindow->bookView->recordlister[row].getName(),mainWindow->bookView->recordlister[row].getAuthorName(),mainWindow->bookView->recordlister[row].getISBN(),mainWindow->bookView->recordlister[row].getClassificationNumber(),mainWindow->bookView->recordlister[row].getClassificationName(),mainWindow->bookView->recordlister[row].getTypeNumber(),mainWindow->bookView->recordlister[row].getPrice(),mainWindow->bookView->recordlister[row].getPublisher().name,mainWindow->bookView->recordlister[row].getPublisher().ISBN,mainWindow->bookView->recordlister[row].getIntroduction());
        recordlister.erase(recordlister.begin()+row);
        tablemodel->updateData(recordlister);
        flushFile();
        mainWindow->bookBorrowOrReserveView->tablemodel->updateData(mainWindow->bookBorrowOrReserveView->recordlister);
        mainWindow->bookBorrowOrReserveView->flushFile();
        mainWindow->readerView->tablemodel->updateData(mainWindow->readerView->recordlister);
        mainWindow->readerView->flushFile();
}

void bookViewWidget::scrollFocusTo()
{//设置搜索哪一列，comboBox的值设置
    QVariant str=searchEdit->text().trimmed();
    QString st=comboBox->currentText();
    if(st=="行数")
    {
        if(str.canConvert(QMetaType::Int))
        {
            tableView->verticalScrollBar()->setValue(str.toInt()-1);//定位到这一行
        }
    }
    else
    {
        if(st=="书名")
        {
            proxymodel->setFilterKeyColumn(1);
        }
        else if(st=="作者")
        {
            proxymodel->setFilterKeyColumn(2);
        }
        else if(st=="ISBN")
        {
            proxymodel->setFilterKeyColumn(3);
        }
        else if(st=="出版社")
        {
            proxymodel->setFilterKeyColumn(4);
        }
        else if(st=="分类号")
        {
            proxymodel->setFilterKeyColumn(5);
        }
        else if(st=="分类名")
        {
            proxymodel->setFilterKeyColumn(6);
        }
        else if(st=="种次号")
        {
            proxymodel->setFilterKeyColumn(7);
        }
        else if(st=="价格")
        {
            proxymodel->setFilterKeyColumn(8);
        }
        else if(st=="借阅率%")
        {
            proxymodel->setFilterKeyColumn(9);
        }
        filterChanged(str.toString());
    }
}

void bookViewWidget::showAllRows()
{//显示全部，就相当于正则表达式为空，就全部显示出来了
    filterChanged("");
}

void bookViewWidget::sendBookInfo()
{
    QVector<int> tmpVector;
    QModelIndexList mIndexList=tableView->selectedIndexes();
    if(mIndexList.isEmpty())
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    int row=mIndexList[0].data().toInt()-1;
    int i;
    QVector<SameBookDistinction> tmpSbdVector=recordlister[row].getSameBookDistinctionVector();
    for(i=0;i<tmpSbdVector.size();i++)
    {
        if(!tmpSbdVector[i].isReserved&&!tmpSbdVector[i].isBorrowed)
            break;
    }
    if(i==tmpSbdVector.size())//说明没有书可借，则加入等待队列，加入等待队列前要先检查队列中是否已有该读者
    {
        QMessageBox message(QMessageBox::Warning,"警告","该书已被全部借阅/预约，是否加入等待队列？",QMessageBox::Yes|QMessageBox::No,NULL);
        message.setButtonText(QMessageBox::Yes,"是");
        message.setButtonText(QMessageBox::No,"否");
        if(message.exec()==QMessageBox::Yes)
        {//加入等待队列
            emit bookWait(row);
            this->close();
        }
        return;
    }
    tmpVector.push_back(row);
    tmpVector.push_back(tmpSbdVector[i].sameNumber);
    emit bookInfo(tmpVector);
    this->close();
}


void bookViewWidget::filterChanged(QString str)
{
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(QRegExp::FixedString);
    QRegExp regExp(str, Qt::CaseInsensitive, syntax);
    proxymodel->setFilterRegExp(regExp);
    tableView->verticalScrollBar()->setValue(0);//滚动条滚至顶端
}

void bookViewWidget::excelExport()
{
    QString filepath=QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
                                                  tr("Excel Files (*.xls)"));
    ExportExcelObject temp(filepath,tr("图书信息"),tableView);
    temp.addField(0,"行数","char(20)");
    for(int i=0;i<headerName.size();i++)
    {
        temp.addField(i+1,headerName.at(i),"char(200)");
    }
    temp.export2Excel();
}

void bookViewWidget::keyPressEvent(QKeyEvent *e)
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

QString bookViewWidget::getIntroductionContents(QString isbn)
{
    QString location;
    location.append("../dat/introduction/");
    location.append(isbn);
    location.append(".txt");
    QFile file(location);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text))
    {
        qDebug()<<"open file error bookviewwidget.cpp 3";
        return "";
    }
    QTextStream in(&file);
    return in.readAll();
}

void bookViewWidget::saveIntroductionContents(QString info,QString isbn)
{
    QString location;
    location.append("../dat/introduction/");
    location.append(isbn);
    location.append(".txt");
    QFile file(location);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"open file error bookviewwidget.cpp 4";
        return;
    }
    QTextStream out(&file);
    out<<info;
}
#include <QDebug>
void bookViewWidget::closeEvent(QCloseEvent *e)
{
    if(windowHash[3])
    {
        windowPointer[3]->close();
    }
    windowHash[1]=false;
    isOpen=false;
    this->select->setEnabled(false);
    isBorrowing=false;
    isReserving=false;
    isReaderLooking=false;
    flushFile();
    flushTypeNumberHashFile();
    e->accept();
    QWidget::closeEvent(e);
}

void bookViewWidget::showEvent(QShowEvent *e)
{
    statusBar->readerInformationButton->setHidden(true);
    statusBar->readerInformButton->setHidden(true);
    if(!(isBorrowing||isReserving||isReaderLooking))
    {
        searchEdit->setText("");
        filterChanged("");
        comboBox->setCurrentText("书名");
    }
    if(isReaderLooking)
    {
        lookButton->setHidden(false);
        reserveButton->setHidden(false);
        addItem->setHidden(true);
        deleteItem->setHidden(true);
        select->setHidden(true);
        change->setHidden(true);
    }
    else
    {
        lookButton->setHidden(true);
        reserveButton->setHidden(true);
        addItem->setHidden(false);
        deleteItem->setHidden(false);
        select->setHidden(false);
        change->setHidden(false);
    }
    windowHash[1]=true;
    windowPointer[1]=qobject_cast<QWidget *>(this);
    isOpen=true;
    QWidget::showEvent(e);
}
//bug
void bookViewWidget::initTypeNumberHash()
{
    QFile file("../dat/typeNumberHash.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug()<<"open file error bookviewwidget.cpp";
        return;
    }
    QTextStream in(&file);
    QString str1,str2;
    while(!in.atEnd())
    {
        in>>str1>>str2;
        classificationToTypeNumberCount[str1]=str2.toInt();
        //qDebug()<<str1<<str2<<endl;
    }
    file.close();
}
//bug
void bookViewWidget::flushTypeNumberHashFile()
{
    QFile file("../dat/typeNumberHash.txt");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"open file error bookviewwidget.cpp";
        return;
    }
    QTextStream out(&file);
    QMap<QString,int>::iterator it;
    if(classificationToTypeNumberCount.size())
    {
        it=classificationToTypeNumberCount.begin();
        while(it.value()==0&&it!=classificationToTypeNumberCount.end())
            it++;
        if(it!=classificationToTypeNumberCount.end())
        {
            out<<it.key()<<"    "<<it.value();
            for(it++;it!=classificationToTypeNumberCount.end();it++)
            {
                if(it.value()!=0)
                    out<<endl<<it.key()<<"  "<<it.value();
            }
        }
    }
    file.close();
}

void bookViewWidget::updateBorrowAndReserveInfo(BorrowOrReserve rec)
{
    SameBookDistinction tmpSBD;
    int bookPos=rec.bookPos;
    int sameNumber=rec.bookSameNumber;
    if(rec.borrowOrReserveFlag)
    {
        tmpSBD=recordlister[bookPos].getSameBookDistinction(sameNumber);
        tmpSBD.isBorrowed=true;
        tmpSBD.isReserved=false;
        tmpSBD.borrowerID=mainWindow->readerView->recordlister[rec.readerPos].getId();
        recordlister[bookPos].replaceSameBookDistinction(tmpSBD,sameNumber);
    }
    else
    {
        tmpSBD=recordlister[bookPos].getSameBookDistinction(sameNumber);
        tmpSBD.isReserved=true;
        tmpSBD.isBorrowed=false;
        tmpSBD.reserverID=mainWindow->readerView->recordlister[rec.readerPos].getId();
        recordlister[bookPos].replaceSameBookDistinction(tmpSBD,sameNumber);
    }
    tablemodel->updateData(recordlister);
    flushFile();
}

void bookViewWidget::onReserveButtonClicked()
{
    QModelIndexList mIndexList=tableView->selectedIndexes();
    if(mIndexList.size())
    {
        if(mainWindow->readerView->recordlister[mainWindow->currentReaderPos].getFeeAccount()<0)
        {
            QMessageBox message(QMessageBox::Warning,"警告","您的余额不足，请尽快充值，否则将无法借阅或预约",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        int bookPos=mIndexList[0].data().toInt()-1;
        if(recordlister[bookPos].checkThisReaderHasBorrowedOrReserved(mainWindow->readerView->recordlister[mainWindow->currentReaderPos].getId()))
        {
            QMessageBox message(QMessageBox::Warning,"警告","您已借阅或预约该书",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        int pos;
        if(recordlister[bookPos].checkWhetherThisReaderInReserveQueue(mainWindow->currentReaderPos,pos))
        {
            QMessageBox message(QMessageBox::Warning,"警告",QString("您已加入该书的提前预约等待队列，当前位序%1").arg(pos),QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        int auth=mainWindow->readerView->recordlister[mainWindow->currentReaderPos].getAuth();
        int authPos=mainWindow->authorityView->findAuthPos(auth);
        int bookAmountsLimit;
        if(authPos!=-1)
            bookAmountsLimit=mainWindow->authorityView->recordlister[authPos].getBorrowBookAmountsLimit();
        else
            bookAmountsLimit=5;
        int bookAlreadyBorrowedAmounts=mainWindow->readerView->recordlister[mainWindow->currentReaderPos].getBookBorrowOrReserveVector().size();
        if(bookAlreadyBorrowedAmounts>=bookAmountsLimit)
        {
            QMessageBox message(QMessageBox::Warning,"警告","该读者借阅/预约量已达上限",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        int sameNumber;
        if(!recordlister[bookPos].isThereBookAvailable(mainWindow->readerView->recordlister[mainWindow->currentReaderPos].getId(),sameNumber))
        {
            QMessageBox message(QMessageBox::Warning,"警告","该书没有空余，是否加入提前预约等待队列？",QMessageBox::Yes|QMessageBox::No,NULL);
            message.setButtonText(QMessageBox::Yes,"是");
            message.setButtonText(QMessageBox::No,"否");
            if(message.exec()==QMessageBox::Yes)
            {//如果要加入预约队列
                recordlister[bookPos].reserveQueue.push_back(mainWindow->currentReaderPos);
                tablemodel->updateData(recordlister);
                flushFile();
                QMessageBox message(QMessageBox::Information,"恭喜",QString("加入提前预约队列成功，当前位序%1").arg(recordlister[bookPos].reserveQueue.size()),QMessageBox::Yes,NULL);
                message.setButtonText(QMessageBox::Yes,"好的");
                message.exec();
            }
            return;
        }
        //预约
        BorrowOrReserve rec;
        rec.addDate=mytime.getDate();
        rec.bookPos=bookPos;
        rec.bookSameNumber=sameNumber;
        rec.borrowOrReserveFlag=false;
        rec.readerPos=mainWindow->currentReaderPos;
        rec.renewTimes=0;
        mainWindow->bookBorrowOrReserveView->addOrChangeItem=0;
        mainWindow->bookBorrowOrReserveView->addData(rec);
        QMessageBox message(QMessageBox::Information,"恭喜","预约成功",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();

        //要先搜索一遍该读者是否借阅或者预约了该图书，如果有，则警告，结束
        //如果没有，要查询该读者是否已经位于等待队列中，如果有，则警告，结束
        //如果没有，则查询该读者是否已达借阅或预约上限，如果有，则警告，结束
        //如果没有，要查询该书是否还有空余，如果没有，则警告，询问是否加入提前预约等待队列，若是则加入队列，结束
        //如果有，则该读者成功预约该书，结束
    }
}

void bookViewWidget::onItemClicked()
{
    QModelIndexList mIndexList=tableView->selectedIndexes();
    if(mIndexList.size()&&isReaderLooking)
    {

    }
}

void bookViewWidget::clearAllBookBorrowOrReserveInformation()
{
    for(int i=0;i<recordlister.size();i++)
    {
        recordlister[i].setAllNoReserved();
        recordlister[i].setAllNoBorrowed();
    }
}

void bookViewWidget::onDateChanged(int year, int month, int day)
{
    Q_UNUSED(year);
    Q_UNUSED(month);
    Q_UNUSED(day);
    tablemodel->updateData(recordlister);
}

void bookViewWidget::clearAllEverBorrowedTimes()
{
    for(int i=0;i<recordlister.size();i++)
    {
        recordlister[i].clearAllEverBorrowedTimes();
    }
    tablemodel->updateData(recordlister);
}
