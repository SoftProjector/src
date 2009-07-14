#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H


#include <QtGui/QDialog>
#include "softprojector.h"

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SettingsDialog)
public:
    explicit SettingsDialog(QWidget *parent = 0);
    virtual ~SettingsDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SettingsDialog *ui;
    SoftProjector *softProjector;

private slots:
    void on_change_background_button_clicked();
    void on_change_font_button_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

};

#endif // SETTINGSDIALOG_H
