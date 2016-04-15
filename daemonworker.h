#ifndef DAEMONWORKER_H
#define DAEMONWORKER_H
#include "datadef.h"
#include<fstream>
#include<sstream>
#include <QThread>
using namespace std;
class DaemonWorker: public QThread
{
public:
    string method;
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
    vector <CommonFragment> GetCommonFragmentFromGenus(GenusCollection targetGenus,SimpleGenusCollection sourceGenus,int coverage);
    vector <string> GetCommonFragmentFromSpecificGenus(Genus genus,SimpleGenusCollection sourceGenus,int coverage);
    int GetCoverage(string fragment,Genus genus);
    bool IfSpecific(string fragment,SimpleGenusCollection sourceGenus,Genus targetGenus);
    vector <FragmentPair> GetFragmentPosFromSpecificGenus(string species,vector <string> fragments);
    vector <PairInfo> Pair(vector <FragmentPair> in, int MIN, int MAX);
    vector <GenusPrimerPair> MakePair(GenusCollection targetGenus,vector <CommonFragment> commonFragmentCollection, int MIN, int MAX);
    GenusPrimerPair MakePairInSpecificGenus(Genus targetGenus,CommonFragment commonFragmentCollection, int MIN, int MAX);
    bool IfTmDiff(string strA,string strB,int t);
    bool IfRepeat(string str,int num);
    bool IfHairpinStructure(string str,int num);
    bool IfDimer(string strA,string strB,int num);
    void Reverse(string &in);
    vector <GenusPrimerPair> FilterFragment(vector <GenusPrimerPair> source);
    //vector <string> GetGenusCommonFragment()
protected:
    void run();
};

#endif // DAEMONWORKER_H
