/******************************************************************
文件名: widget.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/21
描　述: 程序主要界面
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef WIDGET_H
#define WIDGET_H

#include "titlebar.h"
#include "maintabwidget.h"
#include "mytime.h"
#include "statusbar.h"
#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPoint>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QCloseEvent>
#include <QShowEvent>
#include <qt_windows.h>
#include <windowsx.h>
#include "bookviewwidget.h"
#include "readerviewwidget.h"
#include "timesimulation.h"
#include "publishertableviewwidget.h"
#include "institutewidget.h"
#include "authoritywidget.h"
#include "addbookborroworreservewidget.h"
#include "bookborroworreservemanagewidget.h"
#include "administratormanagewidget.h"
#include "searchlineedit.h"
#include "twosizebutton.h"
#include "addbooksvariety.h"
#include "readerinformationwidget.h"
#include"statisticsgraph.h"
#include<QVector>
#include<QPair>
class MainTabWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    MainTabWidget *mainTabWidget;                                     //主界面
    titleBar *titlebar;
    QMenuBar * menubar;                                               //菜单栏
    mainStatusBar *statusbar;
    QVBoxLayout *mainVLayout;

    bookViewWidget *bookView;                                         //图书管理界面
    readerViewWidget *readerView;                                     //读者管理界面
    timeSimulation *timeSimulationWidget;
    StatisticsGraph *statistic;                                       //统计图部分
    publisherTableViewWidget *publisherView;                          //出版社界面
    instituteManageWidget *instituteView;                             //院系界面
    authorityWidget *authorityView;                                   //权限管理界面
    bookBorrowOrReserveManageWidget *bookBorrowOrReserveView;         //图书预约或借阅界面
    administratorManageWidget *administratorManageView;               //管理员界面
    addBooksVariety *bookVarietiesView;

    twoSizeButton *signOutButton;                                     //界面上的各种标签，按钮
    QLabel *searchBookLabel;
    SearchLineEdit *searchBookEdit;
    QHBoxLayout *searchBookLayout;

    QMenu *menu1,*menu2,*menu3,*menu4,*menu5;                         //各种菜单
    QAction *startSimulationAction,*stopSimulationAction,*changeUpdateTimeAction,*resetDateAction;
    QAction *readerManageAction,*bookManageAction,*publisherManageAction,*instituteManageAction,*authorityManageAction,*bookBorrowOrReserveManageAction,*varietiesManageAction;
    QAction *addNewBookAction,*addNewReaderAction,*addNewPublisherAction,*addNewAuthorityAction,*addNewInstitutionAction,*addNewBorrowAndReserveAction,*addFeeAcountAction;
    QAction *setSimulationSpeedAction;
    QAction *changeAccountAction,*administratorAccountManageAction;
    QActionGroup *simulationActionGroup,*manageActionGroup,*addNewActionGroup,*setActionGroup,*accountManageActionGroup;

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
    void initAction();
    void completeBookViewAndReaderView();

    bool isManager;
    QString currentAccountName;
    int currentReaderPos;
    int cursorX,cursorY;

    InformationWidget *readerInformationWidgetPointer;
public slots:                                                          //以下函数均为点击后创建对应子页面的函数
    void createBookViewWidget();
    void createReaderViewWidget();
    void createPublisherViewWidget();
    void createInstituteViewWidget();
    void createAuthorityViewWidget();
    void createBookBorrowOrReserveViewWidget();
    void createAdministratorManageViewWidget();
    void createVarietiesManageViewWidget();
    void onAddFeeAcountTriggered();
    void onChangeAccountActionTriggered();
    void onSearchButtonClicked();
    void createStatisticGraph1();                                      //创建统计图
    void createStatisticGraph2();
    void createStatisticGraph3();
};

#endif // WIDGET_H
