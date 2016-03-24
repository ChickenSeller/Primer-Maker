#include "daemonworker.h"
#include <QDir>

DaemonWorker::DaemonWorker()
{

}

GenusCollection DaemonWorker::LoadSourceGenus(string filePath){
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
    for(int i=2;i<fileInfo->count();i++){
        Genus currentGenus = GetGenus(fileInfo->at(i).filePath().toStdString());
        currentCollection.genus.push_back(currentGenus);
    }
    return currentCollection;
}

Genus DaemonWorker::GetGenus(string filePath){
    Genus currentGenus;
    Species currentSpecies;
    ifstream genusFile;
    genusFile.open(filePath);
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

void DaemonWorker::trimStr(string &str){
    string buff(str);
    char space = ' ';
    str.assign(buff.begin() + buff.find_first_not_of(space),
    buff.begin() + buff.find_last_not_of(space) + 1);
}

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
        string stringA2;
        if(fragmentPair[i].posA+fragmentLengthTop<=stringA.length()){
            if(fragmentPair[i].posA+fragmentLengthBottom/2-fragmentLengthTop>=0){
               stringA2 = stringA.substr(fragmentPair[i].posA+fragmentLengthBottom/2-fragmentLengthTop,fragmentLengthTop*2-fragmentLengthBottom/2);
            }else{
                stringA2 = stringA.substr(0,fragmentLengthTop*2-fragmentLengthBottom/2);
            }
        }else{
            if(fragmentPair[i].posA+fragmentLengthBottom/2-fragmentLengthTop>=0 ){
                stringA2 = stringA.substr(fragmentPair[i].posA+fragmentLengthBottom/2-fragmentLengthTop,stringA.length()-fragmentPair[i].posA-fragmentLengthBottom/2+fragmentLengthTop);
            }else{
                stringA2 = stringA;
            }
        }
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
        vector <string> res = GetCommonFragmentEx(stringA2,stringB2,fragmentLengthBottom,fragmentLengthTop);
        for(int j=0;j<res.size();j++){
            stringResult.push_back(res[j]);
        }
    }
    return stringResult;

}

vector <string> DaemonWorker::GetCommonFragmentEx(string stringA, string stringB,int fragmentLengthBottom,int fragmentLengthTop){
    vector <string> result;
    for(int j=fragmentLengthBottom;j<=fragmentLengthTop;j++){
        for(int i=0;i<stringA.length()-j+1;i++){
            string temp_str = stringA.substr(i,j);
            if(stringB.find(temp_str)!=string::npos){
                result.push_back(temp_str);
            }else{
                continue;
            }
        }
    }
    return result;
}
