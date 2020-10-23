/******************************************************************
文件名: bookborroworreservemanagewidget.cpp
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/21
描　述: 管理员对借阅以及预约信息的管理
版　本: v5.00 Copyright(c).
******************************************************************/
#include "bookborroworreservemanagewidget.h"
#include "functions.h"
#include "widget.h"
#include "logs.h"

bookBorrowOrReserveManageWidget::bookBorrowOrReserveManageWidget(QWidget *parent):QWidget(parent)
{//构建显示界面
    this->setWindowTitle("借阅/预约信息管理");
    this->setObjectName("outsider");
    this->setFixedSize(1000,600);
    addItem = new QPushButton(this);
    deleteItem = new QPushButton(this);
    renew = new QPushButton(this);
    reserveToBorrow = new QPushButton(this);
    search = new QPushButton(this);
    showAll = new QPushButton(this);
    change = new QPushButton(this);
    exportItems = new QPushButton(this);
    tableView = new friendTableView(this);
    searchEdit = new QLineEdit(this);
    upButtonLayout = new QHBoxLayout;
    downButtonLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;
    tablemodel = new bookBorrowOrReserveTableModel;
    proxymodel = new QSortFilterProxyModel;
    searchComboBox = new comboBoxWithDropDown(this);
    borrowOrReserveComboBox = new comboBoxWithDropDown(this);
    statusBar = new mainStatusBar(this);

    borrowOrReserveComboBox->addItem("借阅");
    borrowOrReserveComboBox->addItem("预约");
    borrowOrReserveComboBox->addItem("借阅已超期");
    borrowOrReserveComboBox->addItem("全部");//同时显示借阅和预约信息
    borrowOrReserveComboBox->setCurrentText("全部");

    tableView->setObjectName("tableView");

    searchComboBox->addItem("类型");//借阅或者预约
    searchComboBox->addItem("书名");
    searchComboBox->addItem("索书号");
    searchComboBox->addItem("读者名");
    searchComboBox->addItem("学/工号");
    searchComboBox->addItem("起始日期");
    searchComboBox->addItem("截止日期");

    searchComboBox->setCurrentText("学/工号");

    QStringList list;
    list<<"类型"<<"书名"<<"索书号"<<"读者名"<<"学/工号"<<"起始日期"<<"截止日期";
    tablemodel->setHeaderName(list);
    headerName=list;

    addItem->setText(tr("借阅/预约"));
    addItem->setFocusPolicy(Qt::NoFocus);
    deleteItem->setText(tr("归还/取消预约"));
    deleteItem->setFocusPolicy(Qt::NoFocus);
    renew->setText(tr("续借"));
    renew->setFocusPolicy(Qt::NoFocus);
    reserveToBorrow->setText(tr("预约改借阅"));
    reserveToBorrow->setFocusPolicy(Qt::NoFocus);

    search->setText(tr("搜索"));
    search->setFocusPolicy(Qt::NoFocus);

    showAll->setText(tr("显示全部"));
    showAll->setFocusPolicy(Qt::NoFocus);
    change->setText(tr("查看"));
    change->setFocusPolicy(Qt::NoFocus);
    exportItems->setText(tr("导出"));
    exportItems->setFocusPolicy(Qt::NoFocus);

    upButtonLayout->addWidget(searchEdit);
    upButtonLayout->addWidget(search);
    upButtonLayout->addWidget(searchComboBox);
    upButtonLayout->addWidget(showAll);
    upButtonLayout->addWidget(borrowOrReserveComboBox);
    downButtonLayout->addWidget(addItem);
    downButtonLayout->addWidget(deleteItem);
    downButtonLayout->addWidget(reserveToBorrow);
    downButtonLayout->addWidget(renew);
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
    tableView->setColumnWidth(2,263);

    QFile file("../dat/borrowOrReserve.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error bookBorrowOrReserve.cpp";
    QTextStream in(&file);
    QString str;
    class BorrowOrReserve rec;
    while(!in.atEnd())
    {//将文件中的信息读到容器中
        in>>str;
        if(str=="1")//1表示借阅，0表示预约
           rec.borrowOrReserveFlag=true;
        else
            rec.borrowOrReserveFlag=false;
        in>>str;
        rec.readerPos=str.toInt();
        in>>str;
        rec.bookPos=str.toInt();
        in>>str;
        rec.bookSameNumber=str.toInt();
        in>>str;
        rec.addDate=myToDate(str);
        in>>str;
        rec.renewTimes=str.toInt();
        recordlister.push_back(rec);
    }
    file.close();
    renew->setEnabled(false);
    reserveToBorrow->setEnabled(false);
    tablemodel->updateData(recordlister);//别忘了update data of model
    connect(tablemodel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(updateFileData(QModelIndex)));
    //如果model的信息有变，主widget中的容器也要相应变化，以便能够保存最新信息到文件中
    connect(renew,SIGNAL(clicked()),this,SLOT(updateRenewTimes()));
    connect(addItem,SIGNAL(clicked()),this,SLOT(isAddItem()));//点击新建按钮
    connect(deleteItem,SIGNAL(clicked()),this,SLOT(deleteData()));//点击删除按钮
    connect(search,SIGNAL(clicked()),this,SLOT(scrollFocusTo()));//点击搜索按钮
    connect(showAll,SIGNAL(clicked()),this,SLOT(showAllRows()));//点击显示全部按钮
    connect(change,SIGNAL(clicked()),this,SLOT(isChangeItem()));//点击修改按钮
    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(isChangeItem()));//双击信号
    connect(exportItems,SIGNAL(clicked()),this,SLOT(excelExport()));
    connect(searchEdit,SIGNAL(returnPressed()),search,SIGNAL(clicked()));//在搜索框内按下回车，自动按下搜索键
    connect(borrowOrReserveComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(onBorrowOrReserveComboBoxCurrentTextChanged(QString)));
    connect(tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(updateButtonEnabledSituation()));
    connect(&mytime,SIGNAL(dateChanged(int,int,int)),this,SLOT(onDateChanged(int,int,int)));
    connect(reserveToBorrow,SIGNAL(clicked()),this,SLOT(onReserveOrBorrowButtonClicked()));
    statusBar->connectLater();
    statusBar->readerInformationButton->setHidden(true);
    statusBar->readerInformButton->setHidden(true);
    statusBar->currentAccountLabel->setHidden(true);
}

