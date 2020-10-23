/******************************************************************
文件名: bookborrowedbyreadertablewidget.cpp
创建人: 顾溢杰
日　期: 2017/4/10
修改人: 杨正浩、姜兴国
日　期: 2017/5/21
描　述: 读者借阅以及预约信息
版　本: v5.00 Copyright(c).
******************************************************************/
#include "bookborrowedbyreadertablewidget.h"
#include "functions.h"
#include "widget.h"
bookBorrowedByReaderWidget::bookBorrowedByReaderWidget(QWidget *parent):friendTableWidget(parent)
{

}

bookBorrowedByReaderWidget::bookBorrowedByReaderWidget(QVector<BookBorrowOrReserve> recVector, int readerPos,QWidget *parent):friendTableWidget(parent)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    updateData(recVector,readerPos);
}

bookBorrowedByReaderWidget::~bookBorrowedByReaderWidget()
{

}

void bookBorrowedByReaderWidget::init()//新建界面
{

}
void bookBorrowedByReaderWidget::updateData(QVector<BookBorrowOrReserve> recVector,int readerPos)//界面的更新
{
    this->clear();
    BookBorrowOrReserve tmpBBOR;
    this->setColumnCount(5);
    this->setRowCount(recVector.size());
    QStringList tmpList;//有以下几个表头
    tmpList<<"类型"<<"书名"<<"索书号"<<"起始日期"<<"到期时间";
    this->setColumnWidth(0,50);
    this->setColumnWidth(1,255);
    this->setColumnWidth(2,115);
    this->setColumnWidth(3,115);
    this->setColumnWidth(4,115);
    this->setHorizontalHeaderLabels(tmpList);
    int i;
    for(i=0;i<recVector.size();i++)//循环遍历 将借阅信息输出显示出来
    {
        tmpBBOR=recVector[i];
        if(tmpBBOR.borrowOrReserveFlag)
            this->setItem(i,0,new QTableWidgetItem("借阅"));
        else
            this->setItem(i,0,new QTableWidgetItem("预约"));
        this->setItem(i,1,new QTableWidgetItem(mainWindow->bookView->recordlister[tmpBBOR.bookPos].getName()));
        QString str;
        str.append(mainWindow->bookView->recordlister[tmpBBOR.bookPos].getClassificationNumber());
        str.append("/");
        str.append(mainWindow->bookView->recordlister[tmpBBOR.bookPos].getTypeNumber());
        str.append(".");
        str.append(QString("%1").arg(tmpBBOR.sameNumber));
        this->setItem(i,2,new QTableWidgetItem(str));
        this->setItem(i,3,new QTableWidgetItem(myDateToString(tmpBBOR.startDate)));
        if(tmpBBOR.borrowOrReserveFlag)
        {
            int auth=mainWindow->readerView->recordlister[readerPos].getAuth();
            int authPos=mainWindow->authorityView->findAuthPos(auth);
            int days;
            if(authPos!=-1)
                days=mainWindow->authorityView->recordlister[authPos].getBorrowBookDaysLimit();
            else
                days=20;
            this->setItem(i,4,new QTableWidgetItem(myDateToString(addDate(tmpBBOR.startDate,days+days*tmpBBOR.renewTimes))));
        }
        else
        {
            this->setItem(i,4,new QTableWidgetItem(myDateToString(addDate(tmpBBOR.startDate,3))));
        }
    }
}
