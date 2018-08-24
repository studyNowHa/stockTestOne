#include "stockwatchthread.h"
//#include "main.cpp"

stockWatchThread::stockWatchThread()
{

}

stockWatchThread::~stockWatchThread()
{
    delete mw;
}

void stockWatchThread::run()
{
    stockWatchThreadIn(str_dir,mw);
}

void stockWatchThread::stockThreadSetDir(QVector<QString> vector_str)
{
    str_dir = vector_str;
}

void stockWatchThread::stockThreadSetWindow(MainWindow *mw)
{
    this->mw = mw;
}
