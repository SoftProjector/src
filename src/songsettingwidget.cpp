#include "songsettingwidget.h"
#include "ui_songsettingwidget.h"

SongSettingWidget::SongSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongSettingWidget)
{
    ui->setupUi(this);
}

SongSettingWidget::~SongSettingWidget()
{
    delete ui;
}

void SongSettingWidget::setSettings(SongSettings settings)
{
    mySettings = settings;
    loadSettings();
}

SongSettings SongSettingWidget::getSettings()
{
    // Save Song info
    mySettings.showStanzaTitle = ui->checkBox_stanzaTitle->isChecked();
    mySettings.showSongKey = ui->checkBox_songKey->isChecked();
    mySettings.showSongNumber = ui->checkBox_songNumber->isChecked();

    // Save song ending
    mySettings.showSongEnding = ui->groupBox_songEnding->isChecked();
    if(ui->radioButton_astrict->isChecked())
        mySettings.songEndingType = 0;
    else if(ui->radioButton_songCopyInfo)
        mySettings.songEndingType = 1;

    // Save song background
    mySettings.useBackground = ui->groupBox_SongBackground->isChecked();
    mySettings.backgroundPath = ui->lineEdit_SongBackground->text();

    // Save alingment
    mySettings.textAlingmentV = ui->comboBox_verticalAling->currentIndex();
    mySettings.textAlingmentH = ui->comboBox_horizontalAling->currentIndex();

    //

    return mySettings;
}

void SongSettingWidget::loadSettings()
{
    // Set Song Information
    ui->checkBox_stanzaTitle->setChecked(mySettings.showStanzaTitle);
    ui->checkBox_songKey->setChecked(mySettings.showSongKey);
    ui->checkBox_songNumber->setChecked(mySettings.showSongNumber);

    // Set Song Ending
    ui->groupBox_songEnding->setChecked(mySettings.showSongEnding);
    if(mySettings.songEndingType == 0)
        ui->radioButton_astrict->setChecked(true);
    else if(mySettings.songEndingType == 1)
        ui->radioButton_songCopyInfo->setChecked(true);

    // Set Song Background
    ui->groupBox_SongBackground->setChecked(mySettings.useBackground);
    ui->lineEdit_SongBackground->setText(mySettings.backgroundPath);

    // Set alingment
    ui->comboBox_verticalAling->setCurrentIndex(mySettings.textAlingmentV);
    ui->comboBox_horizontalAling->setCurrentIndex(mySettings.textAlingmentH);

    // Set text color
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicView_textColor->setPalette(p);

}

void SongSettingWidget::on_button_SongBackground_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for song wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));
    if(!filename.isNull())
        ui->lineEdit_SongBackground->setText(filename);
}

void SongSettingWidget::on_button_textColor_clicked()
{
    mySettings.textColor = QColorDialog::getColor(mySettings.textColor,this);
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicView_textColor->setPalette(p);
}

void SongSettingWidget::on_button_font_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.textFont,this);
    if(ok)
        mySettings.textFont = font;
}

void SongSettingWidget::on_pushButton_default_clicked()
{
    SongSettings s;
    mySettings = s;
    loadSettings();
}
