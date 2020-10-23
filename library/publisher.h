/******************************************************************
文件名: publisher.h
创建人: 顾溢杰
日　期: 2017/4/2
修改人: 杨正浩
日　期: 2017/5/25
描　述: 出版社对应的数据结构
版　本: v5.00 Copyright(c).
******************************************************************/

#ifndef PUBLISHER
#define PUBLISHER
#include <QString>

// PUBLISHER
struct Publisher{
    QString name;                   //出版社名字
    QString location;               //出版社所在地（城市）
    QString ISBN;                   //出版社对应的ISBN编号
};
#endif
