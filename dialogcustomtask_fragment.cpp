#include "dialogcustomtask_fragment.h"
#include "ui_dialogcustomtask_fragment.h"

DialogCustomTask_Fragment::DialogCustomTask_Fragment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustomTask_Fragment)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() &~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
}

DialogCustomTask_Fragment::~DialogCustomTask_Fragment()
{
    delete ui;
}
