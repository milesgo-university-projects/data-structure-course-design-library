/******************************************************************
文件名: readerviewwidget.cpp
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/29
描　述: 读者信息管理 如添加更新删除等
版　本: v5.00 Copyright(c).
******************************************************************/
#include "readerviewwidget.h"
#include "widget.h"
#include "functions.h"
#include <QApplication>
#include <QModelIndex>
#include <QDebug>
#include "logs.h"

//构造函数
readerViewWidget::readerViewWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("读者管理");                            //各种初始化，各种设置标签，各种new一个实体
    this->setObjectName("outsider");
    this->setFixedSize(875,600);
    isBorrowingOrReserving=false;
    isAddFeeAcount=false;
    isChangePassword=false;
    addItem = new QPushButton(this);
    deleteItem = new QPushButton(this);
    search = new QPushButton(this);
    showAll = new QPushButton(this);
    select = new QPushButton(this);
    change = new QPushButton(this);
    exportItems = new QPushButton(this);
    tableView = new friendTableView(this);
    searchEdit = new QLineEdit(this);
    upButtonLayout = new QHBoxLayout;                           //布局
    downButtonLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;
    tablemodel = new readerModel;
    proxymodel = new QSortFilterProxyModel;
    comboBox = new comboBoxWithDropDown(this);
    statusBar = new mainStatusBar(this);

    tableView->setObjectName("tableView");

    comboBox->addItem("行数");                                   //选择框，选择按某种方式搜索
    comboBox->addItem("姓名");
    comboBox->addItem("学/工号");
    comboBox->addItem("性别");
    comboBox->addItem("权限/读者类型");
    comboBox->addItem("逾期天数");
    comboBox->addItem("账户余额");
    comboBox->addItem("所在院系");
    comboBox->setCurrentText("学/工号");

    QStringList list;
    list<<"姓名"<<"学/工号"<<"性别"<<"权限/读者类型"<<"逾期天数"<<"账户余额"<<"所在院系/部门";
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

    upButtonLayout->addWidget(searchEdit);
    upButtonLayout->addWidget(search);
    upButtonLayout->addWidget(comboBox);
    upButtonLayout->addWidget(showAll);
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
    tableView->setColumnWidth(1,150);
    tableView->setColumnWidth(2,150);
    tableView->setColumnWidth(3,50);
    tableView->setColumnWidth(4,100);
    tableView->setColumnWidth(5,80);
    tableView->setColumnWidth(6,90);
    tableView->setColumnWidth(7,150);


    QFile file("../dat/reader.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error readerviewwidget.cpp";
    QTextStream in(&file);
    QStringList tmpList;
    QString str;
    class Reader rec;
    while(!in.atEnd())
    {//将文件中的信息读到容器中
        tmpList.clear();
        for(int i=0;i<8;i++)
        {
            in>>str;
            tmpList<<str;
        }
        if(tmpList[0]!="")
        {
            rec.nameChange(tmpList.at(0));
            rec.idChange(tmpList.at(1));
            rec.setGender(tmpList.at(2));
            str=tmpList.at(3);
            rec.bookAuthorityChange(str.toInt());
            rec.setAccount(tmpList.at(4));
            //rec.setAccount(50);
            rec.setOverdueTime(tmpList.at(5));
            rec.schoolBelongedChange(tmpList.at(6));
            rec.setMd5PassWord(tmpList.at(7));
            recordlister.append(rec);
        }
    }
    file.close();
    tablemodel->updateData(recordlister);//别忘了update data of model
    connect(tablemodel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(updateFileData(QModelIndex)));
    //如果model的信息有变，主widget中的容器也要相应变化，以便能够保存最新信息到文件中
    connect(addItem,SIGNAL(clicked()),this,SLOT(isAddItem()));//点击新建按钮
    connect(deleteItem,SIGNAL(clicked()),this,SLOT(deleteData()));//点击删除按钮
    connect(search,SIGNAL(clicked()),this,SLOT(scrollFocusTo()));//点击搜索按钮
    connect(showAll,SIGNAL(clicked()),this,SLOT(showAllRows()));//点击显示全部按钮
    connect(select,SIGNAL(clicked()),this,SLOT(sendReaderInfo()));
    connect(change,SIGNAL(clicked()),this,SLOT(isChangeItem()));//点击修改按钮
    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(isChangeItem()));//双击信号
    connect(exportItems,SIGNAL(clicked()),this,SLOT(excelExport()));
    connect(searchEdit,SIGNAL(returnPressed()),search,SIGNAL(clicked()));//在搜索框内按下回车，自动按下搜索键
    connect(&mytime,SIGNAL(dateChanged(int,int,int)),this,SLOT(onDateChanged(int,int,int)));
    statusBar->connectLater();
}
//析构函数
readerViewWidget::~readerViewWidget()
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
    delete statusBar;
    delete downButtonLayout;
    delete mainLayout;
}

