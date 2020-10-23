#include "maintabwidget.h"
#include "functions.h"
#include <QDebug>

class Widget;

MainTabWidget::MainTabWidget(Widget *parent):QTabWidget(parent)
{
    setTabShape(QTabWidget::Rounded);

    bookManageWidget = new bookManageButtonsWidget;
    readerManageWidget = new readerManageButtonsWidget;
    bookBORManageWidget = new bookBorrowOrReserveManageButtonsWidget;
    statistic=new statisticWidget;
    label4 = new QLabel;

    label4->setText(tr("label4"));

    addTab(bookManageWidget,tr("图书管理"));
    addTab(readerManageWidget,tr("读者管理"));
    addTab(bookBORManageWidget,tr("借阅/预约"));
    addTab(statistic,tr("统计"));
    addTab(parent->timeSimulationWidget,tr("日期模拟"));

    connect(bookManageWidget->bookManageButton,SIGNAL(clicked()),parent,SLOT(createBookViewWidget()));
    connect(bookManageWidget->addBookButton,SIGNAL(clicked()),mainWindow->bookView->addItem,SIGNAL(clicked()));
    connect(bookManageWidget->varietyManageButton,SIGNAL(clicked()),parent,SLOT(createVarietiesManageViewWidget()));
    connect(readerManageWidget->readerManageButton,SIGNAL(clicked()),parent,SLOT(createReaderViewWidget()));
    connect(readerManageWidget->addReaderButton,SIGNAL(clicked()),mainWindow->readerView->addItem,SIGNAL(clicked()));
    connect(readerManageWidget->readerTypeManageButton,SIGNAL(clicked()),parent,SLOT(createAuthorityViewWidget()));
    connect(readerManageWidget->addReaderTypeButton,SIGNAL(clicked()),mainWindow->authorityView->addItem,SIGNAL(clicked()));
    connect(readerManageWidget->addFeeAcountButton,SIGNAL(clicked()),mainWindow,SLOT(onAddFeeAcountTriggered()));
    connect(bookBORManageWidget->bookBorrowOrReserveManageButton,SIGNAL(clicked()),parent,SLOT(createBookBorrowOrReserveViewWidget()));
    connect(bookBORManageWidget->addBookBorrowOrReserveButton,SIGNAL(clicked()),mainWindow->bookBorrowOrReserveView->addItem,SIGNAL(clicked()));
    connect(statistic->bookRentSevenday,SIGNAL(clicked()),mainWindow,SLOT(createStatisticGraph1()));//生成统计图
    connect(statistic->bookRentTopTen,SIGNAL(clicked()),mainWindow,SLOT(createStatisticGraph2()));
    connect(statistic->bookRentPerMonth,SIGNAL(clicked()),mainWindow,SLOT(createStatisticGraph3()));
}

MainTabWidget::~MainTabWidget()
{
    delete readerManageWidget;
    delete bookManageWidget;
    delete statistic;
    delete label4;
}

bookManageButtonsWidget::bookManageButtonsWidget(QWidget *parent):QWidget(parent)
{
    bookManageButton = new QPushButton(this);
    bookManageButton->setFocusPolicy(Qt::NoFocus);
    bookManageButton->setText("图书管理");
    addBookButton = new QPushButton(this);
    addBookButton->setFocusPolicy(Qt::NoFocus);
    addBookButton->setText("新建图书");
    varietyManageButton = new QPushButton(this);
    varietyManageButton->setFocusPolicy(Qt::NoFocus);
    varietyManageButton->setText("图书分类管理");
    vLayout = new QVBoxLayout;
    bookManageButton->setFixedHeight(100);
    vLayout->addWidget(bookManageButton);
    addBookButton->setFixedHeight(100);
    vLayout->addWidget(addBookButton);
    varietyManageButton->setFixedHeight(100);
    vLayout->addWidget(varietyManageButton);
    setLayout(vLayout);
}

bookManageButtonsWidget::~bookManageButtonsWidget()
{
    delete bookManageButton;
    delete addBookButton;
    delete varietyManageButton;
    delete vLayout;
}

