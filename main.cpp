//#include "mainwindow.h"
//#include <QApplication>
//#include <QDebug>
//#include <QThread>
//#include <QString>
//#include <qtconcurrentrun.h>
//#include <QApplication>
//#include <QAxObject>
//#include <QNetworkReply>
//#include <QNetworkAccessManager>
//#include "globalstock.h"
//#include "structstock.h"
//#include <QWidget>
//#include<QTime>
//#include<QtGlobal>
//#include "threadstock.h"
//#include <QTimer>
#include "main.h"

//void qdebugStock(QVector<structStock> vector_struct, MainWindow *mw);
//QString getStockDate(QVector<QString> vector_str_get_stock_data);
//int updateStockVector(structStock struct_stock);
//QVector<structStock> processStockDate(QString str);
//bool getStockDateAll(QVector<QString> vector_str_get_stock_data_all, MainWindow *mw);
//QVector <QString> stockString(QString str_stock_dir);
//QString stockHistory(QString stock_str_history);
//structStock stockProcesDate(QString str);


using namespace QtConcurrent;


QString time(QString str_time){
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    //    qDebug()<<current_date<<str_time<<endl;
    return current_date;
}

/*
线程入口
*/
void stockWatchThreadIn(QVector<QString> str_dir,MainWindow *mw){
    QVector<QString> vector_str_tmp;
    QString str_thread_tmp;
    structStock vecto_structstock_tmp;
    QVector<structStock> stock_vector_struct;
    while(1){
        stock_vector_struct.clear();
        for(int stock_int_watch_thread=0;stock_int_watch_thread<str_dir.size();stock_int_watch_thread++){
            vector_str_tmp.clear();
            vector_str_tmp.append(str_dir.at(stock_int_watch_thread));
            str_thread_tmp = getStockDate(vector_str_tmp);
            vecto_structstock_tmp = stockProcesDate(str_thread_tmp);
            if(vecto_structstock_tmp.stock_state == false){break;}
            str_thread_tmp = stockHistory(str_dir.at(stock_int_watch_thread));
            str_thread_tmp = stockHistoryProcesDate(str_thread_tmp);
            if(str_thread_tmp == "error"){break;}
            vecto_structstock_tmp.stock_volume_history = str_thread_tmp;
            stock_vector_struct.append(vecto_structstock_tmp);
        }
        stockTableViewUpdate(stock_vector_struct,mw);
        emit mw->updateTableSignal();
    }
}

QString stockPercent(structStock vector_struct){
    float float_tmp = vector_struct.stock_volume.toFloat()/vector_struct.stock_volume_history.toFloat();
    return (QString("%1").arg(float_tmp*100,0,10,0))+"%";
}

void stockTableViewUpdate(QVector<structStock> vector_struct,MainWindow *mw){
    for(int i=0; i<vector_struct.size(); i++){
        mw->models.setItem(i,0,new QStandardItem(vector_struct.at(i).stock_name));
        mw->models.setItem(i,1,new QStandardItem(vector_struct.at(i).stock_uplift_percent));
        mw->models.setItem(i,2,new QStandardItem(stockPercent(vector_struct.at(i))));
        mw->models.setItem(i,3,new QStandardItem(time("time")));
    }
}


QString stockHistoryProcesDate(QString str){
    if(str.contains("\\n\\",Qt::CaseSensitive)){
        QStringList spList= str.split("\\n\\");//再分别填充
        if(spList.size()-2>=0){
            QString str_tmp= spList.at(spList.size()-2);
            if(str.contains(" ",Qt::CaseSensitive)){
                spList = str_tmp.split(" ");
                if(spList.size()-1>=0)
                return spList.at(spList.size()-1);
            }
        }
    }return "error";
}

