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
#include "daemontask.h"
#include <QTime>
#include <QThread>
#include "QStandardItemModel"

string currentdoing;
float totalwork;
float workdone;
Config config;
int timer;
vector <CommonFragment> CommonFragments;
vector <GenusPrimerPair> genusPrimerPairRegular;
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
    //ui->listWidget->addItem(QString::fromStdString(" "));

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
    DaemonTask *t=new DaemonTask();
    //QThread *t;
    //tx->moveToThread(t);
    QObject::connect(t, SIGNAL(signal_complete_proccess(QString)), this, SLOT(set_complete_proccess(QString)));
    QObject::connect(t, SIGNAL(signal_status(QString)), this, SLOT(set_status(QString)));

    t->start();

}



void MainWindow::RenderFragmentList(vector<CommonFragment> &commonFragment){
    //ui->listWidget->addItem(QString::fromStdString("test"));
    for(int i=0;i<commonFragment.size();i++){
        ui->listWidget->addItem(QString::fromStdString(commonFragment[i].name));
    }
    //QObject::connect(ui->listWidget, SIGNAL(currentTextChanged(const QString &)), this, SLOT(RenderFragmentListDetail(const QString &)));
    //ui->lineEdit->setText( ui->listWidget->currentItem()->text());
    //RenderFragmentListDetail(ui->listWidget->currentItem()->text());
}
void MainWindow::RenderFragmentListDetail(const QString &name){
    int i=0;
    for(i=0;i<CommonFragments.size();i++){
        if(name==QString::fromStdString( CommonFragments[i].name)){
            break;
        }
    }
    QStringList list1;
    for(int j=0;j<CommonFragments[i].fragments.size();j++){
        list1<<QString::fromStdString(CommonFragments[i].fragments[j]);
    }
    ui->listWidget_2->clear();
    ui->listWidget_2->addItems(list1);
}

void MainWindow::on_pushButton_3_clicked()
{

}

void MainWindow::on_listWidget_currentTextChanged(const QString &currentText)
{
    RenderFragmentListDetail(currentText);
}

void MainWindow::RenderRegularPairList(vector<GenusPrimerPair> &genusPrimerPair){
    for(int i=0;i<genusPrimerPair.size();i++){
        ui->listWidget_3->addItem(QString::fromStdString(genusPrimerPair[i].name));
    }
}
void MainWindow::RenderRegularPairListDetail(const QString &name){
    ui->tableView->setEditTriggers(QTableView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    QStandardItemModel *model = new QStandardItemModel;
     model->setColumnCount(6);
     model->setHeaderData(0,Qt::Horizontal,tr("引物1序列"));
     model->setHeaderData(1,Qt::Horizontal,tr("引物1头部位置"));
     model->setHeaderData(2,Qt::Horizontal,tr("引物1长度"));
     model->setHeaderData(3,Qt::Horizontal,tr("引物2序列"));
     model->setHeaderData(4,Qt::Horizontal,tr("引物2头部位置"));
     model->setHeaderData(5,Qt::Horizontal,tr("引物2长度"));


    int i=0;
    for(i=0;i<genusPrimerPairRegular.size();i++){
        if(name==QString::fromStdString(genusPrimerPairRegular[i].name)){
            break;
        }
    }
    for(int j=0;j<genusPrimerPairRegular[i].pairs.size();j++){
        stringstream newstr;
        string pos1,pos2,length1,length2;
        newstr<<genusPrimerPairRegular[i].pairs[j].pos1;
        newstr>>pos1;
        newstr.clear();
        newstr<<genusPrimerPairRegular[i].pairs[j].pos2;
        newstr>>pos2;
        newstr.clear();
        newstr<<genusPrimerPairRegular[i].pairs[j].length1;
        newstr>>length1;
        newstr.clear();
        newstr<<genusPrimerPairRegular[i].pairs[j].length2;
        newstr>>length2;
        newstr.clear();
        QStandardItem* item1 = new QStandardItem(tr(genusPrimerPairRegular[i].pairs[j].fragment1.c_str()));
        QStandardItem* item2 = new QStandardItem(tr(pos1.c_str()));
        QStandardItem* item3 = new QStandardItem(tr(length1.c_str()));
        QStandardItem* item4 = new QStandardItem(tr(genusPrimerPairRegular[i].pairs[j].fragment2.c_str()));
        QStandardItem* item5 = new QStandardItem(tr(pos2.c_str()));
        QStandardItem* item6 = new QStandardItem(tr(length2.c_str()));
        QList<QStandardItem*> item;
         item << item1 << item2 << item3 << item4 << item5 << item6;
         model->appendRow(item);
    }
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();

}

void MainWindow::on_listWidget_3_currentTextChanged(const QString &currentText)
{
    RenderRegularPairListDetail(currentText);
}

void MainWindow::set_complete_proccess(QString message){
    string s = message.toStdString();
    stringstream newstr;
    /*
    for(int i=0;i<CommonFragments.size();i++){
        vector <string> tempString = CommonFragments[i].fragments;
        s+=CommonFragments[i].name+"\n";
        for(int j=0;j<tempString.size();j++){
            s+=tempString[j]+"\n";
        }
    }
    */

    //ui->lineEdit->setText(QString::fromStdString("Success"));

    ui->plainTextEdit->setPlainText(QString::fromStdString(s));
    QApplication::processEvents();
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
        if(!temp->exists(QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/result")))){
            temp->mkdir(QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/result")));
        }
        for(int i=0;i<CommonFragments.size();i++){
            ofstream ofile;
            QString filename = QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/filtered/"+ CommonFragments[i].name.substr(0,CommonFragments[i].name.length()-3)));
            ofile.open(filename.toStdString().c_str());
            string tempstr;
            for(int j=0;j<CommonFragments[i].fragments.size();j++){
                tempstr+=CommonFragments[i].fragments[j]+"\n";
            }
            ofile << tempstr;
            ofile.close();
        }

        for(int i=0;i<genusPrimerPairRegular.size();i++){
            ofstream ofile;
            QString filename = QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/paired/"+ genusPrimerPairRegular[i].name.substr(0,genusPrimerPairRegular[i].name.length()-3)));
            ofile.open(filename.toStdString().c_str());
            string tempstr2;
            GenusPrimerPair tempPair = genusPrimerPairRegular[i];
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
        for(int i=0;i<genusPrimerPairRegular.size();i++){
            ofstream ofile;
            QString filename = QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/result/"+ genusPrimerPairRegular[i].name.substr(0,genusPrimerPairRegular[i].name.length()-3)));
            ofile.open(filename.toStdString().c_str());
            string tempstr2;
            GenusPrimerPair tempPair = genusPrimerPairRegular[i];
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
        //int mmm=genusPrimerPairRegularx.size();
        RenderFragmentList(CommonFragments);
        RenderRegularPairList(genusPrimerPairRegular);
}

void MainWindow::set_status(QString message){
    ui->plainTextEdit->setPlainText(QString::fromStdString("\n")+message+ui->plainTextEdit->toPlainText());
}
