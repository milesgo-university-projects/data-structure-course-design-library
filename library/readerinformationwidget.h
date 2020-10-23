/******************************************************************
文件名: readerinformation.h
创建人: 杨正浩
日　期: 2017/4/26
修改人: 顾溢杰，姜兴国
日　期: 2017/5/23
描　述: 显示所有读者信息的窗体
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef READERINFORMATIONWIDGET
#define READERINFORMATIONWIDGET

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QShowEvent>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QTextStream>
#include <QHeaderView>
#include <QDate>

//存部分信息
class readerInformation
{
public:
    bool hasRead;
    QDate date;
    QStringList informationDivideByLine;
};

//显示窗口
class InformationBlock:public QWidget
{
  Q_OBJECT
public:
    InformationBlock(QWidget *parent=0);
    ~InformationBlock();
    QCheckBox *checkBox;
    QLabel *informationLabel;
    QHBoxLayout *layout;
    void setInformationLabel(QDate date,QStringList info);
};

class InformationWidget:public QWidget
{
  Q_OBJECT
public:
    InformationWidget(QWidget *parent=0);
    InformationWidget(QString readerID,QWidget *parent=0);
    ~InformationWidget();
    QTableWidget *informationTableWidget;                              //窗体
    QPushButton *deleteButton,*readAllButton;
    QCheckBox *selectAllCheckBox;
    QList<InformationBlock *> informationBlockPointer;
    QHBoxLayout *buttonLayout;                                         //布局
    QVBoxLayout *mainLayout;
    QList<readerInformation> recordlister;
    void readFile(QString readerID);                                   //从文件中读取信息
    void updateInformationTableWidget(QString readerID);               //更改信息
    void fllushFile(QString readerID);                                 //有任何变化就刷新文件
    QString tmpReaderID;
public slots:
    void onCellClicked(int row);                                       //单元被点击了
    void onSelectAllCheckBoxToggled(bool isToggled);
    void onDeleteButtonClicked();
    void onReadAllButtonClicked();
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // READERINFORMATIONWIDGET

