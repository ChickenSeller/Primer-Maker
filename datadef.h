
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
    vector <Species> species;
};
class GenusCollection{
public:
    vector <Genus> genus;
};
#ifndef DATASTRUCT_H
#define DATASTRUCT_H

extern Config config;
#endif // DATASTRUCT_H





