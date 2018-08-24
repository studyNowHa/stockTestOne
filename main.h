#include "stockwatchthread.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QString>
#include <qtconcurrentrun.h>
#include <QApplication>
#include <QAxObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "globalstock.h"
#include "structstock.h"
#include <QWidget>
#include<QTime>
#include<QtGlobal>
#include "threadstock.h"
#include <QTimer>

void qdebugStock(QVector<structStock> vector_struct, MainWindow *mw);
QString getStockDate(QVector<QString> vector_str_get_stock_data);
int updateStockVector(structStock struct_stock);
QVector<structStock> processStockDate(QString str);
bool getStockDateAll(QVector<QString> vector_str_get_stock_data_all, MainWindow *mw);
QVector <QString> stockString(QString str_stock_dir);
QString stockHistory(QString stock_str_history);
structStock stockProcesDate(QString str);
void loopStock(QVector<QString> str_dir,MainWindow *mw);
void stockWatchThreadIn(QVector<QString> str_dir,MainWindow *mw);
QString stockHistoryProcesDate(QString str);
void stockTableViewUpdate(QVector<structStock> vector_struct,MainWindow *mw);
QString stockPercent(structStock vector_struct);
