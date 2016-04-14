#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
//#include "datadef.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <fstream>
#include <sstream>
#include <QApplication>
#include <QDir>
#include "qmessagebox.h"
#include "daemonworker.h"
#include <QTime>

Config config;
int timer;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //config = new Config;
    LoadConfig();
    //config.sourceGenus = "text";
    //ui->lineEdit->setText(QString::fromStdString(config.sourceGenus));
    DaemonWorker worker1;


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_4_triggered()
{
    Form_Setting_Properties = new DialogSettingProperties();
    LoadConfig();
    if(Form_Setting_Properties->exec() == QDialog::Accepted){
        SaveConfig();
    }
}

void MainWindow::on_action_triggered()
{
    Form_Rapid_Task = new DialogRapidTask();
    if(Form_Rapid_Task->exec() == QDialog::Accepted){
        //DaemonWorker worker;
        test();
        stringstream newstr;
        string tempInt;
        newstr<<timer;
        newstr>>tempInt;
        ui->plainTextEdit->setPlainText(QString::fromStdString(tempInt+"\n")+ui->plainTextEdit->toPlainText());
    }

}

void MainWindow::on_action_2_triggered()
{
    Form_Custom_Task_File = new DialogCustomTask_File();
    if(Form_Custom_Task_File->exec() == QDialog::Accepted){
        Form_Custom_Task_Fragment = new DialogCustomTask_Fragment();
        if(Form_Custom_Task_Fragment->exec() == QDialog::Accepted){
            Form_Custom_Task_Pair = new DialogCustomTask_Pair();
            if(Form_Custom_Task_Pair->exec() == QDialog::Accepted){
                SaveConfig();
            }
        }

    }
}

void MainWindow::on_action_3_triggered()
{
    About_Me = new DialogAboutMe();
    About_Me->exec();
}
void MainWindow::SaveConfig(){
    QJsonObject json = JsonConvert::ConfigToJson(config);
    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    ofstream ofile;
    QString configPath = QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/config.json"));
    ofile.open(configPath.toStdString().c_str());
    ofile << json_str.toStdString();
    ofile.close();
}

void MainWindow::LoadConfig(){
    ifstream ifile;
    QString configPath = QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/config.json"));
    ifile.open(configPath.toStdString().c_str());
    string json;
    ifile>>json;
    ifile.close();
    QJsonParseError err;
    QJsonDocument document = QJsonDocument::fromJson(QByteArray::fromStdString(json),&err);
    if(err.error == QJsonParseError::NoError){
        if(document.isObject()){
            QJsonObject json_obj = document.object();
            config.fragmentCoverage = json_obj.take("fragmentCoverage").toInt();
            config.fragmentInaccuracy = json_obj.take("fragmentInaccuracy").toInt();
            config.fragmentLengthBottom = json_obj.take("fragmentLengthBottom").toInt();
            config.fragmentLengthTop = json_obj.take("fragmentLengthTop").toInt();
            config.paddingLength = json_obj.take("paddingLength").toInt();
            config.productLengthBottom = json_obj.take("productLengthBottom").toInt();
            config.productLengthTop = json_obj.take("productLengthTop").toInt();
            config.sourceGenus = json_obj.take("sourceGenus").toString().toStdString();
            config.targetGenus = json_obj.take("targetGenus").toString().toStdString();
        }else{
            config.fragmentCoverage = 70;
            config.fragmentInaccuracy = 3;
            config.fragmentLengthBottom = 20;
            config.fragmentLengthTop = 25;
            config.paddingLength = 25;
            config.productLengthBottom = 200;
            config.productLengthTop = 300;
            config.sourceGenus = "";
            config.targetGenus = "";
        }
    }else{
        QMessageBox::StandardButton reply;
            reply = QMessageBox::critical(this, tr("配置加载错误"),
                                            "配置文件无法正确加载,改用默认配置",
                                            QMessageBox::Abort);
            config.fragmentCoverage = 70;
            config.fragmentInaccuracy = 3;
            config.fragmentLengthBottom = 20;
            config.fragmentLengthTop = 25;
            config.paddingLength = 25;
            config.productLengthBottom = 200;
            config.productLengthTop = 300;
            config.sourceGenus = "";
            config.targetGenus = "";
    }


    ui->lineEdit->setText(QString::fromStdString(json));
}

