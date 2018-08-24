#ifndef STOCKTHREAD_H
#define STOCKTHREAD_H
#include <QThread>

class stockThread : public QThread
{
public:
    stockThread();
    void run();
};

#endif // STOCKTHREAD_H
