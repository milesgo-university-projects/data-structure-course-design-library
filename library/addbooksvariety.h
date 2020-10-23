/******************************************************************
文件名: addbooksvariety.h
创建人: 姜兴国
日　期: 2017/4/10
修改人: 姜兴国、顾溢杰
日　期: 2017/5/25
描　述: 构建图书分类管理界面，并进行相关的管理
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef ADDBOOKSVARIETY_H
#define ADDBOOKSVARIETY_H

#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>

#include <QFile>
#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <locale>
#include <QLocale>
#include <QtCore/QCoreApplication>
#include <QMessageBox>
#include <QString>
#include <QChar>
#include <QCloseEvent>
#include <QShowEvent>
#include "statusbar.h"

class addBooksVariety : public QWidget
{
    Q_OBJECT

public:
    explicit addBooksVariety(QWidget *parent = 0);
    ~addBooksVariety();

    QTreeWidget *tree;//创建树形结构
    QFormLayout *mainform;
    QHBoxLayout *buttonlayout1,*mainlay;    //构建按钮水平布局
    QLabel *addclass;//添加类别标签
    QLineEdit *addnumber;//要添加的类别的文本框
    QLineEdit *addcontent;//要添加的内容的文本框
    QPushButton *buttonyes1;//确认按钮
    QPushButton *select;//选择按钮
    QPushButton *buttoncancel;//删除按钮
    QVBoxLayout *buttonlayout2,*trueMainLayout;
    QString addClassNum;//获取的输入添加的类编号
    QString addClassContent;//获取的输入添加的内容
    QString deleClass;//获取删除的类或内容
    mainStatusBar *statusBar;

    void InitTree(QTreeWidget *&,QTextStream &);//创建
    QString findChildTree(QString,QTextStream &,QTreeWidgetItem *&,QString &,QString);
    void addClassFunc(QString &,QString &);//新建类别并写入文件的操作
    bool updateTree(QTreeWidgetItem*,QString,QString);//显示的树形结构的添加
    void deleClassAndHisChildrenFunc(QString &);//删除类别
    bool isEditingBook;
signals:
    void bookClassificationInfo(QString classificationNumber,QString classificationName);
public slots:
    void getAddFunc();
    void onDeleteButtonClicked();
    void onselectButtonClicked();
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // ADDBOOKSVARIETY_H
