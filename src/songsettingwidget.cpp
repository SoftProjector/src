/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2014  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

#include "songsettingwidget.h"
#include "ui_songsettingwidget.h"

SongSettingWidget::SongSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongSettingWidget)
{
    ui->setupUi(this);
}

void SongSettingWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
     switch ( e->type() ) {
     case QEvent::LanguageChange:
         ui->retranslateUi(this);
         break;
     default:
         break;
     }
}

SongSettingWidget::~SongSettingWidget()
{
    delete ui;
}

void SongSettingWidget::setSettings(SongSettings &settings, SongSettings &settings2)
{
    mySettings = settings;
    mySettings2 = settings2;
    loadSettings();
}

void SongSettingWidget::getSettings(SongSettings &settings, SongSettings &settings2)
{
    // Text
    mySettings.isNotCommonFont = ui->checkBoxCommonFont->isChecked();
    mySettings2.isNotCommonFont = ui->checkBoxCommonFont2->isChecked();
    mySettings.isNotCommonColor = ui->checkBoxCommonColor->isChecked();
    mySettings2.isNotCommonColor = ui->checkBoxCommonColor2->isChecked();

    // Alingment
    mySettings.textAlingmentV = ui->comboBoxVerticalAling->currentIndex();
    mySettings.textAlingmentH = ui->comboBoxHorizontalAling->currentIndex();
    mySettings2.textAlingmentV = ui->comboBoxVerticalAling2->currentIndex();
    mySettings2.textAlingmentH = ui->comboBoxHorizontalAling2->currentIndex();

    // Song Information
    mySettings.showStanzaTitle = ui->checkBoxStanzaTitle->isChecked();
    mySettings.showSongKey = ui->checkBoxSongKey->isChecked();
    mySettings.showSongNumber = ui->checkBoxSongNumber->isChecked();
    mySettings.isNotSameInfoFont = ui->checkBoxSameInfoFont->isChecked();
    mySettings.isNotSameInfoColor = ui->checkBoxSameInfoColor->isChecked();
    mySettings.infoAling = ui->comboBoxInfoAlign->currentIndex();
    mySettings2.showStanzaTitle = ui->checkBoxStanzaTitle2->isChecked();
    mySettings2.showSongKey = ui->checkBoxSongKey2->isChecked();
    mySettings2.showSongNumber = ui->checkBoxSongNumber2->isChecked();
    mySettings2.isNotSameInfoFont = ui->checkBoxSameInfoFont2->isChecked();
    mySettings2.isNotSameInfoColor = ui->checkBoxSameInfoColor2->isChecked();
    mySettings2.infoAling = ui->comboBoxInfoAlign2->currentIndex();

    // Song Ending
    mySettings.showSongEnding = ui->groupBoxSongEnding->isChecked();
    mySettings.isNotSameEndingFont = ui->checkBoxSameEndingFont->isChecked();
    mySettings.isNotSameEndingColor = ui->checkBoxSameEndingColor->isChecked();
    mySettings.endingType = ui->comboBoxEndingType->currentIndex();
    mySettings.endingPosition = ui->comboBoxEndingPosition->currentIndex();
    mySettings2.showSongEnding = ui->groupBoxSongEnding2->isChecked();
    mySettings2.isNotSameEndingFont = ui->checkBoxSameEndingFont2->isChecked();
    mySettings2.isNotSameEndingColor = ui->checkBoxSameEndingColor2->isChecked();
    mySettings2.endingType = ui->comboBoxEndingType2->currentIndex();
    mySettings2.endingPosition = ui->comboBoxEndingPosition2->currentIndex();

    // Transition
    mySettings.transitionType = ui->comboBoxTransitionType->currentIndex() - 1;
    mySettings2.transitionType = ui->comboBoxTransitionType2->currentIndex() - 1;

    // Effects
    mySettings.transitionType = ui->comboBoxTextEffects->currentIndex() - 1;
    mySettings2.transitionType = ui->comboBoxTextEffects2->currentIndex() - 1;

    // Max screen use
    mySettings.isNotCommonLayout = ui->checkBoxCommonLayout->isChecked();
    mySettings.screenUse = ui->spinBoxMaxScreen->value();
    mySettings.screenPosition = ui->comboBoxScreenPosition->currentIndex();
    mySettings2.isNotCommonLayout = ui->checkBoxCommonLayout2->isChecked();
    mySettings2.screenUse = ui->spinBoxMaxScreen2->value();
    mySettings2.screenPosition = ui->comboBoxScreenPosition2->currentIndex();

    // Get if to use secodary screen settings
    mySettings2.useSameForDisp2 = ui->checkBoxDisp2->isChecked();

    settings = mySettings;
    settings2 = mySettings2;
}

