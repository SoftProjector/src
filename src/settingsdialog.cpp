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

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    generalSettingswidget = new GeneralSettingWidget;
    bibleSettingswidget = new BibleSettingWidget;
    songSettingswidget = new SongSettingWidget;
    announcementSettingswidget = new AnnouncementSettingWidget;

    ui->scrollAreaGeneralSettings->setWidget(generalSettingswidget);
    ui->scrollAreaBibleSettings->setWidget(bibleSettingswidget);
    ui->scrollAreaSongSettings->setWidget(songSettingswidget);
    ui->scrollAreaAnnouncementSettings->setWidget(announcementSettingswidget);

    btnOk = new QPushButton(tr("OK"));
    btnCancel = new QPushButton(tr("Cancel"));
    btnApply = new QPushButton(tr("Apply"));

    ui->buttonBox->addButton(btnOk,QDialogButtonBox::AcceptRole);
    ui->buttonBox->addButton(btnCancel,QDialogButtonBox::RejectRole);
    ui->buttonBox->addButton(btnApply,QDialogButtonBox::ApplyRole);

    // Connect display screen slot
    connect(generalSettingswidget,SIGNAL(setDisp2Use(bool)),this,SLOT(setUseDispScreen2(bool)));

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
    bibleSettingswidget->setSettings(allSettings.bible, allSettings.bible2);
    songSettingswidget->setSettings(allSettings.song, allSettings.song2);
    announcementSettingswidget->setSettings(allSettings.announce, allSettings.announce2);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;

    //    delete softProjector;
    delete generalSettingswidget;
    delete bibleSettingswidget;
    delete songSettingswidget;
    delete announcementSettingswidget;

    delete btnOk;
    delete btnCancel;
    delete btnApply;
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

void SettingsDialog::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}

void SettingsDialog::setUseDispScreen2(bool toUse)
{
    bibleSettingswidget->setDispScreen2Visible(toUse);
    songSettingswidget->setDispScreen2Visible(toUse);
    announcementSettingswidget->setDispScreen2Visible(toUse);
}

void SettingsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(button == btnOk)
    {
        applySettings();
        close();
    }
    else if(button == btnCancel)
        close();
    else if(button == btnApply)
        applySettings();
}

void SettingsDialog::applySettings()
{
    allSettings.general = generalSettingswidget->getSettings();
    bibleSettingswidget->getSettings(allSettings.bible, allSettings.bible2);
    songSettingswidget->getSettings(allSettings.song, allSettings.song2);
    announcementSettingswidget->getSettings(allSettings.announce, allSettings.announce2);

    // Apply settings
    emit updateSettings(allSettings);

    // Update <display_on_top> only when changed, or when screen location has been changed
    if(is_always_on_top!=allSettings.general.displayIsOnTop
            || current_display_screen!=allSettings.general.displayScreen
            || currentDisplayScreen2!=allSettings.general.displayScreen2)
        emit positionsDisplayWindow();

    // Redraw the screen:
    emit updateScreen();

    // reset display holders
    is_always_on_top = allSettings.general.displayIsOnTop;
    current_display_screen = allSettings.general.displayScreen;
    currentDisplayScreen2 = allSettings.general.displayScreen2;
}