//更新某一行信息
void readerViewWidget::updateFileData(QModelIndex index)
{
    class Reader record;
    record=tablemodel->pList.at(index.row());
    recordlister.replace(index.row(),record);
}
//保存文件到文档中
void readerViewWidget::flushFile()
{//保存到文件
    QFile file("../dat/reader.txt");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        qDebug()<<"open file error readerviewwidget.cpp";
    QTextStream out(&file);
    int i;
    for(i=0;i<recordlister.count()-1;i++)
    {
        out<<recordlister[i].getName()<<" "<<recordlister[i].getId()<<" "<<recordlister[i].getGender1()
        <<" "<<QString::number(recordlister[i].getAuth())<<" "<<QString::number(recordlister[i].getFeeAccount(),'f',2)
        <<" "<<QString::number(recordlister[i].getOverdueTime())<<" "<<recordlister[i].getSchool()<<" "<<recordlister[i].getPassword()<<endl;
    }
    if(recordlister.size())
    {
        out<<recordlister[i].getName()<<" "<<recordlister[i].getId()<<" "<<recordlister[i].getGender1()
        <<" "<<QString::number(recordlister[i].getAuth())<<" "<<QString::number(recordlister[i].getFeeAccount(),'f',2)
        <<" "<<QString::number(recordlister[i].getOverdueTime())<<" "<<recordlister[i].getSchool()<<" "<<recordlister[i].getPassword();
    }
}
//添加新元素
void readerViewWidget::isAddItem()
{//点击新建按钮后，创建输入框
    addOrChangeItem=0;
    addReaderBox *inputDialog = new addReaderBox;
    connect(inputDialog,SIGNAL(newData(QStringList)),this,SLOT(addData(QStringList)));
    inputDialog->show();
}

//判断是否膝盖元素
void readerViewWidget::isChangeItem()
{
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
    class Reader rec=recordlister.at(currentRow);
    addReaderBox *inputDialog = new addReaderBox(rec,currentRow);//调用另外一个构造函数，以初始化输入框信息
    connect(inputDialog,SIGNAL(newData(QStringList)),this,SLOT(addData(QStringList)));
    inputDialog->show();
}

//增加数据
void readerViewWidget::addData(QStringList strList)
{
    class Reader record;
    QString str;
    record.nameChange(strList.at(0));
    record.idChange(strList.at(1));
    record.setGender(strList.at(2));
    record.bookAuthorityChange(authorityNameToNumber[strList.at(3)]);
    record.setOverdueTime(strList.at(4));
    record.setAccount(strList.at(5));
    record.schoolBelongedChange(strList.at(6));
    record.passwordChange(record.getId());
    if(addOrChangeItem==0)//如果是新建，就直接append增加
    {
        Logs::AddReader(record.getName(),record.getId(),record.getSchool(),record.getPassword(),authorityNumberToName[record.getAuth()],record.getAuth(),record.getFeeAccount());
        recordlister.append(record);
    }
    else//如果是修改，就replace替换
    {
        Logs::ReplaceReader(mainWindow->readerView->recordlister[currentRow].getName(),mainWindow->readerView->recordlister[currentRow].getId(),mainWindow->readerView->recordlister[currentRow].getSchool(),mainWindow->readerView->recordlister[currentRow].getPassword(),authorityNumberToName[mainWindow->readerView->recordlister[currentRow].getAuth()],mainWindow->readerView->recordlister[currentRow].getAuth(),mainWindow->readerView->recordlister[currentRow].getFeeAccount(),record.getName(),record.getId(),record.getSchool(),record.getPassword(),authorityNumberToName[record.getAuth()],record.getAuth(),record.getFeeAccount());
        recordlister.replace(currentRow,record);
    }
    tablemodel->updateData(recordlister);
    flushFile();
}

//删除数据
void readerViewWidget::deleteData()
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
        int size=recordlister[row].getBookBorrowOrReserveVector().size();
        if(size)
        {
            QMessageBox message(QMessageBox::Warning,"警告",QString("该读者仍有%1条借阅/预约记录，无法删除").arg(size),QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        for(int i=0;i<mainWindow->bookBorrowOrReserveView->recordlister.size();i++)
        {
            if(mainWindow->bookBorrowOrReserveView->recordlister[i].readerPos>row)
            {
                mainWindow->bookBorrowOrReserveView->recordlister[i].readerPos--;
            }
        }
        Logs::DeleReader(mainWindow->readerView->recordlister[row].getName(),mainWindow->readerView->recordlister[row].getId(),mainWindow->readerView->recordlister[row].getSchool(),mainWindow->readerView->recordlister[row].getPassword(),authorityNumberToName[mainWindow->readerView->recordlister[row].getAuth()],mainWindow->readerView->recordlister[row].getAuth(),mainWindow->readerView->recordlister[row].getFeeAccount());
        recordlister.erase(recordlister.begin()+row);
        mainWindow->bookBorrowOrReserveView->tablemodel->updateData(mainWindow->bookBorrowOrReserveView->recordlister);
        mainWindow->bookBorrowOrReserveView->flushFile();
        tablemodel->updateData(recordlister);
        flushFile();
}
//设置搜索哪一列，comboBox的值设置
void readerViewWidget::scrollFocusTo()
{
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
        if(st=="姓名")
        {
            proxymodel->setFilterKeyColumn(1);
        }
        else if(st=="学/工号")
        {
            proxymodel->setFilterKeyColumn(2);
        }
        else if(st=="性别")
        {
            proxymodel->setFilterKeyColumn(3);
        }
        else if(st=="权限/读者类型")
        {
            proxymodel->setFilterKeyColumn(4);
        }
        else if(st=="逾期天数")
        {
            proxymodel->setFilterKeyColumn(5);
        }
        else if(st=="账户余额")
        {
            proxymodel->setFilterKeyColumn(6);
        }
        else if(st=="所在院系")
        {
            proxymodel->setFilterKeyColumn(7);
        }
        filterChanged(str.toString());
    }
}