void SongSettingWidget::loadSettings()
{
    QPalette p;

    // Text Font
    ui->checkBoxCommonFont->setChecked(mySettings.isNotCommonFont);
    on_checkBoxCommonFont_stateChanged(mySettings.isNotCommonFont);
    ui->labelFont->setText(getFontText(mySettings.textFont));
    ui->checkBoxCommonFont2->setChecked(mySettings2.isNotCommonFont);
    on_checkBoxCommonFont2_stateChanged(mySettings2.isNotCommonFont);
    ui->labelFont2->setText(getFontText(mySettings2.textFont));

    // Text Color
    ui->checkBoxCommonColor->setChecked(mySettings.isNotCommonColor);
    on_checkBoxCommonColor_stateChanged(mySettings.isNotCommonColor);
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
    ui->checkBoxCommonColor2->setChecked(mySettings2.isNotCommonColor);
    on_checkBoxCommonColor2_stateChanged(mySettings2.isNotCommonColor);
    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);

    // Text Shadow Color
    p.setColor(QPalette::Base,mySettings.textShadowColor);
    ui->graphicViewShadowColor->setPalette(p);
    p.setColor(QPalette::Base,mySettings2.textShadowColor);
    ui->graphicViewShadowColor2->setPalette(p);

    // Alingment
    ui->comboBoxVerticalAling->setCurrentIndex(mySettings.textAlingmentV);
    ui->comboBoxHorizontalAling->setCurrentIndex(mySettings.textAlingmentH);
    ui->comboBoxVerticalAling2->setCurrentIndex(mySettings2.textAlingmentV);
    ui->comboBoxHorizontalAling2->setCurrentIndex(mySettings2.textAlingmentH);

    // Song Information
    ui->checkBoxStanzaTitle->setChecked(mySettings.showStanzaTitle);
    ui->checkBoxSongKey->setChecked(mySettings.showSongKey);
    ui->checkBoxSongNumber->setChecked(mySettings.showSongNumber);
    ui->checkBoxSameInfoFont->setChecked(mySettings.isNotSameInfoFont);
    on_checkBoxSameInfoFont_stateChanged(mySettings.isNotSameInfoFont);
    ui->labelInfoFont->setText(getFontText(mySettings.infoFont));
    ui->checkBoxSameInfoColor->setChecked(mySettings.isNotSameInfoColor);
    on_checkBoxSameInfoColor_stateChanged(mySettings.isNotSameInfoColor);
    p.setColor(QPalette::Base,mySettings.infoColor);
    ui->graphicViewInfoColor->setPalette(p);
    p.setColor(QPalette::Base,mySettings.infoShadowColor);
    ui->graphicViewInfoShadowColor->setPalette(p);
    ui->comboBoxInfoAlign->setCurrentIndex(mySettings.infoAling);
    updateInfoButtons();

    ui->checkBoxStanzaTitle2->setChecked(mySettings2.showStanzaTitle);
    ui->checkBoxSongKey2->setChecked(mySettings2.showSongKey);
    ui->checkBoxSongNumber2->setChecked(mySettings2.showSongNumber);
    ui->checkBoxSameInfoFont2->setChecked(mySettings2.isNotSameInfoFont);
    on_checkBoxSameInfoFont2_stateChanged(mySettings2.isNotSameInfoFont);
    ui->labelInfoFont2->setText(getFontText(mySettings2.infoFont));
    ui->checkBoxSameInfoColor2->setChecked(mySettings2.isNotSameInfoColor);
    on_checkBoxSameInfoColor2_stateChanged(mySettings2.isNotSameInfoColor);
    p.setColor(QPalette::Base,mySettings2.infoColor);
    ui->graphicViewInfoColor2->setPalette(p);
    p.setColor(QPalette::Base,mySettings2.infoShadowColor);
    ui->graphicViewInfoShadowColor2->setPalette(p);
    ui->comboBoxInfoAlign2->setCurrentIndex(mySettings2.infoAling);
    updateInfoButtons2();

    // Song Ending
    ui->groupBoxSongEnding->setChecked(mySettings.showSongEnding);
    ui->checkBoxSameEndingFont->setChecked(mySettings.isNotSameEndingFont);
    on_checkBoxSameEndingFont_stateChanged(mySettings.isNotSameEndingFont);
    ui->labelEndingFont->setText(getFontText(mySettings.endingFont));
    ui->checkBoxSameEndingColor->setChecked(mySettings.isNotSameEndingColor);
    on_checkBoxSameEndingColor_stateChanged(mySettings.isNotSameEndingColor);
    p.setColor(QPalette::Base,mySettings.endingColor);
    ui->graphicViewEndingColor->setPalette(p);
    p.setColor(QPalette::Base,mySettings.endingShadowColor);
    ui->graphicViewEndingShadowColor->setPalette(p);
    ui->comboBoxEndingType->setCurrentIndex(mySettings.endingType);
    ui->comboBoxEndingPosition->setCurrentIndex(mySettings.endingPosition);

    ui->groupBoxSongEnding2->setChecked(mySettings2.showSongEnding);
    ui->checkBoxSameEndingFont2->setChecked(mySettings2.isNotSameEndingFont);
    on_checkBoxSameEndingFont2_stateChanged(mySettings2.isNotSameEndingFont);
    ui->labelEndingFont2->setText(getFontText(mySettings2.endingFont));
    ui->checkBoxSameEndingColor2->setChecked(mySettings2.isNotSameEndingColor);
    on_checkBoxSameEndingColor2_stateChanged(mySettings2.isNotSameEndingColor);
    p.setColor(QPalette::Base,mySettings2.endingColor);
    ui->graphicViewEndingColor2->setPalette(p);
    p.setColor(QPalette::Base,mySettings2.endingShadowColor);
    ui->graphicViewEndingShadowColor2->setPalette(p);
    ui->comboBoxEndingType2->setCurrentIndex(mySettings2.endingType);
    ui->comboBoxEndingPosition2->setCurrentIndex(mySettings2.endingPosition);

    // Transition
    ui->comboBoxTransitionType->setCurrentIndex(mySettings.transitionType + 1);
    ui->comboBoxTransitionType2->setCurrentIndex(mySettings2.transitionType + 1);

    // Effects
    ui->comboBoxTextEffects->setCurrentIndex(mySettings.effectsType + 1);
    ui->comboBoxTextEffects2->setCurrentIndex(mySettings2.effectsType + 1);

    // Background
    ui->comboBoxBackgoundType->setCurrentIndex(mySettings.backgroundType + 1);
    p.setColor(QPalette::Base,mySettings.backgroundColor);
    ui->graphicsViewBackgroundColor->setPalette(p);
    if(mySettings.backgroundType == 1)
        ui->lineEditBackPath->setText(mySettings.backgroundName);
    else if(mySettings.backgroundType == 2)
        ui->lineEditBackPath->setText(mySettings.backgroundVideoPath);

    ui->comboBoxBackgoundType2->setCurrentIndex(mySettings2.backgroundType + 1);
    p.setColor(QPalette::Base,mySettings2.backgroundColor);
    ui->graphicsViewBackgroundColor2->setPalette(p);
    if(mySettings2.backgroundType == 1)
        ui->lineEditBackPath2->setText(mySettings2.backgroundName);
    else if(mySettings2.backgroundType == 2)
        ui->lineEditBackPath2->setText(mySettings2.backgroundVideoPath);

    // Screen Use
    ui->checkBoxCommonLayout->setChecked(mySettings.isNotCommonLayout);
    on_checkBoxCommonLayout_stateChanged(mySettings.isNotCommonLayout);
    ui->spinBoxMaxScreen->setValue(mySettings.screenUse);
    ui->comboBoxScreenPosition->setCurrentIndex(mySettings.screenPosition);
    ui->checkBoxCommonLayout2->setChecked(mySettings2.isNotCommonLayout);
    on_checkBoxCommonLayout2_stateChanged(mySettings2.isNotCommonLayout);
    ui->spinBoxMaxScreen2->setValue(mySettings2.screenUse);
    ui->comboBoxScreenPosition2->setCurrentIndex(mySettings2.screenPosition);

    // Secondary Screen
    ui->checkBoxDisp2->setChecked(mySettings.useSameForDisp2);
    on_checkBoxDisp2_stateChanged(mySettings.useSameForDisp2);
}

