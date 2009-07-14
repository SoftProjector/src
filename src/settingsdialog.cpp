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
    ui->primary_bible_menu->setCurrentIndex(softProjector->bibleWidget->getPrimary());
    ui->secondary_bible_menu->setCurrentIndex(softProjector->bibleWidget->getSecondary());

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

    int primaryIndex = ui->primary_bible_menu->currentIndex();
    int secondaryIndex = ui->secondary_bible_menu->currentIndex();

    softProjector->bibleWidget->setPrimary(primaryIndex);
    softProjector->bibleWidget->setSecondary(secondaryIndex);

    // ui->use_fading_effects_box

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
