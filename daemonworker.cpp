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
