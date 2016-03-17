#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    Form_Custom_Task_File->exec();
    std::string str = "";
    if(Form_Custom_Task_File->Accepted){
        Form_Custom_Task_Fragment = new DialogCustomTask_Fragment();
        Form_Custom_Task_Fragment->exec();
    }
}
