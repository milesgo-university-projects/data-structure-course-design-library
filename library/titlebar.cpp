#include "titlebar.h"
#include <QDebug>

//构造函数
titleBar::titleBar(QWidget *parent):QWidget(parent)
{
    setFixedHeight(30);
    isPressed=false;

    closeButton = new QPushButton;                            //实例化
    minSizeButton = new QPushButton;
    closeButtonLabel = new QLabel(closeButton);
    minSizeButtonLabel = new QLabel(minSizeButton);

    iconLabel = new QLabel;
    titleLabel = new QLabel;
    titleLayout = new QHBoxLayout;

    iconLabel->setFixedSize(20,20);
    iconLabel->setScaledContents(true);                        //自适应大小

    closeButton->setFixedSize(26,26);
    minSizeButton->setFixedSize(26,26);
    closeButtonLabel->setFixedSize(26,26);
    minSizeButtonLabel->setFixedSize(26,26);

    closeButtonLabel->setAttribute(Qt::WA_TranslucentBackground);
    minSizeButtonLabel->setAttribute(Qt::WA_TranslucentBackground);
    closeButtonLabel->setPixmap(QPixmap("../option/titlebarImages/close.png"));
    minSizeButtonLabel->setPixmap(QPixmap("../option/titlebarImages/minimize.png"));
    titleLabel->setText(tr("默认"));

    closeButton->setObjectName("closeButton");
    minSizeButton->setObjectName("minSizeButton");
    titleLabel->setObjectName("titleLabel");

    closeButton->setToolTip(tr("关闭"));
    minSizeButton->setToolTip(tr("最小化"));

    closeButton->setFocusPolicy(Qt::NoFocus);
    minSizeButton->setFocusPolicy(Qt::NoFocus);

    titleLayout->addWidget(iconLabel);                        //布局
    titleLayout->addSpacing(5);
    titleLayout->addWidget(titleLabel);
    titleLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    titleLayout->addWidget(minSizeButton);
    titleLayout->addWidget(closeButton);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(5,0,5,0);
    setLayout(titleLayout);

    connect(closeButton,SIGNAL(clicked()),this,SLOT(onClicked()));
    connect(minSizeButton,SIGNAL(clicked()),this,SLOT(onClicked()));
}

//析构函数
titleBar::~titleBar()
{
    delete closeButton;
    delete minSizeButton;
    delete iconLabel;
    delete titleLabel;
    delete titleLayout;
}

//鼠标移动处理
void titleBar::mouseMoveEvent(QMouseEvent *e)
{
    if(isPressed)
    {
        QWidget *controledWindow = this->window();
        controledWindow->move(controledWindow->pos()+e->pos()-point);
    }
}
//鼠标点击处理
void titleBar::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
    {
        isPressed=true;
        point=e->pos();
    }
}
//鼠标释放事件
void titleBar::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    isPressed=false;
}

//判断是否点击
void titleBar::onClicked()
{
    QPushButton *pushedButton = qobject_cast<QPushButton *>(sender());
    QWidget *controledWindow = this->window();

    if(controledWindow->isTopLevel())
    {
        if(pushedButton==closeButton)
        {
            controledWindow->close();
        }
        else if(pushedButton==minSizeButton)
        {
            controledWindow->showMinimized();
        }
    }
}

//依据不同的事件对应处理
bool titleBar::eventFilter(QObject *obj, QEvent *e)
{
    QWidget *pwidget=qobject_cast<QWidget *>(obj);
    switch(e->type())
    {
        case QEvent::WindowTitleChange:
        {
            if(pwidget)
            {
                titleLabel->setText(pwidget->windowTitle());
            }
            return true;
        }
        case QEvent::WindowIconChange:
        {
            if(pwidget)
            {
                QIcon icon=pwidget->windowIcon();
                iconLabel->setPixmap(icon.pixmap(iconLabel->size()));
            }
            return true;
        }
        default:
            break;
    }
    return QWidget::eventFilter(obj,e);
}