bookBorrowOrReserveManageWidget::~bookBorrowOrReserveManageWidget()
{//按顺序释放各个指针
    delete addItem;
    delete deleteItem;
    delete tableView;
    delete downButtonLayout;
    delete mainLayout;
    delete tablemodel;
    delete proxymodel;
    delete renew;
    delete reserveToBorrow;
    delete searchEdit;
    delete search;
    delete showAll;
    delete searchComboBox;
    delete borrowOrReserveComboBox;
    delete change;
    delete exportItems;
}

void bookBorrowOrReserveManageWidget::closeEvent(QCloseEvent *e)
{
    windowHash[8]=false;
    isOpen=false;
    flushFile();
    e->accept();
    QWidget::closeEvent(e);
}

void bookBorrowOrReserveManageWidget::showEvent(QShowEvent *e)
{
    searchEdit->setText("");
    filterChanged("");
    searchComboBox->setCurrentText("学/工号");
    borrowOrReserveComboBox->setCurrentText("全部");
    windowHash[8]=true;
    windowPointer[8]=qobject_cast<QWidget *>(this);
    isOpen=true;
    QWidget::showEvent(e);
}

void bookBorrowOrReserveManageWidget::updateFileData(QModelIndex index)//文件中数据更新
{
    BorrowOrReserve record;
    record=tablemodel->pList.at(index.row());
    recordlister.replace(index.row(),record);
}

