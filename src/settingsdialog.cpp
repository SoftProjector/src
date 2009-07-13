#include <QtSql>
#include <QMessageBox>
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    // ui->primary_bible_menu
    // ui->secondary_bible_menu
    // ui->use_fading_effects_box

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

    // ui->primary_bible_menu
    // ui->secondary_bible_menu
    // ui->use_fading_effects_box

    /*
    PRIMARY

    if( RUSSIAN )
        bibleWidget->setPrimary(0);
    elif( UKRAINIAN )
        bibleWidget->setPrimary(1);
    elif( ENGLISH )
        bibleWidget->setPrimary(2);

    SECONDARY
    if( RUSSIAN )
        bibleWidget->setSecondary(0);
    elif( UKRAINIAN )
        bibleWidget->setSecondary(1);
    elif( ENGLISH )
        bibleWidget->setSecondary(2);
    */

    close();
}



void SettingsDialog::on_change_font_button_clicked()
{
    // Change the font
    // NOTE: This change should take place ONLY if the user pressed OK
}

void SettingsDialog::on_change_background_button_clicked()
{
    // Change background
    // NOTE: This change should take place ONLY if the user pressed OK
}
