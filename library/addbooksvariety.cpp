/******************************************************************
文件名: addbooksvariety.cpp
创建人: 姜兴国
日　期: 2017/4/10
修改人: 姜兴国、顾溢杰
日　期: 2017/5/25
描　述: 添加图书分类以及进行相应的管理
版　本: v5.00 Copyright(c).
******************************************************************/
#include "addbooksvariety.h"
#include "string.h"
#include "widget.h"
#include "logs.h"
class Widget;
using namespace std;
addBooksVariety::addBooksVariety(QWidget *parent) :QWidget(parent)//构建当前界面
{
    this->setWindowTitle("图书分类管理");
    isEditingBook=false;
    this->setFixedSize(900,600);
    tree=new QTreeWidget();
    tree->setColumnWidth(0,200);
    mainform = new QFormLayout();

    mainform->setSizeConstraint(QLayout::SetFixedSize);
    mainform->setVerticalSpacing(40);//组组间隔
    mainform->setHorizontalSpacing(10);//行行间隔
    mainform->setRowWrapPolicy(QFormLayout::DontWrapRows);//每行换行方式
    mainform->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);//field拓展方式
    mainform->setLabelAlignment(Qt::AlignRight|Qt::AlignCenter);//label对齐方式
    mainform->setFormAlignment(Qt::AlignCenter);//field对齐方式
    //逐行创建
    addclass = new QLabel("添加类别");
    addclass->setAlignment(Qt::AlignLeft);//左对齐

    addnumber = new QLineEdit();
    addnumber->setDragEnabled(1);//鼠标抓起是否被允许
    addnumber->setAlignment(Qt::AlignLeft);
    addnumber->setFrame(0);//是否有边框

    addcontent = new QLineEdit();
    addcontent->setDragEnabled(1);//鼠标抓起是否被允许
    addcontent->setAlignment(Qt::AlignLeft);
    addcontent->setFrame(0);//是否有边框

    buttonyes1 = new QPushButton("确定");//添加部分布局
    buttonyes1->setFocusPolicy(Qt::NoFocus);
    buttonlayout2 = new QVBoxLayout();
    buttonlayout2->setSizeConstraint(QLayout::SetFixedSize);
    buttonlayout2->addWidget(buttonyes1);
    buttonlayout2->setAlignment(Qt::AlignRight);

    select = new QPushButton("选择");//获取当前选中行的按钮
    select->setFocusPolicy(Qt::NoFocus);
    select->setFixedSize(150,30);

    buttoncancel = new QPushButton("删除选中分类");//删除部分布局
    buttoncancel->setFocusPolicy(Qt::NoFocus);
    buttoncancel->setFixedSize(150,30);

    buttonlayout1 = new QHBoxLayout();//删除部分布局
    buttonlayout1->setSizeConstraint(QLayout::SetFixedSize);
    buttonlayout1->addWidget(select);
    buttonlayout1->addSpacing(30);
    buttonlayout1->addWidget(buttoncancel);
    buttonlayout1->setAlignment(Qt::AlignHCenter);

    mainform->addRow(addclass);//右侧文本框和按钮布局
    mainform->addRow("&编号：",addnumber);
    mainform->addRow("&类名：",addcontent);
    mainform->addRow(buttonlayout2);
    mainform->addRow(buttonlayout1);

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());//打开文件
    QFile file("../dat/library classify.txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug()<<"Can't open the file!"<<endl;
        }

        QTextStream in(&file);//定义文件指针
        in.setCodec("GBK");
        in.seek(0);
    InitTree(tree,in);//根据

        mainlay = new QHBoxLayout;//上方的列表显示
        mainlay->addWidget(tree,2);
        mainlay->addLayout(mainform,1);

        statusBar = new mainStatusBar(this);
        trueMainLayout = new QVBoxLayout;
        trueMainLayout->addLayout(mainlay);
        trueMainLayout->addWidget(statusBar);

    //定义第一个信号和信号槽
    in.seek(0);
    file.close();
    connect(buttonyes1,SIGNAL(clicked()),this,SLOT(getAddFunc()));
    connect(buttoncancel,SIGNAL(clicked()),this,SLOT(onDeleteButtonClicked()));
    connect(select,SIGNAL(clicked()),this,SLOT(onselectButtonClicked()));
    statusBar->connectLater();
    setLayout(trueMainLayout);
}

