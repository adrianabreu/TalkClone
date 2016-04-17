#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;

}

bool validateIP(Ui::SettingsDialog *ui);

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();


public slots:
    void done(int r);

private slots:
    void on_browseButton_clicked();

    void on_serverModeCheckBox_toggled(bool checked);

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
