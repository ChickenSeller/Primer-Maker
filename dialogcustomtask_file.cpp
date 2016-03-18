#include "dialogcustomtask_file.h"
#include "ui_dialogcustomtask_file.h"

DialogCustomTask_File::DialogCustomTask_File(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustomTask_File)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() &~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
}

DialogCustomTask_File::~DialogCustomTask_File()
{
    delete ui;
}
