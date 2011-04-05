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

    // Get Bibles that exist in the database
    QSqlQuery sq;
    sq.exec("SELECT bible_name, id FROM BibleVersions");
    while(sq.next()){
        bibles << sq.value(0).toString();
        bible_id_list << sq.value(1).toString();
    }
    sq.clear();

    // Fill primary and secondary Bible comboboxes
    ui->primary_bible_menu->addItems(bibles);
    ui->secondary_bible_menu->addItem(tr("None"));
    ui->secondary_bible_menu->addItems(bibles);

    // Set current item for primary Bible
    int primary_index = bible_id_list.indexOf(softProjector->bibleWidget->getPrimary());
    ui->primary_bible_menu->setCurrentIndex(primary_index);

    // Remove the primary bible from the secondary list:
    updateSecondaryBibleMenu();

    // Set current item for secondary Bible
    if (softProjector->bibleWidget->getSecondary() == "none")
        ui->secondary_bible_menu->setCurrentIndex(0);
    else {
        int secondary_index = second_id_list.indexOf(softProjector->bibleWidget->getSecondary());
        ui->secondary_bible_menu->setCurrentIndex(secondary_index+1);
    }

    // Set Display window always on top or not
    if (softProjector->display_on_top)
        ui->display_on_top_box->setChecked(true);

    // Set to use or not to use fading
    ui->use_fading_effects_box->setChecked(softProjector->display->useFading());

    // Set to use blured shadow or not
    ui->use_blur_check_box->setChecked(softProjector->display->useBlur());

    new_font = softProjector->display->getFont();

    new_wallpaper_path = softProjector->display->getWallpaper();
    if(new_wallpaper_path == "") {
        ui->wallpaper_ef->setText(tr("None"));
        ui->remove_wallpaper_button->setEnabled(false);
    }
    else
        ui->wallpaper_ef->setText(new_wallpaper_path);

    new_passive_wallpaper_path = softProjector->display->getPassiveWallpaper();
    if(new_passive_wallpaper_path == "") {
        ui->passive_wallpaper_ef->setText(tr("None"));
        ui->remove_passive_wallpaper_button->setEnabled(false);
    }
    else
        ui->passive_wallpaper_ef->setText(new_passive_wallpaper_path);

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

void SettingsDialog::updateSecondaryBibleMenu()
{
    QString pbible = ui->primary_bible_menu->currentText();
    QString sbible = ui->secondary_bible_menu->currentText();
    QStringList secondary_bibles = bibles;
    secondary_bibles.removeOne(pbible);

    second_id_list = bible_id_list;
    second_id_list.removeAt(ui->primary_bible_menu->currentIndex());
    ui->secondary_bible_menu->clear();
    ui->secondary_bible_menu->addItem(tr("None"));
    ui->secondary_bible_menu->addItems(secondary_bibles);

    int i = ui->secondary_bible_menu->findText(sbible);
    if( i != -1 )
        // The same secondary bible is still available
        ui->secondary_bible_menu->setCurrentIndex(i);
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
    int primaryBibleInd = ui->primary_bible_menu->currentIndex();
    QString primaryBible, secondaryBible;
    if( primaryBibleInd != -1 )
    {
        // If there are bibles in the database
        primaryBible = bible_id_list[primaryBibleInd];
        int secondaryBibleInd = ui->secondary_bible_menu->currentIndex();

        if (secondaryBibleInd <= 0)
            secondaryBible = "none";
        else
            secondaryBible = second_id_list[ui->secondary_bible_menu->currentIndex()-1];
    }
    else
    {
        // There are no bibles in the database
        primaryBible = "none";
        secondaryBible = "none";
    }


    bool use_fading = ui->use_fading_effects_box->isChecked();
    bool display_on_top = ui->display_on_top_box->isChecked();
    bool use_blur = ui->use_blur_check_box->isChecked();
    bool show_stanza_title = ui->stanza_title_checkBox->isChecked();
    bool show_song_number = ui->song_number_checkBox->isChecked();
    bool show_song_key = ui->song_key_checkBox->isChecked();

    softProjector->display_on_top = display_on_top;

    softProjector->show_stanza_title = show_stanza_title;
    softProjector->show_song_number = show_song_number;
    softProjector->show_song_key = show_song_key;

    softProjector->bibleWidget->loadBibles(primaryBible, secondaryBible);

    softProjector->display->setNewFont(new_font);
    softProjector->display->setNewWallpaper(new_wallpaper_path);
    softProjector->display->setNewPassiveWallpaper(new_passive_wallpaper_path);
    softProjector->display->setFading(use_fading);
    softProjector->display->setBlur(use_blur);
    softProjector->display->setForegroundColor(new_foreground_color);
    softProjector->writeXMLConfigurationFile();

    // Redraw the screen:
    softProjector->updateScreen();
    // Update <display_on_top>:
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

void SettingsDialog::on_set_passive_wallpaper_clicked()
{
        // Change background
    QString filename1 = QFileDialog::getOpenFileName(this,
                         tr("Select a picture for the wallpaper"),
                         ".",
                         tr("Images (*.png *.jpg *.jpeg)"));

    if( !filename1.isNull() ) {
        new_passive_wallpaper_path = filename1;
        ui->passive_wallpaper_ef->setText(filename1);
        ui->remove_passive_wallpaper_button->setEnabled(true);
    }
}

void SettingsDialog::on_remove_wallpaper_button_clicked()
{
    new_wallpaper_path = "";
    ui->wallpaper_ef->setText(tr("None"));
    ui->remove_wallpaper_button->setEnabled(false);
}

void SettingsDialog::on_primary_bible_menu_activated(QString )
{
    updateSecondaryBibleMenu();
}

void SettingsDialog::on_remove_passive_wallpaper_button_clicked()
{
    new_passive_wallpaper_path = "";
    ui->passive_wallpaper_ef->setText(tr("None"));
    ui->remove_passive_wallpaper_button->setEnabled(false);
}


void SettingsDialog::on_choose_color_button_clicked()
{
    new_foreground_color = QColorDialog::getColor(new_foreground_color, this);
    ui->text_color_view->setBackgroundBrush(QBrush(new_foreground_color));
    //ui->text_color_view->show();
    //ui->text_color_view->update();
}
