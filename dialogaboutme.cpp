#include "dialogaboutme.h"
#include "ui_dialogaboutme.h"
#include "datadef.h"
//extern Config config;
DialogAboutMe::DialogAboutMe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAboutMe)
{
    ui->setupUi(this);
    //ui->label_3->setText(QString::fromStdString(config.sourceGenus));
}

DialogAboutMe::~DialogAboutMe()
{
    delete ui;
}

void DialogAboutMe::on_pushButton_clicked()
{
    accept();
}