structStock stockProcesDate(QString str){
    structStock struct_stock;
    if(str.contains("~",Qt::CaseSensitive)){
        QStringList spList= str.split("~");//再分别填充
        if(spList.size()>48){
            struct_stock.stock_name = spList.at(1);
            struct_stock.stock_num = spList.at(2);
            struct_stock.stock_price = spList.at(3);
            struct_stock.stock_volume = spList.at(36);
            struct_stock.stock_sell_volume = spList.at(20);
            struct_stock.stock_time = spList.at(30);
            struct_stock.stock_uplift_percent = spList.at(32);
            struct_stock.stock_total_money = spList.at(37);
            struct_stock.stock_limit_up_price = spList.at(47);
            struct_stock.stock_buy_total_money = spList.at(9).toFloat()*spList.at(10).toFloat()/100;
            struct_stock.stock_sell_total_money = spList.at(19).toFloat()*spList.at(20).toFloat()/100;
            struct_stock.stock_state = true;
        }return struct_stock;
    }else{
        struct_stock.stock_state = false;
        return struct_stock;
    }
}
/*
历史成交数据
*/
QString stockHistory(QString stock_str_history){
    QString stock_str_time;
    QVector<QString> stock_vector_str_get_time;
    stock_vector_str_get_time.append("sh000001");
    stock_str_time=getStockDate(stock_vector_str_get_time);
    stock_str_time.contains("~",Qt::CaseSensitive);
    QStringList stock_str_list = stock_str_time.split("~");
    stock_str_time = stock_str_list.at(30).mid(2,2);                     //获取时间

    QString str_tmp;
    QNetworkReply *replys;
    QNetworkAccessManager netWork;
    QEventLoop eventLoop;
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()),&eventLoop,SLOT(quit()));
    QObject::connect(&netWork, SIGNAL(finished(QNetworkReply*)),&eventLoop,SLOT(quit()));
    timer.start(1000);
    replys =netWork.get(QNetworkRequest(QUrl("http://data.gtimg.cn/flashdata/hushen/daily/"+stock_str_time+"/"+stock_str_history+".js")));
    eventLoop.exec();
    QString str_get_stock_data_return = QString::fromLocal8Bit(replys->readAll());
    delete replys;
    return str_get_stock_data_return;
}

/*
涨停股票池
*/
void limtUpStock(){
    bool limt_up_global_state = false;
    //    stock_vector_limt_up_global->clear();
    //排完序之后可以根据涨幅辅助判断
    //    qDebug()<<stock_vector_global->size();
    for(int ab=0; ab<stock_vector_global->size(); ab++){
        //涨幅必须大于9.8%
        //        qDebug("%d",ab);
        if(stock_vector_global->at(ab).stock_uplift_percent.toFloat()< stock_percent){
            break;
        }
        if(stock_vector_global->at(ab).stock_limit_up_price.toFloat() == stock_vector_global->at(ab).stock_price.toFloat()&&
                stock_vector_global->at(ab).stock_sell_volume.toFloat() == 0){
            //去重复
            int limt_up_int = stock_vector_limt_up_global->size();
            if(limt_up_int==0){
                stock_vector_limt_up_global->append(stock_vector_global->at(ab));
            }
            for(int limt_up_int_for=0; limt_up_int_for<limt_up_int;limt_up_int_for++){
                if(stock_vector_global->at(ab).stock_num == stock_vector_limt_up_global->at(limt_up_int_for).stock_num){
                    limt_up_global_state = false;
                    break;
                }else {limt_up_global_state=true;}
            }
            if(limt_up_global_state){
                stock_vector_limt_up_global->append(stock_vector_global->at(ab));
            }
        }
    }
}
void loopStock(QVector<QString> str_dir,MainWindow *mw){
    while(1){
        time("while");
        getStockDateAll(str_dir,mw);
    }
}

/*
 排序-->涨幅
 */
bool compareStock(const structStock &stock_data1, const structStock &stock_data2)
{
    if(stock_data1.stock_uplift_percent.toFloat() > stock_data2.stock_uplift_percent.toFloat()){//正排序
        return true;
    }else {
        return false;
    }
}

/*
 排序-->卖一总金额
 */
bool compareStockSellTotalMoney(const structStock &stock_data1, const structStock &stock_data2)
{
    if(stock_data1.stock_sell_total_money > stock_data2.stock_sell_total_money){//正排序
        return true;
    }else {
        return false;
    }
}

/*
 排序-->买一总金额
 */
bool compareStockBuyTotalMoney(const structStock &stock_data1, const structStock &stock_data2)
{
    if(stock_data1.stock_buy_total_money > stock_data2.stock_buy_total_money){//正排序
        return true;
    }else {
        return false;
    }
}

/*
刷新卖一表格数据
*/
void refreshSellTotalMoneyStock(){
    //    w->view->setUpdatesEnabled(false);
    for(int i=0; i<stock_vector_global->size(); i++){
        model.setItem(i,5,new QStandardItem(stock_vector_global->at(i).stock_name));
        model.setItem(i,6,new QStandardItem(QString::number(stock_vector_global->at(i).stock_sell_total_money,'f',2)+"万"));
        //        qDebug()<<stock_vector_global->at(i).stock_sell_total_money;
    }
    //    emit w->updateTableSignal();
}