void SongSettingWidget::setDispScreen2Visible(bool visible)
{
    ui->checkBoxDisp2->setVisible(visible);
    if(!mySettings.useSameForDisp2)
        ui->groupBoxDisplay2->setVisible(visible);
}

void SongSettingWidget::on_checkBoxCommonFont_stateChanged(int arg1)
{
    ui->toolButtonTextFont->setEnabled(arg1);
}

void SongSettingWidget::on_checkBoxCommonFont2_stateChanged(int arg1)
{
    ui->toolButtonTextFont2->setEnabled(arg1);
}

void SongSettingWidget::on_toolButtonTextFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.textFont,this);
    if(ok)
        mySettings.textFont = font;

    ui->labelFont->setText(getFontText(mySettings.textFont));
}

void SongSettingWidget::on_toolButtonTextFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.textFont,this);
    if(ok)
        mySettings2.textFont = font;

    ui->labelFont2->setText(getFontText(mySettings2.textFont));
}

void SongSettingWidget::on_checkBoxCommonColor_stateChanged(int arg1)
{
    ui->toolButtonTextColor->setEnabled(arg1);
    ui->toolButtonShadowColor->setEnabled(arg1);
}

void SongSettingWidget::on_checkBoxCommonColor2_stateChanged(int arg1)
{
    ui->toolButtonTextColor2->setEnabled(arg1);
    ui->toolButtonShadowColor2->setEnabled(arg1);
}

