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
    mySettings.resetChangeHandles();
    mySettings2.resetChangeHandles();
}

void SongSettingWidget::getSettings(SongSettings &settings, SongSettings &settings2)
{
    settings = mySettings;
    settings2 = mySettings2;
    mySettings.resetChangeHandles();
    mySettings2.resetChangeHandles();
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
    on_comboBoxBackgoundType_currentIndexChanged(mySettings.backgroundType + 1);
    p.setColor(QPalette::Base,mySettings.backgroundColor);
    ui->graphicsViewBackgroundColor->setPalette(p);
    if(mySettings.backgroundType == 1)
        ui->lineEditBackPath->setText(mySettings.backgroundName);
    else if(mySettings.backgroundType == 2)
        ui->lineEditBackPath->setText(mySettings.backgroundVideoPath);

    ui->comboBoxBackgoundType2->setCurrentIndex(mySettings2.backgroundType + 1);
    on_comboBoxBackgoundType2_currentIndexChanged(mySettings2.backgroundType + 1);
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
    mySettings.isNotCommonFont = arg1;
    mySettings.isChangedNotFont = true;
}

void SongSettingWidget::on_checkBoxCommonFont2_stateChanged(int arg1)
{
    ui->toolButtonTextFont2->setEnabled(arg1);
    mySettings2.isNotCommonFont = arg1;
    mySettings2.isChangedNotFont = true;
}

void SongSettingWidget::on_toolButtonTextFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.textFont,this);
    if(ok)
    {
        mySettings.textFont = font;
        mySettings.isChangedTextFont = true;
    }

    ui->labelFont->setText(getFontText(mySettings.textFont));
}

void SongSettingWidget::on_toolButtonTextFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.textFont,this);
    if(ok)
    {
        mySettings2.textFont = font;
        mySettings2.isChangedTextFont = true;
    }

    ui->labelFont2->setText(getFontText(mySettings2.textFont));
}

void SongSettingWidget::on_checkBoxCommonColor_stateChanged(int arg1)
{
    ui->toolButtonTextColor->setEnabled(arg1);
    ui->toolButtonShadowColor->setEnabled(arg1);
    mySettings.isNotCommonColor = arg1;
    mySettings.isChangedNotColor = true;
}

void SongSettingWidget::on_checkBoxCommonColor2_stateChanged(int arg1)
{
    ui->toolButtonTextColor2->setEnabled(arg1);
    ui->toolButtonShadowColor2->setEnabled(arg1);
    mySettings2.isNotCommonColor = arg1;
    mySettings2.isChangedNotColor = true;
}

void SongSettingWidget::on_toolButtonTextColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textColor,this));
    if(c.isValid())
    {
        mySettings.textColor = c;
        mySettings.isChangedTextColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonTextColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textColor,this));
    if(c.isValid())
    {
        mySettings2.textColor = c;
        mySettings2.isChangedTextColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);
}

void SongSettingWidget::on_toolButtonShadowColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textShadowColor,this));
    if(c.isValid())
    {
        mySettings.textShadowColor = c;
        mySettings.isChangedTextShadowColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textShadowColor);
    ui->graphicViewShadowColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonShadowColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textShadowColor,this));
    if(c.isValid())
    {
        mySettings2.textShadowColor = c;
         mySettings2.isChangedTextShadowColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textShadowColor);
    ui->graphicViewShadowColor2->setPalette(p);
}

void SongSettingWidget::on_comboBoxVerticalAling_currentIndexChanged(int index)
{
    mySettings.textAlingmentV = index;
    mySettings.isChangedAlingV = true;
}

void SongSettingWidget::on_comboBoxVerticalAling2_currentIndexChanged(int index)
{
    mySettings2.textAlingmentV = index;
    mySettings2.isChangedAlingV = true;
}

void SongSettingWidget::on_comboBoxHorizontalAling_currentIndexChanged(int index)
{
    mySettings.textAlingmentH = index;
    mySettings.isChangedAlingH = true;
}

void SongSettingWidget::on_comboBoxHorizontalAling2_currentIndexChanged(int index)
{
    mySettings2.textAlingmentH = index;
    mySettings2.isChangedAlingH = true;
}

void SongSettingWidget::on_checkBoxStanzaTitle_stateChanged(int arg1)
{
    mySettings.showStanzaTitle = arg1;
    mySettings.isChangedShowTitle = true;
    updateInfoButtons();
}

void SongSettingWidget::on_checkBoxStanzaTitle2_stateChanged(int arg1)
{
    mySettings2.showStanzaTitle = arg1;
    mySettings2.isChangedShowTitle = true;
    updateInfoButtons2();
}

