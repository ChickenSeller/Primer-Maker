#include "daemonworker.h"
#include <QDir>
#include <QDebug>

DaemonWorker::DaemonWorker()
{

}
//加载目标属
GenusCollection DaemonWorker::LoadTargetGenus(string filePath,string targetFileName){//filepath==环境属文件夹 target==目标属文件路径
    if(filePath==""){
        throw "NULL_PATH";
    }
    QDir *dir = new QDir(QString::fromStdString(filePath));
    QStringList filter;
            //filter<<"*.dat";
            //dir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    if(fileInfo->count()<=2){
        throw "NULL_PATH";
    }
    GenusCollection currentCollection;
    TargetGenusList targetGenusList=GetTargetGenusList(targetFileName);
    for(int j=0;j<targetGenusList.genus.size();j++){
        for(int i=2;i<fileInfo->count();i++){
            if(fileInfo->at(i).fileName().toStdString()==targetGenusList.genus[j]){
                Genus currentGenus = GetGenus(fileInfo->at(i).filePath().toStdString(),fileInfo->at(i).fileName().toStdString());
                currentCollection.genus.push_back(currentGenus);
                break;
            }
        }
    }
    return currentCollection;
}
//加载环境属
SimpleGenusCollection DaemonWorker::LoadSimpleSourceGenus(string filePath){
    if(filePath==""){
        throw "NULL_PATH";
    }
    QDir *dir = new QDir(QString::fromStdString(filePath));
    QStringList filter;
            //filter<<"*.dat";
            //dir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    if(fileInfo->count()<=2){
        throw "NULL_PATH";
    }
    SimpleGenusCollection currentCollection;
    for(int i=2;i<fileInfo->count();i++){
        ifstream sourceFile;
        sourceFile.open(fileInfo->at(i).filePath().toStdString().c_str());
        string fileContent;
        string tempLine;
        while(getline(sourceFile,tempLine)){
            trimStr(tempLine);
            fileContent +=tempLine;
        }
        Species tempSpecies;
        tempSpecies.fragment = fileContent;
        tempSpecies.name = fileInfo->at(i).fileName().toStdString();
        currentCollection.genus.push_back(tempSpecies);
    }
    return currentCollection;
}
//加载目标属文件名列表
TargetGenusList DaemonWorker::GetTargetGenusList(string filename){
    ifstream targetFile;
    targetFile.open(filename.c_str());
    TargetGenusList res;
    string tempLine;
    while(getline(targetFile,tempLine)){
        trimStr(tempLine);
        res.genus.push_back(tempLine+".fa");
    }
    return res;
}
//按照属名加载特定属
Genus DaemonWorker::GetGenus(string filePath,string fileName){
    Genus currentGenus;
    Species currentSpecies;
    ifstream genusFile;
    currentGenus.name = fileName;
    genusFile.open(filePath.c_str());
    string tempLine;
        while(getline(genusFile,tempLine))
        {
            trimStr(tempLine);
            if(tempLine.substr(0, 1)==">"){
                if(currentSpecies.name!=""){
                    currentGenus.species.push_back(currentSpecies);
                }
                currentSpecies.name=tempLine;
                currentSpecies.fragment="";
            }else{
                currentSpecies.fragment +=tempLine;
            }
        }
        genusFile.close();
        currentGenus.species.push_back(currentSpecies);
        return currentGenus;
}
//去除首尾空格
void DaemonWorker::trimStr(string &str){
    string buff(str);
    char space = ' ';
    str.assign(buff.begin() + buff.find_first_not_of(space),
    buff.begin() + buff.find_last_not_of(space) + 1);
}
//匹配指定两个种的公共子串
vector <string> DaemonWorker::GetCommonFragment(string stringA, string stringB,int fragmentLengthBottom,int fragmentLengthTop){
    vector <string> stringArray;
    vector <FragmentPair> fragmentPair;
    vector <string> stringResult;
    int fragmentGroupLength = fragmentLengthBottom/2;
    for(int i=0;i<stringA.length()/fragmentGroupLength;i++){
        string tempStr;
        tempStr = stringA.substr(i,fragmentGroupLength);
        stringArray.push_back(tempStr);
    }
    for(int i=0;i<stringArray.size();i++){
        size_t pos = stringB.find(stringArray[i]);
        if(pos==string::npos){
            continue;
        }else{
            FragmentPair tempFragmentPair;
            tempFragmentPair.fragment = stringArray[i];
            tempFragmentPair.posB = pos;
            tempFragmentPair.posA = i*fragmentGroupLength;
            fragmentPair.push_back(tempFragmentPair);
        }
    }
    for(int i=0;i<fragmentPair.size();i++){
        string stringB2;
        if(fragmentPair[i].posB+fragmentLengthTop<=stringB.length()){
            if(fragmentPair[i].posB+fragmentLengthBottom/2-fragmentLengthTop>=0){
               stringB2 = stringB.substr(fragmentPair[i].posB+fragmentLengthBottom/2-fragmentLengthTop,fragmentLengthTop*2-fragmentLengthBottom/2);
            }else{
                stringB2 = stringB.substr(0,fragmentLengthTop*2-fragmentLengthBottom/2);
            }
        }else{
            if(fragmentPair[i].posB+fragmentLengthBottom/2-fragmentLengthTop>=0 ){
                stringB2 = stringB.substr(fragmentPair[i].posB+fragmentLengthBottom/2-fragmentLengthTop,stringB.length()-fragmentPair[i].posB-fragmentLengthBottom/2+fragmentLengthTop);
            }else{
                stringB2 = stringB;
            }
        }
        vector <string> res = GetCommonFragmentEx(stringA,stringB2,fragmentLengthBottom,fragmentLengthTop);
        for(int j=0;j<res.size();j++){
            stringResult.push_back(res[j]);
        }
    }
    return Unique(stringResult);

}
//简单匹配公共字符串
vector <string> DaemonWorker::GetCommonFragmentEx(string stringA, string stringB,int fragmentLengthBottom,int fragmentLengthTop){
    vector <string> result;

    //QDebug(stringA);
    for(int j=fragmentLengthBottom;j<=fragmentLengthTop;j++){
        for(int i=0;i<stringB.length()-j+1;i++){
            string temp_str = stringB.substr(i,j);
            if(stringA.find(temp_str)!=string::npos){
                result.push_back(temp_str);
            }else{
                continue;
            }
        }
    }
    return result;
}
//字符串去重
vector <string> DaemonWorker::Unique(vector<string> rawString){
    vector<string>::iterator newIterater;
    sort(rawString.begin(),rawString.end());
    newIterater = unique(rawString.begin(),rawString.end());
    rawString.erase(newIterater,rawString.end());
    return rawString;
}
//获取所有目标属特征片段
vector <CommonFragment> DaemonWorker::GetCommonFragmentFromGenus(GenusCollection targetGenus, SimpleGenusCollection sourceGenus, int coverage){
    vector <CommonFragment> commonFragments;
    for(int i=0;i<targetGenus.genus.size();i++){
        CommonFragment tempFragment;
        tempFragment.name = targetGenus.genus[i].name;
        if(targetGenus.genus[i].species.size()<2){
            if(targetGenus.genus[i].species.size()==1){
                targetGenus.genus[i].species.push_back(targetGenus.genus[i].species[0]);
            }else{
                continue;
            }
        }
        tempFragment.fragments = GetCommonFragmentFromSpecificGenus(targetGenus.genus[i],sourceGenus,coverage);
        commonFragments.push_back(tempFragment);
    }
    return commonFragments;
}
//获取指定属特征片段
vector <string> DaemonWorker::GetCommonFragmentFromSpecificGenus(Genus genus, SimpleGenusCollection sourceGenus, int coverage){
    vector <string> rawCommonFragment;
    vector <string> commonFragment;

    for(int i=0;i<genus.species.size();i++){
        for(int j=i;j<genus.species.size();j++){
            vector <string> tempCommonFragment = GetCommonFragment(genus.species[i].fragment,genus.species[j].fragment,config.fragmentLengthBottom,config.fragmentLengthTop);
            rawCommonFragment.insert(rawCommonFragment.end(),tempCommonFragment.begin(),tempCommonFragment.end());
        }
    }
    rawCommonFragment = Unique(rawCommonFragment);

    for(int i=0;i<rawCommonFragment.size();i++){
        //commonFragment.push_back(rawCommonFragment[i]);
        if(GetCoverage(rawCommonFragment[i],genus)>=coverage){
            commonFragment.push_back(rawCommonFragment[i]);
        }

    }

    rawCommonFragment.clear();
    for(int i=0;i<commonFragment.size();i++){

        if(IfSpecific(commonFragment[i],sourceGenus,genus)){
            rawCommonFragment.push_back(commonFragment[i]);
        }
    }

    return Unique(rawCommonFragment);
}
//计算片段覆盖率
int DaemonWorker::GetCoverage(string fragment, Genus genus){
    int hit=0;
    for(int i=0;i<genus.species.size();i++){
        if(genus.species[i].fragment.find(fragment)!=string::npos){
            hit++;
            //timer++;
        }
    }
    //timer = hit*100/genus.species.size();
    int m = hit*100/genus.species.size();
    return m;
}
//判定是否为特征片段，属外查找
bool DaemonWorker::IfSpecific(string fragment, SimpleGenusCollection sourceGenus,Genus targetGenus){
    for(int i=0;i<sourceGenus.genus.size();i++){
       if(sourceGenus.genus[i].name == targetGenus.name){
            continue;
       }
       if(sourceGenus.genus[i].fragment.find(fragment)!=string::npos){
            //timer++;
            return false;
        }
    }
    timer++;
    return true;
}
//获取指定属的特征片段位置
vector <FragmentPair> DaemonWorker::GetFragmentPosFromSpecificGenus(string species, vector<string> fragments){
    vector <FragmentPair> result;
    for(int i=0;i<fragments.size();i++){
        size_t pos = species.find(fragments[i]);
        if(pos==string::npos){
            continue;
        }
        FragmentPair temp;
        temp.fragment=fragments[i];
        temp.posA = pos;
        temp.posB = fragments[i].length();
        result.push_back(temp);
    }
    sort(result.begin(),result.end());
    return result;
}
//获取引物对位置
vector <PairInfo> DaemonWorker::Pair(vector<FragmentPair> in, int MIN, int MAX){
        int num;
        num = in.size();
        vector<PairInfo> ans;
        int i, j;
        int t1, t2;
        t1 = t2 = 0;
        for(i = 0; i <= num - 2; i++){
            while(in[t1].posA < in[i].posA + MIN){
                t1++;
                if(t1 >= num - 1)return ans;
            }
            if(in[t1].posA > in[i].posA + MAX){continue;}
            if(t2 < t1)t2 = t1;
            while(in[t2+1].posA < in[i].posA + MAX){
                t2++;
                if(t2 >= num-1)break;
            }
            for(j = t1; j <= t2; j++){
                ans.push_back(PairInfo(in[i].posA, in[i].posB, in[j].posA, in[j].posB,in[i].fragment,in[j].fragment));
                //cout << in[i].posA << ' ' << in[j].posA <<endl;
            }
        }
        return ans;
}
//
vector <GenusPrimerPair> DaemonWorker::MakePair(GenusCollection targetGenus, vector <CommonFragment> commonFragmentCollection, int MIN, int MAX){
    vector <GenusPrimerPair> result;
    for(int i=0 ;i<targetGenus.genus.size();i++){

        int k=0;
        for(k=0;k<commonFragmentCollection.size();k++){
            if(commonFragmentCollection[k].name==targetGenus.genus[i].name){
                //tempCommonFragmentInSpecificSpecies = commonFragmentCollection[k].fragments;
                break;
            }
        }
        GenusPrimerPair tempGenusPrimePair = MakePairInSpecificGenus(targetGenus.genus[i],commonFragmentCollection[k], MIN, MAX);
        result.push_back(tempGenusPrimePair);
    }
    return result;
}
GenusPrimerPair DaemonWorker::MakePairInSpecificGenus(Genus targetGenus, CommonFragment commonFragmentCollection, int MIN, int MAX){
    GenusPrimerPair tempGenusPrimePair;
    tempGenusPrimePair.name = targetGenus.name;
    for(int j=0;j<targetGenus.species.size();j++){
        //vector <string> tempCommonFragmentInSpecificSpecies;
        vector <FragmentPair> tempPos = GetFragmentPosFromSpecificGenus(targetGenus.species[j].fragment,commonFragmentCollection.fragments);
        if(tempPos.size()==0){
            continue;
        }
        sort(tempPos.begin(),tempPos.end());
        vector <PairInfo> tempPairInfo = Pair(tempPos,MIN,MAX);
        tempGenusPrimePair.pairs.insert(tempGenusPrimePair.pairs.begin(),tempPairInfo.begin(),tempPairInfo.end());

    }
    return tempGenusPrimePair;
}

