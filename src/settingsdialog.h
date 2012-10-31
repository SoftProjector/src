/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2011  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

#include "settings.h"
#include "generalsettingwidget.h"
#include "biblesettingwidget.h"
#include "songsettingwidget.h"
#include "announcementsettingwidget.h"

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

    int current_display_screen;
    bool is_always_on_top;

    Settings allSettings;
    GeneralSettingWidget *generalSettingswidget;
    BibleSettingWidget *bibleSettingswidget;
    SongSettingWidget *songSettingswidget;
    AnnouncementSettingWidget *announcementSettingswidget;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_listWidget_currentRowChanged(int currentRow);

public slots:
    void loadSettings(Settings& sets);
};

#endif // SETTINGSDIALOG_H
