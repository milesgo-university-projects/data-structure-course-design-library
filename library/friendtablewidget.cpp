#include "friendtablewidget.h"

friendTableWidget::friendTableWidget(QWidget *parent):QTableWidget(parent)
{

}

friendTableWidget::~friendTableWidget()
{

}

void friendTableWidget::keyPressEvent(QKeyEvent *e)
{
    addBookBox *parent=qobject_cast<addBookBox*>(this->parent());
    if(e->key()==Qt::Key_Delete)
        parent->deleteData();
    QTableWidget::keyPressEvent(e);
}

friendListWidget::friendListWidget(QWidget *parent):QListWidget(parent)
{

}

friendListWidget::~friendListWidget()
{

}

friendTableView::friendTableView(QWidget *parent):QTableView(parent)
{

}

friendTableView::~friendTableView()
{

}