void MainWindow::test(){
    QTime time;
    time.start();
    QDir *dir = new QDir(QString::fromStdString(config.sourceGenus));
    DaemonWorker worker;
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
    GenusCollection collection = worker.LoadTargetGenus(config.sourceGenus,config.targetGenus);
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
    ui->lineEdit->setText(QString::fromStdString("Computing"));
    QApplication::processEvents();
    //ui->plainTextEdit->setPlainText(QString::fromStdString(currentGenus.species[3].name+"\n"+currentGenus.species[3].fragment));
    //SimpleGenusCollection sourceGnenus = worker.LoadSimpleSourceGenus(config.sourceGenus);
    //ui->plainTextEdit->setPlainText(QString::fromStdString(sourceGnenus.genus[1].name+"\n"+sourceGnenus.genus[1].fragment));
    SimpleGenusCollection x = worker.LoadSimpleSourceGenus(config.sourceGenus);
    vector <CommonFragment> CommonFragment = worker.GetCommonFragmentFromGenus(collection,x,config.fragmentCoverage);
    //vector <string> CommonFragment = worker.GetCommonFragmentFromSpecificGenus(currentGenus,x,config.fragmentCoverage); //GetCommonFragment(currentSpecies.fragment,currentGenus.species[4].fragment,config.fragmentLengthBottom,config.fragmentLengthTop);
    ui->lineEdit->setText(QString::fromStdString("Rendering"));
    long t =time.elapsed();
    newstr<<t;
    newstr>>tempInt;
    newstr.clear();
    ui->lineEdit->setText(QString::fromStdString(tempInt+"ms"));
    QApplication::processEvents();

    for(int i=0;i<CommonFragment.size();i++){
        vector <string> tempString = CommonFragment[i].fragments;
        s+=CommonFragment[i].name+"\n";
        for(int j=0;j<tempString.size();j++){
            s+=tempString[j]+"\n";
        }
    }

    //ui->lineEdit->setText(QString::fromStdString("Success"));

    ui->plainTextEdit->setPlainText(QString::fromStdString(s));
    QApplication::processEvents();
    newstr<<CommonFragment.size();
    newstr>>tempInt;
    QTime time2;
    time2.start();
    vector <GenusPrimerPair> xxx = worker.MakePair(collection,CommonFragment,config.productLengthBottom,config.productLengthTop);
    int num = xxx.size();
    long t2 = time2.elapsed();
    newstr<<t2;
    newstr>>tempInt;
    newstr.clear();
    ui->lineEdit->setText(ui->lineEdit->text()+QString::fromStdString(" | "+tempInt+"ms"));

    //worker.GetFragmentPosFromSpecificGenus()
    //ui->plainTextEdit->setPlainText(QString::fromStdString(currentSpecies.fragment+"\n\n"+currentGenus.species[2].fragment));

    QDir *temp = new QDir;
        bool exist = temp->exists(QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/filtered")));
        if(exist){

        }else
        {
            bool ok = temp->mkdir(QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/filtered")));
        }
        if(!temp->exists(QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/paired")))){
            temp->mkdir(QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/paired")));
        }
        for(int i=0;i<CommonFragment.size();i++){
            ofstream ofile;
            QString filename = QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/filtered/"+ CommonFragment[i].name.substr(0,CommonFragment[i].name.length()-3)));
            ofile.open(filename.toStdString().c_str());
            string tempstr;
            for(int j=0;j<CommonFragment[i].fragments.size();j++){
                tempstr+=CommonFragment[i].fragments[j]+"\n";
            }
            ofile << tempstr;
            ofile.close();
        }
        for(int i=0;i<xxx.size();i++){
            ofstream ofile;
            QString filename = QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/paired/"+ xxx[i].name.substr(0,xxx[i].name.length()-3)));
            ofile.open(filename.toStdString().c_str());
            string tempstr2;
            GenusPrimerPair tempPair = xxx[i];
            for(int j=0;j<tempPair.pairs.size();j++){
                string pos1,pos2,length1,length2;
                newstr<<tempPair.pairs[j].pos1;
                newstr>>pos1;
                newstr.clear();
                newstr<<tempPair.pairs[j].pos2;
                newstr>>pos2;
                newstr.clear();
                newstr<<tempPair.pairs[j].length1;
                newstr>>length1;
                newstr.clear();
                newstr<<tempPair.pairs[j].length2;
                newstr>>length2;
                newstr.clear();
                tempstr2+=tempPair.pairs[j].fragment1+"\t\t"+pos1+"\t\t"
                        +length1+"\t\t"+tempPair.pairs[j].fragment2+"\t\t"
                        +pos2+"\t\t"+length2+"\n";
            }
            ofile << tempstr2;
            ofile.close();
        }
        vector <GenusPrimerPair> xxxx = worker.FilterFragment(xxx);
        int mmm=xxxx.size();

}


void MainWindow::on_pushButton_3_clicked()
{



}
