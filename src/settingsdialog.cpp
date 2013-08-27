/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyleft (Ɔ) 2013  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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
    passiveSettingwidget = new PassiveSettingWidget;
    bibleSettingswidget = new BibleSettingWidget;
    songSettingswidget = new SongSettingWidget;
    pictureSettingWidget = new PictureSettingWidget;
    announcementSettingswidget = new AnnouncementSettingWidget;

    ui->scrollAreaGeneralSettings->setWidget(generalSettingswidget);
    ui->scrollAreaPassiveSettings->setWidget(passiveSettingwidget);
    ui->scrollAreaBibleSettings->setWidget(bibleSettingswidget);
    ui->scrollAreaSongSettings->setWidget(songSettingswidget);
    ui->scrollAreaPicture->setWidget(pictureSettingWidget);
    ui->scrollAreaAnnouncementSettings->setWidget(announcementSettingswidget);

    btnOk = new QPushButton(tr("OK"));
    btnCancel = new QPushButton(tr("Cancel"));
    btnApply = new QPushButton(tr("Apply"));

    ui->buttonBox->addButton(btnOk,QDialogButtonBox::AcceptRole);
    ui->buttonBox->addButton(btnCancel,QDialogButtonBox::RejectRole);
    ui->buttonBox->addButton(btnApply,QDialogButtonBox::ApplyRole);

    // Connect display screen slot
    connect(generalSettingswidget,SIGNAL(setDisp2Use(bool)),this,SLOT(setUseDispScreen2(bool)));
    connect(generalSettingswidget,SIGNAL(themeChanged(int)),this,SLOT(changeTheme(int)));

}

void SettingsDialog::loadSettings(GeneralSettings &sets, Theme &thm, SlideShowSettings &ssets,
                                  BibleVersionSettings &bsets, BibleVersionSettings &bsets2)
{
    gsettings = sets;
    theme = thm;
    bsettings = bsets;
    bsettings2 = bsets2;
    ssettings = ssets;

    // remember main display window setting if they will be changed
    is_always_on_top = gsettings.displayIsOnTop;
    current_display_screen = gsettings.displayScreen;
    currentDisplayScreen2 = gsettings.displayScreen2;

    // Set individual items
    generalSettingswidget->setSettings(gsettings);
    bibleSettingswidget->setBibleVersions(bsettings,bsettings2);
    pictureSettingWidget->setSettings(ssettings);
    setThemes();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;

    delete generalSettingswidget;
    delete passiveSettingwidget;
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
    passiveSettingwidget->setDispScreen2Visible(toUse);
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
    gsettings = generalSettingswidget->getSettings();
    bibleSettingswidget->getBibleVersions(bsettings,bsettings2);
    pictureSettingWidget->getSettings(ssettings);
    getThemes();

    // Apply settings
    emit updateSettings(gsettings,theme,ssettings,bsettings,bsettings2);

    // Update <display_on_top> only when changed, or when screen location has been changed
    if(is_always_on_top!=gsettings.displayIsOnTop
            || current_display_screen!=gsettings.displayScreen
            || currentDisplayScreen2!=gsettings.displayScreen2)
        emit positionsDisplayWindow();

    // Redraw the screen:
    emit updateScreen();

    // Save Settings
    theme.saveThemeUpdate();

    // reset display holders
    is_always_on_top = gsettings.displayIsOnTop;
    current_display_screen = gsettings.displayScreen;
    currentDisplayScreen2 = gsettings.displayScreen2;
}

void SettingsDialog::getThemes()
{
    passiveSettingwidget->getSettings(theme.passive, theme.passive2);
    bibleSettingswidget->getSettings(theme.bible, theme.bible2);
    songSettingswidget->getSettings(theme.song, theme.song2);
    announcementSettingswidget->getSettings(theme.announce, theme.announce2);
}

void SettingsDialog::setThemes()
{
    passiveSettingwidget->setSetings(theme.passive, theme.passive2);
    bibleSettingswidget->setSettings(theme.bible, theme.bible2);
    songSettingswidget->setSettings(theme.song, theme.song2);
    announcementSettingswidget->setSettings(theme.announce, theme.announce2);
}

void SettingsDialog::changeTheme(int theme_id)
{
    // First save existing changes to the theme
    getThemes();
    theme.saveThemeUpdate();

    // Then load changed theme
    theme.setThemeId(theme_id);
    theme.loadTheme();
    setThemes();
}
