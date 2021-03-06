#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include<iostream>
#include<string>
#include<QJsonObject>
#include<vector>

using namespace std;

class Config{
public:
    string targetGenus;
    string sourceGenus;
    int fragmentLengthBottom;
    int fragmentLengthTop;
    int fragmentInaccuracy;
    int fragmentCoverage;
    int productLengthBottom;
    int productLengthTop;
    int paddingLength;
    Config(){
        targetGenus = "";
        sourceGenus = "";
        fragmentCoverage = 80;
        fragmentInaccuracy = 3;
        fragmentLengthBottom = 20;
        fragmentLengthTop = 25;
        productLengthBottom = 200;
        productLengthTop = 300;
        paddingLength = 25;
    }
};
class JsonConvert{
public:
    static QJsonObject ConfigToJson(Config config){
        QJsonObject json;
        json.insert(QString::fromStdString("targetGenus"),QString::fromStdString(config.targetGenus));
        json.insert(QString::fromStdString("sourceGenus"),QString::fromStdString(config.sourceGenus));
        json.insert(QString::fromStdString("fragmentLengthBottom"),config.fragmentLengthBottom);
        json.insert(QString::fromStdString("fragmentLengthTop"),config.fragmentLengthTop);
        json.insert(QString::fromStdString("fragmentInaccuracy"),config.fragmentInaccuracy);
        json.insert(QString::fromStdString("fragmentCoverage"),config.fragmentCoverage);
        json.insert(QString::fromStdString("productLengthBottom"),config.productLengthBottom);
        json.insert(QString::fromStdString("productLengthTop"),config.productLengthTop);
        json.insert(QString::fromStdString("paddingLength"),config.paddingLength);
        return json;
    }
};
class Species{
public:
    std::string name;
    std::string fragment;
};
class Genus{
public:
    std::string name;
    vector <Species> species;
};
class GenusCollection{
public:
    vector <Genus> genus;
};
class FragmentPair{
public:
    string fragment;
    int posA;
    int posB;
    bool operator <(const FragmentPair &a){
        return posA<a.posA;
    }
};
class GenusList{
public:
    vector <Species> genus;
};
class TargetGenusList{
public:
    vector <string> genus;
};
class SimpleGenusCollection{
public:
    vector <Species> genus;
};
class CommonFragment{
public:
    string name;
    vector <string> fragments;
};
class PairInfo{
public:
    int pos1;
    int length1;
    string fragment1;
    int pos2;
    int length2;
    string fragment2;
    int totallength;
    PairInfo(int p1,int l1,int p2,int l2,string str1,string str2,int tl){
            pos1 = p1;
            length1 = l1;
            pos2 = p2;
            length2 = l2;
            fragment1 = str1;
            fragment2 = str2;
            totallength = tl;
        }
};
class PrimerPair{
public:
    string primerA;
    int posA;
    string primerB;
    int posB;
};
class GenusPrimerPair{
public:
    string name;
    vector <PairInfo> pairs;
};
class GenusNotPaired{
public:
    string name;
    int num;
};

extern int timer;
extern Config config;
extern vector <CommonFragment> CommonFragments;
extern vector <GenusPrimerPair> genusPrimerPairRegular;
extern vector <GenusPrimerPair> genusPrimerPairExt;
#endif // DATASTRUCT_H





