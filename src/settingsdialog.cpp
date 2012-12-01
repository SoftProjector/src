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

#include <QtSql>
#include <QMessageBox>
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "softprojector.h"


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    softProjector = (SoftProjector*)parent;
    generalSettingswidget = new GeneralSettingWidget;
    bibleSettingswidget = new BibleSettingWidget;
    songSettingswidget = new SongSettingWidget;
    announcementSettingswidget = new AnnouncementSettingWidget;

    ui->scrollAreaGeneralSettings->setWidget(generalSettingswidget);
    ui->scrollAreaBibleSettings->setWidget(bibleSettingswidget);
    ui->scrollAreaSongSettings->setWidget(songSettingswidget);
    ui->scrollAreaAnnouncementSettings->setWidget(announcementSettingswidget);

}

void SettingsDialog::loadSettings(Settings& sets)
{
    allSettings = sets;

    // remember main display window setting if they will be changed
    is_always_on_top = allSettings.general.displayIsOnTop;
    current_display_screen = allSettings.general.displayScreen;
    currentDisplayScreen2 = allSettings.general.displayScreen2;

    // Set individual items
    generalSettingswidget->setSettings(allSettings.general);
    bibleSettingswidget->setSettings(allSettings.bible);
    songSettingswidget->setSettings(allSettings.song);
    announcementSettingswidget->setSettings(allSettings.announce);

}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch ( e->type() ) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void SettingsDialog::on_buttonBox_rejected()
{
    close();
}

void SettingsDialog::on_buttonBox_accepted()
{
    allSettings.general = generalSettingswidget->getSettings();
    allSettings.bible = bibleSettingswidget->getSettings();
    allSettings.song = songSettingswidget->getSettings();
    allSettings.announce = announcementSettingswidget->getSettings();

    // Apply settings
    softProjector->updateSetting(allSettings);

    // Redraw the screen:
    softProjector->updateScreen();

    // Update <display_on_top> only when changed, or when screen location has been changed
    if(is_always_on_top!=allSettings.general.displayIsOnTop
            || current_display_screen!=allSettings.general.displayScreen
            || currentDisplayScreen2!=allSettings.general.displayScreen2)
        softProjector->positionDisplayWindow();

    close();
}

void SettingsDialog::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}