addBooksVariety::~addBooksVariety()//按顺序释放掉相关指针
{
    delete tree;
    delete addclass;
    delete addnumber;
    delete addcontent;
    delete buttonyes1;
    delete select;
    delete buttoncancel;
    delete buttonlayout2;
    delete buttonlayout1;
    delete mainform;
    delete mainlay;
    delete statusBar;
    delete trueMainLayout;
}
//递归构建字母大类的子树
QString addBooksVariety::findChildTree(QString parentNum,QTextStream &in,QTreeWidgetItem* &A,QString &codeNum,QString codeLetter){
    Q_UNUSED(parentNum);
    QString head,content,next,current=codeNum,currentLetter=codeLetter;
    QStringList columItemList;
    QTreeWidgetItem *child;

    for(;!in.atEnd();){
        if(current.length()<codeNum.length())  return current;
        columItemList.clear();
        content=in.readLine();//构建子树
        columItemList<<current<<content;
        child = new QTreeWidgetItem(columItemList);
        A->addChild(child);

        head=in.read(1);
        if(head=="#"){
            head=in.read(1);
            currentLetter=head;
            current="";
            if(currentLetter!=codeLetter)   break;//如果到下一字母大类了，跳出

            for(;current.length()<codeNum.length()&&head!=" ";){
                current=current+head;
                head=in.read(1);
            }
            if(head==" "&&current.length()<codeNum.length())  return current;
            if(head!=" "){
                current=current+head;
                in.read(1);
                current=findChildTree(head,in,child,current,currentLetter);
                }
        }
}
    return "";
}

