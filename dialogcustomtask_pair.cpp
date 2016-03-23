#include "dialogcustomtask_pair.h"
#include "ui_dialogcustomtask_pair.h"
#include "datadef.h"
#include <sstream>

DialogCustomTask_Pair::DialogCustomTask_Pair(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustomTask_Pair)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() &~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
    getConfig();
}

DialogCustomTask_Pair::~DialogCustomTask_Pair()
{
    delete ui;
}

void DialogCustomTask_Pair::on_pushButton_clicked()
{
    config.productLengthBottom = ui->lineEdit_7->text().toInt();
    config.productLengthTop = ui->lineEdit_9->text().toInt();
    config.paddingLength = ui->lineEdit_8->text().toInt();
    accept();
}
void DialogCustomTask_Pair::getConfig(){
    stringstream newstr;
    string tempInt;
    newstr<<config.productLengthBottom;
    newstr>>tempInt;
    newstr.clear();
    ui->lineEdit_7->setText(QString::fromStdString(tempInt));
    newstr<<config.productLengthTop;
    newstr>>tempInt;
    newstr.clear();
    ui->lineEdit_9->setText(QString::fromStdString(tempInt));
    newstr<<config.paddingLength;
    newstr>>tempInt;
    newstr.clear();
    ui->lineEdit_8->setText(QString::fromStdString(tempInt));
}
