#ifndef DAEMONWORKER_H
#define DAEMONWORKER_H
#include "datadef.h"
#include<fstream>

using namespace std;
class DaemonWorker
{
public:
    DaemonWorker();
    GenusCollection LoadSourceGenus(string filePath);
    Genus GetGenus(string filePath);
    void trimStr(string& str);
};

#endif // DAEMONWORKER_H