void SongSettingWidget::on_checkBoxSongKey_stateChanged(int arg1)
{
    mySettings.showSongKey = arg1;
    mySettings.isChangedShowKey = true;
    updateInfoButtons();
}

void SongSettingWidget::on_checkBoxSongKey2_stateChanged(int arg1)
{
    mySettings2.showSongKey = arg1;
    mySettings2.isChangedShowKey = true;
    updateInfoButtons2();
}

void SongSettingWidget::on_checkBoxSongNumber_stateChanged(int arg1)
{
    mySettings.showSongNumber = arg1;
    mySettings.isChangedShowNum = true;
    updateInfoButtons();
}

void SongSettingWidget::on_checkBoxSongNumber2_stateChanged(int arg1)
{
    mySettings2.showSongNumber = arg1;
    mySettings2.isChangedShowNum = true;
    updateInfoButtons2();
}

void SongSettingWidget::on_checkBoxSameInfoFont_stateChanged(int arg1)
{
    ui->toolButtonInfoFont->setEnabled(arg1);
    mySettings.isNotSameInfoFont = arg1;
    mySettings.isChangedNotSameInfoFont = true;
}

void SongSettingWidget::on_checkBoxSameInfoFont2_stateChanged(int arg1)
{
    ui->toolButtonInfoFont2->setEnabled(arg1);
    mySettings2.isNotSameInfoFont = arg1;
    mySettings2.isChangedNotSameInfoFont = true;
}

void SongSettingWidget::on_toolButtonInfoFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.infoFont,this);
    if(ok)
    {
        mySettings.infoFont = font;
        mySettings.isChangedInfoFont = true;
    }

    ui->labelInfoFont->setText(getFontText(mySettings.infoFont));
}

void SongSettingWidget::on_toolButtonInfoFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.infoFont,this);
    if(ok)
    {
        mySettings2.infoFont = font;
        mySettings2.isChangedInfoFont = true;
    }

    ui->labelInfoFont2->setText(getFontText(mySettings2.infoFont));
}

void SongSettingWidget::on_checkBoxSameInfoColor_stateChanged(int arg1)
{
    ui->toolButtonInfoColor->setEnabled(arg1);
    ui->toolButtonInfoShadowColor->setEnabled(arg1);
    mySettings.isNotSameInfoColor = arg1;
    mySettings.isChangedNotSameInfoColor = true;
}

void SongSettingWidget::on_checkBoxSameInfoColor2_stateChanged(int arg1)
{
    ui->toolButtonInfoColor2->setEnabled(arg1);
    ui->toolButtonInfoShadowColor2->setEnabled(arg1);
    mySettings2.isNotSameInfoColor = arg1;
    mySettings2.isChangedNotSameInfoColor = true;
}

void SongSettingWidget::on_toolButtonInfoColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.infoColor,this));
    if(c.isValid())
    {
        mySettings.infoColor = c;
        mySettings.isChangedInfoColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings.infoColor);
    ui->graphicViewInfoColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonInfoColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.infoColor,this));
    if(c.isValid())
    {
        mySettings2.infoColor = c;
        mySettings2.isChangedInfoColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.infoColor);
    ui->graphicViewInfoColor2->setPalette(p);
}

void SongSettingWidget::on_toolButtonInfoShadowColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.infoShadowColor,this));
    if(c.isValid())
    {
        mySettings.infoShadowColor = c;
        mySettings.isChangedInfoShadColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings.infoShadowColor);
    ui->graphicViewInfoShadowColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonInfoShadowColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.infoShadowColor,this));
    if(c.isValid())
    {
        mySettings2.infoShadowColor = c;
        mySettings.isChangedInfoShadColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.infoShadowColor);
    ui->graphicViewInfoShadowColor2->setPalette(p);
}

void SongSettingWidget::on_comboBoxInfoAlign_currentIndexChanged(int index)
{
    mySettings.infoAling = index;
    mySettings.isChangedInfoAlign = true;
}

void SongSettingWidget::on_comboBoxInfoAlign2_currentIndexChanged(int index)
{
    mySettings2.infoAling = index;
    mySettings2.isChangedInfoAlign = true;
}

void SongSettingWidget::on_groupBoxSongEnding_toggled(bool arg1)
{
    mySettings.showSongEnding = arg1;
    mySettings.isChangedShowEnding = true;
}

void SongSettingWidget::on_groupBoxSongEnding2_toggled(bool arg1)
{
    mySettings2.showSongEnding = arg1;
    mySettings2.isChangedShowEnding = true;
}

void SongSettingWidget::on_checkBoxSameEndingFont_stateChanged(int arg1)
{
    ui->toolButtonEndingFont->setEnabled(arg1);
    mySettings.isNotSameEndingFont = arg1;
    mySettings.isChangedNotSameEndingFont = true;
}

