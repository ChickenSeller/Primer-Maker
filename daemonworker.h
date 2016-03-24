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
    vector <string> GetCommonFragment(string stringA,string stringB,int fragmentLengthBottom,int fragmentLengthTop);
    vector <string> GetCommonFragmentEx(string stringA, string stringB,int fragmentLengthBottom,int fragmentLengthTop);
    GenusList genusList;
};

#endif // DAEMONWORKER_H
