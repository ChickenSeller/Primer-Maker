#include "dialogsettingproperties.h"
#include "ui_dialogsettingproperties.h"
#include "qfiledialog.h"
#include "datadef.h"
#include <sstream>

DialogSettingProperties::DialogSettingProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettingProperties)
{
    ui->setupUi(this);
    getConfig();
}

DialogSettingProperties::~DialogSettingProperties()
{
    delete ui;
}

void DialogSettingProperties::on_pushButton_3_clicked()
{
    config.fragmentCoverage = ui->lineEdit_3->text().toInt();
    config.fragmentLengthBottom = ui->lineEdit_4->text().toInt();
    config.fragmentLengthTop = ui->lineEdit_5->text().toInt();
    config.fragmentInaccuracy = ui->lineEdit_6->text().toInt();
    config.productLengthBottom = ui->lineEdit_7->text().toInt();
    config.productLengthTop = ui->lineEdit_9->text().toInt();
    config.paddingLength = ui->lineEdit_8->text().toInt();
    config.sourceGenus = ui->lineEdit_2->text().toStdString();
    config.targetGenus = ui->lineEdit->text().toStdString();
    accept();
}

void DialogSettingProperties::on_pushButton_clicked()
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

void DialogSettingProperties::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getExistingDirectory(this,"模板文件夹",QDir::currentPath());
    if(!filename.isNull()){
        ui->lineEdit_2->setText(filename);
    }

}
void DialogSettingProperties::getConfig(){
    ui->lineEdit->setText(QString::fromStdString(config.targetGenus));
    ui->lineEdit_2->setText(QString::fromStdString(config.sourceGenus));
    std::stringstream newstr;
    newstr<<config.fragmentCoverage;
    string tempInt = "";
    newstr>>tempInt;
    ui->lineEdit_3->setText(QString::fromStdString(tempInt));
    newstr.clear();
    newstr<<config.fragmentLengthBottom;
    newstr>>tempInt;
    ui->lineEdit_4->setText(QString::fromStdString(tempInt));
    newstr.clear();
    newstr<<config.fragmentLengthTop;
    newstr>>tempInt;
    ui->lineEdit_5->setText(QString::fromStdString(tempInt));
    newstr.clear();
    newstr<<config.fragmentInaccuracy;
    newstr>>tempInt;
    ui->lineEdit_6->setText(QString::fromStdString(tempInt));
    newstr.clear();
    newstr<<config.productLengthBottom;
    newstr>>tempInt;
    ui->lineEdit_7->setText(QString::fromStdString(tempInt));
    newstr.clear();
    newstr<<config.paddingLength;
    newstr>>tempInt;
    ui->lineEdit_8->setText(QString::fromStdString(tempInt));
    newstr.clear();
    newstr<<config.productLengthTop;
    newstr>>tempInt;
    ui->lineEdit_9->setText(QString::fromStdString(tempInt));
}
