#include "widget.h"
#include <QDebug>
#include "functions.h"
#include "Reader.h"
#include "addfeeacount.h"
#include "login.h"
struct BookBorrowOrReserve;
class instituteManageWidget;

//构造函数
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    mainWindow=this;                                 //此即为全局变量中的主窗口
    readerView=NULL;                                 //赋值为NULL代表现在暂时不需要
    bookView=NULL;
    publisherView=NULL;
    instituteView=NULL;
    authorityView=NULL;
    statistic=NULL;
    bookBorrowOrReserveView=NULL;
    administratorManageView=NULL;
    bookVarietiesView=NULL;
    timeSimulationWidget = new timeSimulation(this); //这个的创建必须在tabwidget之前
    readerView = new readerViewWidget;
    readerView->select->setEnabled(false);
    bookView = new bookViewWidget;
    bookView->select->setEnabled(false);
    publisherView = new publisherTableViewWidget;
    instituteView = new instituteManageWidget;
    authorityView = new authorityWidget;
    administratorManageView = new administratorManageWidget;
    bookVarietiesView = new addBooksVariety;

    completeBookViewAndReaderView();

    //mytime.setDate(myToDate(settings->value("date").toString()));
    this->move(settings->value("position",QPoint(30,30)).toPoint());
    this->setFixedSize(400,500);                     //设置大小
    this->setWindowFlags(Qt::FramelessWindowHint);

    titlebar = new titleBar(this);
    menubar = new QMenuBar(this);
    //toolbar = new QToolBar(this);
    statusbar = new mainStatusBar(this);
    bookBorrowOrReserveView = new bookBorrowOrReserveManageWidget;
    mainTabWidget = new MainTabWidget(this);

    signOutButton = new twoSizeButton("sign_out_16px","sign_out_24px",QSize(30,30),QSize(16,16),QSize(24,24));
    signOutButton->setStyleSheet("border: none");
    //signOutButtonLabel = new QLabel(signOutButton);
    //signOutButtonLabel->setFixedSize(24,24);
    //signOutButtonLabel->setAttribute(Qt::WA_TranslucentBackground);
    //signOutButtonLabel->setStyleSheet("background: none");
    //signOutButtonLabel->setPixmap(QPixmap("../images/sign_out.png"));
    searchBookLabel = new QLabel(this);              //创建并设置各种标签，输入框
    searchBookLabel->setText("请输入书名进行搜索");
    searchBookEdit = new SearchLineEdit(this);
    searchBookEdit->setFixedHeight(40);
    searchBookLayout = new QHBoxLayout;
    searchBookLayout->setSpacing(10);
    signOutButton->setFixedHeight(30);
    searchBookLayout->addWidget(signOutButton);
    searchBookLayout->addWidget(searchBookLabel);
    searchBookLayout->addWidget(searchBookEdit);

    mainVLayout = new QVBoxLayout;

    initAction();

    installEventFilter(titlebar);
    setWindowTitle(tr("图书管理系统"));                 //设置程序名
    setWindowIcon(QIcon("../option/titlebarImages/library.png"));
    menubar->setFixedHeight(30);

    mainVLayout->setSpacing(0);
    mainVLayout->addWidget(titlebar);
    mainVLayout->addLayout(searchBookLayout);
    mainVLayout->addWidget(menubar);
    //mainVLayout->addWidget(toolbar);
    mainVLayout->addWidget(mainTabWidget);
//    statusbar->setFixedHeight(20);
//    mainVLayout->addWidget(statusbar,Qt::AlignBottom);
    mainVLayout->addWidget(statusbar);
    setLayout(mainVLayout);
                                                      //连接各种回调函数
    connect(bookBorrowOrReserveView,SIGNAL(newBorrowAndReserve(BorrowOrReserve)),bookView,SLOT(updateBorrowAndReserveInfo(BorrowOrReserve)));
    connect(bookBorrowOrReserveView,SIGNAL(newBorrowAndReserve(BorrowOrReserve)),readerView,SLOT(updateBorrowAndReserveInfo(BorrowOrReserve)));
    connect(signOutButton,SIGNAL(clicked()),this,SLOT(onChangeAccountActionTriggered()));
    connect(searchBookEdit->searchButton,SIGNAL(clicked()),mainWindow,SLOT(onSearchButtonClicked()));

    statusbar->connectLater();
    //qDebug()<<statusbar->height();
}