void SongSettingWidget::on_checkBoxSameEndingFont2_stateChanged(int arg1)
{
    ui->toolButtonEndingFont2->setEnabled(arg1);
    mySettings2.isNotSameEndingFont = arg1;
    mySettings2.isChangedNotSameEndingFont = true;
}

void SongSettingWidget::on_toolButtonEndingFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.endingFont,this);
    if(ok)
    {
        mySettings.endingFont = font;
        mySettings.isChangedEndingFont = true;
    }

    ui->labelEndingFont->setText(getFontText(mySettings.endingFont));
}

void SongSettingWidget::on_toolButtonEndingFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.endingFont,this);
    if(ok)
    {
        mySettings2.endingFont = font;
        mySettings2.isChangedEndingFont = true;
    }

    ui->labelEndingFont2->setText(getFontText(mySettings2.endingFont));
}

void SongSettingWidget::on_checkBoxSameEndingColor_stateChanged(int arg1)
{
    ui->toolButtonEndingColor->setEnabled(arg1);
    ui->toolButtonEndingShadowColor->setEnabled(arg1);
    mySettings.isNotSameEndingColor = arg1;
    mySettings.isChangedNotSameEndingColor = true;
}

void SongSettingWidget::on_checkBoxSameEndingColor2_stateChanged(int arg1)
{
    ui->toolButtonEndingColor2->setEnabled(arg1);
    ui->toolButtonEndingShadowColor2->setEnabled(arg1);
    mySettings2.isNotSameEndingColor = arg1;
    mySettings2.isChangedNotSameEndingColor = true;
}

void SongSettingWidget::on_toolButtonEndingColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.endingColor,this));
    if(c.isValid())
    {
        mySettings.endingColor = c;
        mySettings.isChangedEndingColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings.endingColor);
    ui->graphicViewEndingColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonEndingColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.endingColor,this));
    if(c.isValid())
    {
        mySettings2.endingColor = c;
        mySettings2.isChangedEndingColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.endingColor);
    ui->graphicViewEndingColor2->setPalette(p);
}

void SongSettingWidget::on_toolButtonEndingShadowColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.endingShadowColor,this));
    if(c.isValid())
    {
        mySettings.endingShadowColor = c;
        mySettings.isChangedEndingShadColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings.endingShadowColor);
    ui->graphicViewEndingShadowColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonEndingShadowColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.endingShadowColor,this));
    if(c.isValid())
    {
        mySettings2.endingShadowColor = c;
        mySettings2.isChangedEndingShadColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.endingShadowColor);
    ui->graphicViewEndingShadowColor2->setPalette(p);
}

void SongSettingWidget::on_comboBoxEndingType_currentIndexChanged(int index)
{
    mySettings.endingType = index;
    mySettings.isChangedEndingType = true;
}

void SongSettingWidget::on_comboBoxEndingType2_currentIndexChanged(int index)
{
    mySettings2.endingType = index;
    mySettings2.isChangedEndingType = true;
}

void SongSettingWidget::on_comboBoxEndingPosition_currentIndexChanged(int index)
{
    mySettings.endingPosition = index;
    mySettings.isChangedEndingPosition = true;
}

void SongSettingWidget::on_comboBoxEndingPosition2_currentIndexChanged(int index)
{
    mySettings2.endingPosition = index;
    mySettings2.isChangedEndingPosition = true;
}

void SongSettingWidget::on_comboBoxTransitionType_currentIndexChanged(int index)
{
    mySettings.transitionType = index - 1;
    mySettings.isChangesTranType = true;
}

void SongSettingWidget::on_comboBoxTransitionType2_currentIndexChanged(int index)
{
    mySettings2.transitionType = index - 1;
    mySettings2.isChangesTranType = true;
}

void SongSettingWidget::on_comboBoxTextEffects_currentIndexChanged(int index)
{
    mySettings.effectsType = index - 1;
    mySettings.isChangedEffectType = true;
}

void SongSettingWidget::on_comboBoxTextEffects2_currentIndexChanged(int index)
{
    mySettings2.effectsType = index - 1;
    mySettings2.isChangedEffectType = true;
}

