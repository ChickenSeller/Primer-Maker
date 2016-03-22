#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "datadef.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <fstream>
#include <QApplication>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_4_triggered()
{
    Form_Setting_Properties = new DialogSettingProperties();
    Form_Setting_Properties->exec();
}

void MainWindow::on_action_triggered()
{
    Form_Rapid_Task = new DialogRapidTask();
    Form_Rapid_Task->exec();
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
    ui->lineEdit->setText(configPath);
    ofile.open(configPath.toStdString());
    ofile << json_str.toStdString();
    ofile.close();
}
