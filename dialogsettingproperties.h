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

private slots:
    void on_pushButton_3_clicked();

private:
    Ui::DialogSettingProperties *ui;
};

#endif // DIALOGSETTINGPROPERTIES_H
