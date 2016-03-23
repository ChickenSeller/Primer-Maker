#include "dialogcustomtask_file.h"
#include "ui_dialogcustomtask_file.h"
#include "qfiledialog.h"
#include "datadef.h"

DialogCustomTask_File::DialogCustomTask_File(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustomTask_File)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() &~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
    getConfig();
}

DialogCustomTask_File::~DialogCustomTask_File()
{
    delete ui;
}
void DialogCustomTask_File::getConfig(){
    ui->lineEdit->setText(QString::fromStdString(config.targetGenus));
    ui->lineEdit_2->setText(QDir::currentPath());
}

void DialogCustomTask_File::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
       this,
       "目标属目录文件",
       QDir::currentPath(),
       "Document files (All files(*.*)");
    if(!filename.isNull()){
        ui->lineEdit->setText(filename);
    }
}

void DialogCustomTask_File::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getExistingDirectory(this,"模板文件夹",QDir::currentPath());
    if(!filename.isNull()){
        ui->lineEdit_2->setText(filename);
    }
}

void DialogCustomTask_File::on_pushButton_3_clicked()
{
    config.sourceGenus = ui->lineEdit_2->text().toStdString();
    config.targetGenus = ui->lineEdit->text().toStdString();
    accept();
}