void bookBorrowOrReserveManageWidget::flushFile()
{//保存到文件
    QFile file("../dat/borrowOrReserve.txt");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        qDebug()<<"open file error bookBorrowOrReserveManageWidget.cpp";
    QTextStream out(&file);
    int i;
    for(i=0;i<recordlister.count()-1;i++)
    {
        if(recordlister[i].borrowOrReserveFlag)
            out<<"1"<<" ";
        else
            out<<"0"<<" ";
        out<<recordlister[i].readerPos<<" "<<recordlister[i].bookPos<<" "
              <<recordlister[i].bookSameNumber<<" "
              <<myDateToString(recordlister[i].addDate)<<"  "<<recordlister[i].renewTimes<<endl;
    }
    if(recordlister.size())
    {
        if(recordlister[i].borrowOrReserveFlag)
            out<<"1"<<" ";
        else
            out<<"0"<<" ";
        out<<recordlister[i].readerPos<<" "<<recordlister[i].bookPos<<" "
              <<recordlister[i].bookSameNumber<<" "
              <<myDateToString(recordlister[i].addDate)<<"  "<<recordlister[i].renewTimes;
    }
    file.close();
}

void bookBorrowOrReserveManageWidget::isAddItem()
{//点击新建按钮后，创建输入框
    addOrChangeItem=0;
    addBookBorrowOrReserveWidget *inputDialog = new addBookBorrowOrReserveWidget;
    connect(inputDialog,SIGNAL(newData(BorrowOrReserve)),this,SLOT(addData(BorrowOrReserve)));
    inputDialog->show();
}

void bookBorrowOrReserveManageWidget::isChangeItem()
{//需要改变当前某个借阅信息
    QModelIndexList mIndexList=tableView->selectedIndexes();
    if(mIndexList.isEmpty())
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    currentRow=mIndexList[0].data().toInt()-1;
    addOrChangeItem=1;
    BorrowOrReserve rec=recordlister.at(currentRow);
    addBookBorrowOrReserveWidget *inputDialog = new addBookBorrowOrReserveWidget(rec);//调用另外一个构造函数，以初始化输入框信息
    connect(inputDialog,SIGNAL(newData(BorrowOrReserve)),this,SLOT(addData(BorrowOrReserve)));

    inputDialog->searchReaderLineEdit->setFocusPolicy(Qt::NoFocus);
    inputDialog->searchBookLineEdit->setFocusPolicy(Qt::NoFocus);
    inputDialog->addBorrowOrReserveComboBox->clear();
    QString str;
    if(rec.borrowOrReserveFlag)
        str="借阅";
    else
        str="预约";
    inputDialog->addBorrowOrReserveComboBox->addItem(str);
    inputDialog->addBorrowOrReserveComboBox->setCurrentText(str);
    inputDialog->acceptButton->setHidden(true);
    inputDialog->rejectButton->setHidden(true);
    inputDialog->searchBookLineEdit->searchButton->setHidden(true);
    inputDialog->searchReaderLineEdit->searchButton->setHidden(true);
    inputDialog->show();
}

