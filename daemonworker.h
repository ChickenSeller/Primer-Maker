#ifndef DAEMONWORKER_H
#define DAEMONWORKER_H
#include "datadef.h"
#include<fstream>
#include<sstream>
using namespace std;
class DaemonWorker
{
public:
    DaemonWorker();
    GenusCollection LoadTargetGenus(string filePath,string targetFileName);
    SimpleGenusCollection LoadSimpleSourceGenus(string filePath);
    Genus GetGenus(string filePath,string fileName);
    void trimStr(string& str);
    vector <string> GetCommonFragment(string stringA,string stringB,int fragmentLengthBottom,int fragmentLengthTop);
    vector <string> GetCommonFragmentEx(string stringA, string stringB,int fragmentLengthBottom,int fragmentLengthTop);
    GenusList genusList;
    TargetGenusList GetTargetGenusList(string filename);
    void test();
    //vector <string> GetGenusCommonFragment()
};

#endif // DAEMONWORKER_H