readerManageButtonsWidget::readerManageButtonsWidget(QWidget *parent):QWidget(parent)
{
    readerManageButton = new QPushButton(this);
    readerManageButton->setFocusPolicy(Qt::NoFocus);
    readerManageButton->setFixedHeight(50);
    readerManageButton->setText("读者管理");
    addReaderButton = new QPushButton(this);
    addReaderButton->setFocusPolicy(Qt::NoFocus);
    addReaderButton->setFixedHeight(50);
    addReaderButton->setText("添加读者");
    readerTypeManageButton = new QPushButton(this);
    readerTypeManageButton->setFocusPolicy(Qt::NoFocus);
    readerTypeManageButton->setFixedHeight(50);
    readerTypeManageButton->setText("读者权限管理");
    addReaderTypeButton = new QPushButton(this);
    addReaderTypeButton->setFocusPolicy(Qt::NoFocus);
    addReaderTypeButton->setFixedHeight(50);
    addReaderTypeButton->setText("添加读者权限");
    addFeeAcountButton = new QPushButton(this);
    addFeeAcountButton->setFocusPolicy(Qt::NoFocus);
    addFeeAcountButton->setFixedHeight(50);
    addFeeAcountButton->setText("读者余额充值");

    vLayout = new QVBoxLayout;
    vLayout->addWidget(readerManageButton);
    vLayout->addWidget(addReaderButton);
    vLayout->addWidget(readerTypeManageButton);
    vLayout->addWidget(addReaderTypeButton);
    vLayout->addWidget(addFeeAcountButton);

    setLayout(vLayout);
}

readerManageButtonsWidget::~readerManageButtonsWidget()
{
    delete readerManageButton;
    delete addReaderButton;
    delete readerTypeManageButton;
    delete addReaderTypeButton;
    delete addFeeAcountButton;
    delete vLayout;
}
statisticWidget::statisticWidget(QWidget *parent):QWidget(parent)
{
    bookRentSevenday=new QPushButton(this);
    bookRentSevenday->setFocusPolicy(Qt::NoFocus);
    bookRentSevenday->setText("七天借阅率");
    bookRentTopTen=new QPushButton(this);
    bookRentTopTen->setFocusPolicy(Qt::NoFocus);
    bookRentTopTen->setText("被借阅书籍前十");
    bookRentPerMonth=new QPushButton(this);
    bookRentPerMonth->setFocusPolicy(Qt::NoFocus);
    bookRentPerMonth->setText("读者借书前十");

    vLayout= new  QVBoxLayout;
    bookRentSevenday->setFixedHeight((100));
    vLayout->addWidget(bookRentSevenday);
    bookRentTopTen->setFixedHeight(100);
    vLayout->addWidget(bookRentTopTen);
    bookRentPerMonth->setFixedHeight((100));
    vLayout->addWidget(bookRentPerMonth);
    setLayout(vLayout);
}
//析构函数
statisticWidget::~statisticWidget()
{
    delete bookRentSevenday;
    delete bookRentTopTen;
    delete bookRentPerMonth;
    delete vLayout;
}

bookBorrowOrReserveManageButtonsWidget::bookBorrowOrReserveManageButtonsWidget(QWidget *parent):QWidget(parent)
{
    bookBorrowOrReserveManageButton = new QPushButton(this);
    bookBorrowOrReserveManageButton->setFocusPolicy(Qt::NoFocus);
    bookBorrowOrReserveManageButton->setText("图书借阅/预约管理");
    addBookBorrowOrReserveButton = new QPushButton(this);
    addBookBorrowOrReserveButton->setFocusPolicy(Qt::NoFocus);
    addBookBorrowOrReserveButton->setText("添加图书借阅/预约");

    vLayout = new QVBoxLayout;
    bookBorrowOrReserveManageButton->setFixedHeight(150);
    vLayout->addWidget(bookBorrowOrReserveManageButton);
    addBookBorrowOrReserveButton->setFixedHeight(150);
    vLayout->addWidget(addBookBorrowOrReserveButton);

    setLayout(vLayout);
}

bookBorrowOrReserveManageButtonsWidget::~bookBorrowOrReserveManageButtonsWidget()
{
    delete bookBorrowOrReserveManageButton;
    delete addBookBorrowOrReserveButton;
    delete vLayout;
}
