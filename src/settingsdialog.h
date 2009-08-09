#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H


#include <QtGui/QDialog>
#include <QtGui/QFileDialog>
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
    QFont new_font;
    QString new_wallpaper_path;
    QStringList bible_id_list;

private slots:
    void on_remove_wallpaper_button_clicked();
    void on_set_wallpaper_button_clicked();
    void on_change_font_button_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

};

#endif // SETTINGSDIALOG_H
