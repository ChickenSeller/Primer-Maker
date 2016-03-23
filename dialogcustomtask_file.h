#ifndef DIALOGCUSTOMTASK_FILE_H
#define DIALOGCUSTOMTASK_FILE_H

#include <QDialog>

namespace Ui {
class DialogCustomTask_File;
}

class DialogCustomTask_File : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCustomTask_File(QWidget *parent = 0);
    ~DialogCustomTask_File();
    void getConfig();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::DialogCustomTask_File *ui;
};

#endif // DIALOGCUSTOMTASK_FILE_H
