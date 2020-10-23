/******************************************************************
文件名: login.h
创建人: 顾溢杰
日　期: 2017/3/25
修改人: 顾溢杰
日　期: 2017/4/25
描　述: 登录功能
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef LOGIN
#define LOGIN

#include "qpasswordlineedit.h"
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QCheckBox>
#include <QFile>
#include <QTextStream>
#include <QObject>
#include <QWidget>

//实现登录功能
class loginDialog:public QDialog
{
    Q_OBJECT
public:
    loginDialog(QWidget *parent=0);
    ~loginDialog();
    bool checkLoginInfo(QString user,QString password);
    QPushButton *loginButton;                              //按钮
    QPushButton *closeButton;
    QPasswordLineEdit *passwordLineEdit;
    QLineEdit *userLineEdit;
    QLabel *messageLabel;                                  //标签，空间旁边的文字
    QLabel *passwordLabel;
    QLabel *userLabel;
    QCheckBox *checkBox;
    QHBoxLayout *passwordLayout;                           //布局
    QHBoxLayout *userLayout;
    QHBoxLayout *buttonLayout;
    QVBoxLayout *vLayout;
public slots:
    void checkPushed();
};

#endif // LOGIN

