/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2010  Vladislav Kobzar and Matvey Adzhigirey
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

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
    QString new_wallpaper_path;
    QString new_passive_wallpaper_path;
    QStringList bible_id_list;
    QStringList second_id_list;

private slots:
    void on_remove_passive_wallpaper_button_clicked();
    void on_primary_bible_menu_activated(QString );
    void on_remove_wallpaper_button_clicked();
    void on_set_passive_wallpaper_clicked();
    void on_set_wallpaper_button_clicked();
    void on_change_font_button_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // SETTINGSDIALOG_H
