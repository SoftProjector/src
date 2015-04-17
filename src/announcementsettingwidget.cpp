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

void AnnouncementSettingWidget::changeEvent(QEvent *e)
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

void AnnouncementSettingWidget::setSettings(TextSettings &settings, TextSettings &settings2)
{
    mySettings = settings;
    mySettings2 = settings2;
    loadSettings();
}

void AnnouncementSettingWidget::loadSettings()
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

    mySettings.resetMainChangeHandles();
    mySettings2.resetMainChangeHandles();
}

void AnnouncementSettingWidget::getSettings(TextSettings &settings, TextSettings &settings2)
{
    settings = mySettings;
    settings2 = mySettings2;
    mySettings.resetMainChangeHandles();
    mySettings2.resetMainChangeHandles();
}


void AnnouncementSettingWidget::on_checkBoxCommonFont_stateChanged(int arg1)
{
    ui->toolButtonTextFont->setEnabled(arg1);
    mySettings.isNotCommonFont = arg1;
    mySettings.isChangedNotFont = true;
}

void AnnouncementSettingWidget::on_checkBoxCommonFont2_stateChanged(int arg1)
{
    ui->toolButtonTextFont2->setEnabled(arg1);
    mySettings2.isNotCommonFont = arg1;
    mySettings2.isChangedNotFont = true;
}

void AnnouncementSettingWidget::on_toolButtonTextFont_clicked()
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

void AnnouncementSettingWidget::on_toolButtonTextFont2_clicked()
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

void AnnouncementSettingWidget::on_checkBoxCommonColor_stateChanged(int arg1)
{
    ui->toolButtonTextColor->setEnabled(arg1);
    ui->toolButtonShadowColor->setEnabled(arg1);
    mySettings.isNotCommonColor = arg1;
    mySettings.isChangedNotColor = true;
}

void AnnouncementSettingWidget::on_checkBoxCommonColor2_stateChanged(int arg1)
{
    ui->toolButtonTextColor2->setEnabled(arg1);
    ui->toolButtonShadowColor2->setEnabled(arg1);
    mySettings2.isNotCommonColor = arg1;
    mySettings2.isChangedNotColor = true;
}

void AnnouncementSettingWidget::on_toolButtonTextColor_clicked()
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

void AnnouncementSettingWidget::on_toolButtonTextColor2_clicked()
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

void AnnouncementSettingWidget::on_toolButtonShadowColor_clicked()
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

void AnnouncementSettingWidget::on_toolButtonShadowColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textShadowColor,this));
    if(c.isValid())
    {
        mySettings2.textShadowColor = c;
        mySettings.isChangedTextShadowColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textShadowColor);
    ui->graphicViewShadowColor2->setPalette(p);
}

void AnnouncementSettingWidget::on_comboBoxVerticalAling_currentIndexChanged(int index)
{
    mySettings.textAlingmentV = index;
    mySettings.isChangedAlingV = true;
}

void AnnouncementSettingWidget::on_comboBoxVerticalAling2_currentIndexChanged(int index)
{
    mySettings2.textAlingmentV = index;
    mySettings2.isChangedAlingV = true;
}

void AnnouncementSettingWidget::on_comboBoxHorizontalAling_currentIndexChanged(int index)
{
    mySettings.textAlingmentH = index;
    mySettings.isChangedAlingH = true;
}

void AnnouncementSettingWidget::on_comboBoxHorizontalAling2_currentIndexChanged(int index)
{
    mySettings2.textAlingmentH = index;
    mySettings2.isChangedAlingH = true;
}

void AnnouncementSettingWidget::on_comboBoxTransitionType_currentIndexChanged(int index)
{
    mySettings.transitionType = index - 1;
    mySettings.isChangesTranType = true;
}

void AnnouncementSettingWidget::on_comboBoxTransitionType2_currentIndexChanged(int index)
{
    mySettings2.transitionType = index - 1;
    mySettings2.isChangesTranType = true;
}

void AnnouncementSettingWidget::on_comboBoxTextEffects_currentIndexChanged(int index)
{
    mySettings.effectsType = index - 1;
    mySettings.isChangedEffectType = true;
}

void AnnouncementSettingWidget::on_comboBoxTextEffects2_currentIndexChanged(int index)
{
    mySettings2.effectsType = index - 1;
    mySettings2.isChangedEffectType = true;
}

