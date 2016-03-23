#ifndef DIALOGSETTINGPROPERTIES_H
#define DIALOGSETTINGPROPERTIES_H

#include <QDialog>

namespace Ui {
class DialogSettingProperties;
}

class DialogSettingProperties : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettingProperties(QWidget *parent = 0);
    ~DialogSettingProperties();
    void getConfig();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:
    Ui::DialogSettingProperties *ui;
};

#endif // DIALOGSETTINGPROPERTIES_H
