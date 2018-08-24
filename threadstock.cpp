#include "threadstock.h"
//#include "main.cpp"

threadStock::threadStock()
{

}

threadStock::~threadStock()
{
    delete mw;
}

void threadStock::run()
{
    loopStock(str_dir,mw);
}

void threadStock::stockThreadSetDir(QVector<QString> vector_str)
{
    str_dir = vector_str;
}

void threadStock::stockThreadSetWindow(MainWindow *mw)
{
   this->mw = mw;
}