bool DaemonWorker::IfTmDiff(string strA, string strB, int t){
    string a=strA;
    string b=strB;
    int length;
        int d1, d2, i;
        d1 = d2 = 0;
        length = a.length();
        for(i = 0; i <= length - 1; i++){
            if(a[i] == 'A' || a[i] == 'T')
                d1 += 2;
            else
                d1 += 4;
        }
        length = b.length();
        for(i = 0; i <= length - 1; i++){
            if(b[i] == 'A' || b[i] == 'T')
                d2 += 2;
            else
                d2 += 4;
        }
        if(d1 - d2 < t && d1 - d2 > -t)
            return true;
        else
            return false;
}

bool DaemonWorker::IfRepeat(string str, int num){
    string in = str;
    string a, t, c, g;
        a = t = c = g = "";
        int i;
        for(i = 1; i <= num; i++){
            a += "A";
            t += "T";
            c += "C";
            g += "G";
        }
        if(in.find(a) != -1 || in.find(t) != -1 || in.find(c) != -1 || in.find(g) != -1)return false;
        return true;
}

bool DaemonWorker::IfHairpinStructure(string str, int num){
    int length, i, j;
        string temp,in;
        in=str;
        length = in.length();
        for(i = 0; i <= length - num; i++){
            temp = "";
            for(j = 0; j <= num - 1; j++){
                if(in[i + j] == 'A')temp += "T";
                if(in[i + j] == 'T')temp += "A";
                if(in[i + j] == 'C')temp += "G";
                if(in[i + j] == 'G')temp += "C";
            }
            reverse(temp.begin(), temp.end());
            if(in.find(temp) != -1)return false;
        }
        return true;
}

