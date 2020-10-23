/******************************************************************
文件名: statisticsgraph.h
创建人: 杨正浩
日　期: 2017/5/10
修改人: 杨正浩
日　期: 2017/5/23
描　述: 画出统计图
版　本: v5.00 Copyright(c).
******************************************************************/
#ifndef STATISTICSGRAPH_H
#define STATISTICSGRAPH_H
#include<qcustomplot.h>
#include<QWidget>
#include<QVector>
#include<QString>
#include<QCloseEvent>
#include<QShowEvent>

//用于画出统计图的类
class StatisticsGraph:public QWidget
{
public:
    StatisticsGraph(QWidget *parent = 0);
    ~StatisticsGraph();
    void set(QString name,QString xlabel,QString ylabel);                              //设置窗口名，横坐标，纵坐标
    void barChart(const QVector<QString>&label,const QVector<double> &data);           //条形统计图
    void lineChart(const QVector<QString>&label,const QVector<double> &data);          //折线统计图
private:
    QCustomPlot *myPlot;
    QString Xlabel;                                                                    //x轴
    QString Ylabel;                                                                    //y轴
    QString widgetName;                                                                //窗口名
protected:
    void closeEvent(QCloseEvent *e);
    void showEvent(QShowEvent *e);
};

#endif // STATISTICSGRAPH_H
