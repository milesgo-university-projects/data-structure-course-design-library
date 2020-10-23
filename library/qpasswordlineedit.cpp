#include "qpasswordlineedit.h"
#include <QDebug>

//设置模式为密码模式
QPasswordLineEdit::QPasswordLineEdit(QWidget *parent):QLineEdit(parent)
{
    this->setEchoMode(QLineEdit::Password);
}

//
void QPasswordLineEdit::keyPressEvent(QKeyEvent *e)
{
    if(e->matches(QKeySequence::SelectAll)||        //全选， 复制，粘贴
        e->matches(QKeySequence::Copy)||
        e->matches(QKeySequence::Paste))
    {
        return;
    }
    QLineEdit::keyPressEvent(e);
}

//鼠标移动
void QPasswordLineEdit::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);                                    //未使用
}

//双击
void QPasswordLineEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e);                                    //未使用
}
