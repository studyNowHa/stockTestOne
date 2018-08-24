#ifndef STOCKWATCHTHREAD_H
#define STOCKWATCHTHREAD_H
#include <QThread>
#include "mainwindow.h"
#include "main.h"

class stockWatchThread:public QThread
{
public:
    stockWatchThread();
    ~stockWatchThread();
    void run();
    void stockThreadSetDir(QVector<QString> vector_str);
    void stockThreadSetWindow(MainWindow *mw);
private:
    MainWindow *mw;
    QVector<QString> str_dir;
};

#endif // STOCKWATCHTHREAD_H
