#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dialogsettingproperties.h"
#include "dialogcustomtask_file.h"
#include "dialogcustomtask_fragment.h"
#include "dialogcustomtask_pair.h"
#include "dialograpidtask.h"
#include "dialogaboutme.h"
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
    DialogCustomTask_Pair *Form_Custom_Task_Pair;
    DialogAboutMe *About_Me;
    void SaveConfig();
    void LoadConfig();
    void test();




private slots:
    void on_action_4_triggered();

    void on_action_triggered();

    void on_action_2_triggered();

    void on_action_3_triggered();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
