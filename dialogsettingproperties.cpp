#include "dialogsettingproperties.h"
#include "ui_dialogsettingproperties.h"

DialogSettingProperties::DialogSettingProperties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettingProperties)
{
    ui->setupUi(this);
}

DialogSettingProperties::~DialogSettingProperties()
{
    delete ui;
}

void DialogSettingProperties::on_pushButton_3_clicked()
{

}