//析构函数
Widget::~Widget()
{
//    if(timeSimulationWidget==NULL)
//        qDebug()<<"YES";
    delete timeSimulationWidget;//如果先delete tabwidget会出现问题，为什么？tabwidget delete的时候会把其中的widget也一并delete吗？即便这些widget的parent并非tabwidget
    delete mainTabWidget;

    delete addNewBookAction;
    delete addNewReaderAction;
    delete addNewPublisherAction;
    delete addNewAuthorityAction;
    delete addNewInstitutionAction;
    delete addNewBorrowAndReserveAction;
    delete addFeeAcountAction;
    delete addNewActionGroup;
    delete menu1;

    delete startSimulationAction;
    delete stopSimulationAction;
    delete simulationActionGroup;
    delete menu2;

    delete bookManageAction;
    delete readerManageAction;
    delete publisherManageAction;
    delete instituteManageAction;
    delete varietiesManageAction;
    delete manageActionGroup;
    delete menu3;

    delete setSimulationSpeedAction;
    delete setActionGroup;
    delete menu4;

    delete changeAccountAction;
    delete administratorAccountManageAction;
    delete accountManageActionGroup;
    delete menu5;

    //delete signOutButtonLabel;
    delete signOutButton;
    delete searchBookLabel;
    delete searchBookEdit;
    delete searchBookLayout;
    delete menubar;
    delete titlebar;
    //delete toolbar;
    delete statusbar;
    delete mainVLayout;
                                                    //以下均为在主界面关闭的情况下，子界面同时关闭
    if(readerView!=NULL)                            //以下if均为，判断是否存在
    {
        delete readerView;
        readerView=NULL;
    }
    if(bookView!=NULL)
    {
        delete bookView;
        bookView=NULL;
    }
    if(publisherView!=NULL)
    {
        delete publisherView;
        publisherView=NULL;
    }
    if(instituteView!=NULL)
    {
        delete instituteView;
        instituteView=NULL;
    }
    if(authorityView!=NULL)
    {
        delete authorityView;
        authorityView=NULL;
    }
    if(bookBorrowOrReserveView!=NULL)
    {
        delete bookBorrowOrReserveView;
        bookBorrowOrReserveView=NULL;
    }
    if(statistic!=NULL)
    {
        delete statistic;
        statistic=NULL;
    }
}

//对一些基本事件的响应
bool Widget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)

    const int m_nBorder=5;
    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
        case WM_NCHITTEST:
        {
            int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
            int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

            // 如果鼠标位于子控件上，则不进行处理
            if (childAt(nX, nY) != NULL)
                return QWidget::nativeEvent(eventType, message, result);

            *result = HTCAPTION;

            // 鼠标区域位于窗体边框，进行缩放
            if ((nX > 0) && (nX < m_nBorder))
                *result = HTLEFT;

            if ((nX > this->width() - m_nBorder) && (nX < this->width()))
                *result = HTRIGHT;

            if ((nY > 0) && (nY < m_nBorder))
                *result = HTTOP;

            if ((nY > this->height() - m_nBorder) && (nY < this->height()))
                *result = HTBOTTOM;

            if ((nX > 0) && (nX < m_nBorder) && (nY > 0)
                    && (nY < m_nBorder))
                *result = HTTOPLEFT;

            if ((nX > this->width() - m_nBorder) && (nX < this->width())
                    && (nY > 0) && (nY < m_nBorder))
                *result = HTTOPRIGHT;

            if ((nX > 0) && (nX < m_nBorder)
                    && (nY > this->height() - m_nBorder) && (nY < this->height()))
                *result = HTBOTTOMLEFT;

            if ((nX > this->width() - m_nBorder) && (nX < this->width())
                    && (nY > this->height() - m_nBorder) && (nY < this->height()))
                *result = HTBOTTOMRIGHT;

            return true;
        }
        default:
            break;
    }
    return QWidget::nativeEvent(eventType, message, result);
}

void Widget::closeEvent(QCloseEvent *e)
{
    for(int i=0;i<30;i++)
    {
        if(windowHash[i])
        {
            windowHash[i]=false;
            windowPointer[i]->close();
        }
    }
    windowHash[0]=false;
    settings->setValue("size",this->size());
    settings->setValue("position",this->pos());
    settings->setValue("date",myDateToString(mytime.getDate()));
    QWidget::closeEvent(e);
}