void qdebugStock(QVector<structStock> vector_struct,MainWindow *mw){
    for(int i=0; i<vector_struct.size(); i++){
        mw->models.setItem(i,0,new QStandardItem(vector_struct.at(i).stock_name/*+"\t\n"+stock_vector_global->at(i).stock_num)*/));
        mw->models.setItem(i,1,new QStandardItem(vector_struct.at(i).stock_uplift_percent));
        mw->models.setItem(i,2,new QStandardItem(time("time")));
    }
    //    for(int i1=0; i1<stock_vector_limt_up_global->size(); i1++){
    //        model.setItem(i1,3,new QStandardItem(stock_vector_limt_up_global->at(i1).stock_name));
    //        model.setItem(i1,4,new QStandardItem(stock_vector_limt_up_global->at(i1).stock_uplift_percent));
    //        //        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    //        //        int a1 =qrand()%9;
    //        //        model.setItem(i1,6,new QStandardItem(QString::number(a1)));
    //        //        qDebug()<<stock_vector_limt_up_global->at(i1).stock_num<<stock_vector_limt_up_global->at(i1).stock_uplift_percent<<a1;
    //    }
    //    //    emit w->updateTableSignal();
}
int updateStockVector(structStock struct_stock){
    int size = stock_vector_global->size();
    if(size == 0){
        stock_vector_global->append(struct_stock);
        return stock_vector_global->size();
    }else{
        for(int i=0; i<size; i++){
            if(stock_vector_global->at(i).stock_num == struct_stock.stock_num){
                stock_vector_global->replace(i,struct_stock);
                return i;
            }
        }
        stock_vector_global->append(struct_stock);
    }
    return stock_vector_global->size();
}

/*
处理获取到的股票数据
1.添加股票卖一总金额和买一总金额
*/
QVector<structStock> processStockDate(QString str){
    //    stock_vector_global->clear();//清空vector
    QVector<structStock> stock_vector;
    structStock struct_stock;
    QStringList spList;
    if(str.contains(";",Qt::CaseSensitive)){
        QStringList sList = str.split(";");//先分组
        for(int i=0; i<sList.size()-1; i++){
            if(sList.at(i).contains("~",Qt::CaseSensitive)){
                spList= sList.at(i).split("~");//再分别填充
                if(spList.size()>48){
                    struct_stock.stock_name = spList.at(1);
                    struct_stock.stock_num = spList.at(2);
                    struct_stock.stock_price = spList.at(3);
                    struct_stock.stock_sell_volume = spList.at(20);
                    struct_stock.stock_time = spList.at(30);
                    struct_stock.stock_uplift_percent = spList.at(32);
                    struct_stock.stock_total_money = spList.at(37);
                    struct_stock.stock_limit_up_price = spList.at(47);

                    struct_stock.stock_buy_total_money = spList.at(9).toFloat()*spList.at(10).toFloat()/100;
                    struct_stock.stock_sell_total_money = spList.at(19).toFloat()*spList.at(20).toFloat()/100;
                    stock_vector.append(struct_stock);
                }
            }
        }
    }
    return stock_vector;
}

/*
处理股票数据
*/
bool dealStockData(QString str_deal_stock_data, MainWindow *mw){
    QVector<structStock> vector_struct = processStockDate(str_deal_stock_data);
    qSort(vector_struct.begin(),vector_struct.end(),compareStock);
    //    limtUpStock();
    qdebugStock(vector_struct,mw);
    //    qSort(stock_vector_global->begin(),stock_vector_global->end(),compareStockSellTotalMoney);
    //    refreshSellTotalMoneyStock();
    emit mw->updateTableSignal();
}

