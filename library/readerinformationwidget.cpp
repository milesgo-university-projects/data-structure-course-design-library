#include "readerinformationwidget.h"
#include <QDebug>
#include "functions.h"
#include "widget.h"
class Widget;

//构造函数
InformationBlock::InformationBlock(QWidget *parent):QWidget(parent)
{
    this->setFixedSize(960,100);
    checkBox = new QCheckBox(this);
    checkBox->setFixedSize(85,18);
    informationLabel = new QLabel(this);
    informationLabel->setFixedSize(920,80);
    informationLabel->setStyleSheet("border: none");
    layout = new QHBoxLayout;
    layout->addWidget(checkBox);
    layout->addWidget(informationLabel);
    setLayout(layout);
}
//析构函数
InformationBlock::~InformationBlock()
{
    delete checkBox;
    delete informationLabel;
    delete layout;
}

void InformationBlock::setInformationLabel(QDate date,QStringList info)
{//在这里按照长度分行加入endl，使其规范显示
    QString str;
    checkBox->setText(myDateToString(date));
    int i;
    for(i=0;i<info.size()-1;i++)
    {
        str+=info[i];
        str+="\n";
    }
    if(info.size())
        str+=info[i];
    informationLabel->setText(str);
}
//显示窗口
InformationWidget::InformationWidget(QString readerID, QWidget *parent):QWidget(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle("通知");
    tmpReaderID=readerID;
    this->setFixedSize(1000,700);
    informationTableWidget = new QTableWidget(this);                        //各种按钮
    deleteButton = new QPushButton(this);
    deleteButton->setText("删除");
    deleteButton->setFocusPolicy(Qt::NoFocus);
    readAllButton = new QPushButton(this);
    readAllButton->setText("全部标记为已阅读");
    readAllButton->setFocusPolicy(Qt::NoFocus);
    selectAllCheckBox = new QCheckBox(this);
    selectAllCheckBox->setText("全选");
    buttonLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;
    buttonLayout->addWidget(selectAllCheckBox);                             //按钮布局
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(readAllButton);

    mainLayout->addWidget(informationTableWidget);                          //布局
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    updateInformationTableWidget(readerID);

    connect(informationTableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(onCellClicked(int)));
    connect(selectAllCheckBox,SIGNAL(toggled(bool)),this,SLOT(onSelectAllCheckBoxToggled(bool)));
    connect(deleteButton,SIGNAL(clicked()),this,SLOT(onDeleteButtonClicked()));
    connect(readAllButton,SIGNAL(clicked()),this,SLOT(onReadAllButtonClicked()));
}

InformationWidget::InformationWidget(QWidget *parent):QWidget(parent)
{

}
//窗体的析构函数
InformationWidget::~InformationWidget()
{
    delete informationTableWidget;
    delete deleteButton;
    delete readAllButton;
    delete selectAllCheckBox;
    delete buttonLayout;
    delete mainLayout;
}

//从文件中读取各种信息
void InformationWidget::readFile(QString readerID)
{
    recordlister.clear();
    QFile file("../dat/readerinformation/"+readerID+".txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"open file error";
        return;
    }
    QTextStream in(&file);
    QString str;
    in>>str;
    int bound=str.toInt();
    QList<int> informationLineSize;
    QList<int> isRead;
    QStringList dateList;
    for(int i=0;i<bound;i++)
    {
        in>>str;
        informationLineSize.push_back(str.toInt());
        in>>str;
        isRead.push_back(str.toInt());
        in>>str;
        dateList.push_back(str);
    }
    in.readLine();
    for(int i=0;i<bound;i++)
    {
        readerInformation rec;
        if(isRead[i]==0)
            rec.hasRead=false;
        else
            rec.hasRead=true;
        rec.date=myToDate(dateList[i]);
        for(int j=0;j<informationLineSize[i];j++)
        {
            str=in.readLine();
            rec.informationDivideByLine<<str;
        }
        recordlister.push_front(rec);
    }
    file.close();
}