void bookBorrowOrReserveManageWidget::addData(BorrowOrReserve rec)
{//借阅信息的添加以及修改
        if(addOrChangeItem==0)//如果是新建，就直接append增加
        {
            recordlister.append(rec);//写入数组 添加到最后
            Logs::AddBorrowOrReserveLogs(rec.borrowOrReserveFlag,mainWindow->readerView->recordlister[rec.readerPos].getName(),mainWindow->readerView->recordlister[rec.readerPos].getId(),mainWindow->bookView->recordlister[rec.bookPos].getName(),mainWindow->bookView->recordlister[rec.bookPos].getClassificationNumber(),mainWindow->bookView->recordlister[rec.bookPos].getTypeNumber(),mainWindow->bookView->recordlister[rec.bookPos].getClassificationName(),QString("%1").arg(rec.bookSameNumber));
            Logs::ReaderHistoryAdd(rec.borrowOrReserveFlag,mainWindow->readerView->recordlister[rec.readerPos].getName(),mainWindow->readerView->recordlister[rec.readerPos].getId(),mainWindow->bookView->recordlister[rec.bookPos].getName(),mainWindow->bookView->recordlister[rec.bookPos].getClassificationNumber(),mainWindow->bookView->recordlister[rec.bookPos].getTypeNumber(),mainWindow->bookView->recordlister[rec.bookPos].getClassificationName());
        }
        else//如果是修改，就replace替换
            recordlister.replace(currentRow,rec);
        if(rec.borrowOrReserveFlag)
        {
            statisticdata.today.bookCountPerDay++;
            statisticdata.statisticAddBookTotal(mainWindow->bookView->recordlister[rec.bookPos].getName(),
                    mainWindow->bookView->recordlister[rec.bookPos].getISBN());
            statisticdata.statisticAddReaderTotal(mainWindow->readerView->recordlister[rec.readerPos].getName(),
                    mainWindow->readerView->recordlister[rec.readerPos].getId());
        }
        tablemodel->updateData(recordlister);//更新数据
        flushFile();
        statisticdata.flushStatistic();
        emit newBorrowAndReserve(rec);
}
#include <QDebug>
void bookBorrowOrReserveManageWidget::deleteData()
{//删除
    QModelIndexList mIndexList=tableView->selectedIndexes();
    if(mIndexList.isEmpty())
    {
//        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
//        message.setButtonText(QMessageBox::Yes,"好的");
//        message.exec();
        return;
    }
    int row=mIndexList[0].data().toInt()-1;
    int readerPos=recordlister[row].readerPos;
    int bookPos=recordlister[row].bookPos;
    Logs::DeleBorrowOrReserveLogs(mainWindow->bookBorrowOrReserveView->recordlister[row].borrowOrReserveFlag,mainWindow->readerView->recordlister[readerPos].getName(),mainWindow->readerView->recordlister[readerPos].getId(),mainWindow->bookView->recordlister[bookPos].getName(),mainWindow->bookView->recordlister[bookPos].getClassificationNumber(),mainWindow->bookView->recordlister[bookPos].getTypeNumber(),mainWindow->bookView->recordlister[bookPos].getClassificationName(),QString("%1").arg(recordlister[row].bookSameNumber));
    Logs::ReaderHistoryDele(mainWindow->bookBorrowOrReserveView->recordlister[row].borrowOrReserveFlag,mainWindow->readerView->recordlister[readerPos].getName(),mainWindow->readerView->recordlister[readerPos].getId(),mainWindow->bookView->recordlister[bookPos].getName(),mainWindow->bookView->recordlister[bookPos].getClassificationNumber(),mainWindow->bookView->recordlister[bookPos].getTypeNumber(),mainWindow->bookView->recordlister[bookPos].getClassificationName());
    if(recordlister[row].borrowOrReserveFlag)
    {//如果是借阅，需要增加历史借阅天数
        mainWindow->bookView->recordlister[recordlister[row].bookPos].addEverBorrowedTimes(recordlister[row].bookSameNumber,subtractDate(recordlister[row].addDate,mytime.getDate()));
    }
    updateBookAndReaderView(row);
    mainWindow->readerView->tablemodel->updateData(mainWindow->readerView->recordlister);
    mainWindow->readerView->flushFile();
    mainWindow->bookView->tablemodel->updateData(mainWindow->bookView->recordlister);
    mainWindow->bookView->flushFile();
    recordlister.erase(recordlister.begin()+row);
    tablemodel->updateData(recordlister);
    flushFile();
}

void bookBorrowOrReserveManageWidget::checkCanReserveTo(BorrowOrReserve rec)
{//检查是否可以借阅
    int bookPos=rec.bookPos;
    int readerPos=mainWindow->bookView->recordlister[bookPos].checkReserveQueue();
    if(readerPos!=-1)
    {//预约该读者
        rec.addDate=mytime.getDate();
        rec.borrowOrReserveFlag=false;
        rec.readerPos=readerPos;
        rec.renewTimes=0;
        addOrChangeItem=0;
        addData(rec);
        QStringList tmpList;
        QString str;
        str="已正式预约图书《"+mainWindow->bookView->recordlister[bookPos].getName()+"》";
        tmpList<<str;
        str="请于三天内前往图书馆借阅，否则将自动取消预约";
        tmpList<<str;
        addReaderInformation(mainWindow->readerView->recordlister[readerPos].getId(),tmpList);
    }
    if(windowHash[19]&&mainWindow->currentReaderPos==readerPos)
        mainWindow->readerInformationWidgetPointer->updateInformationTableWidget(mainWindow->readerView->recordlister[readerPos].getId());
}