void SongSettingWidget::on_toolButtonTextColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textColor,this));
    if(c.isValid())
        mySettings.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonTextColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textColor,this));
    if(c.isValid())
        mySettings2.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);
}

void SongSettingWidget::on_toolButtonShadowColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textShadowColor,this));
    if(c.isValid())
        mySettings.textShadowColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textShadowColor);
    ui->graphicViewShadowColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonShadowColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textShadowColor,this));
    if(c.isValid())
        mySettings2.textShadowColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textShadowColor);
    ui->graphicViewShadowColor2->setPalette(p);
}

void SongSettingWidget::on_checkBoxStanzaTitle_stateChanged(int arg1)
{
    updateInfoButtons();
}

void SongSettingWidget::on_checkBoxStanzaTitle2_stateChanged(int arg1)
{
    updateInfoButtons2();
}

void SongSettingWidget::on_checkBoxSongKey_stateChanged(int arg1)
{
    updateInfoButtons();
}

void SongSettingWidget::on_checkBoxSongKey2_stateChanged(int arg1)
{
    updateInfoButtons2();
}

void SongSettingWidget::on_checkBoxSongNumber_stateChanged(int arg1)
{
    updateInfoButtons();
}

void SongSettingWidget::on_checkBoxSongNumber2_stateChanged(int arg1)
{
    updateInfoButtons2();
}

void SongSettingWidget::on_checkBoxSameInfoFont_stateChanged(int arg1)
{
    ui->toolButtonInfoFont->setEnabled(arg1);
    mySettings.isNotSameInfoFont = arg1;
}

void SongSettingWidget::on_checkBoxSameInfoFont2_stateChanged(int arg1)
{
    ui->toolButtonInfoFont2->setEnabled(arg1);
    mySettings2.isNotSameInfoFont = arg1;
}

void SongSettingWidget::on_toolButtonInfoFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.infoFont,this);
    if(ok)
        mySettings.infoFont = font;

    ui->labelInfoFont->setText(getFontText(mySettings.infoFont));
}

void SongSettingWidget::on_toolButtonInfoFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.infoFont,this);
    if(ok)
        mySettings2.infoFont = font;

    ui->labelInfoFont2->setText(getFontText(mySettings2.infoFont));
}