void SongSettingWidget::on_comboBoxBackgoundType_currentIndexChanged(int index)
{
    mySettings.backgroundType = index - 1;
    mySettings.isChangedBackType = true;

    ui->toolButtonBrowseBackround->setVisible(mySettings.backgroundType != -1);
    ui->graphicsViewBackgroundColor->setVisible(mySettings.backgroundType != -1);
    ui->lineEditBackPath->setVisible(mySettings.backgroundType != -1);
//    ui->buttonBrowseBackground->setEnabled(mySettings.backgroundType != -1);

    if(mySettings.backgroundType == 0)
    {
        ui->graphicsViewBackgroundColor->setVisible(true);
        ui->lineEditBackPath->setVisible(false);
        ui->toolButtonBrowseBackround->setText(tr("Select Color..."));
    }
    else if(mySettings.backgroundType >= 1)
    {
        if(mySettings.backgroundType == 1)
            ui->lineEditBackPath->setText(mySettings.backgroundName);
        else if(mySettings.backgroundType == 2)
            ui->lineEditBackPath->setText(mySettings.backgroundVideoPath);
        ui->graphicsViewBackgroundColor->setVisible(false);
        ui->lineEditBackPath->setVisible(true);
        ui->toolButtonBrowseBackround->setText(tr("Browse..."));
    }
}

void SongSettingWidget::on_comboBoxBackgoundType2_currentIndexChanged(int index)
{
    mySettings2.backgroundType = index - 1;
    mySettings2.isChangedBackType = true;

    ui->toolButtonBrowseBackround2->setVisible(mySettings.backgroundType != -1);
    ui->graphicsViewBackgroundColor2->setVisible(mySettings.backgroundType != -1);
    ui->lineEditBackPath2->setVisible(mySettings.backgroundType != -1);
//    ui->buttonBrowseBackground2->setEnabled(mySettings.backgroundType != -1);

    if(mySettings2.backgroundType == 0)
    {
        ui->graphicsViewBackgroundColor2->setVisible(true);
        ui->lineEditBackPath2->setVisible(false);
        ui->toolButtonBrowseBackround2->setText(tr("Select Color..."));
    }
    else if(mySettings2.backgroundType >= 1)
    {
        if(mySettings2.backgroundType == 1)
            ui->lineEditBackPath2->setText(mySettings2.backgroundName);
        else if(mySettings2.backgroundType == 2)
            ui->lineEditBackPath2->setText(mySettings2.backgroundVideoPath);
        ui->graphicsViewBackgroundColor2->setVisible(false);
        ui->lineEditBackPath2->setVisible(true);
        ui->toolButtonBrowseBackround2->setText(tr("Browse..."));
    }
}

void SongSettingWidget::on_toolButtonBrowseBackround_clicked()
{
    QString filename;
    if (mySettings.backgroundType == 0)
    {
        QColor c(QColorDialog::getColor(mySettings.backgroundColor,this));
        if(c.isValid())
        {
            mySettings.backgroundColor = c;
            mySettings.isChangedBackColor = true;
        }
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
            mySettings.isChangedBackPix = true;
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
            mySettings.isChangedBackVid = true;
        }
    }
}

void SongSettingWidget::on_toolButtonBrowseBackround2_clicked()
{
    QString filename;
    if (mySettings2.backgroundType == 0)
    {
        QColor c(QColorDialog::getColor(mySettings2.backgroundColor,this));
        if(c.isValid())
        {
            mySettings2.backgroundColor = c;
            mySettings2.isChangedBackColor = true;
        }
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
            mySettings2.isChangedBackPix = true;
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
            mySettings2.isChangedBackVid = true;
        }
    }
}

void SongSettingWidget::on_checkBoxCommonLayout_stateChanged(int arg1)
{
    ui->spinBoxMaxScreen->setEnabled(arg1);
    ui->comboBoxScreenPosition->setEnabled(arg1);
    mySettings.isNotCommonLayout = arg1;
    mySettings.isChangedNotLayout = true;
}

void SongSettingWidget::on_checkBoxCommonLayout2_stateChanged(int arg1)
{
    ui->spinBoxMaxScreen2->setEnabled(arg1);
    ui->comboBoxScreenPosition2->setEnabled(arg1);
    mySettings2.isNotCommonLayout = arg1;
    mySettings2.isChangedNotLayout = true;
}

void SongSettingWidget::on_spinBoxMaxScreen_editingFinished()
{
    mySettings.screenUse = ui->spinBoxMaxScreen->value();
    mySettings.isChangedScreenUse = true;
}

void SongSettingWidget::on_spinBoxMaxScreen2_editingFinished()
{
    mySettings2.screenUse = ui->spinBoxMaxScreen2->value();
    mySettings2.isChangedScreenUse = true;
}

void SongSettingWidget::on_comboBoxScreenPosition_currentIndexChanged(int index)
{
    mySettings.screenPosition = index;
    mySettings.isChangedScreenPos = true;
}

void SongSettingWidget::on_comboBoxScreenPosition2_currentIndexChanged(int index)
{
    mySettings2.screenPosition = index;
    mySettings2.isChangedScreenPos = true;
}

void SongSettingWidget::on_checkBoxDisp2_stateChanged(int arg1)
{
    mySettings.useSameForDisp2 = arg1;
    mySettings.isChangedSameDisp2 = true;
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
