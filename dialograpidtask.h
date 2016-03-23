#ifndef DIALOGRAPIDTASK_H
#define DIALOGRAPIDTASK_H

#include <QDialog>

namespace Ui {
class DialogRapidTask;
}

class DialogRapidTask : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRapidTask(QWidget *parent = 0);
    ~DialogRapidTask();
    void getConfig();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogRapidTask *ui;
};

#endif // DIALOGRAPIDTASK_H