void SongSettingWidget::on_checkBoxSameInfoColor_stateChanged(int arg1)
{
    ui->toolButtonInfoColor->setEnabled(arg1);
    ui->toolButtonInfoShadowColor->setEnabled(arg1);
    mySettings.isNotSameInfoColor = arg1;
}

void SongSettingWidget::on_checkBoxSameInfoColor2_stateChanged(int arg1)
{
    ui->toolButtonInfoColor2->setEnabled(arg1);
    ui->toolButtonInfoShadowColor2->setEnabled(arg1);
    mySettings2.isNotSameInfoColor = arg1;
}

void SongSettingWidget::on_toolButtonInfoColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.infoColor,this));
    if(c.isValid())
        mySettings.infoColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.infoColor);
    ui->graphicViewInfoColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonInfoColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.infoColor,this));
    if(c.isValid())
        mySettings2.infoColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.infoColor);
    ui->graphicViewInfoColor2->setPalette(p);
}

void SongSettingWidget::on_toolButtonInfoShadowColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.infoShadowColor,this));
    if(c.isValid())
        mySettings.infoShadowColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.infoShadowColor);
    ui->graphicViewInfoShadowColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonInfoShadowColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.infoShadowColor,this));
    if(c.isValid())
        mySettings2.infoShadowColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.infoShadowColor);
    ui->graphicViewInfoShadowColor2->setPalette(p);
}

void SongSettingWidget::on_checkBoxSameEndingFont_stateChanged(int arg1)
{
    ui->toolButtonEndingFont->setEnabled(arg1);
}

void SongSettingWidget::on_checkBoxSameEndingFont2_stateChanged(int arg1)
{
    ui->toolButtonEndingFont2->setEnabled(arg1);
}

void SongSettingWidget::on_toolButtonEndingFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.endingFont,this);
    if(ok)
        mySettings.endingFont = font;

    ui->labelEndingFont->setText(getFontText(mySettings.endingFont));
}

void SongSettingWidget::on_toolButtonEndingFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.endingFont,this);
    if(ok)
        mySettings2.endingFont = font;

    ui->labelEndingFont2->setText(getFontText(mySettings2.endingFont));
}

void SongSettingWidget::on_checkBoxSameEndingColor_stateChanged(int arg1)
{
    ui->toolButtonEndingColor->setEnabled(arg1);
    ui->toolButtonEndingShadowColor->setEnabled(arg1);
}

void SongSettingWidget::on_checkBoxSameEndingColor2_stateChanged(int arg1)
{
    ui->toolButtonEndingColor2->setEnabled(arg1);
    ui->toolButtonEndingShadowColor2->setEnabled(arg1);
}

void SongSettingWidget::on_toolButtonEndingColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.endingColor,this));
    if(c.isValid())
        mySettings.endingColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.endingColor);
    ui->graphicViewEndingColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonEndingColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.endingColor,this));
    if(c.isValid())
        mySettings2.endingColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.endingColor);
    ui->graphicViewEndingColor2->setPalette(p);
}

void SongSettingWidget::on_toolButtonEndingShadowColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.endingShadowColor,this));
    if(c.isValid())
        mySettings.endingShadowColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.endingShadowColor);
    ui->graphicViewEndingShadowColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonEndingShadowColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.endingShadowColor,this));
    if(c.isValid())
        mySettings2.endingShadowColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.endingShadowColor);
    ui->graphicViewEndingShadowColor2->setPalette(p);
}

void SongSettingWidget::on_comboBoxBackgoundType_currentIndexChanged(int index)
{
    mySettings.backgroundType = index - 1;

    ui->buttonBrowseBackground->setEnabled(mySettings.backgroundType != -1);

    if(mySettings.backgroundType == 0)
    {
        ui->graphicsViewBackgroundColor->setVisible(true);
        ui->lineEditBackPath->setVisible(false);
        ui->buttonBrowseBackground->setText(tr("Select Color..."));
    }
    else if(mySettings.backgroundType >= 1)
    {
        if(mySettings.backgroundType == 1)
            ui->lineEditBackPath->setText(mySettings.backgroundName);
        else if(mySettings.backgroundType == 2)
            ui->lineEditBackPath->setText(mySettings.backgroundVideoPath);
        ui->graphicsViewBackgroundColor->setVisible(false);
        ui->lineEditBackPath->setVisible(true);
        ui->buttonBrowseBackground->setText(tr("Browse..."));
    }
}