//更新信息的窗口
void InformationWidget::updateInformationTableWidget(QString readerID)
{
    readFile(readerID);
    informationBlockPointer.clear();
    informationTableWidget->clear();

    QHeaderView *headerView=informationTableWidget->horizontalHeader();
    headerView->setHidden(true);
    informationTableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    informationTableWidget->setColumnCount(1);
    informationTableWidget->setRowCount(recordlister.size());
    for(int i=0;i<recordlister.size();i++)
        informationTableWidget->setRowHeight(i,100);
    informationTableWidget->setColumnWidth(0,980);
    for(int i=0;i<recordlister.size();i++)
    {
        informationBlockPointer.push_back(new InformationBlock);
        if(recordlister[i].hasRead)//设置字体浅
            informationBlockPointer[i]->informationLabel->setFont(QFont("Microsoft YaHei",10,10));
        else//设置字体深
            informationBlockPointer[i]->informationLabel->setFont(QFont("Microsoft YaHei",10,200));
        informationBlockPointer[i]->setInformationLabel(recordlister[i].date,recordlister[i].informationDivideByLine);
        informationTableWidget->setCellWidget(i,0,informationBlockPointer[i]);
    }
}
//刷新，使得文件和界面的数据始终保持一致
void InformationWidget::fllushFile(QString readerID)
{//有任何的操作就要flushFile
    QFile file("../dat/readerinformation/"+readerID+".txt");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"open file error";
        return;
    }
    QTextStream out(&file);
    out<<recordlister.size()<<" ";
    for(int i=recordlister.size()-1;i>=0;i--)
    {
        out<<recordlister[i].informationDivideByLine.size()<<" ";
        if(recordlister[i].hasRead)
            out<<"1 ";
        else
            out<<"0 ";
        out<<myDateToString(recordlister[i].date)<<" ";
    }
    out<<"\n";
    for(int i=recordlister.size()-1;i>=0;i--)
    {
        for(int j=0;j<recordlister[i].informationDivideByLine.size();j++)
            out<<recordlister[i].informationDivideByLine[j]<<"\n";
    }
    file.close();
}

//某一个单元被点击了
void InformationWidget::onCellClicked(int row)
{
    if(!recordlister[row].hasRead)
    {
        recordlister[row].hasRead=true;
        informationBlockPointer[row]->informationLabel->setFont(QFont("Microsoft YaHei",10,25));
        fllushFile(tmpReaderID);
        updateInformationTableWidget(tmpReaderID);
    }
}
//某个勾选框被选中了
void InformationWidget::onSelectAllCheckBoxToggled(bool isToggled)
{
    for(int i=0;i<recordlister.size();i++)
        informationBlockPointer[i]->checkBox->setChecked(isToggled);
}
//删除所选的某一列
void InformationWidget::onDeleteButtonClicked()
{
    for(int i=recordlister.size()-1;i>=0;i--)
    {
        if(informationBlockPointer[i]->checkBox->isChecked())
        {
            recordlister.erase(recordlister.begin()+i);
            delete informationBlockPointer[i];
            informationBlockPointer.erase(informationBlockPointer.begin()+i);
        }
    }
    fllushFile(tmpReaderID);
    updateInformationTableWidget(tmpReaderID);
}
//读取所有的
void InformationWidget::onReadAllButtonClicked()
{
    for(int i=0;i<recordlister.size();i++)
    {
        if(!recordlister[i].hasRead)
        {
            recordlister[i].hasRead=true;
            informationBlockPointer[i]->informationLabel->setFont(QFont("Microsoft YaHei",10,25));
        }
    }
    fllushFile(tmpReaderID);
    updateInformationTableWidget(tmpReaderID);
}

//关闭事件
void InformationWidget::closeEvent(QCloseEvent *e)
{
    windowHash[19]=false;
    mainWindow->readerInformationWidgetPointer=NULL;
    e->accept();
    QWidget::closeEvent(e);
}
//显示事件
void InformationWidget::showEvent(QShowEvent *e)
{
    windowHash[19]=true;
    mainWindow->readerInformationWidgetPointer=this;
    windowPointer[19]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}
