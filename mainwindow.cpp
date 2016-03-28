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
        DaemonWorker worker;
        worker.test();
        stringstream newstr;
        string tempInt;
        newstr<<timer;
        newstr>>tempInt;
        ui->plainTextEdit->setPlainText(QString::fromStdString(tempInt));
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
                test();
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
void MainWindow::test(){
    Config MainConfig;
    QJsonObject json = JsonConvert::ConfigToJson(MainConfig);
    QJsonDocument document;
    document.setObject(json);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);
    ofstream ofile;
    QString configPath = QDir::toNativeSeparators(QDir::currentPath()+QString::fromStdString("/config.json"));
    config.sourceGenus = "modified";
    config.targetGenus = "test";
    ui->lineEdit->setText(QString::fromStdString(config.sourceGenus));
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