void SongSettingWidget::on_comboBoxBackgoundType2_currentIndexChanged(int index)
{
    mySettings2.backgroundType = index - 1;

    ui->buttonBrowseBackground2->setEnabled(mySettings.backgroundType != -1);

    if(mySettings2.backgroundType == 0)
    {
        ui->graphicsViewBackgroundColor2->setVisible(true);
        ui->lineEditBackPath2->setVisible(false);
        ui->buttonBrowseBackground2->setText(tr("Select Color..."));
    }
    else if(mySettings2.backgroundType >= 1)
    {
        if(mySettings2.backgroundType == 1)
            ui->lineEditBackPath2->setText(mySettings2.backgroundName);
        else if(mySettings2.backgroundType == 2)
            ui->lineEditBackPath2->setText(mySettings2.backgroundVideoPath);
        ui->graphicsViewBackgroundColor2->setVisible(false);
        ui->lineEditBackPath2->setVisible(true);
        ui->buttonBrowseBackground2->setText(tr("Browse..."));
    }
}

void SongSettingWidget::on_buttonBrowseBackground_clicked()
{
    QString filename;
    if (mySettings.backgroundType == 0)
    {
        QColor c(QColorDialog::getColor(mySettings.backgroundColor,this));
        if(c.isValid())
            mySettings.backgroundColor = c;
        QPalette p;
        p.setColor(QPalette::Base,mySettings.backgroundColor);
        ui->graphicsViewBackgroundColor->setPalette(p);
    }
    else if (mySettings.backgroundType == 1)
    {
        filename = QFileDialog::getOpenFileName(this, tr("Select a image for Bible wallpaper"),
                                                ".", tr("Images(%1)").arg(getSupportedImageFormats()));
        if(!filename.isNull())
        {
            QPixmap pix(filename);
            if(pix.width()>1280 || pix.height()>1280)
                mySettings.backgroundPix = pix.scaled(1280,1280,Qt::KeepAspectRatio);
            else
                mySettings.backgroundPix = pix;
            QFileInfo fip(filename);
            filename = fip.fileName();
            mySettings.backgroundName = filename;
            ui->lineEditBackPath->setText(filename);
        }
    }
    else if (mySettings.backgroundType == 2)
    {
        filename = QFileDialog::getOpenFileName(this, tr("Select a video for Bible wallpaper"),
                                                ".", "*");
        if(!filename.isNull())
        {
            mySettings.backgroundVideoPath = filename;
            QFileInfo fiv(filename);
            filename = fiv.fileName();

            ui->lineEditBackPath->setText(filename);
        }
    }
}

void SongSettingWidget::on_buttonBrowseBackground2_clicked()
{
    QString filename;
    if (mySettings2.backgroundType == 0)
    {
        QColor c(QColorDialog::getColor(mySettings2.backgroundColor,this));
        if(c.isValid())
            mySettings2.backgroundColor = c;
        QPalette p;
        p.setColor(QPalette::Base,mySettings2.backgroundColor);
        ui->graphicsViewBackgroundColor2->setPalette(p);
    }
    else if (mySettings2.backgroundType == 1)
    {
        filename = QFileDialog::getOpenFileName(this, tr("Select a image for Bible wallpaper"),
                                                ".", tr("Images(%1)").arg(getSupportedImageFormats()));
        if(!filename.isNull())
        {
            QPixmap pix(filename);
            if(pix.width()>1280 || pix.height()>1280)
                mySettings2.backgroundPix = pix.scaled(1280,1280,Qt::KeepAspectRatio);
            else
                mySettings2.backgroundPix = pix;
            QFileInfo fip(filename);
            filename = fip.fileName();
            mySettings2.backgroundName = filename;
            ui->lineEditBackPath2->setText(filename);
        }
    }
    else if (mySettings2.backgroundType == 2)
    {
        filename = QFileDialog::getOpenFileName(this, tr("Select a video for Bible wallpaper"),
                                                ".", "*");
        if(!filename.isNull())
        {
            mySettings2.backgroundVideoPath = filename;
            QFileInfo fiv(filename);
            filename = fiv.fileName();

            ui->lineEditBackPath2->setText(filename);
        }
    }
}

