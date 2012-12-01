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
    // Set Effects
    ui->checkBox_useShadow->setChecked(mySettings.useShadow);
    ui->checkBox_useFading->setChecked(mySettings.useFading);
    ui->checkBox_useBlurredShadow->setChecked(mySettings.useBlurShadow);

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

    // Set text font lable
    QString st(QString("%1: %2").arg(mySettings.textFont.rawName()).arg(mySettings.textFont.pointSize()));
    if(mySettings.textFont.bold())
        st += ", Bold";
    if(mySettings.textFont.italic())
        st += ", Italic";
    if(mySettings.textFont.strikeOut())
        st += ", StrikeOut";
    if(mySettings.textFont.underline())
        st += ", Underline";
    ui->label_font->setText(st);
}

AnnounceSettings AnnouncementSettingWidget::getSettings()
{
    // Effects
    mySettings.useShadow = ui->checkBox_useShadow->isChecked();
    mySettings.useFading = ui->checkBox_useFading->isChecked();
    mySettings.useBlurShadow = ui->checkBox_useBlurredShadow->isChecked();

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
    QColor c(QColorDialog::getColor(mySettings.textColor,this));
    if(c.isValid())
        mySettings.textColor = c;
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

    QString st(QString("%1: %2").arg(mySettings.textFont.rawName()).arg(mySettings.textFont.pointSize()));
    if(mySettings.textFont.bold())
        st += ", Bold";
    if(mySettings.textFont.italic())
        st += ", Italic";
    if(mySettings.textFont.strikeOut())
        st += ", StrikeOut";
    if(mySettings.textFont.underline())
        st += ", Underline";
    ui->label_font->setText(st);
}

void AnnouncementSettingWidget::on_pushButton_default_clicked()
{
    AnnounceSettings a;
    mySettings = a;
    loadSettings();
}

void AnnouncementSettingWidget::on_checkBox_useShadow_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBox_useBlurredShadow->setEnabled(true);
    else
    {
        ui->checkBox_useBlurredShadow->setChecked(false);
        ui->checkBox_useBlurredShadow->setEnabled(false);
    }
}