void AnnouncementSettingWidget::on_comboBoxBackgoundType_currentIndexChanged(int index)
{
    mySettings.backgroundType = index - 1;

    ui->toolButtonBrowseBackground->setVisible(mySettings.backgroundType != -1);
    ui->graphicsViewBackgroundColor->setVisible(mySettings.backgroundType != -1);
    ui->lineEditBackPath->setVisible(mySettings.backgroundType != -1);

    if(mySettings.backgroundType == 0)
    {
        ui->graphicsViewBackgroundColor->setVisible(true);
        ui->lineEditBackPath->setVisible(false);
        ui->toolButtonBrowseBackground->setText(tr("Select Color..."));
    }
    else if(mySettings.backgroundType >= 1)
    {
        if(mySettings.backgroundType == 1)
            ui->lineEditBackPath->setText(mySettings.backgroundName);
        else if(mySettings.backgroundType == 2)
            ui->lineEditBackPath->setText(mySettings.backgroundVideoPath);
        ui->graphicsViewBackgroundColor->setVisible(false);
        ui->lineEditBackPath->setVisible(true);
        ui->toolButtonBrowseBackground->setText(tr("Browse..."));
    }
    mySettings.isChangedBackType = true;
}

void AnnouncementSettingWidget::on_comboBoxBackgoundType2_currentIndexChanged(int index)
{
    mySettings2.backgroundType = index - 1;

    ui->toolButtonBrowseBackground->setVisible(mySettings.backgroundType != -1);
    ui->graphicsViewBackgroundColor->setVisible(mySettings.backgroundType != -1);
    ui->lineEditBackPath->setVisible(mySettings.backgroundType != -1);

    if(mySettings2.backgroundType == 0)
    {
        ui->graphicsViewBackgroundColor2->setVisible(true);
        ui->lineEditBackPath2->setVisible(false);
        ui->toolButtonBrowseBackground2->setText(tr("Select Color..."));
    }
    else if(mySettings2.backgroundType >= 1)
    {
        if(mySettings2.backgroundType == 1)
            ui->lineEditBackPath2->setText(mySettings2.backgroundName);
        else if(mySettings2.backgroundType == 2)
            ui->lineEditBackPath2->setText(mySettings2.backgroundVideoPath);
        ui->graphicsViewBackgroundColor2->setVisible(false);
        ui->lineEditBackPath2->setVisible(true);
        ui->toolButtonBrowseBackground2->setText(tr("Browse..."));
    }
    mySettings2.isChangedBackType = true;
}

void AnnouncementSettingWidget::on_toolButtonBrowseBackground_clicked()
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

void AnnouncementSettingWidget::on_toolButtonBrowseBackground2_clicked()
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
            mySettings.isChangedBackVid = true;
        }
    }
}

void AnnouncementSettingWidget::on_checkBoxCommonLayout_stateChanged(int arg1)
{
    ui->spinBoxMaxScreen->setEnabled(arg1);
    ui->comboBoxScreenPosition->setEnabled(arg1);
    mySettings.isNotCommonLayout = arg1;
    mySettings.isChangedNotLayout = true;
}

void AnnouncementSettingWidget::on_checkBoxCommonLayout2_stateChanged(int arg1)
{
    ui->spinBoxMaxScreen2->setEnabled(arg1);
    ui->comboBoxScreenPosition2->setEnabled(arg1);
    mySettings2.isNotCommonLayout = arg1;
    mySettings2.isChangedNotLayout = true;
}

void AnnouncementSettingWidget::on_spinBoxMaxScreen_editingFinished()
{
    mySettings.screenUse = ui->spinBoxMaxScreen->value();
    mySettings.isChangedScreenUse = true;
}

void AnnouncementSettingWidget::on_spinBoxMaxScreen2_editingFinished()
{
    mySettings2.screenUse = ui->spinBoxMaxScreen2->value();
    mySettings2.isChangedScreenUse = true;
}

void AnnouncementSettingWidget::on_comboBoxScreenPosition_currentIndexChanged(int index)
{
    mySettings.screenPosition = index;
    mySettings.isChangedScreenPos = true;
}

void AnnouncementSettingWidget::on_comboBoxScreenPosition2_currentIndexChanged(int index)
{
    mySettings2.screenPosition = index;
    mySettings2.isChangedScreenPos = true;
}

void AnnouncementSettingWidget::on_checkBoxDisp2_stateChanged(int arg1)
{
    mySettings.useSameForDisp2 = arg1;
    mySettings.isChangedSameDisp2 = true;
    ui->groupBoxUseDisp2->setVisible(!arg1);
}

void AnnouncementSettingWidget::setDispScreen2Visible(bool visible)
{
    ui->checkBoxDisp2->setVisible(visible);
    if(!mySettings.useSameForDisp2)
        ui->groupBoxUseDisp2->setVisible(visible);
}

void AnnouncementSettingWidget::on_pushButtonDefault_clicked()
{
    TextSettings a;
    mySettings = a;
    mySettings2 = a;
    loadSettings();
}

QString AnnouncementSettingWidget::getFontText(QFont font)
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