//显示全部，就相当于正则表达式为空，就全部显示出来了
void readerViewWidget::showAllRows()
{
    filterChanged("");
}
//发送某一行的数据给某些槽函数
void readerViewWidget::sendReaderInfo()
{
    QModelIndexList mIndexList=tableView->selectedIndexes();
    if(mIndexList.isEmpty())                               //未选中某一行，特殊处理
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    int row=mIndexList[0].data().toInt()-1;
    emit readerInfo(row);
    this->close();
}

//修改搜索所用的正则表达式
void readerViewWidget::filterChanged(QString str)
{//这个可以直接copy
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(QRegExp::FixedString);
    QRegExp regExp(str, Qt::CaseInsensitive, syntax);
    proxymodel->setFilterRegExp(regExp);
    tableView->verticalScrollBar()->setValue(0);//滚动条滚至顶端
}
//导出目前窗体中的数据
void readerViewWidget::excelExport()
{
    QString filepath=QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
                                                  tr("Excel Files (*.xls)"));
    ExportExcelObject temp(filepath,tr("读者信息"),tableView);
    temp.addField(0,"行数","char(20)");
    for(int i=0;i<headerName.size();i++)
    {
        temp.addField(i+1,headerName.at(i),"char(200)");
    }
    temp.export2Excel();
}

//快捷键
void readerViewWidget::keyPressEvent(QKeyEvent *e)
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
//关闭事件，处理在关闭前需要处理的事
void readerViewWidget::closeEvent(QCloseEvent *e)
{
    windowHash[4]=false;
    isOpen=false;
    isBorrowingOrReserving=false;
    isAddFeeAcount=false;
    isChangePassword=false;
    this->select->setEnabled(false);
    flushFile();
    e->accept();
    QWidget::closeEvent(e);
}

//显示事件
void readerViewWidget::showEvent(QShowEvent *e)
{
    statusBar->readerInformationButton->setHidden(true);
    statusBar->readerInformButton->setHidden(true);
    if(!(isBorrowingOrReserving||isAddFeeAcount||isChangePassword))
    {
        searchEdit->setText("");
        filterChanged("");
        comboBox->setCurrentText("学/工号");
    }
    windowHash[4]=true;
    windowPointer[4]=qobject_cast<QWidget *>(this);
    isOpen=true;
    QWidget::showEvent(e);
}

//修改借阅或者预约信息
void readerViewWidget::updateBorrowAndReserveInfo(BorrowOrReserve rec)
{
    BookBorrowOrReserve tmpBBOR;
    tmpBBOR.bookPos=rec.bookPos;
    tmpBBOR.borrowOrReserveFlag=rec.borrowOrReserveFlag;
    tmpBBOR.renewTimes=rec.renewTimes;
    tmpBBOR.sameNumber=rec.bookSameNumber;
    tmpBBOR.startDate=rec.addDate;
    recordlister[rec.readerPos].addBookBorrowOrReserveVector(tmpBBOR);
    tablemodel->updateData(recordlister);
    flushFile();
}

//与扣费相关，响应日期变化
void readerViewWidget::onDateChanged(int year, int month, int day)
{
    Q_UNUSED(year);
    Q_UNUSED(month);
    Q_UNUSED(day);
    for(int i=0;i<recordlister.size();i++)
    {
        if(recordlister[i].getFeeAccount()<0&&!recordlister[i].hasSentAcountNotEnoughInformation)
        {//发送通知
            QStringList tmpList;
            QString str;
            str="您的余额不足,请尽快充值";
            tmpList<<str;
            str="否则将无法预约或借阅";
            tmpList<<str;
            addReaderInformation(recordlister[i].getId(),tmpList);
            if(windowHash[19]&&mainWindow->currentReaderPos==i)
                mainWindow->readerInformationWidgetPointer->updateInformationTableWidget(recordlister[i].getId());
            recordlister[i].hasSentAcountNotEnoughInformation=true;
        }
    }
}

//清理所有预约信息
void readerViewWidget::clearAllBookBorrowOrReserveInformation()
{
    for(int i=0;i<recordlister.size();i++)
    {
        recordlister[i].clearBBOR();
    }
}