/*
获取股票数据
对股票长度超过900个的进行分割，以900为一组进行分割，不够900单独一组
*/
bool getStockDateAll(QVector<QString> vector_str_get_stock_data_all,MainWindow *mw){


    QVector<QString> vector_str_copy;
    QString str_copy;
    while(1){
        if(vector_str_get_stock_data_all.size()>900){
            vector_str_copy.clear();
            for(int int_copy=0;int_copy<900;int_copy++){
                vector_str_copy.append(vector_str_get_stock_data_all.at(int_copy));
            }
            vector_str_get_stock_data_all.remove(0,900-1);

            str_copy += getStockDate(vector_str_copy);

        }else{
            str_copy += getStockDate(vector_str_get_stock_data_all);
            break;
        }
    }
    //    qDebug()<<endl;
    dealStockData(str_copy,mw);
}
QString getStockDate(QVector<QString> vector_str_get_stock_data){
    QString str_tmp;
    for(int i=0; i<vector_str_get_stock_data.size(); i++){
        str_tmp.append(vector_str_get_stock_data.at(i));
        str_tmp.append(",");
    }
    QNetworkReply *replys;
    QNetworkAccessManager netWork;
    QEventLoop eventLoop;
    QTimer timer;
    QObject::connect(&timer, SIGNAL(timeout()),&eventLoop,SLOT(quit()));
    QObject::connect(&netWork, SIGNAL(finished(QNetworkReply*)),&eventLoop,SLOT(quit()));
    timer.start(1000);
    replys =netWork.get(QNetworkRequest(QUrl("http://qt.gtimg.cn/q="+str_tmp )));
    eventLoop.exec();
    QString str_get_stock_data_return = QString::fromLocal8Bit(replys->readAll());
    delete replys;
    return str_get_stock_data_return;
}

QVector <QString> stockString(QString str_stock_dir){
    //读取Excel表格
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", false); //隐藏打开的excel文件界面
    QAxObject *workbooks = excel.querySubObject("WorkBooks");
    QAxObject *workbook = workbooks->querySubObject("Open(QString, QVariant)", /*"d:\\stock.xlsx"*/str_stock_dir); //打开文件
    QAxObject * worksheet = workbook->querySubObject("WorkSheets(int)", 1); //访问第一个工作表
    QAxObject * usedrange = worksheet->querySubObject("UsedRange");
    QAxObject * rows = usedrange->querySubObject("Rows");
    QAxObject *columns = usedrange->querySubObject("Columns");
    int intRows = rows->property("Count").toInt(); //行数
    int intColumns = columns->property("Count").toInt(); //列数

    QString Range = "A1:C" +QString::number(intRows);
    QAxObject *allEnvData = worksheet->querySubObject("Range(QString)", Range); //读取范围
    QVariant allEnvDataQVariant = allEnvData->property("Value");
    QVariantList allEnvDataList = allEnvDataQVariant.toList();
    QVector <QString> str;
    QString data1;
    QVariantList allEnvDataList_i;
    for(int i=0; i< intRows; i++)
    {
        allEnvDataList_i =  allEnvDataList[i].toList() ;
        for(int int_columns_for=0; int_columns_for<allEnvDataList_i.size(); int_columns_for++){
            data1 = allEnvDataList_i[int_columns_for].toString(); //第i行第int_columns_for列的数据
            //            qDebug()<<data1.toFloat();
            int int_sh = data1.toInt();
            if(600000<=int_sh&&int_sh<=700000){
                str.append("sh"+data1);
            }
            if(300000<=int_sh&&int_sh<=400000){
                str.append("sz"+data1);
            }
            if(000000<int_sh&&int_sh<=100000){
                str.append("sz"+data1);
            }
        }
    }
    workbooks->dynamicCall("Close()");
    excel.dynamicCall("Quit()");
    return str;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //    MainWindow w;

    //    w->view->setEditTriggers(QAbstractItemView::NoEditTriggers);//不可编辑
    //    QStringList str_list;
    //    str_list.append("lie1");
    //    str_list.append("lie2");
    //    w->models.setHorizontalHeaderLabels(str_list);
    //    w->models.setVerticalHeaderLabels(str_list);
    MainWindow *w = new MainWindow;
    threadStock thread;
    thread.stockThreadSetDir(stockString("d:\\stock1.xlsx"));
    thread.stockThreadSetWindow(w);
    thread.start();
    //    MainWindow *w2 = new MainWindow;
    //    threadStock thread2;
    //    thread2.stockThreadSetDir(stockString("d:\\stock2.xlsx"));
    //    thread2.stockThreadSetWindow(w2);
    //    thread2.start();
    MainWindow *w3 = new MainWindow;
    threadStock thread3;
    thread3.stockThreadSetDir(stockString("d:\\stock.xlsx"));
    thread3.stockThreadSetWindow(w3);
    thread3.start();

    stockWatchThread watch_thread;
    MainWindow *w4 = new MainWindow;
    QStringList str_list;
    str_list.append("股票名称");
    str_list.append("涨幅");
    str_list.append("成交量占比");
    str_list.append("刷新时间");
    w4->models.setHorizontalHeaderLabels(str_list);
    watch_thread.stockThreadSetDir(stockString("d:\\stock1.xlsx"));
    watch_thread.stockThreadSetWindow(w4);
    watch_thread.start();
    return a.exec();
}