void addBooksVariety::InitTree(QTreeWidget *&treeWidget,QTextStream &in){//递归构建字母大类的孩子

    QString mainKeyWord;
    int count=0;
    treeWidget->setWindowTitle("QTreeWidget");
    //设定头项名称
    treeWidget->setHeaderLabels(QStringList()<<"图书类别信息检索"<<"类别名称");
    //设定各个项

    QStringList();

    char line[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    QString content;
    char *lineptr=line;
    QTreeWidgetItem* A;

    for(;count<26&&!in.atEnd();count++,lineptr++){
        for(;!in.atEnd();){
            mainKeyWord=in.read(1);
            QByteArray abc=mainKeyWord.toLatin1();
            char *MainKeyWordptr=abc.data();
            in.read(1);
            content=in.readLine();
            if(*MainKeyWordptr==*lineptr){
                A = new QTreeWidgetItem(QStringList()<<mainKeyWord<<content);
                A->setIcon(0, QIcon(":images/images/data.png"));
                treeWidget->addTopLevelItem(A);

                QString codeNum;
                in.seek(0);//递归创建子树
                QString key,nextKey,current;

                for(;!in.atEnd();){
                    key = in.read(1);
                    if(key=="#"){
                        key = in.read(1);
                        if(key==mainKeyWord){
                            nextKey=in.read(1);
                            current=key+nextKey;
                            in.read(1);
                            findChildTree(nextKey,in,A,current,mainKeyWord);
                        }
                    }
                    else
                        key=in.readLine();
                }
            }

        }
        in.seek(0);

}

}

void addBooksVariety::getAddFunc(){//获取信息 添加图书分类
    addClassNum = addnumber->text();
    addClassContent = addcontent->text();
    QTreeWidgetItem *item;
    QString str;
    for(int i=0;i<tree->topLevelItemCount();i++)
    {
        item=tree->topLevelItem(i);
        str=item->data(0,Qt::DisplayRole).toString();
        if(str==addClassNum)
        {//警告
            QMessageBox message(QMessageBox::Warning,"警告","添加的分类编号已存在",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
            break;
        }
        else if(str[0]==addClassNum[0])
        {
            if(updateTree(item,addClassNum,addClassContent))//对树形结构添加孩子并刷新显示
            {
                addClassFunc(addClassNum,addClassContent);
                Logs::AddBooksVariety(mainWindow->currentAccountName,addClassContent,addClassNum);
                tree->topLevelItem(i)->setExpanded(true);
            }
            break;
        }
    }
}

void addBooksVariety::addClassFunc(QString &addNum, QString &addContent){//对于文件添加图书分类
    if(addNum!=""&&addContent!=""){

        QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
        QFile file("../dat/library classify.txt");
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                qDebug()<<"Can't open the file!"<<endl;
            }
        QTextStream in(&file);
        in.setCodec("GBK");
        in.seek(0);
        QFile file1("../dat/temptxt1.txt");
        if(!file1.open(QIODevice::ReadWrite | QIODevice::Truncate )) {
                qDebug()<<"Can't open the file!"<<endl;
            }

            QTextStream temp1(&file1);
            temp1.setCodec("GBK");
            temp1.seek(0);

        QString current,ppp="、";
        current=in.read(1);
        temp1<<current;//保留不动的文件写入temp1
        int i=1;
        for(;current!="#";){//检测大类
            if(current==addNum){//检测到
                in.read(1);
                temp1<<" "<<addContent;
                for(;!in.atEnd();){
                    current=in.readLine();
                    temp1<<current<<endl;
                }
                in.seek(0);
                file.resize(0);//清空文件
                temp1.seek(0);
                for(;!temp1.atEnd();){
                    current=in.readLine();
                    temp1<<current<<endl;
                }
                file.close();
                file1.close();
                return;
            }
            current=in.readLine();
            temp1<<current<<endl;
            current=in.read(1);
            temp1<<current;
        }
        if(addNum=="A1"){//若为A1后面添加信息
            in.read(3);
            temp1<<"A1 "<<addContent;
            for(;!in.atEnd();){
                current=in.readLine();
                temp1<<current<<endl;
            }
            in.seek(0);
            file.resize(0);//清空文件
            temp1.seek(0);
            for(;!temp1.atEnd();){
                current=in.readLine();
                temp1<<current<<endl;
            }
            file.close();
            file1.close();
            return;
        }
        else{//不是添加A1的话
            current=in.readLine();
            temp1<<current<<endl;
            current=in.read(1);
        }

        QString content;//addNum中的当前字符
        QString nowCurrent="";//用于保存当行current的字符，用于统计长度作比较


        i=0;

        for(;current=="#"&&!in.atEnd()&&addNum!="A1";i=0){//添加以“#”开头的子类
            current=in.readLine();
            for(;current[i]!=' ';i++){//一行内的一位一位读入
                if(current[i]<addNum[i]){
                    temp1<<"#"<<current<<endl;
                    break;
                }
                else if(current[i]>addNum[i]){//同长变末
                    temp1<<"#"<<addNum<<" "<<addContent<<endl;
                    temp1<<"#"<<current<<endl;
                    for(;!in.atEnd();){
                        current=in.readLine();
                        temp1<<current<<endl;
                    }
                    in.seek(0);
                    file.resize(0);//清空文件
                    temp1.seek(0);
                    for(;!temp1.atEnd();){
                        current=temp1.readLine();
                        in<<current<<endl;
                    }
                    file.close();
                    file1.close();
                    return;
                }
              }


            if(current[i]==' '&&addNum.length()==i+1){//添加子类
                in.read(1);
                nowCurrent=in.readLine();
                if(nowCurrent[i]>='0'&&nowCurrent[i]<='9'){
                    temp1<<"#"<<current<<endl;
                    current=nowCurrent;
                    if(nowCurrent[i]>addNum[i]){
                        temp1<<"#"<<addNum<<" "<<addContent<<endl;
                        temp1<<"#"<<nowCurrent<<endl;
                        for(;!in.atEnd();){
                            current=in.readLine();
                            temp1<<current<<endl;
                        }
                        in.seek(0);
                        file.resize(0);//清空文件
                        temp1.seek(0);
                        for(;!temp1.atEnd();){
                            current=temp1.readLine();
                            in<<current<<endl;
                        }
                        file.close();
                        file1.close();
                        return;
                    }
                    else{
                        temp1<<"#"<<nowCurrent<<endl;
                        current=in.read(1);
                    }
                }
                else{
                    temp1<<"#"<<current<<endl;
                    temp1<<"#"<<addNum<<" "<<addContent<<endl;
                    temp1<<"#"<<nowCurrent<<endl;
                    for(;!in.atEnd();){
                        current=in.readLine();
                        temp1<<current<<endl;
                    }
                    in.seek(0);
                    file.resize(0);//清空文件
                    temp1.seek(0);
                    for(;!temp1.atEnd();){
                        current=temp1.readLine();
                        in<<current<<endl;
                    }
                    file.close();
                    file1.close();
                    return;
                }

            }
            else if(current[i]==' '&&addNum.length()==i){//同长变末
                temp1<<"#"<<current<<ppp<<addContent<<endl;
                for(;!in.atEnd();){
                    current=in.readLine();
                    temp1<<current<<endl;
                }
                in.seek(0);
                file.resize(0);//清空文件
                temp1.seek(0);
                for(;!temp1.atEnd();){
                    current=temp1.readLine();
                    in<<current<<endl;
                }
                file.close();
                file1.close();
                return;
            }
           else if(current[i]==' '){//下一类
                    temp1<<"#"<<current<<endl;
                    current=in.read(1);
                }
           else
                current=in.read(1);
            }
        }
}

void addBooksVariety::onDeleteButtonClicked()
{
    QList<QTreeWidgetItem *> treeItemList=tree->selectedItems();
    if(treeItemList.size())
    {
        QString str=treeItemList[0]->data(0,Qt::DisplayRole).toString();
        QString sstr=treeItemList[0]->data(1,Qt::DisplayRole).toString();
        if(str.size()>1)
        {
            Logs::DeleBooksVariety(mainWindow->currentAccountName,sstr,str);
            deleClassAndHisChildrenFunc(str);
            delete treeItemList[0];
        }
        else
        {
            QMessageBox message(QMessageBox::Warning,"警告","顶层分类无法删除",QMessageBox::Yes,NULL);
            message.setButtonText(QMessageBox::Yes,"好的");
            message.exec();
        }
    }
    else
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
    }
}