//显示事件
void Widget::showEvent(QShowEvent *e)
{
    windowHash[0]=true;
    windowPointer[0]=qobject_cast<QWidget *>(this);
    if(!isManager)
    {
        setFixedSize(600,150);
        menubar->setHidden(true);
        statusbar->readerInformationButton->setHidden(false);
        statusbar->readerInformButton->setHidden(false);
        mainTabWidget->setHidden(true);
        signOutButton->setHidden(false);
        searchBookEdit->setHidden(false);
        searchBookLabel->setHidden(false);
    }
    else
    {
        setFixedSize(400,500);
        menubar->setHidden(false);
        statusbar->readerInformationButton->setHidden(true);
        statusbar->readerInformButton->setHidden(true);
        mainTabWidget->setHidden(false);
        signOutButton->setHidden(true);
        searchBookEdit->setHidden(true);
        searchBookLabel->setHidden(true);
    }
    QWidget::showEvent(e);
}

//创建图书管理折线图
void Widget::createBookViewWidget()
{
    if(bookView==NULL)
        bookView = new bookViewWidget;
    bookView->show();
}

//创建最近七天的折线图
void Widget::createStatisticGraph1()
{
    if(statistic!=NULL)
    {
        delete statistic;
        statistic=NULL;
    }
    if(statistic==NULL)
        statistic=new StatisticsGraph;
    statistic->set("七天借阅率","","借阅率");
    QVector<bookRentRate> temp=statisticdata.getRate();
    QVector<QString> label;
    QVector<double> data;
    for(int i=0;i<temp.size();i++)
    {
        label.push_back(temp[i].date);
        data.push_back(temp[i].bookCountPerDay/double(temp[i].bookNums));
    }
    statistic->lineChart(label,data);

}

//创建借阅书籍前十的柱状图
void Widget::createStatisticGraph2()
{
    if(statistic!=NULL)
    {
        delete statistic;
        statistic=NULL;
    }
    if(statistic==NULL)
        statistic=new StatisticsGraph;
    statistic->set("借阅书籍前十","","借阅数 单位:本");
    QVector<QPair<QString,int> > a=statisticdata.getToptenBook();
    QVector<QString> label;
    QVector<double> data;
    for(int i=0;i<a.size();i++)
    {
        label.push_back(a[i].first);
        data.push_back(a[i].second);
    }
    statistic->barChart(label,data);
}

//创建借阅次数前十的读者的统计图
void Widget::createStatisticGraph3()
{
    if(statistic!=NULL)
    {
        delete statistic;
        statistic=NULL;
    }
    if(statistic==NULL)
        statistic=new StatisticsGraph;
    statistic->set("读者借书前十","","借阅数 单位：本");
    QVector<QPair<QString,int> > a=statisticdata.getToptenReader();
    QVector<QString> label;
    QVector<double> data;
    for(int i=0;i<a.size();i++)
    {
        label.push_back(a[i].first);
        data.push_back(a[i].second);
    }
    statistic->barChart(label,data);
}
void Widget::createReaderViewWidget()
{
    if(readerView==NULL)
        readerView = new readerViewWidget;
    readerView->show();
}

//创建管理出版的的窗口
void Widget::createPublisherViewWidget()
{
    QObject *object=qobject_cast<QObject *>(sender());
    if(publisherView==NULL)
    {
        publisherView = new publisherTableViewWidget;
    }
    publisherView->show();
    if(object!=NULL&&object->objectName()=="publisherSearchButton")
    {
        publisherView->select->setHidden(false);
    }
}

//创建管理学院的窗口
void Widget::createInstituteViewWidget()
{
    if(instituteView==NULL)
    {
        instituteView = new instituteManageWidget;
    }
    instituteView->show();
}

//创建管理权限的窗口
void Widget::createAuthorityViewWidget()
{
    if(authorityView==NULL)
    {
        authorityView = new authorityWidget;
    }
    authorityView->show();
}

//创建图书借阅预约窗口
void Widget::createBookBorrowOrReserveViewWidget()
{
    if(bookBorrowOrReserveView==NULL)
    {
        bookBorrowOrReserveView = new bookBorrowOrReserveManageWidget;
    }
    bookBorrowOrReserveView->show();
}

//创建管理员窗口
void Widget::createAdministratorManageViewWidget()
{
    if(administratorManageView==NULL)
    {
        administratorManageView = new administratorManageWidget;
    }
    administratorManageView->show();
}

//创建分类的窗口
void Widget::createVarietiesManageViewWidget()
{
    QObject *object = qobject_cast<QObject *>(sender());
    if(object!=NULL&&object->objectName()=="editing")
        bookVarietiesView->isEditingBook=true;
    if(bookVarietiesView==NULL)
    {
        bookVarietiesView = new addBooksVariety;
    }
    bookVarietiesView->show();
}

