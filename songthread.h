#ifndef SONGTHREAD
#define SONGTHREAD
#include <QThread>
#include <qstring.h>
#include "timerthread.h"

class SongThread : public QThread
{
    Q_OBJECT
public:
    explicit SongThread(QObject *parent = 0);
    bool flag;
    bool timeThreadUsed =  false;

    QString filePath;
    TimerThread *timethread;

protected:
    void run();

signals:
    void SongSignal(double a);

private:
    int msg;

};

#endif // SONGTHREAD

