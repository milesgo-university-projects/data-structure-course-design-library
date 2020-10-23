#include "twosizebutton.h"
#include <QDebug>

//构造函数
twoSizeButton::twoSizeButton(QString smallButtonName,QString bigButtonName,QSize buttonSize,QSize smallSize,QSize bigSize,QWidget *parent):QPushButton(parent)
{
    this->setFocusPolicy(Qt::NoFocus);
    this->resize(buttonSize);
    this->smallSize=smallSize;
    this->bigSize=bigSize;

    smallLabel = new QLabel(this);                  //新建
    bigLabel = new QLabel(this);
    smallLabel->resize(smallSize);                  //更改大小
    bigLabel->resize(bigSize);
    smallLabel->setAttribute(Qt::WA_TranslucentBackground);
    smallLabel->setPixmap(QPixmap("../images/"+smallButtonName+".png")); //加载图片
    bigLabel->setAttribute(Qt::WA_TranslucentBackground);
    bigLabel->setPixmap(QPixmap("../images/"+bigButtonName+".png"));
    smallLabel->move((width()-smallSize.width())/2,(height()-smallSize.height())/2);
    bigLabel->move((width()-bigSize.width())/2,(height()-bigSize.height())/2);

    smallLabel->show();                             //显示
    bigLabel->hide();
}

//析构函数
twoSizeButton::~twoSizeButton()
{
    delete smallLabel;
    delete bigLabel;
}

//设置小尺寸按钮尺寸
void twoSizeButton::setSmallSize(QSize size)
{
    smallSize=size;
}
//设置小按钮
void twoSizeButton::setSmallSize(int w,int h)
{
    smallSize=QSize(w,h);
}
//对象不同，功能同上
void twoSizeButton::setBigSize(QSize size)
{
    bigSize=size;
}
//对象不同，功能同上
void twoSizeButton::setBigSize(int w,int h)
{
    bigSize=QSize(w,h);
}
//设置图片
void twoSizeButton::setPicture(QString smallButtonName, QString bigButtonName)
{
    smallLabel->setPixmap(QPixmap("../images/"+smallButtonName+".png"));
    bigLabel->setPixmap(QPixmap("../images/"+bigButtonName+".png"));
}

//进入，大按钮显示，小按钮隐藏
void twoSizeButton::enterEvent(QEvent *e)
{
    smallLabel->hide();
    bigLabel->show();
    QPushButton::enterEvent(e);
}
//离开，与上函数功能相反
void twoSizeButton::leaveEvent(QEvent *e)
{
    smallLabel->show();
    bigLabel->hide();
    QPushButton::leaveEvent(e);
}
