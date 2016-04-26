#ifndef DAEMONTASK_H
#define DAEMONTASK_H

#include <QThread>
#include "daemonworker.h"
#include "datadef.h"

class DaemonTask : public QThread
{
    Q_OBJECT
public:
    DaemonTask();
    ~DaemonTask();
protected:
    void run();
signals:
    void signal_complete_proccess(QString);

};

#endif // DAEMONTASK_H
