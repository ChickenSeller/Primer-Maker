#ifndef DIALOGCUSTOMTASK_FRAGMENT_H
#define DIALOGCUSTOMTASK_FRAGMENT_H

#include <QDialog>

namespace Ui {
class DialogCustomTask_Fragment;
}

class DialogCustomTask_Fragment : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCustomTask_Fragment(QWidget *parent = 0);
    ~DialogCustomTask_Fragment();
    void getConfig();


private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogCustomTask_Fragment *ui;
};

#endif // DIALOGCUSTOMTASK_FRAGMENT_H
