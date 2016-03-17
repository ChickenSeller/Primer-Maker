#include "dialograpidtask.h"
#include "ui_dialograpidtask.h"

DialogRapidTask::DialogRapidTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRapidTask)
{
    ui->setupUi(this);
}

DialogRapidTask::~DialogRapidTask()
{
    delete ui;
}
