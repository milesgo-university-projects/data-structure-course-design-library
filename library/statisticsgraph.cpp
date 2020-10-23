#include "statisticsgraph.h"
#include "functions.h"
#include<QFont>

//析构函数
StatisticsGraph::~StatisticsGraph()
{
    delete myPlot;
}
//统计图构造函数
StatisticsGraph::StatisticsGraph(QWidget *parent)
    : QWidget(parent)
{

     myPlot = new QCustomPlot(this);
     myPlot-> setFixedSize(600,400);
     widgetName="";
     Xlabel="";
     Ylabel="";

}

//设置窗口名，x坐标y坐标
void StatisticsGraph::set(QString name,QString xlabel,QString ylabel)
{
    widgetName=name;
    Xlabel=xlabel;
    Ylabel=ylabel;
}

//柱状图
 void StatisticsGraph::barChart(const QVector<QString>&label,const QVector<double> &data)
 {

     double max=-1,min=10000000000;
     for(int i=0;i<data.size();i++)
     {
         min=(data[i]<min)?data[i]:min;
         max=(data[i]>max)?data[i]:max;
     }
     QLinearGradient gradient(0, 0, 0, 400);                                               //设置背景颜色
     gradient.setColorAt(0, QColor(90, 90, 90));
     gradient.setColorAt(0.38, QColor(105, 105, 105));
     gradient.setColorAt(1, QColor(70, 70, 70));
     myPlot->setBackground(QBrush(gradient));                                              //设置填充颜色

     QCPBars *bars=new QCPBars(myPlot->xAxis,myPlot->yAxis);
     bars->setAntialiased(false);
     bars->setStackingGap(1);
     bars->setName(widgetName);
     bars->setPen(QPen(QColor(0, 168, 140).lighter(130)));
     bars->setBrush(QColor(0, 168, 140));


     int size=label.size();
     QVector<double> index;                                                                 //画数据
     for(int i=0;i<size;++i)
         index.append(i+1);
     QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
     textTicker->addTicks(index,label);

     myPlot->xAxis->setTicker(textTicker);                                                  //设置x轴坐标，标签坐标等信息
     myPlot->xAxis->setTickLabelRotation(60);
     myPlot->xAxis->setSubTicks(false);
     myPlot->xAxis->setTickLength(0,4);
     myPlot->xAxis->setRange(0,13);
     myPlot->xAxis->setBasePen(QPen(Qt::white));
     myPlot->xAxis->setTickPen(QPen(Qt::white));
     myPlot->xAxis->grid()->setVisible(true);
     myPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
     myPlot->xAxis->setTickLabelColor(Qt::white);
     myPlot->xAxis->setLabelColor(Qt::white);

     if(max<5)
         myPlot->yAxis->setRange(0,5);
     else if(max<12)
         myPlot->yAxis->setRange(0, 12);
     else
     {
         min=(min>3)?min-2:0;
         myPlot->yAxis->setRange(min,max+2);
     }
     myPlot->yAxis->setPadding(5);                                                          //设置y轴坐标，标签坐标等信息
     myPlot->yAxis->setBasePen(QPen(Qt::white));
     myPlot->yAxis->setTickPen(QPen(Qt::white));
     myPlot->yAxis->setSubTickPen(QPen(Qt::white));
     myPlot->yAxis->grid()->setSubGridVisible(true);
     myPlot->yAxis->setTickLabelColor(Qt::white);
     myPlot->yAxis->setLabelColor(Qt::white);
     myPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));          //设置颜色
     myPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
     myPlot->legend->setVisible(true);
     myPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignHCenter);
     myPlot->legend->setBrush(QColor(255, 255, 255, 100));
     myPlot->legend->setBorderPen(Qt::NoPen);
     myPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
     bars->setData(index,data);                                                             //设置数据
     myPlot->yAxis->setLabel(Ylabel);
     myPlot->xAxis->setLabel(Xlabel);
     this->setWindowTitle(widgetName);
     this->show();
 }

 //折线图
void StatisticsGraph:: lineChart(const QVector<QString>&label,const QVector<double> &data)
{
    QVector<double> X;
    for(int i=1;i<=data.size();i++)
        X.append(i);
    QCPGraph *graph1 = myPlot->addGraph();                                                 //添加一张图
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph1->setPen(QPen(QColor(120, 120, 120), 2));                                        //设置颜色

    myPlot->xAxis->setBasePen(QPen(Qt::white, 1));                                         //设置x轴，y轴颜色，标签，数据等
    myPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    myPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    myPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    myPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    myPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    myPlot->xAxis->setTickLabelColor(Qt::white);
    myPlot->yAxis->setTickLabelColor(Qt::white);
    myPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));            //设置界面的样式
    myPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    myPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    myPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    myPlot->xAxis->grid()->setSubGridVisible(true);
    myPlot->yAxis->grid()->setSubGridVisible(true);
    myPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    myPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    myPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    myPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    myPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    myPlot->axisRect()->setBackground(axisRectGradient);
    myPlot->rescaleAxes();

    QVector<double> index;                                                                //画数据
    for(int i=0;i<label.size();++i)
        index.append(i*0.6+0.2);
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(index,label);
    graph1->setData(index,data);
    myPlot->xAxis->setTicker(textTicker);
    myPlot->xAxis->setTickLabelRotation(60);
    myPlot->xAxis->setSubTicks(false);
    myPlot->xAxis->setTickLength(0,3);
    myPlot->xAxis->setBasePen(QPen(Qt::white));
    myPlot->xAxis->setTickPen(QPen(Qt::white));
    myPlot->xAxis->grid()->setVisible(true);
    myPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    myPlot->xAxis->setTickLabelColor(Qt::white);
    myPlot->xAxis->setLabelColor(Qt::white);
    double max=-1,min=10000000000;
    for(int i=0;i<data.size();i++)                                                        //找最小值，便于规定写坐标的范围
    {
        min=(data[i]<min)?data[i]:min;
        max=(data[i]>max)?data[i]:max;
    }
    if(max<1)
        myPlot->yAxis->setRange(-0.2,1);                                                  //设置坐标范围
    else if(max<12)
        myPlot->yAxis->setRange(0, 12);
    else
    {
        min=(min>3)?min-2:0;
        myPlot->yAxis->setRange(min,max+2);
    }
    myPlot->yAxis->setLabelColor(Qt::white);
    myPlot->yAxis->setLabel(Ylabel);
    myPlot->xAxis->setLabel(Xlabel);
    this->setWindowTitle(widgetName);
    this->show();
}

void StatisticsGraph::closeEvent(QCloseEvent *e)
{
    windowHash[21]=false;
    QWidget::closeEvent(e);
}

void StatisticsGraph::showEvent(QShowEvent *e)
{
    windowHash[21]=true;
    windowPointer[21]=qobject_cast<QWidget *>(this);
    QWidget::showEvent(e);
}
