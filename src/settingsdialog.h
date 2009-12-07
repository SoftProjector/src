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
    void updateSecondaryBibleMenu();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SettingsDialog *ui;
    SoftProjector *softProjector;
    QStringList bibles;
    QFont new_font;
    QString display_wallpaper_path, fill_wallpaper_path;
    int wallpaper_state; // 0 for blank, 1 for display, 2 for fill
    QStringList bible_id_list;
    QStringList second_id_list;

private slots:
    void on_fill_wallpaper_rbutton_toggled(bool checked);
    void on_fill_wallaper_pushButton_clicked();
    void on_primary_bible_menu_activated(QString );
    void on_remove_wallpaper_button_clicked();
    void on_set_wallpaper_button_clicked();
    void on_change_font_button_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

};

#endif // SETTINGSDIALOG_H