//初始化函数
void Widget::initAction()
{
    addNewBookAction = new QAction(QIcon("../images/add_24px.png"),tr("添加图书"),this);         //各种action
    addNewBookAction->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_B));
    addNewReaderAction = new QAction(QIcon("../images/add_24px.png"),tr("添加读者"),this);
    addNewReaderAction->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_R));
    addNewPublisherAction = new QAction(QIcon("../images/add_24px.png"),tr("添加出版社"),this);
    addNewPublisherAction->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_P));
    addNewAuthorityAction = new QAction(QIcon("../images/add_24px.png"),tr("添加权限/读者类型"),this);
    addNewAuthorityAction->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_A));
    addNewInstitutionAction = new QAction(QIcon("../images/add_24px.png"),tr("添加学院/部门"),this);
    addNewInstitutionAction->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_I));
    addNewBorrowAndReserveAction = new QAction(QIcon("../images/add_24px.png"),tr("添加借阅/预约"),this);
    addNewBorrowAndReserveAction->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_L));
    addFeeAcountAction = new QAction(QIcon("../images/add_24px.png"),tr("读者余额充值"),this);//充值余额
    addFeeAcountAction->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_M));


    addNewActionGroup = new QActionGroup(this);                                               //各种组
    addNewActionGroup->addAction(addNewBookAction);
    addNewActionGroup->addAction(addNewReaderAction);
    addNewActionGroup->addAction(addNewPublisherAction);
    addNewActionGroup->addAction(addNewAuthorityAction);
    addNewActionGroup->addAction(addNewInstitutionAction);
    addNewActionGroup->addAction(addNewBorrowAndReserveAction);
    addNewActionGroup->addAction(addFeeAcountAction);

    menu1 = new QMenu(tr("新建"));                                                             //各种菜单
    menu1->addAction(addNewBookAction);
    menu1->addAction(addNewReaderAction);
    menu1->addAction(addNewPublisherAction);
    menu1->addAction(addNewAuthorityAction);
    menu1->addAction(addNewInstitutionAction);
    menu1->addAction(addNewBorrowAndReserveAction);
    menu1->addAction(addFeeAcountAction);
    menubar->addMenu(menu1);
                                                                                               //连接槽函数和信号
    connect(addNewBookAction,SIGNAL(triggered()),mainWindow->bookView->addItem,SIGNAL(clicked()));
    connect(addNewReaderAction,SIGNAL(triggered()),mainWindow->readerView->addItem,SIGNAL(clicked()));
    connect(addNewPublisherAction,SIGNAL(triggered()),mainWindow->publisherView->addItemButton,SIGNAL(clicked()));
    connect(addNewAuthorityAction,SIGNAL(triggered()),mainWindow->authorityView->addItem,SIGNAL(clicked()));
    connect(addNewInstitutionAction,SIGNAL(triggered()),mainWindow->instituteView->addButton,SIGNAL(clicked()));
    connect(addNewBorrowAndReserveAction,SIGNAL(triggered()),mainWindow->bookBorrowOrReserveView->addItem,SIGNAL(clicked()));
    connect(addFeeAcountAction,SIGNAL(triggered()),this,SLOT(onAddFeeAcountTriggered()));

    startSimulationAction = new QAction(QIcon("../images/start.png"),tr("开始模拟"),this);     //模拟部分
    startSimulationAction->setShortcut(QKeySequence(Qt::SHIFT+Qt::Key_S));
    stopSimulationAction = new QAction(QIcon("../images/pause.png"),tr("停止模拟"),this);
    stopSimulationAction->setShortcut(QKeySequence(Qt::SHIFT+Qt::Key_T));
    changeUpdateTimeAction = new QAction(QIcon("../images/option.png"),tr("调整速率"),this);
    changeUpdateTimeAction->setShortcut(QKeySequence(Qt::SHIFT+Qt::Key_C));
    resetDateAction = new QAction(QIcon("../images/option.png"),tr("日期重置"),this);
    resetDateAction->setShortcut(QKeySequence(Qt::SHIFT+Qt::Key_R));

    startSimulationAction->setObjectName("insider");
    stopSimulationAction->setObjectName("insider");
    simulationActionGroup = new QActionGroup(this);
    simulationActionGroup->addAction(startSimulationAction);
    simulationActionGroup->addAction(stopSimulationAction);
    simulationActionGroup->addAction(changeUpdateTimeAction);
    simulationActionGroup->addAction(resetDateAction);
    startSimulationAction->setStatusTip(tr("点击开始模拟"));
    stopSimulationAction->setStatusTip(tr("点击停止模拟"));
    changeUpdateTimeAction->setStatusTip(tr("点击调整模拟速率"));
    resetDateAction->setStatusTip(tr("点击重置日期"));

    menu2 = new QMenu(tr("模拟"));
    menu2->addAction(startSimulationAction);
    menu2->addAction(stopSimulationAction);
    menu2->addAction(changeUpdateTimeAction);
    menu2->addAction(resetDateAction);
    menubar->addMenu(menu2);

    connect(startSimulationAction,SIGNAL(triggered()),timeSimulationWidget,SLOT(startsimulation()));
    connect(stopSimulationAction,SIGNAL(triggered()),timeSimulationWidget,SLOT(stopsimulation()));
    connect(changeUpdateTimeAction,SIGNAL(triggered()),timeSimulationWidget,SLOT(changeupdatetime()));
    connect(resetDateAction,SIGNAL(triggered()),timeSimulationWidget,SLOT(resetsimulation()));

    bookManageAction = new QAction(QIcon("../images/book.png"),tr("图书管理"),this);           //new各种action
    bookManageAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_B));
    readerManageAction = new QAction(QIcon("../images/reader.png"),tr("读者管理"),this);
    readerManageAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_R));
    publisherManageAction = new QAction(QIcon("../images/publisher.png"),tr("出版社管理"),this);
    publisherManageAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
    instituteManageAction = new QAction(QIcon("../images/school.png"),tr("学院/部门管理"),this);
    instituteManageAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_I));
    authorityManageAction = new QAction(QIcon("../images/authority.png"),tr("权限/读者类型管理"),this);
    authorityManageAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_A));
    bookBorrowOrReserveManageAction = new QAction(QIcon("../images/reader_and_book.png"),tr("借阅/预约管理"),this);
    bookBorrowOrReserveManageAction->setShortcut(QKeySequence(Qt::SHIFT+Qt::Key_B));
    varietiesManageAction = new QAction(QIcon("../images/classification.png"),tr("图书分类管理"),this);
    varietiesManageAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));

    manageActionGroup = new QActionGroup(this);                                             //各种管理的类
    manageActionGroup->addAction(bookManageAction);
    manageActionGroup->addAction(readerManageAction);
    manageActionGroup->addAction(publisherManageAction);
    manageActionGroup->addAction(instituteManageAction);
    manageActionGroup->addAction(authorityManageAction);
    manageActionGroup->addAction(bookBorrowOrReserveManageAction);
    manageActionGroup->addAction(varietiesManageAction);

    bookManageAction->setStatusTip(tr("点击管理图书"));                                       //设置各种标签
    readerManageAction->setStatusTip(tr("点击管理读者"));
    publisherManageAction->setStatusTip(tr("点击管理出版社"));
    instituteManageAction->setStatusTip(tr("点击管理学院/部门信息"));
    authorityManageAction->setStatusTip(tr("点击管理权限/读者类型"));
    bookBorrowOrReserveManageAction->setStatusTip(tr("点击管理借阅/预约信息"));
    varietiesManageAction->setStatusTip(tr("点击管理图书分类"));

    menu3 = new QMenu(tr("管理"));
    menu3->addAction(bookManageAction);
    menu3->addAction(readerManageAction);
    menu3->addAction(publisherManageAction);
    menu3->addAction(instituteManageAction);
    menu3->addAction(authorityManageAction);
    menu3->addAction(bookBorrowOrReserveManageAction);
    menu3->addAction(varietiesManageAction);
    menubar->addMenu(menu3);

    connect(bookManageAction,SIGNAL(triggered()),this,SLOT(createBookViewWidget()));
    connect(readerManageAction,SIGNAL(triggered()),this,SLOT(createReaderViewWidget()));
    connect(publisherManageAction,SIGNAL(triggered()),this,SLOT(createPublisherViewWidget()));
    connect(instituteManageAction,SIGNAL(triggered()),this,SLOT(createInstituteViewWidget()));
    connect(authorityManageAction,SIGNAL(triggered()),this,SLOT(createAuthorityViewWidget()));
    connect(bookBorrowOrReserveManageAction,SIGNAL(triggered()),this,SLOT(createBookBorrowOrReserveViewWidget()));
    connect(varietiesManageAction,SIGNAL(triggered()),this,SLOT(createVarietiesManageViewWidget()));

    setSimulationSpeedAction = new QAction(QIcon("../images/option.png"),tr("设置模拟速率"),this);

    setActionGroup = new QActionGroup(this);
    setActionGroup->addAction(setSimulationSpeedAction);

    setSimulationSpeedAction->setStatusTip(tr("点击设置模拟速率"));

    menu4 = new QMenu(tr("设置"));
    menu4->addAction(setSimulationSpeedAction);
    menubar->addMenu(menu4);

    connect(setSimulationSpeedAction,SIGNAL(triggered()),mainWindow->timeSimulationWidget,SLOT(changeupdatetime()));

    changeAccountAction = new QAction(QIcon("../images/account.png"),tr("更改账号"),this);
    administratorAccountManageAction = new QAction(QIcon("../images/account.png"),tr("管理员账户管理"),this);

    accountManageActionGroup = new QActionGroup(this);
    accountManageActionGroup->addAction(changeAccountAction);
    accountManageActionGroup->addAction(administratorAccountManageAction);

    changeAccountAction->setStatusTip(tr("点击更改账号"));
    administratorAccountManageAction->setStatusTip(tr("点击管理管理员账户"));

    menu5 = new QMenu(tr("账号"));
    menu5->addAction(changeAccountAction);
    menu5->addAction(administratorAccountManageAction);

    menubar->addMenu(menu5);

    connect(changeAccountAction,SIGNAL(triggered(bool)),this,SLOT(onChangeAccountActionTriggered()));
    connect(administratorAccountManageAction,SIGNAL(triggered(bool)),this,SLOT(createAdministratorManageViewWidget()));
}

