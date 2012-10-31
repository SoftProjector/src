#include "announcementsettingwidget.h"
#include "ui_announcementsettingwidget.h"

AnnouncementSettingWidget::AnnouncementSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnnouncementSettingWidget)
{
    ui->setupUi(this);
}

AnnouncementSettingWidget::~AnnouncementSettingWidget()
{
    delete ui;
}

void AnnouncementSettingWidget::setSettings(AnnounceSettings& settings)
{
    mySettings = settings;
    loadSettings();
}

void AnnouncementSettingWidget::loadSettings()
{
    // Set background
    ui->groupBox_Background->setChecked(mySettings.useBackground);
    ui->lineEdit_Background->setText(mySettings.backgroundPath);

    // Set Alingment
    ui->comboBox_verticalAling->setCurrentIndex(mySettings.textAlingmentV);
    ui->comboBox_horizontalAling->setCurrentIndex(mySettings.textAlingmentH);

    // Set text color
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicView_textColor->setPalette(p);
}

AnnounceSettings AnnouncementSettingWidget::getSettings()
{
    // Get Background
    mySettings.useBackground = ui->groupBox_Background->isChecked();
    mySettings.backgroundPath = ui->lineEdit_Background->text();

    // Alingmet
    mySettings.textAlingmentV = ui->comboBox_verticalAling->currentIndex();
    mySettings.textAlingmentH = ui->comboBox_horizontalAling->currentIndex();

    return mySettings;
}

void AnnouncementSettingWidget::on_button_Background_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for announcement wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));
    if(!filename.isNull())
        ui->lineEdit_Background->setText(filename);
}

void AnnouncementSettingWidget::on_button_textColor_clicked()
{
    mySettings.textColor = QColorDialog::getColor(mySettings.textColor,this);
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicView_textColor->setPalette(p);
}

void AnnouncementSettingWidget::on_button_font_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.textFont,this);
    if(ok)
        mySettings.textFont = font;
}

void AnnouncementSettingWidget::on_pushButton_default_clicked()
{
    AnnounceSettings a;
    mySettings = a;
    loadSettings();
}
