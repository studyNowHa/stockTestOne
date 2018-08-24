#include "globalstock.h"


QVector<structStock> *stock_vector_global = new QVector<structStock>;//全局变量 所有股票池
QVector<structStock> *stock_vector_limt_up_global = new QVector<structStock>;//全局变量 所有涨停股票池

QStandardItemModel model;
