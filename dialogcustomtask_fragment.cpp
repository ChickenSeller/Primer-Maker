#include "dialogcustomtask_fragment.h"
#include "ui_dialogcustomtask_fragment.h"
#include "datadef.h"
#include <sstream>

DialogCustomTask_Fragment::DialogCustomTask_Fragment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustomTask_Fragment)
{
    ui->setupUi(this);
    //隐藏容差选项
    ui->label_6->setVisible(false);
    ui->lineEdit_6->setVisible(false);
    setWindowFlags(windowFlags() &~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
    getConfig();
}

DialogCustomTask_Fragment::~DialogCustomTask_Fragment()
{
    delete ui;
}

void DialogCustomTask_Fragment::on_pushButton_clicked()
{
    config.fragmentCoverage = ui->lineEdit_3->text().toInt();
    config.fragmentLengthBottom = ui->lineEdit_4->text().toInt();
    config.fragmentLengthTop = ui->lineEdit_5->text().toInt();
    config.fragmentInaccuracy = ui->lineEdit_6->text().toInt();
    accept();
}
void DialogCustomTask_Fragment::getConfig(){
    stringstream newstr;
    string tempInt;
    newstr<<config.fragmentCoverage;
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
}