bool addBooksVariety::updateTree(QTreeWidgetItem *currentItem,QString str1, QString str2)//界面添加子树
{
    QTreeWidgetItem *item;
    QString str;
    int i;
    int size=currentItem->data(0,Qt::DisplayRole).toString().size();
    if(str1.size()==size+1)
    {//1.和当前某一分类同名，警告;2.插入到某个位置
        for(i=0;i<currentItem->childCount();i++)
        {
            str=currentItem->child(i)->data(0,Qt::DisplayRole).toString();
            if(str==str1)
            {//存在同名，警告
                QMessageBox message(QMessageBox::Warning,"警告","添加的分类编号已存在",QMessageBox::Yes,NULL);
                message.setButtonText(QMessageBox::Yes,"好的");
                message.exec();
                return false;
            }
            else if(str1[size]<str[size])
                break;
        }
        //在此处新建item
        item = new QTreeWidgetItem(QStringList()<<str1<<str2);
        currentItem->insertChild(i,item);
        return true;
    }
    else
    {//1.前size+1位和当前某一分类同名，递归这一分类;2.没有前size+1位相等的，说明未非法插入
        for(i=0;i<currentItem->childCount();i++)
        {
            str=currentItem->child(i)->data(0,Qt::DisplayRole).toString();
            if(str[size]==str1[size])
            {
                currentItem->child(i)->setExpanded(true);
                return updateTree(currentItem->child(i),str1,str2);
            }
        }
        //警告，添加非法
        QMessageBox message(QMessageBox::Warning,"警告","分类编号非法，添加失败",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
        return false;
    }
}

void addBooksVariety::deleClassAndHisChildrenFunc(QString &deleClass)
{
    if(deleClass!="")
    {
        //QTextCodec *codec = QTextCodec::codecForName("GBK");//打开一个文件
        QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
        QFile file("../dat/library classify.txt");
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                qDebug()<<"Can't open the file!"<<endl;
            }
        QTextStream in(&file);
        in.setCodec("GBK");
        in.seek(0);
        QFile file1("../dat/temptxt1.txt");
        if(!file1.open(QIODevice::ReadWrite | QIODevice::Truncate )) {
                qDebug()<<"Can't open the file!"<<endl;
            }




        QTextStream temp1(&file1);
        temp1.setCodec("GBK");
        temp1.seek(0);
        QString current,nowCurrent="";
        current=in.read(1);

        while(current!="#"){
            temp1<<current;
            current=in.readLine();
            temp1<<current<<endl;
            current=in.read(1);
        }
        QString str1,str2;
        while(!in.atEnd()){
            if(current==" "){//检测到空格
                if(nowCurrent=="#"+deleClass){//检测到当前行就是要删去的行
                    in.readLine();//将检索到的行删去
                    while(1)
                    {
                        in>>str1>>str2;//进行界面的节点的删除
                        if(str1.isEmpty())
                            break;
                        unsigned int found=str1.toStdString().find(nowCurrent.toStdString());
                        if(found!=string::npos)
                            continue;
                        else
                        {
                            temp1<<str1<<" "<<str2;
                            break;
                        }
                    }
                    for(;!in.atEnd();){
                        current=in.readLine();
                        temp1<<current<<endl;
                    }
                    in.seek(0);
                    file.resize(0);//清空文件
                    temp1.seek(0);
                    for(;!temp1.atEnd();){
                        current=temp1.readLine();
                        in<<current<<endl;
                    }
                    file.close();
                    file1.close();
                    return;
                }
                temp1<<nowCurrent<<" ";//当前行不对 直接下一行
                current=in.readLine();
                temp1<<current<<endl;//存入辅助文件中
                nowCurrent="";
                current="";

            }
            nowCurrent=nowCurrent+current;//一位一位读入
            current=in.read(1);
        }
    }
}

void addBooksVariety::closeEvent(QCloseEvent *e)
{
    windowHash[17]=false;
    isEditingBook=false;
    e->accept();
    QWidget::closeEvent(e);
}

void addBooksVariety::showEvent(QShowEvent *e)
{
    statusBar->readerInformationButton->setHidden(true);
    statusBar->readerInformButton->setHidden(true);
    if(isEditingBook==true)
        select->setEnabled(true);
    else
        select->setEnabled(false);
    windowHash[17]=true;
    windowPointer[17]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}

void addBooksVariety::onselectButtonClicked()//点击“删除当前选中行”时
{
    QTreeWidgetItem *item = tree->currentItem();
    if(item!=NULL)
    {
        emit bookClassificationInfo(item->data(0,Qt::DisplayRole).toString(),item->data(1,Qt::DisplayRole).toString());
        this->close();
    }
    else
    {
        QMessageBox message(QMessageBox::Warning,"警告","请选中一行",QMessageBox::Yes,NULL);
        message.setButtonText(QMessageBox::Yes,"好的");
        message.exec();
    }
}
