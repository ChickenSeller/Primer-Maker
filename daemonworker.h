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
    vector <string> Unique(vector <string> rawString);
    vector <string> GetCommonFragmentFromGenus(GenusCollection targetGenus,SimpleGenusCollection sourceGenus,int coverage);
    vector <string> GetCommonFragmentFromSpecificGenus(Genus genus,SimpleGenusCollection sourceGenus,int coverage);
    int GetCoverage(string fragment,Genus genus);
    bool IfSpecific(string fragment,SimpleGenusCollection sourceGenus,Genus targetGenus);
    //vector <string> GetGenusCommonFragment()
};

#endif // DAEMONWORKER_H
