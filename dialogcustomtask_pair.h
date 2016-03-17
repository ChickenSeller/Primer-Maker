#ifndef DIALOGCUSTOMTASK_PAIR_H
#define DIALOGCUSTOMTASK_PAIR_H

#include <QDialog>

namespace Ui {
class DialogCustomTask_Pair;
}

class DialogCustomTask_Pair : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCustomTask_Pair(QWidget *parent = 0);
    ~DialogCustomTask_Pair();

private:
    Ui::DialogCustomTask_Pair *ui;
};

#endif // DIALOGCUSTOMTASK_PAIR_H
