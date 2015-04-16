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
}

void AnnouncementSettingWidget::getSettings(TextSettings &settings, TextSettings &settings2)
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


void AnnouncementSettingWidget::on_checkBoxCommonFont_stateChanged(int arg1)
{
    ui->toolButtonTextFont->setEnabled(arg1);
}

void AnnouncementSettingWidget::on_checkBoxCommonFont2_stateChanged(int arg1)
{
    ui->toolButtonTextFont2->setEnabled(arg1);
}

void AnnouncementSettingWidget::on_toolButtonTextFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.textFont,this);
    if(ok)
        mySettings.textFont = font;

    ui->labelFont->setText(getFontText(mySettings.textFont));
}

void AnnouncementSettingWidget::on_toolButtonTextFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.textFont,this);
    if(ok)
        mySettings.textFont = font;

    ui->labelFont->setText(getFontText(mySettings.textFont));
}

void AnnouncementSettingWidget::on_checkBoxCommonColor_stateChanged(int arg1)
{
    ui->toolButtonTextColor->setEnabled(arg1);
    ui->toolButtonShadowColor->setEnabled(arg1);
}

void AnnouncementSettingWidget::on_checkBoxCommonColor2_stateChanged(int arg1)
{
    ui->toolButtonTextColor2->setEnabled(arg1);
    ui->toolButtonShadowColor2->setEnabled(arg1);
}

void AnnouncementSettingWidget::on_toolButtonTextColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textColor,this));
    if(c.isValid())
        mySettings.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
}

void AnnouncementSettingWidget::on_toolButtonTextColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textColor,this));
    if(c.isValid())
        mySettings2.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);
}

void AnnouncementSettingWidget::on_toolButtonShadowColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textShadowColor,this));
    if(c.isValid())
        mySettings.textShadowColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textShadowColor);
    ui->graphicViewShadowColor->setPalette(p);
}

void AnnouncementSettingWidget::on_toolButtonShadowColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textShadowColor,this));
    if(c.isValid())
        mySettings2.textShadowColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textShadowColor);
    ui->graphicViewShadowColor2->setPalette(p);
}

void AnnouncementSettingWidget::on_comboBoxBackgoundType_currentIndexChanged(int index)
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

void AnnouncementSettingWidget::on_comboBoxBackgoundType2_currentIndexChanged(int index)
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

void AnnouncementSettingWidget::on_buttonBrowseBackground_clicked()
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

void AnnouncementSettingWidget::on_buttonBrowseBackground2_clicked()
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

void AnnouncementSettingWidget::on_checkBoxCommonLayout_stateChanged(int arg1)
{
    ui->spinBoxMaxScreen->setEnabled(arg1);
    ui->comboBoxScreenPosition->setEnabled(arg1);
}

void AnnouncementSettingWidget::on_checkBoxCommonLayout2_stateChanged(int arg1)
{
    ui->spinBoxMaxScreen2->setEnabled(arg1);
    ui->comboBoxScreenPosition2->setEnabled(arg1);
}

void AnnouncementSettingWidget::on_checkBoxDisp2_stateChanged(int arg1)
{
    mySettings.useSameForDisp2 = arg1;
    ui->groupBoxUseDisp2->setVisible(!arg1);
}


//void AnnouncementSettingWidget::on_checkBoxUseShadow_stateChanged(int arg1)
//{
//    if(arg1==2)
//        ui->checkBoxUseBlurredShadow->setEnabled(true);
//    else
//    {
//        ui->checkBoxUseBlurredShadow->setChecked(false);
//        ui->checkBoxUseBlurredShadow->setEnabled(false);
//    }
//}

//void AnnouncementSettingWidget::on_checkBoxUseShadow2_stateChanged(int arg1)
//{
//    if(arg1==2)
//        ui->checkBoxUseBlurredShadow2->setEnabled(true);
//    else
//    {
//        ui->checkBoxUseBlurredShadow2->setChecked(false);
//        ui->checkBoxUseBlurredShadow2->setEnabled(false);
//    }
//}

//void AnnouncementSettingWidget::on_buttonBackground_clicked()
//{
//    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for announcement wallpaper"),
//                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
//    if(!filename.isNull())
//    {
//        QPixmap p(filename);
//        if(p.width()>1280 || p.height()>1280)
//            mySettings.background = p.scaled(1280,1280,Qt::KeepAspectRatio);
//        else
//            mySettings.background = p;
//        QFileInfo fi(filename);
//        filename = fi.fileName();
//        mySettings.backgroundName = filename;
//        ui->lineEditBackground->setText(filename);
//    }
//}

//void AnnouncementSettingWidget::on_buttonBackground2_clicked()
//{
//    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for announcement wallpaper"),
//                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
//    if(!filename.isNull())
//    {
//        QPixmap p(filename);
//        if(p.width()>1280 || p.height()>1280)
//            mySettings2.background = p.scaled(1280,1280,Qt::KeepAspectRatio);
//        else
//            mySettings2.background = p;
//        QFileInfo fi(filename);
//        filename = fi.fileName();
//        mySettings2.backgroundName = filename;
//        ui->lineEditBackground2->setText(filename);
//    }
//}

void AnnouncementSettingWidget::setDispScreen2Visible(bool visible)
{
    ui->checkBoxDisp2->setVisible(visible);
    if(!mySettings.useSameForDisp2)
        ui->groupBoxUseDisp2->setVisible(visible);
}

//void AnnouncementSettingWidget::on_toolButtonColor_clicked()
//{
//    QColor c(QColorDialog::getColor(mySettings.textColor,this));
//    if(c.isValid())
//        mySettings.textColor = c;
//    QPalette p;
//    p.setColor(QPalette::Base,mySettings.textColor);
//    ui->graphicViewTextColor->setPalette(p);
//}

//void AnnouncementSettingWidget::on_toolButtonColor2_clicked()
//{
//    QColor c(QColorDialog::getColor(mySettings2.textColor,this));
//    if(c.isValid())
//        mySettings2.textColor = c;
//    QPalette p;
//    p.setColor(QPalette::Base,mySettings2.textColor);
//    ui->graphicViewTextColor2->setPalette(p);
//}

//void AnnouncementSettingWidget::on_toolButtonFont_clicked()
//{
//    bool ok;
//    QFont font = QFontDialog::getFont(&ok,mySettings.textFont,this);
//    if(ok)
//        mySettings.textFont = font;

//    ui->labelFont->setText(getFontText(mySettings.textFont));
//}

//void AnnouncementSettingWidget::on_toolButtonFont2_clicked()
//{
//    bool ok;
//    QFont font = QFontDialog::getFont(&ok,mySettings2.textFont,this);
//    if(ok)
//        mySettings2.textFont = font;

//    ui->labelFont2->setText(getFontText(mySettings2.textFont));
//}

//void AnnouncementSettingWidget::on_groupBoxUseDisp2_toggled(bool arg1)
//{
//    ui->groupBoxBackground2->setVisible(arg1);
//    ui->groupBoxEffects2->setVisible(arg1);
//    ui->groupBoxTextProperties2->setVisible(arg1);
//}

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

//void AnnouncementSettingWidget::on_pushButtonApplyToAll_clicked()
//{
//    emit applyBackToAll(3,mySettings.backgroundName,mySettings.background);
//}

//void AnnouncementSettingWidget::setBackgroungds(QString name, QPixmap back)
//{
//    mySettings.backgroundName = name;
//    mySettings.background = back;
//    mySettings2.backgroundName = name;
//    mySettings2.background = back;
//    ui->lineEditBackground->setText(name);
//    ui->lineEditBackground2->setText(name);
//}

