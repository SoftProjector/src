#include <QtSql>
#include <QMessageBox>
#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "softprojector.h"


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    QStringList bibles;
    
    ui->setupUi(this);
    softProjector = (SoftProjector*)parent;
    
    bibles.append("Russian");
    bibles.append("Ukrainian");
    bibles.append("English (KJV)");

    for (int i = 0; i < bibles.size(); i++) {
	ui->primary_bible_menu->addItem( bibles.at(i) );
        ui->secondary_bible_menu->addItem( bibles.at(i) );
    }
    int primary_index = ui->primary_bible_menu->findText(softProjector->bibleWidget->getPrimary());
    ui->primary_bible_menu->setCurrentIndex(primary_index);

    int secondary_index = ui->secondary_bible_menu->findText(softProjector->bibleWidget->getSecondary());
    ui->secondary_bible_menu->setCurrentIndex(secondary_index);

    // ui->use_fading_effects_box->setChecked(

    if( softProjector->display->getShowBlack() )
        ui->black_screen_rbutton->setChecked(true);
    else
        ui->wallpaper_rbutton->setChecked(true);

    new_font = softProjector->display->getFont();
    new_wallpaper_path = softProjector->display->getWallpaper();

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
    bool show_black = ui->black_screen_rbutton->isChecked();
    bool verse = ui->verse_rbutton->isChecked();

    QString primaryBible = ui->primary_bible_menu->currentText();
    QString secondaryBible = ui->secondary_bible_menu->currentText();

    bool use_fading = ui->use_fading_effects_box->isChecked();
    bool display_on_top = ui->display_on_top_box->isChecked();

    softProjector->bibleWidget->setPrimary(primaryBible);
    softProjector->bibleWidget->setSecondary(secondaryBible);

    softProjector->display->setNewFont(new_font);
    softProjector->display->setNewWallpaper(new_wallpaper_path);
    softProjector->display->setShowBlack(show_black);
    //softProjector->setVerse(verse);

    softProjector->writeConfigurationFile();

    // Redraw the screen:
    softProjector->display->RenderText();

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

void SettingsDialog::on_change_background_button_clicked()
{
    // Change background
    QString filename = QFileDialog::getOpenFileName(this,
                         "Select a picture for the wallpaper",
                         ".",
                         "Images (*.png *.jpg *.jpeg)");

    if( !filename.isNull() )
        new_wallpaper_path = filename;
}



