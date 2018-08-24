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


void qdebugStock();
bool getStockDate(QVector<QString> str);
int updateStockVector(structStock *struct_stock);
void processStockDate(QString str);
QVector <QString> stockString();
MainWindow *w;

using namespace QtConcurrent;

void loopStock(QVector<QString> str){
    while(1){
        getStockDate(str);
    }
}
bool compareStock(const structStock &stock_data1, const structStock &stock_data2)
{
    if(stock_data1.stock_uplift_percent.toFloat() < stock_data2.stock_uplift_percent.toFloat()){
        return true;
    }else {
        return false;
    }
}

void qdebugStock(){
    w->view->setUpdatesEnabled(false);
    for(int i=0; i<stock_vector_global->size(); i++){
        qDebug()<<stock_vector_global->at(i).stock_num<<stock_vector_global->at(i).stock_uplift_percent<<QString::number(i*1024%9);
        model.setItem(i,0,new QStandardItem(stock_vector_global->at(i).stock_num));
        model.setItem(i,1,new QStandardItem(stock_vector_global->at(i).stock_uplift_percent));
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int a =qrand()%9;
        model.setItem(i,2,new QStandardItem(QString::number(a)));
    }
    w->view->setUpdatesEnabled(true);
}
int updateStockVector(structStock *struct_stock){
    int size = stock_vector_global->size();
    if(size == 0){
        stock_vector_global->append(*struct_stock);
        return stock_vector_global->size();
    }else{
        for(int i=0; i<size; i++){
            if(stock_vector_global->at(i).stock_num == struct_stock->stock_num){
                stock_vector_global->replace(i,*struct_stock);
                return i;
            }
        }
        stock_vector_global->append(*struct_stock);
    }
    return stock_vector_global->size();
}

void processStockDate(QString str){
    if(str.contains(";",Qt::CaseSensitive)){
        QStringList sList = str.split(";");//先分组
        for(int i=0; i<sList.size(); i++){
            if(sList.at(i).contains("~",Qt::CaseSensitive)){
                QStringList spList= sList.at(i).split("~");//再分别填充
                if(spList.size()>48){
                    structStock *struct_stock = new structStock(spList.at(2));
                    struct_stock->stock_name = spList.at(1);
                    struct_stock->stock_price = spList.at(3);
                    struct_stock->stock_sell_volume = spList.at(20);
                    struct_stock->stock_time = spList.at(30);
                    struct_stock->stock_limit_up_price = spList.at(47);
                    struct_stock->stock_uplift_percent = spList.at(32);
                    updateStockVector(struct_stock);
                    delete struct_stock;
                }
            }
        }
    }
}
bool getStockDate(QVector<QString> str){
    QString str_tmp;
    for(int i=0; i<str.size(); i++){
        str_tmp.append(str.at(i));
        str_tmp.append(",");
    }
    QNetworkReply *replys;
    QNetworkAccessManager netWork;
    QEventLoop eventLoop;
    QObject::connect(&netWork, SIGNAL(finished(QNetworkReply*)),&eventLoop,SLOT(quit()));
    replys =netWork.get(QNetworkRequest(QUrl("http://qt.gtimg.cn/q="+str_tmp )));
    eventLoop.exec();
    QString all = QString::fromLocal8Bit(replys->readAll());
    processStockDate(all);
    delete replys;
    qSort(stock_vector_global->begin(),stock_vector_global->end(),compareStock);
    qdebugStock();
}

QVector <QString> stockString(){
    //读取Excel表格
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false); //隐藏打开的excel文件界面
    QAxObject *workbooks = excel.querySubObject("WorkBooks");
    QAxObject *workbook = workbooks->querySubObject("Open(QString, QVariant)", "d:\\stock2.xlsx"); //打开文件
    QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1); //访问第一个工作表
    QAxObject * usedrange = worksheet->querySubObject("UsedRange");
    QAxObject * rows = usedrange->querySubObject("Rows");
    int intRows = rows->property("Count").toInt(); //行数

    QString Range = "A1:B" +QString::number(intRows);
    QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range); //读取范围
    QVariant allEnvDataQVariant = allEnvData->property("Value");
    QVariantList allEnvDataList = allEnvDataQVariant.toList();
    QVector <QString> str;
    for(int i=0; i< intRows; i++)
    {
        QVariantList allEnvDataList_i =  allEnvDataList[i].toList() ;
        QString data1 = allEnvDataList_i[0].toString(); //第i行第0列的数据
        str.append("sh"+data1);
    }
    workbooks->dynamicCall("Close()");
    excel.dynamicCall("Quit()");
    return str;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //    MainWindow w;
    w = new MainWindow;
    w->show();

    QVector<QString> str = stockString();
    run(loopStock, str);//单次请求股票个数不要超过900个

    return a.exec();
}