void SongSettingWidget::on_checkBoxCommonLayout_stateChanged(int arg1)
{
    ui->spinBoxMaxScreen->setEnabled(arg1);
    ui->comboBoxScreenPosition->setEnabled(arg1);
}

void SongSettingWidget::on_checkBoxCommonLayout2_stateChanged(int arg1)
{
    ui->spinBoxMaxScreen2->setEnabled(arg1);
    ui->comboBoxScreenPosition2->setEnabled(arg1);
}

void SongSettingWidget::on_checkBoxDisp2_stateChanged(int arg1)
{
    mySettings.useSameForDisp2 = arg1;
    ui->groupBoxDisplay2->setVisible(!arg1);
}

void SongSettingWidget::on_pushButtonDefault_clicked()
{
    SongSettings s;
    mySettings = s;
    mySettings2 = s;
    loadSettings();
}

QString SongSettingWidget::getFontText(QFont font)
{
    QString f = font.toString().split(",").at(0);
    QString st(QString("%1: %2").arg(f).arg(font.pointSize()));
    if(font.bold())
        st += ", " + tr("Bold");
    if(font.italic())
        st += ", " + tr("Italic");
    if(font.strikeOut())
        st += ", " + tr("StrikeOut");
    if(font.underline())
        st += ", " + tr("Underline");

    return st;
}

void SongSettingWidget::updateInfoButtons()
{
    bool t,k,n;
    t = ui->checkBoxStanzaTitle->isChecked();
    k = ui->checkBoxSongKey->isChecked();
    n = ui->checkBoxSongNumber->isChecked();

    if(!t && !k && !n)
    {
        ui->checkBoxSameInfoFont->setEnabled(false);
        ui->toolButtonInfoFont->setEnabled(false);
        ui->checkBoxSameInfoColor->setEnabled(false);
        ui->toolButtonInfoColor->setEnabled(false);
        ui->toolButtonInfoShadowColor->setEnabled(false);
        ui->comboBoxInfoAlign->setEnabled(false);
    }
    else
    {
        ui->checkBoxSameInfoFont->setEnabled(true);
        ui->toolButtonInfoFont->setEnabled(mySettings.isNotSameInfoFont);
        ui->checkBoxSameInfoColor->setEnabled(true);
        ui->toolButtonInfoColor->setEnabled(mySettings.isNotSameInfoColor);
        ui->toolButtonInfoShadowColor->setEnabled(mySettings.isNotSameInfoColor);
        ui->comboBoxInfoAlign->setEnabled(true);
    }
}

void SongSettingWidget::updateInfoButtons2()
{
    bool t,k,n;
    t = ui->checkBoxStanzaTitle2->isChecked();
    k = ui->checkBoxSongKey2->isChecked();
    n = ui->checkBoxSongNumber2->isChecked();

    if(!t && !k && !n)
    {
        ui->checkBoxSameInfoFont2->setEnabled(false);
        ui->toolButtonInfoFont2->setEnabled(false);
        ui->checkBoxSameInfoColor2->setEnabled(false);
        ui->toolButtonInfoColor2->setEnabled(false);
        ui->toolButtonInfoShadowColor2->setEnabled(false);
        ui->comboBoxInfoAlign2->setEnabled(false);
    }
    else
    {
        ui->checkBoxSameInfoFont2->setEnabled(true);
        ui->toolButtonInfoFont2->setEnabled(mySettings2.isNotSameInfoFont);
        ui->checkBoxSameInfoColor2->setEnabled(true);
        ui->toolButtonInfoColor2->setEnabled(mySettings2.isNotSameInfoColor);
        ui->toolButtonInfoShadowColor2->setEnabled(mySettings2.isNotSameInfoColor);
        ui->comboBoxInfoAlign2->setEnabled(true);
    }
}

