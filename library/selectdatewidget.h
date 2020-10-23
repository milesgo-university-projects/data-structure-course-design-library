/******************************************************************
文件名: selectdatewidget.h
创建人: 顾溢杰
日　期: 2017/5/20
修改人: 杨正浩，姜兴国
日　期: 2017/5/22
描　述: 重置日期时选择日期
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef SELECTDATEWIDGET
#define SELECTDATEWIDGET
#include <QWidget>
#include <QCalendarWidget>
#include <QDateEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCloseEvent>
#include <QShowEvent>

//选择日期用
class SelectDateWidget:public QWidget
{
  Q_OBJECT
public:
    SelectDateWidget(QWidget *parent=0);
    ~SelectDateWidget();
    QHBoxLayout *buttonLayout;                  //布局
    QVBoxLayout *mainLayout;
    QDateEdit *dateEdit;
    QCalendarWidget *calendar;                  //日历
    QPushButton *acceptButton,*rejectButton;    //确定和关闭按钮
public slots:
    void onAcceptButtonClicked();
signals:
    void dateInfo(QDate date);
protected:
    void closeEvent(QCloseEvent *e);            //关闭前需做的事
    void showEvent(QShowEvent *e);              //关闭后需要做的事
};

#endif // SELECTDATEWIDGET

