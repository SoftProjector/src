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
//    setDefaults();

    ui->setupUi(this);
    softProjector = (SoftProjector*)parent;
    generalSettingswidget = new GeneralSettingWidget;
    bibleSettingswidget = new BibleSettingWidget;
    songSettingswidget = new SongSettingWidget;
    announcementSettingswidget = new AnnouncementSettingWidget;

    loadSettings();

    ui->scrollAreaGeneralSettings->setWidget(generalSettingswidget);
    ui->scrollAreaBibleSettings->setWidget(bibleSettingswidget);
    ui->scrollAreaSongSettings->setWidget(songSettingswidget);
    ui->scrollAreaAnnouncementSettings->setWidget(announcementSettingswidget);

    // Get Bibles that exist in the database
    QSqlQuery sq;



    // Set current item for primary Bible
    allSettings.bibleSettings.primaryBible = softProjector->bibleWidget->getPrimary();


    // Set current item for secondary Bible
    allSettings.bibleSettings.secondaryBible = softProjector->bibleWidget->getSecondary();

    is_always_on_top = allSettings.generalSettings.displayIsOnTop;

    new_font = softProjector->display->getFont();

    new_wallpaper_path = softProjector->display->getWallpaper();
    if(new_wallpaper_path == "") {
        ui->wallpaper_ef->setText(tr("None"));
        ui->remove_wallpaper_button->setEnabled(false);
    }
    else
        ui->wallpaper_ef->setText(new_wallpaper_path);

    new_passive_wallpaper_path = softProjector->display->getPassiveWallpaper();


    // Set Song Items
    ui->stanza_title_checkBox->setChecked(softProjector->show_stanza_title);
    ui->song_key_checkBox->setChecked(softProjector->show_song_key);
    ui->song_number_checkBox->setChecked(softProjector->show_song_number);

    // Set the foreground color
    new_foreground_color = softProjector->display->getForegroundColor();

    // Initialize the "font color" widget:
    // FIXME this creates a memory leak:
    QGraphicsScene *scene = new QGraphicsScene();
    ui->text_color_view->setScene(scene);
    ui->text_color_view->show();
    ui->text_color_view->setBackgroundBrush(QBrush(new_foreground_color));
}

void SettingsDialog::setDefaults()
{
    QSqlQuery sq;
    QString gset, bset, sset, aset;

    // Clean existing data

    // Set General defauls
    gset = "displayIsOnTop = false\n"
            "useShadow = true\n"
            "useFading = true\n"
            "useBlurShadow = false\n"
            "useBackground = false\n"
            "backgroundPath = ";
    sq.exec("INSERT INTO Settings (user, type, sets) VALUES ('0', 'general', '" +gset+"')");

    // Set Bible defaults
    bset ="primaryBible = none\n"
            "secondaryBible = none\n"
            "trinaryBible = none\n"
            "operatorBible = same\n"
            "bibleBackground = none\n"
            "bibleTextColor = white\n"
            "bibleTextFont = none\n"
            "versionAbbriviation = 0\n"
            "bibleAlingment = 4129";
    sq.exec("INSERT INTO Settings (user, type, sets) VALUES ('0', 'bible', '" +bset+"')");

}

void SettingsDialog::loadSettings()
{
    QString t,n,v,s,sets; // type, name, value, userValues
    QStringList set,values;
    QSqlQuery sq;
    sq.exec("SELECT type, sets FROM Settings WHERE user = 0");
    while (sq.next())
    {
        t = sq.value(0).toString();
        sets = sq.value(1).toString();

        if(t == "general") // set general setting values
        {
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n == "displayIsOnTop")
                    allSettings.generalSettings.displayIsOnTop = (v=="true");
                else if (n == "useShadow")
                    allSettings.generalSettings.useShadow = (v=="true");
                else if (n == "useFading")
                    allSettings.generalSettings.useFading = (v=="true");
                else if (n == "useBlurShadow")
                    allSettings.generalSettings.useBlurShadow = (v=="true");
                else if (n == "useBackground")
                    allSettings.generalSettings.useBackground = (v=="true");
                else if (n == "backgroundPath")
                    allSettings.generalSettings.backgroundPath = v;
            }
        }
        else if(t == "bible")
        {
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();

                if(n=="primaryBible")
                    allSettings.bibleSettings.primaryBible = v;
                else if(n=="secondaryBible")
                    allSettings.bibleSettings.secondaryBible = v;
                else if(n=="trinaryBible")
                    allSettings.bibleSettings.trinaryBible = v;
                else if(n=="operatorBible")
                    allSettings.bibleSettings.operatorBible = v;
            }
        }

    }

    generalSettingswidget->setSettings(allSettings.generalSettings);
    bibleSettingswidget->setSettings(allSettings.bibleSettings);

}

