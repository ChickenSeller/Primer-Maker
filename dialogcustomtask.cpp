#include "dialogcustomtask.h"
#include "ui_dialogcustomtask.h"

DialogCustomTask::DialogCustomTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustomTask)
{
    ui->setupUi(this);
}

DialogCustomTask::~DialogCustomTask()
{
    delete ui;
}
