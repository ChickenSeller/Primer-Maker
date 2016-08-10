#include "daemonworker.h"
#include <QDir>
#include <QDebug>

extern string currentdoing;
extern float totalwork;
extern float workdone;

DaemonWorker::DaemonWorker()
{

}
//加载目标属     已显示进度
GenusCollection DaemonWorker::LoadTargetGenus(string filePath,string targetFileName){//filepath==环境属文件夹 target==目标属文件路径
    currentdoing = "正在加载目标属";
    emit signal_worker_status(QString::fromStdString(currentdoing));
    totalwork = 0;
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
    totalwork = targetGenusList.genus.size();
    workdone = 0;
    for(int j=0;j<targetGenusList.genus.size();j++){
        for(int i=2;i<fileInfo->count();i++){
            if(fileInfo->at(i).fileName().toStdString()==targetGenusList.genus[j]){
                Genus currentGenus = GetGenus(fileInfo->at(i).filePath().toStdString(),fileInfo->at(i).fileName().toStdString());
                currentCollection.genus.push_back(currentGenus);
                emit signal_worker_status(QString::fromStdString(currentdoing+"\t"+currentGenus.name));
                break;
            }
        }
        workdone++;
    }
    return currentCollection;
}
//加载环境属     已显示进度
SimpleGenusCollection DaemonWorker::LoadSimpleSourceGenus(string filePath){
    currentdoing = "正在加载环境属";
    emit signal_worker_status(QString::fromStdString(currentdoing));
    totalwork = 0;
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
    totalwork = fileInfo->count() - 2;
    workdone = 0;
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
        emit signal_worker_status(QString::fromStdString(currentdoing+"\t"+tempSpecies.name));

        workdone++;
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
        tempStr = stringA.substr(i*fragmentGroupLength,fragmentGroupLength);
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
                int j=0;
                for(j=0;j<targetGenus.genus[i].species[0].fragment.length()/config.fragmentLengthTop;j++){
                    string tempStr = targetGenus.genus[i].species[0].fragment.substr(j*config.fragmentLengthTop,config.fragmentLengthTop);
                    if(IfSpecific(tempStr,sourceGenus,targetGenus.genus[i])){
                        tempFragment.fragments.push_back(tempStr);
                    }

                }
                commonFragments.push_back(tempFragment);
                continue;
            }else{
                continue;
            }
        }
        tempFragment.fragments = GetCommonFragmentFromSpecificGenus(targetGenus.genus[i],sourceGenus,coverage);
        commonFragments.push_back(tempFragment);
    }
    return commonFragments;
}
//获取指定属特征片段     已显示进度
vector <string> DaemonWorker::GetCommonFragmentFromSpecificGenus(Genus genus, SimpleGenusCollection sourceGenus, int coverage){
    vector <string> rawCommonFragment;
    vector <string> commonFragment;
    vector <string> tempCommonFragment;
    int PctSpeciesNum;
    PctSpeciesNum = ceil(genus.species.size() * (1 - (float)coverage / 100));
    currentdoing = "正在获取" + genus.name + "内的特征片段";
    emit signal_worker_status(QString::fromStdString(currentdoing));
    totalwork = PctSpeciesNum * PctSpeciesNum;
    workdone = 0;
    if(PctSpeciesNum == 1){
        emit signal_worker_status(QString::fromStdString(genus.species[0].name+"\t<--->\t"+genus.species[0].name));

        tempCommonFragment = GetCommonFragment(genus.species[0].fragment,genus.species[0].fragment,config.fragmentLengthBottom,config.fragmentLengthTop);
        rawCommonFragment.insert(rawCommonFragment.end(),tempCommonFragment.begin(),tempCommonFragment.end());
        rawCommonFragment = Unique(rawCommonFragment);
        workdone++;
    }
    else{
        for(int i = 0; i < PctSpeciesNum; i++){
            for(int j = i + 1; j < PctSpeciesNum; j++){
                emit signal_worker_status(QString::fromStdString(genus.species[i].name+"\t<--->\t"+genus.species[j].name));
                tempCommonFragment = GetCommonFragment(genus.species[i].fragment,genus.species[j].fragment,config.fragmentLengthBottom,config.fragmentLengthTop);
                rawCommonFragment.insert(rawCommonFragment.end(),tempCommonFragment.begin(),tempCommonFragment.end());
                rawCommonFragment = Unique(rawCommonFragment);
                workdone++;
            }
        }
    }
    /*ofstream ostr;
    ostr.open("d:\\test.txt");
    string str2;
    for(int i=0;i<rawCommonFragment.size();i++){
        str2+=rawCommonFragment[i]+"\n";
    }
    ostr<<str2;
    ostr.close();*/
    rawCommonFragment = Unique(rawCommonFragment);
    emit signal_worker_status(QString::fromStdString("正在计算属"+genus.name+"中的片段覆盖率"));
    for(int i=0;i<rawCommonFragment.size();i++){
        //commonFragment.push_back(rawCommonFragment[i]);
        if(GetCoverage(rawCommonFragment[i],genus)>=coverage){
            commonFragment.push_back(rawCommonFragment[i]);
        }
    }
    rawCommonFragment.clear();
    emit signal_worker_status(QString::fromStdString("正在判定属"+genus.name+"中的特征片段"));
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
    //emit signal_worker_status(QString::fromStdString("正在判定片段"+fragment+"是否为特征片段"));
    for(int i=0;i<sourceGenus.genus.size();i++){
       if(sourceGenus.genus[i].name == targetGenus.name){
            continue;
       }
       if(sourceGenus.genus[i].fragment.find(fragment)!=string::npos){
            return false;
        }
    }
    return true;
}
//获取指定属的特征片段位置
vector <FragmentPair> DaemonWorker::GetFragmentPosFromSpecificGenus(string species, vector<string> fragments){
    vector <FragmentPair> result;
    //emit signal_worker_status(QString::fromStdString("正在计算种"+species+"中的片段覆盖率"));
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
//按特征片段位置获取引物对
vector <PairInfo> DaemonWorker::Pair(vector<FragmentPair> in, int MIN, int MAX){
    int num;
    num = in.size();
    vector<PairInfo> res;
    int i, j;
    int t1, t2;
    t1 = t2 = 0;
    for(i = 0; i <= num - 2; i++){
        while(in[t1].posA < in[i].posA + MIN){
            t1++;
            if(t1 >= num - 1)return res;
        }
        if(in[t1].posA > in[i].posA + MAX){continue;}
        if(t2 < t1)t2 = t1;
        while(t2 < num - 1 && in[t2+1].posA < in[i].posA + MAX){
            t2++;
        }
        for(j = t1; j <= t2; j++){
            res.push_back(PairInfo(in[i].posA, in[i].posB, in[j].posA, in[j].posB, in[i].fragment, in[j].fragment, in[j].posA-in[i].posA+in[j].posB));
        }
    }
    return res;
}
//获取所有引物对
vector <GenusPrimerPair> DaemonWorker::MakePair(GenusCollection targetGenus, vector <CommonFragment> commonFragmentCollection, int MIN, int MAX){
    currentdoing = "正在匹配引物对";
    totalwork = targetGenus.genus.size();
    workdone = 0;
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
        workdone++;
    }
    return result;
}
//在特定属内获取引物对
GenusPrimerPair DaemonWorker::MakePairInSpecificGenus(Genus targetGenus, CommonFragment commonFragmentCollection, int MIN, int MAX){
    GenusPrimerPair tempGenusPrimePair;
    emit signal_worker_status(QString::fromStdString("正在计算属"+targetGenus.name+"中的引物对"));
    tempGenusPrimePair.name = targetGenus.name;
    for(int j=0;j<targetGenus.species.size();j++){
        //vector <string> tempCommonFragmentInSpecificSpecies;
        emit signal_worker_status(QString::fromStdString("正在寻找种"+targetGenus.species[j].name+"中的引物对"));
        vector <FragmentPair> tempPos = GetFragmentPosFromSpecificGenus(targetGenus.species[j].fragment,commonFragmentCollection.fragments);
        if(tempPos.size()==0){
            continue;
        }
        sort(tempPos.begin(),tempPos.end());
        vector <PairInfo> tempPairInfo = Pair(tempPos,MIN,MAX);
        tempGenusPrimePair.pairs.insert(tempGenusPrimePair.pairs.begin(),tempPairInfo.begin(),tempPairInfo.end());
        break;

    }
    return tempGenusPrimePair;
}
//判定温度
bool DaemonWorker::IfTmDiff(string strA, string strB, int t){
    int length;
    int d1, d2, i;
    d1 = d2 = 0;
    length = strA.length();
    for(i = 0; i <= length - 1; i++){
        if(strA[i] == 'a' || strA[i] == 't')
            d1 += 2;
        else
            d1 += 4;
    }
    length = strB.length();
    for(i = 0; i <= length - 1; i++){
        if(strB[i] == 'a' || strB[i] == 't')
            d2 += 2;
        else
            d2 += 4;
    }
    if(d1 - d2 < t && d1 - d2 > -t)
        return true;
    else
        return false;
}
//判定重复
bool DaemonWorker::IfRepeat(string str, int num){
    string a, t, c, g;
    a = t = c = g = "";
    int i;
    for(i = 1; i <= num; i++){
        a += "a";
        t += "t";
        c += "c";
        g += "g";
    }
    if(str.find(a) != -1 || str.find(t) != -1 || str.find(c) != -1 || str.find(g) != -1)return false;
    return true;
}
//判定发卡结构
bool DaemonWorker::IfHairpinStructure(string str, int num){
    int length, i, j;
    string temp1;
    string temp2;
    length = str.length();
    for(i = 0; i <= length - num; i++){
        temp1 = "";
        for(j = 0; j <= num - 1; j++){
            if(str[i + j] == 'a')temp1 += "t";
            if(str[i + j] == 't')temp1 += "a";
            if(str[i + j] == 'c')temp1 += "g";
            if(str[i + j] == 'g')temp1 += "c";
        }
        reverse(temp1.begin(), temp1.end());
        temp2 = str.substr(i + num);
        if(temp2.find(temp1) != -1)return false;
    }
    return true;
}
//判定二聚体
bool DaemonWorker::IfDimer(string strA, string strB, int num){
    int length, i;
    length = strA.length();
    string temp;
    for(i = 0; i <= length - num; i++){
        temp = strA.substr(i, num);
        if(strB.find(temp) != -1)return false;
    }
    return true;
}
//字符串反向互补
void DaemonWorker::Reverse(string &str){
        int length, i;
        string temp;
        temp = "";
        length = str.length();
        reverse(str.begin(), str.end());
        for(i = 0; i <= length - 1; i++){
            if(str[i] == 'a')temp += "t";
            if(str[i] == 't')temp += "a";
            if(str[i] == 'c')temp += "g";
            if(str[i] == 'g')temp += "c";
        }
        str = temp;
}
//判断合格与否
bool DaemonWorker::JudgeStringPair(string strA, string strB){
    if(!IfTmDiff(strA,strB,5)){
        return false;
    }
    if(!IfRepeat(strA,4)){
        return false;
    }
    if(!IfRepeat(strB,4)){
        return false;
    }
    if(!IfHairpinStructure(strA,4)){
        return false;
    }
    if(!IfHairpinStructure(strB,4)){
        return false;
    }
    if(!IfDimer(strA,strB,4)){
        return false;
    }
    return true;
}
//筛选
vector <GenusPrimerPair> DaemonWorker::FilterFragment(vector<GenusPrimerPair> source, vector<GenusNotPaired> &res){
    vector <GenusPrimerPair> result;
    vector <GenusNotPaired> nopairresult;
    emit signal_worker_status(QString::fromStdString("正在筛选引物对"));
    for(int i=0;i<source.size();i++){
        GenusPrimerPair tempGenusPrimerPair;
        tempGenusPrimerPair.name = source[i].name;
        emit signal_worker_status(QString::fromStdString("正在筛选属"+tempGenusPrimerPair.name+"中的引物对"));
        for(int j=0;j<source[i].pairs.size();j++){
            PairInfo tempPairInfo=source[i].pairs[j];
            if(!JudgeStringPair(tempPairInfo.fragment1,tempPairInfo.fragment2)){
                continue;
            }
            Reverse(tempPairInfo.fragment2);
            tempGenusPrimerPair.pairs.push_back(tempPairInfo);
        }
        if(tempGenusPrimerPair.pairs.size()==0){
            GenusNotPaired temp;
            temp.name = source[i].name;
            temp.num = i;
            nopairresult.push_back(temp);
        }else{
            result.push_back(tempGenusPrimerPair);
        }
    }
    res = nopairresult;
    return result;
}
//补全
vector <GenusPrimerPair> DaemonWorker::PairTheLone(GenusCollection targetGenus, vector <CommonFragment> commonFragmentCollection, vector<GenusNotPaired> res){
    vector <GenusPrimerPair> result;
    GenusPrimerPair tempPair;
    emit signal_worker_status(QString::fromStdString("正在补全引物对"));
    int totalnum = res.size();
    for(int i = 0; i <= totalnum - 1; i++){
        Genus thisgenus = targetGenus.genus[res[i].num];
        emit signal_worker_status(QString::fromStdString("正在补全属"+thisgenus.name+"中的引物对"));
        CommonFragment thisFragments = commonFragmentCollection[res[i].num];
        tempPair.name = thisFragments.name;
        int FraNum = thisFragments.fragments.size();
        for(int j = 0; j <= FraNum - 1; j++){
            int speciesnum = thisgenus.species.size();
            for(int k = 0; k <= speciesnum - 1; k++){
                int pos = thisgenus.species[k].fragment.find(thisFragments.fragments[j]);
                if(pos != -1 && pos + 230 < thisgenus.species[k].fragment.length()){
                    int strlength = thisFragments.fragments[j].length();
                    tempPair.pairs.push_back(PairInfo(pos,strlength,pos+200,20,thisFragments.fragments[j],thisgenus.species[k].fragment.substr(pos+200,20),220));
                    tempPair.pairs.push_back(PairInfo(pos,strlength,pos+200,21,thisFragments.fragments[j],thisgenus.species[k].fragment.substr(pos+200,21),221));
                    tempPair.pairs.push_back(PairInfo(pos,strlength,pos+200,22,thisFragments.fragments[j],thisgenus.species[k].fragment.substr(pos+200,22),222));
                    tempPair.pairs.push_back(PairInfo(pos,strlength,pos+200,23,thisFragments.fragments[j],thisgenus.species[k].fragment.substr(pos+200,23),223));
                    tempPair.pairs.push_back(PairInfo(pos,strlength,pos+200,24,thisFragments.fragments[j],thisgenus.species[k].fragment.substr(pos+200,24),224));
                    tempPair.pairs.push_back(PairInfo(pos,strlength,pos+200,25,thisFragments.fragments[j],thisgenus.species[k].fragment.substr(pos+200,25),225));
                    tempPair.pairs.push_back(PairInfo(pos,strlength,pos+200,26,thisFragments.fragments[j],thisgenus.species[k].fragment.substr(pos+200,26),226));
                    tempPair.pairs.push_back(PairInfo(pos,strlength,pos+200,27,thisFragments.fragments[j],thisgenus.species[k].fragment.substr(pos+200,27),227));
                    tempPair.pairs.push_back(PairInfo(pos,strlength,pos+200,28,thisFragments.fragments[j],thisgenus.species[k].fragment.substr(pos+200,28),228));
                    tempPair.pairs.push_back(PairInfo(pos,strlength,pos+200,29,thisFragments.fragments[j],thisgenus.species[k].fragment.substr(pos+200,29),229));
                }
            }
        }
        result.push_back(tempPair);
    }
    return result;
}

void DaemonWorker::run(){
    /*
    switch(method){
    case ""
    }
    */
}


