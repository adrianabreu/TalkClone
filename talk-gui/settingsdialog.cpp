#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QtGlobal>

#include <arpa/inet.h>
#include <string>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    QSettings settings;
    ui->talkPathEdit->setText(settings.value("talkPath").toString());
    ui->serverModeCheckBox->setChecked(settings.value("serverMode").toBool());
    ui->serverAddressEdit->setText(settings.value("serverAddress","127.0.0.1").toString());
    ui->portBox->setValue(settings.value("port",8000).toInt());

    QByteArray username = qgetenv("USER");
    if (! username.isNull())
        ui->usernameEdit->setText(settings.value("username",username).toString());
    else
        ui->usernameEdit->setText(settings.value("username","").toString());

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::done(int r)
{
    if (r == QDialog::Accepted) {
        if(ui->talkPathEdit->text().isEmpty()) {
            QMessageBox::critical(this,"Talk GUI",
                                  "Talk path can not be empty.");
            return;
        } else if(!ui->serverModeCheckBox->isChecked() &&
                  !validateIP(ui) ){
            QMessageBox::critical(this,"Talk GUI",
                                  "You have to specify a valid IP address");
            return;
        } else {
            QFileInfo info1(ui->talkPathEdit->text());
            if (!info1.isExecutable()) {
                QMessageBox::critical(this,"Talk GUI",
                                      "Selected file is not executable.");
                return;
            } else {
                QSettings settings;
                settings.setValue("talkPath",
                                  ui->talkPathEdit->text());
                settings.setValue("serverMode",
                                  ui->serverModeCheckBox->isChecked());
                settings.setValue("serverAddress",
                                  ui->serverAddressEdit->text());
                settings.setValue("port",ui->portBox->value());
                if(ui->usernameEdit->text().isEmpty())
                    settings.remove("username");
                else
                    settings.setValue("username",ui->usernameEdit->text());
                QDialog::done(r);
                return;
            }
        }
    }
    else QDialog::done(r);
}

void SettingsDialog::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    //TODO verify is the file is executable
    if (!fileName.isNull())
        ui->talkPathEdit->setText(fileName);
}

void SettingsDialog::on_serverModeCheckBox_toggled(bool checked)
{
    ui->serverAddressEdit->setDisabled(checked);
    ui->serverAddressLabel->setDisabled(checked);
}


bool validateIP(Ui::SettingsDialog *ui)
{
    struct sockaddr_in sa;

    std::string ipAddress(ui->serverAddressEdit->text().toStdString());

    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    //pton returns 0 if ip isn't valid
    return (result != 0) ? true : false;
}