void SettingsDialog::saveSettings()
{
    QSqlQuery sq;
    QString gset,bset,sset,aset;

    // Prepare general settings
    if(allSettings.generalSettings.displayIsOnTop)
        gset = "displayIsOnTop = true\n";
    else
        gset = "displayIsOnTop = false\n";
    if(allSettings.generalSettings.useShadow)
        gset += "useShadow = true\n";
    else
        gset += "useShadow = fasle'";
    if(allSettings.generalSettings.useFading)
        gset += "useFading = true\n";
    else
        gset += "useFading = false\n";
    if(allSettings.generalSettings.useBlurShadow)
        gset += "useBlurShadow = true\n";
    else
        gset += "useBlurShadow = false\n";
    if(allSettings.generalSettings.useBackground)
        gset += "useBackground = true\n";
    else
        gset += "useBackground = fasle\n";
    gset += "backgroundPath = " + allSettings.generalSettings.backgroundPath;

    // Prepare bible settings
    bset = "primaryBible = " + allSettings.bibleSettings.primaryBible;
    bset += "\nsecondaryBible = " + allSettings.bibleSettings.secondaryBible;
    bset += "\ntrinaryBible = " + allSettings.bibleSettings.trinaryBible;
    bset += "\noperatorBible = " + allSettings.bibleSettings.operatorBible;

//    QSqlDatabase::database().transaction();
    sq.exec("UPDATE Settings SET sets = '"+ gset +"' WHERE type = 'general' AND user = '0'");
    sq.exec("UPDATE Settings SET sets = '"+ bset +"' WHERE type = 'bible' AND user = '0'");
//    QSqlDatabase::database().commit();
}

void SettingsDialog::updateSecondaryBibleMenu()
{

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
    allSettings.generalSettings = generalSettingswidget->getSettings();
    allSettings.bibleSettings = bibleSettingswidget->getSettings();
    saveSettings();

    QString primaryBible = allSettings.bibleSettings.primaryBible;
    QString secondaryBible = allSettings.bibleSettings.secondaryBible;

    bool show_stanza_title = ui->stanza_title_checkBox->isChecked();
    bool show_song_number = ui->song_number_checkBox->isChecked();
    bool show_song_key = ui->song_key_checkBox->isChecked();

    softProjector->display_on_top = allSettings.generalSettings.displayIsOnTop;

    softProjector->show_stanza_title = show_stanza_title;
    softProjector->show_song_number = show_song_number;
    softProjector->show_song_key = show_song_key;

    softProjector->bibleWidget->loadBibles(primaryBible, secondaryBible);

    softProjector->display->setNewFont(new_font);
    softProjector->display->setNewWallpaper(new_wallpaper_path);
    if(allSettings.generalSettings.useBackground)
        softProjector->display->setNewPassiveWallpaper(allSettings.generalSettings.backgroundPath);
    else
        softProjector->display->setNewPassiveWallpaper("");
    softProjector->display->setFading(allSettings.generalSettings.useFading);
    softProjector->display->setBlur(allSettings.generalSettings.useBlurShadow);
    softProjector->display->setForegroundColor(new_foreground_color);
    softProjector->writeXMLConfigurationFile();

    // Redraw the screen:
    softProjector->updateScreen();
    // Update <display_on_top> only when changed:
    if(is_always_on_top!=allSettings.generalSettings.displayIsOnTop)
        softProjector->positionDisplayWindow();

    close();
}



void SettingsDialog::on_change_font_button_clicked()
{
    // Change the font
    bool ok;
    QFont font = QFontDialog::getFont(&ok, new_font, this);
    if (ok)
        new_font = font;
}

void SettingsDialog::on_set_wallpaper_button_clicked()
{
    // Change background
    QString filename = QFileDialog::getOpenFileName(this,
                         tr("Select a picture for the wallpaper"),
                         ".",
                         tr("Images (*.png *.jpg *.jpeg)"));

    if( !filename.isNull() ) {
        new_wallpaper_path = filename;
        ui->wallpaper_ef->setText(filename);
        ui->remove_wallpaper_button->setEnabled(true);
    }
}

void SettingsDialog::on_remove_wallpaper_button_clicked()
{
    new_wallpaper_path = "";
    ui->wallpaper_ef->setText(tr("None"));
    ui->remove_wallpaper_button->setEnabled(false);
}




void SettingsDialog::on_choose_color_button_clicked()
{
    new_foreground_color = QColorDialog::getColor(new_foreground_color, this);
    ui->text_color_view->setBackgroundBrush(QBrush(new_foreground_color));
    //ui->text_color_view->show();
    //ui->text_color_view->update();
}

void SettingsDialog::on_listWidget_currentRowChanged(int currentRow)
{
    ui->stackedWidget->setCurrentIndex(currentRow);
}
