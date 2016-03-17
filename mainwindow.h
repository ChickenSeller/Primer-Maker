#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dialogsettingproperties.h"
#include "dialogcustomtask.h"
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
    DialogCustomTask *Form_Custom_Task;



private slots:
    void on_action_4_triggered();

    void on_action_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