bool DaemonWorker::IfDimer(string strA, string strB, int num){
    string a=strA;
    string b=strB;
    int length, i;
        length = a.length();
        string temp;
        for(i = 0; i <= length - num; i++){
            temp = a.substr(i, num);
            if(b.find(temp) != -1)return false;
        }
        return false;
}

void DaemonWorker::Reverse(string &in){
        int length, i;
        string temp;
        temp = "";
        length = in.length();
        reverse(in.begin(), in.end());
        for(i = 0; i <= length - 1; i++){
            if(in[i] == 'A')temp += "T";
            if(in[i] == 'T')temp += "A";
            if(in[i] == 'C')temp += "G";
            if(in[i] == 'G')temp += "C";
        }
        in = temp;
}

vector <GenusPrimerPair> DaemonWorker::FilterFragment(vector<GenusPrimerPair> source){
    vector <GenusPrimerPair> result;
    for(int i=0;i<source.size();i++){
        GenusPrimerPair tempGenusPrimerPair;
        tempGenusPrimerPair.name = source[i].name;
        for(int j=0;j<source[i].pairs.size();j++){
            PairInfo tempPairInfo=source[i].pairs[j];
            if(!IfTmDiff(tempPairInfo.fragment1,tempPairInfo.fragment2,5)){
                continue;
            }
            if(!IfRepeat(tempPairInfo.fragment1,3)){
                continue;
            }
            if(!IfRepeat(tempPairInfo.fragment2,3)){
                continue;
            }
            if(!IfHairpinStructure(tempPairInfo.fragment1,4)){
                continue;
            }
            if(!IfHairpinStructure(tempPairInfo.fragment2,4)){
                continue;
            }
            if(!IfDimer(tempPairInfo.fragment1,tempPairInfo.fragment2,4)){
                continue;
            }
            Reverse(tempPairInfo.fragment2);
            tempGenusPrimerPair.pairs.push_back(tempPairInfo);
        }
        result.push_back(tempGenusPrimerPair);
    }
    return result;
}


