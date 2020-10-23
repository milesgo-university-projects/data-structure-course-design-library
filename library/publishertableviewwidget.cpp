/******************************************************************
文件名: publishertableviewwidget.cpp
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/27
描　述: 出版社信息显示以及相应的管理
版　本: v5.00 Copyright(c).
******************************************************************/
#include "publishertableviewwidget.h"
#include "widget.h"
#include <QFileDialog>
#include <QMessageBox>
#include<QApplication>
#include <QDebug>
QString ToChineseSpell( QString strText);

publisherTableViewWidget::publisherTableViewWidget(QWidget *parent):QWidget(parent)//构建界面
{
    this->setWindowTitle("出版社管理");
    this->setObjectName("outsider");
    this->setFixedSize(455,600);
    addItemButton = new QPushButton(this);
    addItemButton->setFocusPolicy(Qt::NoFocus);
    deleteItem = new QPushButton(this);
    deleteItem->setFocusPolicy(Qt::NoFocus);
    search = new QPushButton(this);
    search->setFocusPolicy(Qt::NoFocus);
    showAll = new QPushButton(this);
    showAll->setFocusPolicy(Qt::NoFocus);
    select = new QPushButton(this);
    select->setFocusPolicy(Qt::NoFocus);
    change = new QPushButton(this);
    change->setFocusPolicy(Qt::NoFocus);
    exportExcel=new QPushButton(this);
    exportExcel->setFocusPolicy(Qt::NoFocus);
    tableView = new friendTableView(this);
    searchEdit = new QLineEdit(this);
    upButtonLayout = new QHBoxLayout;
    downButtonLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;
    tablemodel = new publisherTableModel;
    proxymodel = new QSortFilterProxyModel;
    comboBox = new comboBoxWithDropDown(this);
    statusBar =new mainStatusBar(this);

    comboBox->addItem("行数");
    comboBox->addItem("名称");
    comboBox->addItem("地点");
    comboBox->addItem("ISBN");
    comboBox->setCurrentText("名称");

    QStringList list;
    list<<"名称"<<"地点"<<"ISBN";
    tablemodel->setHeaderName(list);

    addItemButton->setText(tr("新建"));
    deleteItem->setText(tr("删除"));
    search->setText(tr("搜索"));
    showAll->setText(tr("显示全部"));
    select->setText(tr("选择"));
    change->setText(tr("修改"));
    exportExcel->setText(tr("导出"));

    upButtonLayout->addWidget(searchEdit);
    upButtonLayout->addWidget(search);
    upButtonLayout->addWidget(comboBox);
    upButtonLayout->addWidget(showAll);
    downButtonLayout->addWidget(addItemButton);
    downButtonLayout->addWidget(deleteItem);
    downButtonLayout->addWidget(exportExcel);
    mainLayout->addLayout(upButtonLayout);
    mainLayout->addWidget(tableView);
    mainLayout->addLayout(downButtonLayout);
    mainLayout->addWidget(select);
    mainLayout->addWidget(change);
    mainLayout->addWidget(statusBar);
    setLayout(mainLayout);

    proxymodel->setSourceModel(tablemodel);
    tableView->setModel(proxymodel);
    tableView->setSortingEnabled(true);
    tableView->sortByColumn(0,Qt::AscendingOrder);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setColumnWidth(0,50);
    tableView->setColumnWidth(1,200);
    tableView->setColumnWidth(2,75);
    tableView->setColumnWidth(3,75);

    Publisher rec;
    QFile file("../dat/publisher.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error publishertableviewwidget.cpp";
    QTextStream in(&file);
    while(!in.atEnd())
    {
        in>>rec.name>>rec.location>>rec.ISBN;
        recordlister.append(rec);
    }
    file.close();
    tablemodel->updateData(recordlister);
    connect(tablemodel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(updateFileData(QModelIndex)));
    connect(addItemButton,SIGNAL(clicked()),this,SLOT(isAddItem()));
    connect(change,SIGNAL(clicked()),this,SLOT(isChangeItem()));
    connect(deleteItem,SIGNAL(clicked()),this,SLOT(deleteData()));
    connect(search,SIGNAL(clicked()),this,SLOT(scrollFocusTo()));
    connect(showAll,SIGNAL(clicked()),this,SLOT(showAllRows()));
    connect(select,SIGNAL(clicked()),this,SLOT(sendPublishInfo()));
    connect(tableView,SIGNAL(doubleClicked(QModelIndex)),select,SIGNAL(clicked()));
    connect(exportExcel,SIGNAL(clicked()),this,SLOT(excelExport()));
    connect(searchEdit,SIGNAL(returnPressed()),search,SIGNAL(clicked()));
    statusBar->connectLater();
}

publisherTableViewWidget::~publisherTableViewWidget()
{
    delete addItemButton;
    delete deleteItem;
    delete tableView;
    delete downButtonLayout;
    delete mainLayout;
    delete tablemodel;
    delete proxymodel;
    delete searchEdit;
    delete search;
    delete showAll;
    delete comboBox;
    delete select;
    delete change;
    delete exportExcel;
    delete statusBar;
}

void publisherTableViewWidget::updateFileData(QModelIndex index)
{
    Publisher record;
    record=tablemodel->pList.at(index.row());
    recordlister.replace(index.row(),record);
}

void publisherTableViewWidget::flushFile()
{
    QFile file("../dat/publisher.txt");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        qDebug()<<"open file error publishertableviewwidget.cpp";
    QTextStream out(&file);
    int i;
    for(i=0;i<recordlister.count()-1;i++)
    {
        out<<recordlister[i].name<<" "<<recordlister[i].location<<" "<<recordlister[i].ISBN<<endl;
    }
    if(recordlister.size())
        out<<recordlister[i].name<<" "<<recordlister[i].location<<" "<<recordlister[i].ISBN;
    file.close();
}
#include <QDebug>
void publisherTableViewWidget::isAddItem()
{
    addOrChangeItem=0;
    addPublisherBox *inputDialog = new addPublisherBox;
    connect(inputDialog,SIGNAL(newData(QString,QString,QString)),this,SLOT(addData(QString,QString,QString)));
    inputDialog->show();
}

void publisherTableViewWidget::isChangeItem()
{
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
    Publisher rec=recordlister.at(row);
    addPublisherBox *inputDialog = new addPublisherBox(rec);//调用另外一个构造函数，以初始化输入框信息
    connect(inputDialog,SIGNAL(newData(QString,QString,QString)),this,SLOT(addData(QString,QString,QString)));
    inputDialog->show();
}

void publisherTableViewWidget::addData(QString a,QString b,QString c)
{
    Publisher record;
    record.name=a;
    record.location=b;
    record.ISBN=c;
    if(addOrChangeItem==0)
        recordlister.append(record);
    else
        recordlister.replace(currentRow,record);
    tablemodel->updateData(recordlister);
    flushFile();
}

void publisherTableViewWidget::deleteData()
{
        QModelIndexList mIndexList=tableView->selectedIndexes();
        if(mIndexList.isEmpty())
        {
            QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
        int bound=mIndexList.size()/(tablemodel->headerName.size()+1);
        for(int i=0;i<bound;i++)
        {
            int row=mIndexList[i*4].data().toInt()-1;
            recordlister.erase(recordlister.begin()+row-i);
        }
        tablemodel->updateData(recordlister);
        flushFile();
}

void publisherTableViewWidget::scrollFocusTo()
{
    QVariant str=searchEdit->text().trimmed();
    QString st=comboBox->currentText();
    if(st=="行数")
    {
        if(str.canConvert(QMetaType::Int))
        {
            tableView->verticalScrollBar()->setValue(str.toInt()-1);
        }
    }
    else
    {
        if(st=="名称")
        {
            proxymodel->setFilterKeyColumn(1);
        }
        else if(st=="地点")
        {
            proxymodel->setFilterKeyColumn(2);
        }
        else if(st=="ISBN")
        {
            proxymodel->setFilterKeyColumn(3);
        }
        filterChanged(str.toString());
    }
}

void publisherTableViewWidget::showAllRows()
{
    filterChanged("");
}

void publisherTableViewWidget::sendPublishInfo()
{
    QModelIndexList indexList=tableView->selectedIndexes();
    if(indexList.isEmpty())
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return;
    }
    Publisher pub;
    pub.name=indexList[1].data().toString();
    pub.location=indexList[2].data().toString();
    pub.ISBN=indexList[3].data().toString();
    emit publishInfo(pub);
    this->close();
}


void publisherTableViewWidget::filterChanged(QString str)
{
    QRegExp::PatternSyntax syntax = QRegExp::PatternSyntax(QRegExp::FixedString);
    QRegExp regExp(str, Qt::CaseInsensitive, syntax);
    proxymodel->setFilterRegExp(regExp);
    tableView->verticalScrollBar()->setValue(0);//滚动条滚至顶端
}


void publisherTableViewWidget::excelExport()
{
    QString filepath=QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),
                                                  tr("Excel Files (*.xls)"));
    ExportExcelObject temp(filepath,tr("出版社信息"),tableView);
    temp.addField(0,"行数","char(20)");
    temp.addField(1,"名称","char(200)");
    temp.addField(2,"地点","char(40)");
    temp.addField(3,"IBSN","char(30)");
    temp.export2Excel();
}

void publisherTableViewWidget::keyPressEvent(QKeyEvent *e)
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

#include <QDebug>
void publisherTableViewWidget::closeEvent(QCloseEvent *e)
{
    windowHash[6]=false;
    isOpen=false;
    flushFile();
    e->accept();
    QWidget::closeEvent(e);
}

void publisherTableViewWidget::showEvent(QShowEvent *e)
{
    select->setHidden(true);
    statusBar->readerInformationButton->setHidden(true);
    statusBar->readerInformButton->setHidden(true);
    searchEdit->setText("");
    filterChanged("");
    comboBox->setCurrentText("名称");
    windowHash[6]=true;
    windowPointer[6]=qobject_cast<QWidget *>(this);
    isOpen=true;
    QWidget::showEvent(e);
}