//借阅或者预约窗口
void Widget::completeBookViewAndReaderView()
{
    BookBorrowOrReserve tmpBBOR;
    QFile file2("../dat/borrowOrReserve.txt");                                             //从文件中读取各种信息
    if(!file2.open(QIODevice::ReadOnly|QIODevice::Text))
        qDebug()<<"open file error bookviewwidget.cpp";
    QTextStream in2(&file2);
    QString str;
    while(!in2.atEnd())
    {
        bool borrowOrReserveFlag;
        in2>>str;
        if(str=="1")
            borrowOrReserveFlag=true;
        else
            borrowOrReserveFlag=false;
        tmpBBOR.borrowOrReserveFlag=borrowOrReserveFlag;
        in2>>str;
        int readerRow=str.toInt();
        in2>>str;
        int bookRow=str.toInt();
        tmpBBOR.bookPos=bookRow;
        in2>>str;
        int sameNumber=str.toInt();
        tmpBBOR.sameNumber=sameNumber;
        in2>>str;
        QDate addDater=myToDate(str);
        tmpBBOR.startDate=addDater;
        in2>>str;
        tmpBBOR.renewTimes=str.toInt();
        if(borrowOrReserveFlag)
        {
            bookView->recordlister[bookRow].setIsBorrowed(true,sameNumber);
            bookView->recordlister[bookRow].setBorrowerID(readerView->recordlister[readerRow].getId(),sameNumber);
        }
        else
        {
            bookView->recordlister[bookRow].setIsReserved(true,sameNumber);
            bookView->recordlister[bookRow].setReserverID(readerView->recordlister[readerRow].getId(),sameNumber);
        }
        readerView->recordlister[readerRow].addBookBorrowOrReserveVector(tmpBBOR);
    }
    readerView->tablemodel->updateData(readerView->recordlister);
    readerView->flushFile();
    bookView->tablemodel->updateData(bookView->recordlister);
    bookView->flushFile();
    file2.close();
}

//充值
void Widget::onAddFeeAcountTriggered()
{
    addFeeAcount *inputDialog = new addFeeAcount;
    inputDialog->show();
}

void Widget::onChangeAccountActionTriggered()
{
    for(int i=1;i<30;i++)
    {
        if(windowHash[i])
        {
            QMessageBox message(QMessageBox::Warning,"警告","请先关闭所有子窗口",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            return;
        }
    }
    this->close();
    loginDialog logindialog;
    if(logindialog.exec()==QDialog::Accepted)
        this->show();
}

//搜索按钮
void Widget::onSearchButtonClicked()
{
    QString str=searchBookEdit->text().trimmed();
    mainWindow->bookView->comboBox->setCurrentText("书名");
    mainWindow->bookView->searchEdit->setText(str);
    emit mainWindow->bookView->search->clicked(true);
    mainWindow->bookView->isReaderLooking=true;
    mainWindow->createBookViewWidget();
}
