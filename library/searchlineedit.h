/******************************************************************
文件名: searchlineedit.h
创建人: 顾溢杰
日　期: 2017/4/7
修改人: 杨正浩，姜兴国
日　期: 2017/5/25
描　述: 信息检索
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef SEARCHLINEEDIT
#define SEARCHLINEEDIT
#include <QLineEdit>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QHBoxLayout>

class SearchLineEdit: public QLineEdit//给定下标进行搜索
{
    Q_OBJECT
public:
    SearchLineEdit(QWidget *parent=0);
    ~SearchLineEdit();
    QPushButton *searchButton;
    QLabel *searchButtonLabel;
    QHBoxLayout *layout;
};

#endif // SEARCHLINEEDIT

