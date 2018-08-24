#ifndef THREADSTOCK_H
#define THREADSTOCK_H
#include <QThread>
#include <QVector>
#include <QString>
#include "mainwindow.h"
#include "main.h"

//extern void loopStock(QVector<QString> str_dir,MainWindow *mw);

class threadStock: public QThread
{
public:
    threadStock();
    ~threadStock();
    void run();
    void stockThreadSetDir(QVector<QString> vector_str);
    void stockThreadSetWindow(MainWindow *mw);
private:
    MainWindow *mw;
    QVector<QString> str_dir;
};

#endif // THREADSTOCK_H
