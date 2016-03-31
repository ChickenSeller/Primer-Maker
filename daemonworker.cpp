#include "daemonworker.h"
#include <QDir>
#include <QDebug>

DaemonWorker::DaemonWorker()
{

}

GenusCollection DaemonWorker::LoadTargetGenus(string filePath,string targetFileName){
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

void DaemonWorker::trimStr(string &str){
    string buff(str);
    char space = ' ';
    str.assign(buff.begin() + buff.find_first_not_of(space),
    buff.begin() + buff.find_last_not_of(space) + 1);
}

//vector <string> DaemonWorker

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

vector <string> DaemonWorker::Unique(vector<string> rawString){
    vector<string>::iterator newIterater;
    sort(rawString.begin(),rawString.end());
    newIterater = unique(rawString.begin(),rawString.end());
    rawString.erase(newIterater,rawString.end());
    return rawString;
}

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

int DaemonWorker::GetCoverage(string fragment, Genus genus){
    int hit=0;
    for(int i=0;i<genus.species.size();i++){
        if(genus.species[i].fragment.find(fragment)!=string::npos){
            hit++;
            //timer++;
        }
    }
    //timer = hit*100/genus.species.size();
    return hit*100/genus.species.size();
}

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

void DaemonWorker::test(){
    QDir *dir = new QDir(QString::fromStdString(config.sourceGenus));
    QStringList filter;
            //filter<<"*.dat";
            //dir->setNameFilters(filter);
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    stringstream newstr;
    newstr<<fileInfo->count();
    string tempInt;
    newstr>>tempInt;
    newstr.clear();
    //ui->lineEdit->setText(QString::fromStdString(tempInt));
    string s = "";
    GenusCollection collection = LoadTargetGenus(config.sourceGenus,config.targetGenus);
    /*
    for(int i=0;i<collection.genus.size();i++){
        Genus currentGenus = collection.genus[i];
        for(int j=0;j<currentGenus.species.size();j++){
            Species currentSpecies = currentGenus.species[j];
            stringstream newstr;
            string tempInt;
            newstr<<currentGenus.species.size();
            newstr>>tempInt;
            s += currentSpecies.name +"\n";
            //ui->plainTextEdit->setPlainText(QString::fromStdString(currentSpecies.name+ tempInt +"\n")+ui->plainTextEdit->toPlainText());
        }
    }
    */
    Genus currentGenus = collection.genus[2];
    newstr<<currentGenus.species.size();
    newstr>>tempInt;
    newstr.clear();
    //ui->plainTextEdit->setPlainText(QString::fromStdString(currentGenus.species[3].name+"\n"+currentGenus.species[3].fragment));
    //SimpleGenusCollection sourceGnenus = worker.LoadSimpleSourceGenus(config.sourceGenus);
    //ui->plainTextEdit->setPlainText(QString::fromStdString(sourceGnenus.genus[1].name+"\n"+sourceGnenus.genus[1].fragment));

    Species currentSpecies = currentGenus.species[3];
    vector <string> CommonFragment = GetCommonFragment(currentSpecies.fragment,currentGenus.species[4].fragment,config.fragmentLengthBottom,config.fragmentLengthTop);
    for(int i=0;i<CommonFragment.size();i++){
        s+=CommonFragment[i]+"\n";
        timer +=1;
        //ui->plainTextEdit->setPlainText(QString::fromStdString(CommonFragment[i]+"\n")+ui->plainTextEdit->toPlainText());
        //QApplication::processEvents();
    }
    //ui->plainTextEdit->setPlainText(QString::fromStdString(s));
    newstr<<CommonFragment.size();
    newstr>>tempInt;
    //ui->plainTextEdit->setPlainText(QString::fromStdString(currentSpecies.fragment+"\n\n"+currentGenus.species[2].fragment));

}
