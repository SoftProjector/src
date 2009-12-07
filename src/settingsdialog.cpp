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
    ui->secondary_bible_menu->addItem("None");
    ui->secondary_bible_menu->addItems(bibles);

    // Set current item for primary Bible
    int primary_index = bible_id_list.indexOf(softProjector->bibleWidget->getPrimary());
    ui->primary_bible_menu->setCurrentIndex(primary_index);

    // Remove the primary bible from the secondary list:
    updateSecondaryBibleMenu();

    // Set current item for secondary Bible
    qDebug() << softProjector->bibleWidget->getSecondary();
    if (softProjector->bibleWidget->getSecondary() == "none")
        ui->secondary_bible_menu->setCurrentIndex(0);
    else {
        int secondary_index = second_id_list.indexOf(softProjector->bibleWidget->getSecondary());
        ui->secondary_bible_menu->setCurrentIndex(secondary_index+1);
    }

    // Set Display screen alway on top or not
    if (softProjector->display_on_top)
        ui->display_on_top_box->setChecked(true);

    // Set to use or not to use fading
    ui->use_fading_effects_box->setChecked(softProjector->display->useFading());

    // Set to use blured shadow or not
    ui->use_blur_check_box->setChecked(softProjector->display->useBlur());

    // Set font
    new_font = softProjector->display->getFont();

    // Set type of creen to show Black or Display wallpaper or Fill wallpaper
    wallpaper_state = softProjector->display->getWallState();
    if( wallpaper_state == 0 )
        ui->black_screen_rbutton->setChecked(true);
    else if (wallpaper_state == 1)
        ui->wallpaper_rbutton->setChecked(true);
    else if (wallpaper_state == 2)
        ui->fill_wallpaper_rbutton->setChecked(true);

    // Set wallpaper paths
    display_wallpaper_path = softProjector->display->getWallpaper();
    fill_wallpaper_path = softProjector->display->getFillWallpaper();
    if(display_wallpaper_path == "") {
        ui->wallpaper_ef->setText("None");
        ui->remove_wallpaper_button->setEnabled(false);
        ui->wallpaper_rbutton->setEnabled(false);
//        ui->black_or_wallpaper_group->setEnabled(false);
    }
    else
        ui->wallpaper_ef->setText(display_wallpaper_path);
    ui->fill_wallpaper_ef->setText(fill_wallpaper_path);

    // Set Song Items
    ui->stanza_title_checkBox->setChecked(softProjector->show_stanza_title);
    ui->song_key_checkBox->setChecked(softProjector->show_song_key);
    ui->song_number_checkBox->setChecked(softProjector->show_song_number);

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
    ui->secondary_bible_menu->addItem("None");
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
    // Wallpaper Settings
    if (ui->black_screen_rbutton->isChecked())
        wallpaper_state = 0;
    else if (ui->wallpaper_rbutton->isChecked())
        wallpaper_state = 1;
    else if (ui->fill_wallpaper_rbutton->isChecked())
        wallpaper_state = 2;

//    bool show_black = ui->black_screen_rbutton->isChecked();

    // Bible Settings
    QString primaryBible = bible_id_list[ui->primary_bible_menu->currentIndex()];
    int secondaryBibleInd = ui->secondary_bible_menu->currentIndex();
    QString secondaryBible;
    if (secondaryBibleInd <= 0)
        secondaryBible = "none";
    else
        secondaryBible = second_id_list[ui->secondary_bible_menu->currentIndex()-1];

    // Dispaly Settings
    bool use_fading = ui->use_fading_effects_box->isChecked();
    bool display_on_top = ui->display_on_top_box->isChecked();
    bool use_blur = ui->use_blur_check_box->isChecked();

    // Song settings
    bool show_stanza_title = ui->stanza_title_checkBox->isChecked();
    bool show_song_number = ui->song_number_checkBox->isChecked();
    bool show_song_key = ui->song_key_checkBox->isChecked();

    softProjector->display_on_top = display_on_top;

    softProjector->show_stanza_title = show_stanza_title;
    softProjector->show_song_number = show_song_number;
    softProjector->show_song_key = show_song_key;

    softProjector->bibleWidget->loadBibles(primaryBible, secondaryBible);

    softProjector->display->setNewFont(new_font);
    softProjector->display->setWallpaper(wallpaper_state, display_wallpaper_path, fill_wallpaper_path);
//    softProjector->display->setShowBlack(show_black);
    softProjector->display->setFading(use_fading);
    softProjector->display->setBlur(use_blur);
    softProjector->writeXMLConfigurationFile();


    // Redraw the screen:
    softProjector->updateScreen();
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
                         "Select a picture for the wallpaper",
                         ".",
                         "Images (*.png *.jpg *.jpeg)");

    if( !filename.isNull() ) {
        display_wallpaper_path = filename;
        ui->wallpaper_ef->setText(filename);
        ui->remove_wallpaper_button->setEnabled(true);
        ui->wallpaper_rbutton->setEnabled(true);
//        ui->black_or_wallpaper_group->setEnabled(true);
    }
}


void SettingsDialog::on_remove_wallpaper_button_clicked()
{
    display_wallpaper_path = "";
    ui->wallpaper_ef->setText("None");
    ui->remove_wallpaper_button->setEnabled(false);
    ui->wallpaper_rbutton->setEnabled(false);
    if (ui->wallpaper_rbutton->isChecked())
        ui->black_screen_rbutton->setChecked(true);
//    ui->black_or_wallpaper_group->setEnabled(false);
}

void SettingsDialog::on_primary_bible_menu_activated(QString )
{
    updateSecondaryBibleMenu();
}

void SettingsDialog::on_fill_wallaper_pushButton_clicked()
{
        // Change fill background
    QString filename = QFileDialog::getOpenFileName(this,
                         "Select a picture for the fill wallpaper",
                         ".",
                         "Images (*.png *.jpg *.jpeg)");

    if( !filename.isNull() ) {
        fill_wallpaper_path = filename;
        ui->fill_wallpaper_ef->setText(filename);
    }
}

void SettingsDialog::on_fill_wallpaper_rbutton_toggled(bool checked)
{
    if(checked)
    {
        ui->fill_wallaper_pushButton->setEnabled(true);
        ui->fill_wallpaper_ef->setEnabled(true);
    }
    else
    {
        ui->fill_wallaper_pushButton->setEnabled(false);
        ui->fill_wallpaper_ef->setEnabled(false);
    }
}
