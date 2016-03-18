#include "dialogcustomtask_pair.h"
#include "ui_dialogcustomtask_pair.h"

DialogCustomTask_Pair::DialogCustomTask_Pair(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustomTask_Pair)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() &~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
}

DialogCustomTask_Pair::~DialogCustomTask_Pair()
{
    delete ui;
}