void bookBorrowOrReserveManageWidget::scrollFocusTo()
{//设置搜索哪一列，comboBox的值设置
    QVariant str=searchEdit->text().trimmed();
    QString st=searchComboBox->currentText();
    if(st=="行数")
    {
        if(str.canConvert(QMetaType::Int))
        {
            tableView->verticalScrollBar()->setValue(str.toInt()-1);//定位到这一行
        }
    }
    else
    {
        if(st=="类型")
        {
            proxymodel->setFilterKeyColumn(1);
        }
        else if(st=="书名")
        {
            proxymodel->setFilterKeyColumn(2);
        }
        else if(st=="索书号")
        {
            proxymodel->setFilterKeyColumn(3);
        }
        else if(st=="读者名")
        {
            proxymodel->setFilterKeyColumn(4);
        }
        else if(st=="学/工号")
        {
            proxymodel->setFilterKeyColumn(5);
        }
        else if(st=="起始日期")
        {
            proxymodel->setFilterKeyColumn(6);
        }
        else if(st=="截止日期")
        {
            proxymodel->setFilterKeyColumn(7);
        }
        filterChanged(str.toString());
    }
}

void bookBorrowOrReserveManageWidget::showAllRows()
{//显示全部，就相当于正则表达式为空，就全部显示出来了
    filterChanged("");
}

void bookBorrowOrReserveManageWidget::filterChanged(QString str)
{
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(QRegExp::FixedString);
    QRegExp regExp(str, Qt::CaseInsensitive, syntax);
    proxymodel->setFilterRegExp(regExp);
    tableView->verticalScrollBar()->setValue(0);//滚动条滚至顶端
}

void bookBorrowOrReserveManageWidget::excelExport()
{
    QString filepath=QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
                                                  tr("Excel Files (*.xls)"));
    ExportExcelObject temp(filepath,tr("借阅/预约"),tableView);
    temp.addField(0,"行数","char(20)");
    for(int i=0;i<headerName.size();i++)
    {
        temp.addField(i+1,headerName.at(i),"char(200)");
    }
    temp.export2Excel();
}

void bookBorrowOrReserveManageWidget::keyPressEvent(QKeyEvent *e)
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

void bookBorrowOrReserveManageWidget::onBorrowOrReserveComboBoxCurrentTextChanged(QString str)
{
    proxymodel->setFilterKeyColumn(1);
    if(str=="借阅")
        filterChanged("借阅");
    else if(str=="预约")
        filterChanged("预约");
    else if(str=="借阅已超期")
        filterChanged("借阅已超期");
    else if(str=="全部")
        filterChanged("");
}

void bookBorrowOrReserveManageWidget::updateBookAndReaderView(int row)
{
    int bookPos,readerPos,sameNumber;
    bookPos=recordlister[row].bookPos;
    readerPos=recordlister[row].readerPos;
    sameNumber=recordlister[row].bookSameNumber;

    if(recordlister[row].borrowOrReserveFlag)
        mainWindow->bookView->recordlister[bookPos].setIsBorrowed(false,sameNumber);
    else
        mainWindow->bookView->recordlister[bookPos].setIsReserved(false,sameNumber);

    //检查书的队列有没有人
    checkCanReserveTo(recordlister[row]);
    mainWindow->readerView->recordlister[readerPos].deleteBookAt(bookPos,sameNumber);
}

void bookBorrowOrReserveManageWidget::updateButtonEnabledSituation()
{
    QModelIndexList mIndexList=tableView->selectedIndexes();
    if(mIndexList[1].data().toString()=="借阅"||mIndexList[1].data().toString()=="借阅已超期")
    {
        renew->setEnabled(true);
        reserveToBorrow->setEnabled(false);
    }
    else
    {
        renew->setEnabled(false);
        reserveToBorrow->setEnabled(true);
    }
}

