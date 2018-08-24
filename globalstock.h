#ifndef GLOBALSTOCK_H
#define GLOBALSTOCK_H
#include <QVector>
#include "structstock.h"
#include <QStandardItemModel>

extern QVector<structStock> *stock_vector_global;//全局变量 所有股票池
extern QVector<structStock> *stock_vector_limt_up_global;//全局变量 涨停股票池
extern QVector<structStock> *stock_vector_watch_global;//全局变量 涨停股票池
extern QStandardItemModel model;

#define stock_percent 9.80
#endif // GLOBALSTOCK_H
