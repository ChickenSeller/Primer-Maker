#include "dialograpidtask.h"
#include "ui_dialograpidtask.h"
#include "qfiledialog.h"
#include "datadef.h"
#include <sstream>

DialogRapidTask::DialogRapidTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRapidTask)
{
    ui->setupUi(this);
    getConfig();
}

DialogRapidTask::~DialogRapidTask()
{
    delete ui;
}

void DialogRapidTask::on_pushButton_clicked()
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

void DialogRapidTask::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getExistingDirectory(this,"模板文件夹",QDir::currentPath());
    if(!filename.isNull()){
        ui->lineEdit_2->setText(filename);
    }
}
void DialogRapidTask::getConfig(){
    stringstream newstr;
    string tempInt,tempInt2;
    newstr<<config.fragmentCoverage;
    newstr>>tempInt;
    ui->label_4->setText(QString::fromStdString(tempInt+"%"));
    newstr.clear();
    newstr<<config.fragmentInaccuracy;
    newstr>>tempInt;
    ui->label_8->setText(QString::fromStdString(tempInt));
    newstr.clear();
    newstr<<config.fragmentLengthBottom;
    newstr>>tempInt;
    newstr.clear();
    newstr<<config.fragmentLengthTop;
    newstr>>tempInt2;
    ui->label_6->setText(QString::fromStdString(tempInt+"-"+tempInt2));
    newstr.clear();
    newstr<<config.productLengthBottom;
    newstr>>tempInt;
    newstr.clear();
    newstr<<config.productLengthTop;
    newstr>>tempInt2;
    ui->label_10->setText(QString::fromStdString(tempInt+"-"+tempInt2));
    newstr.clear();
    newstr<<config.paddingLength;
    newstr>>tempInt;
    ui->label_12->setText(QString::fromStdString(tempInt));
    ui->lineEdit->setText(QString::fromStdString(config.targetGenus));
    ui->lineEdit_2->setText(QString::fromStdString(config.sourceGenus));

}

void DialogRapidTask::on_pushButton_3_clicked()
{
    config.sourceGenus = ui->lineEdit_2->text().toStdString();
    config.targetGenus = ui->lineEdit->text().toStdString();
    accept();
}