void bookBorrowOrReserveManageWidget::updateRenewTimes()
{
    QModelIndexList mIndexList=tableView->selectedIndexes();
    if(mIndexList.isEmpty())
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    int row=mIndexList[0].data().toInt()-1;
    int readerPos=recordlister[row].readerPos;
    int auth=mainWindow->readerView->recordlister[readerPos].getAuth();
    int authPos=mainWindow->authorityView->findAuthPos(auth);
    int bookRenewTimesLimit;
    if(authPos!=-1)
        bookRenewTimesLimit=mainWindow->authorityView->recordlister[authPos].getRenewTimesLimit();
    else
        bookRenewTimesLimit=3;
    if(recordlister[row].renewTimes>=bookRenewTimesLimit)
    {
        QMessageBox message(QMessageBox::Warning,"警告","已达续借上限",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    recordlister[row].renewTimes++;
    Logs::ManagerGoOnBorrow(1,mainWindow->readerView->recordlister[readerPos].getName(),mainWindow->currentAccountName,mainWindow->bookView->recordlister[recordlister[row].bookPos].getName(),recordlister[row].renewTimes);
    mainWindow->readerView->recordlister[readerPos].addRenewTimes(recordlister[row].bookPos);
    tablemodel->updateData(recordlister);
    flushFile();
    mainWindow->readerView->tablemodel->updateData(mainWindow->readerView->recordlister);
    mainWindow->readerView->flushFile();
}

void bookBorrowOrReserveManageWidget::onDateChanged(int year, int month, int day)
{
    int i,auth,authPos,days,readerPos,bookPos;
    QDate date;
    QVector<bool> needDelete;
    for(i=0;i<recordlister.size();i++)
    {
        if(!recordlister[i].borrowOrReserveFlag)
        {//如果是预约,则检查预约是否超期
            if(cmpDate(addDate(recordlister[i].addDate,3),QDate(year,month,day)))
                needDelete.push_back(true);
            else
                needDelete.push_back(false);
            continue;
        }
        needDelete.push_back(false);
        readerPos=recordlister[i].readerPos;
        bookPos=recordlister[i].bookPos;
        auth=mainWindow->readerView->recordlister[readerPos].getAuth();
        authPos=mainWindow->authorityView->findAuthPos(auth);
        if(authPos!=-1)
            days=mainWindow->authorityView->recordlister[authPos].getBorrowBookDaysLimit();
        else
            days=20;
        date=addDate(recordlister[i].addDate,days+days*recordlister[i].renewTimes);
        if(cmpDate(date,QDate(year,month,day)))
        {//严格小于，需要扣费
            if(!recordlister[i].hasSendOverdueInformation)
            {
                recordlister[i].hasSendOverdueInformation=true;
                QStringList tmpList;
                QString str;
                str="您借阅的图书《"+mainWindow->bookView->recordlister[bookPos].getName()+"》已超期";
                tmpList<<str;
                str="请尽快归还";
                tmpList<<str;
                addReaderInformation(mainWindow->readerView->recordlister[readerPos].getId(),tmpList);
            }
            if(windowHash[19]&&mainWindow->currentReaderPos==readerPos)
                mainWindow->readerInformationWidgetPointer->updateInformationTableWidget(mainWindow->readerView->recordlister[readerPos].getId());
            mainWindow->readerView->recordlister[readerPos].chargeOverdueFee(0.01*mainWindow->bookView->recordlister[bookPos].getPrice());
            mainWindow->readerView->recordlister[readerPos].addOverdueTime();
        }
    }
    for(int i=recordlister.size()-1;i>=0;i--)
    {
        if(needDelete[i])
        {
            if(!recordlister[i].borrowOrReserveFlag)
            {
                int bookPos,readerPos;
                bookPos=recordlister[i].bookPos;
                readerPos=recordlister[i].readerPos;
                QStringList tmpList;
                QString str;
                str="您预约的图书《"+mainWindow->bookView->recordlister[bookPos].getName()+"》已超期";
                tmpList<<str;
                str="已自动取消";
                tmpList<<str;
                addReaderInformation(mainWindow->readerView->recordlister[readerPos].getId(),tmpList);
                if(windowHash[19]&&mainWindow->currentReaderPos==readerPos)
                    mainWindow->readerInformationWidgetPointer->updateInformationTableWidget(mainWindow->readerView->recordlister[readerPos].getId());
            }
            deleteData(i);
            needDelete.erase(needDelete.begin()+i);
        }
    }
    mainWindow->readerView->tablemodel->updateData(mainWindow->readerView->recordlister);
    mainWindow->readerView->flushFile();
    mainWindow->bookView->tablemodel->updateData(mainWindow->bookView->recordlister);
    mainWindow->bookView->flushFile();
    tablemodel->updateData(recordlister);
    flushFile();
}

void bookBorrowOrReserveManageWidget::addRenewTimes(int bookPos,int bookSameNumber, int readerPos)
{
    for(int i=0;i<recordlister.size();i++)
    {
        if(recordlister[i].bookPos==bookPos&&recordlister[i].bookSameNumber==bookSameNumber&&recordlister[i].readerPos==readerPos)
        {
            recordlister[i].renewTimes++;
            tablemodel->updateData(recordlister);
            break;
        }
    }
}

int bookBorrowOrReserveManageWidget::searchBorrowAndReserve(int bookPos, int bookSameNumber)
{
    for(int i=0;i<recordlister.size();i++)
    {
        if(recordlister[i].bookPos==bookPos&&recordlister[i].bookSameNumber==bookSameNumber)
            return i;
    }
    return -1;//如果没有找到
}

void bookBorrowOrReserveManageWidget::deleteData(int bookPos, int bookSameNumber)
{//需要同时更新reader and book
    int row=searchBorrowAndReserve(bookPos,bookSameNumber);
    if(row==-1)
    {
        qDebug()<<"no such item in book borrow and reserve table widget";
        return;
    }
    deleteData(row);
}

void bookBorrowOrReserveManageWidget::deleteData(int row)
{
    if(row==-1)
    {
        qDebug()<<"no such item in book borrow and reserve table widget";
        return;
    }
    updateBookAndReaderView(row);
    mainWindow->readerView->tablemodel->updateData(mainWindow->readerView->recordlister);
    mainWindow->readerView->flushFile();
    mainWindow->bookView->tablemodel->updateData(mainWindow->bookView->recordlister);
    mainWindow->bookView->flushFile();
    recordlister.erase(recordlister.begin()+row);
    tablemodel->updateData(recordlister);
    flushFile();
}

void bookBorrowOrReserveManageWidget::onReserveOrBorrowButtonClicked()//预约改成借阅
{
    QModelIndexList mIndexList=tableView->selectedIndexes();
    if(mIndexList.size())
    {
        int row=mIndexList[0].data().toInt()-1;
        Logs::ReverseToBorrow(mainWindow->readerView->recordlister[recordlister[row].readerPos].getName(),mainWindow->readerView->recordlister[recordlister[row].readerPos].getId(),mainWindow->bookView->recordlister[recordlister[row].bookPos].getName(),QString("%1").arg(recordlister[row].bookSameNumber));
        recordlister[row].addDate=mytime.getDate();
        recordlister[row].borrowOrReserveFlag=true;
        recordlister[row].renewTimes=0;
        recordlister[row].hasSendOverdueInformation=false;
        //更新书籍和读者信息
        mainWindow->bookView->updateBorrowAndReserveInfo(recordlister[row]);
        mainWindow->readerView->recordlister[recordlister[row].readerPos].reserveToBorrow(recordlister[row].bookPos,recordlister[row].bookSameNumber);
        mainWindow->bookView->tablemodel->updateData(mainWindow->bookView->recordlister);
        mainWindow->bookView->flushFile();
        mainWindow->readerView->tablemodel->updateData(mainWindow->readerView->recordlister);
        mainWindow->readerView->flushFile();

        tablemodel->updateData(recordlister);
        flushFile();
    }
}

void bookBorrowOrReserveManageWidget::clearAllBookBorrowOrReserveInformation()
{//清空所有的借阅信息,这里还要调用读者和图书的函数，清空借阅信息
    mainWindow->bookView->clearAllBookBorrowOrReserveInformation();
    mainWindow->readerView->clearAllBookBorrowOrReserveInformation();
    recordlister.clear();
    mainWindow->bookView->tablemodel->updateData(mainWindow->bookView->recordlister);
    mainWindow->bookView->flushFile();
    mainWindow->readerView->tablemodel->updateData(mainWindow->readerView->recordlister);
    mainWindow->readerView->flushFile();
    this->tablemodel->updateData(this->recordlister);
    this->flushFile();
}
