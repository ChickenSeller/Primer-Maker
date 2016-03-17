#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dialogsettingproperties.h"
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



private slots:
    void on_action_4_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
