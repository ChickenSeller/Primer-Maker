#ifndef DAEMONTASK_H
#define DAEMONTASK_H

#include <QThread>
#include "daemonworker.h"
#include "datadef.h"

class DaemonTask :  public QThread
{
    Q_OBJECT
public:
    DaemonTask();
    ~DaemonTask();
    void HandleWorkerSignal();
protected:
    void run();
signals:
    void signal_complete_proccess(QString);
    void signal_status(QString);
private slots:
    void set_worker_status(QString);

};

#endif // DAEMONTASK_H
