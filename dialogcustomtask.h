#ifndef DIALOGCUSTOMTASK_H
#define DIALOGCUSTOMTASK_H

#include <QDialog>

namespace Ui {
class DialogCustomTask;
}

class DialogCustomTask : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCustomTask(QWidget *parent = 0);
    ~DialogCustomTask();

private:
    Ui::DialogCustomTask *ui;
};

#endif // DIALOGCUSTOMTASK_H
