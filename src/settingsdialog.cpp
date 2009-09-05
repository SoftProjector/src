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
    QStringList bibles;
    QSqlQuery sq;
    sq.exec("SELECT bible_name,bible_id FROM BibleVersions");
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
    sq.exec("SELECT rowid FROM BibleVersions WHERE bible_id like '" +softProjector->bibleWidget->getPrimary() +"'");
    sq.first();
    int primary_index = sq.value(0).toInt();
    ui->primary_bible_menu->setCurrentIndex(primary_index-1);
    sq.clear();

    // Set current item for secondary Bible
    bool has2nd = sq.exec("SELECT rowid FROM BibleVersions WHERE bible_id like '" +softProjector->bibleWidget->getSecondary() +"'");
    if (!has2nd)
        ui->secondary_bible_menu->setCurrentIndex(0);
    else {
        sq.first();
        int secondary_index = sq.value(0).toInt();
        ui->secondary_bible_menu->setCurrentIndex(secondary_index);
    }
    
    // ui->use_fading_effects_box->setChecked(

    if( softProjector->display->getShowBlack() )
        ui->black_screen_rbutton->setChecked(true);
    else
        ui->wallpaper_rbutton->setChecked(true);

    new_font = softProjector->display->getFont();
    new_wallpaper_path = softProjector->display->getWallpaper();
    if(new_wallpaper_path == "") {
        ui->wallpaper_label->setText("Wallpaper: None");
        ui->remove_wallpaper_button->setEnabled(false);
        ui->black_or_wallpaper_group->setEnabled(false);
    }
    else
        ui->wallpaper_label->setText("Wallpaper: " + new_wallpaper_path);

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

    qDebug() << ui->primary_bible_menu->currentIndex();
    QString primaryBible = bible_id_list[ui->primary_bible_menu->currentIndex()];
    int secondaryBibleInd = ui->secondary_bible_menu->currentIndex();
    QString secondaryBible;
    if (secondaryBibleInd <= 0)
        secondaryBible = "none";
    else
        secondaryBible = bible_id_list[ui->secondary_bible_menu->currentIndex()-1];

    bool use_fading = ui->use_fading_effects_box->isChecked();
    bool display_on_top = ui->display_on_top_box->isChecked();

    softProjector->bibleWidget->loadBibles(primaryBible, secondaryBible);

    softProjector->display->setNewFont(new_font);
    softProjector->display->setNewWallpaper(new_wallpaper_path);
    softProjector->display->setShowBlack(show_black);
    softProjector->writeConfigurationFile();

    // Redraw the screen:
    softProjector->on_show_button_clicked();
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
        new_wallpaper_path = filename;
        ui->wallpaper_label->setText("Wallpaper: " + filename);
        ui->remove_wallpaper_button->setEnabled(true);
        ui->black_or_wallpaper_group->setEnabled(true);
    }
}


void SettingsDialog::on_remove_wallpaper_button_clicked()
{
    new_wallpaper_path = "";
    ui->wallpaper_label->setText("Wallpaper: None");
    ui->remove_wallpaper_button->setEnabled(false);
    ui->black_or_wallpaper_group->setEnabled(false);
}
