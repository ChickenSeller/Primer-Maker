#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dialogsettingproperties.h"
#include "dialogcustomtask_file.h"
#include "dialogcustomtask_fragment.h"
#include "dialograpidtask.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    DialogSettingProperties *Form_Setting_Properties;
    DialogRapidTask *Form_Rapid_Task;
    DialogCustomTask_File *Form_Custom_Task_File;
    DialogCustomTask_Fragment *Form_Custom_Task_Fragment;



private slots:
    void on_action_4_triggered();

    void on_action_triggered();

    void on_action_2_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
