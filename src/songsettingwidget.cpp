/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyleft (Ɔ) 2013  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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
    // Save Song info
    mySettings.showStanzaTitle = ui->checkBoxStanzaTitle->isChecked();
    mySettings.showSongKey = ui->checkBoxSongKey->isChecked();
    mySettings.showSongNumber = ui->checkBoxSongNumber->isChecked();

    mySettings2.showStanzaTitle = ui->checkBoxStanzaTitle2->isChecked();
    mySettings2.showSongKey = ui->checkBoxSongKey2->isChecked();
    mySettings2.showSongNumber = ui->checkBoxSongNumber2->isChecked();

    // Save song ending
    mySettings.showSongEnding = ui->groupBoxSongEnding->isChecked();
    if(ui->radioButtonAstrict->isChecked())
        mySettings.songEndingType = 0;
    else if(ui->radioButtonSongCopyInfo)
        mySettings.songEndingType = 1;

    mySettings2.showSongEnding = ui->groupBoxSongEnding2->isChecked();
    if(ui->radioButtonAstrict2->isChecked())
        mySettings2.songEndingType = 0;
    else if(ui->radioButtonSongCopyInfo2)
        mySettings2.songEndingType = 1;

    // Effects
    mySettings.useShadow = ui->checkBoxUseShadow->isChecked();
    mySettings.useFading = ui->checkBoxUseFading->isChecked();
    mySettings.useBlurShadow = ui->checkBoxUseBlurredShadow->isChecked();

    mySettings2.useShadow = ui->checkBoxUseShadow2->isChecked();
    mySettings2.useFading = ui->checkBoxUseFading2->isChecked();
    mySettings2.useBlurShadow = ui->checkBoxUseBlurredShadow2->isChecked();

    // Save song background
    mySettings.useBackground = ui->groupBoxSongBackground->isChecked();
    mySettings.backgroundPath = ui->lineEditSongBackground->text();

    mySettings2.useBackground = ui->groupBoxSongBackground2->isChecked();
    mySettings2.backgroundPath = ui->lineEditSongBackground2->text();

    // Save alingment
    mySettings.textAlingmentV = ui->comboBoxVerticalAling->currentIndex();
    mySettings.textAlingmentH = ui->comboBoxHorizontalAling->currentIndex();

    mySettings2.textAlingmentV = ui->comboBoxVerticalAling2->currentIndex();
    mySettings2.textAlingmentH = ui->comboBoxHorizontalAling2->currentIndex();

    // Use secondary display screen settings
    mySettings2.useDisp2settings = ui->groupBoxDisplay2->isChecked();

    settings = mySettings;
    settings2 = mySettings2;
}

void SongSettingWidget::loadSettings()
{
    // Set Song Information
    ui->checkBoxStanzaTitle->setChecked(mySettings.showStanzaTitle);
    ui->checkBoxSongKey->setChecked(mySettings.showSongKey);
    ui->checkBoxSongNumber->setChecked(mySettings.showSongNumber);

    ui->checkBoxStanzaTitle2->setChecked(mySettings2.showStanzaTitle);
    ui->checkBoxSongKey2->setChecked(mySettings2.showSongKey);
    ui->checkBoxSongNumber2->setChecked(mySettings2.showSongNumber);

    // Set Song Ending
    ui->groupBoxSongEnding->setChecked(mySettings.showSongEnding);
    if(mySettings.songEndingType == 0)
        ui->radioButtonAstrict->setChecked(true);
    else if(mySettings.songEndingType == 1)
        ui->radioButtonSongCopyInfo->setChecked(true);

    ui->groupBoxSongEnding2->setChecked(mySettings2.showSongEnding);
    if(mySettings2.songEndingType == 0)
        ui->radioButtonAstrict2->setChecked(true);
    else if(mySettings2.songEndingType == 1)
        ui->radioButtonSongCopyInfo2->setChecked(true);

    // Set Effects
    ui->checkBoxUseShadow->setChecked(mySettings.useShadow);
    ui->checkBoxUseFading->setChecked(mySettings.useFading);
    ui->checkBoxUseBlurredShadow->setChecked(mySettings.useBlurShadow);

    ui->checkBoxUseShadow2->setChecked(mySettings2.useShadow);
    ui->checkBoxUseFading2->setChecked(mySettings2.useFading);
    ui->checkBoxUseBlurredShadow2->setChecked(mySettings2.useBlurShadow);

    // Set Song Background
    ui->groupBoxSongBackground->setChecked(mySettings.useBackground);
    ui->lineEditSongBackground->setText(mySettings.backgroundPath);

    ui->groupBoxSongBackground2->setChecked(mySettings2.useBackground);
    ui->lineEditSongBackground2->setText(mySettings2.backgroundPath);

    // Set alingment
    ui->comboBoxVerticalAling->setCurrentIndex(mySettings.textAlingmentV);
    ui->comboBoxHorizontalAling->setCurrentIndex(mySettings.textAlingmentH);

    ui->comboBoxVerticalAling2->setCurrentIndex(mySettings2.textAlingmentV);
    ui->comboBoxHorizontalAling2->setCurrentIndex(mySettings2.textAlingmentH);

    // Set text color
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);

    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);

    // Set text font lable
    QString st(QString("%1: %2").arg(mySettings.textFont.rawName()).arg(mySettings.textFont.pointSize()));
    if(mySettings.textFont.bold())
        st += ", " + tr("Bold");
    if(mySettings.textFont.italic())
        st += ", " + tr("Italic");
    if(mySettings.textFont.strikeOut())
        st += ", " + tr("StrikeOut");
    if(mySettings.textFont.underline())
        st += ", " + tr("Underline");
    ui->labelFont->setText(st);

    st = QString("%1: %2").arg(mySettings2.textFont.rawName()).arg(mySettings2.textFont.pointSize());
        if(mySettings2.textFont.bold())
            st += ", " + tr("Bold");
        if(mySettings2.textFont.italic())
            st += ", " + tr("Italic");
        if(mySettings2.textFont.strikeOut())
            st += ", " + tr("StrikeOut");
        if(mySettings2.textFont.underline())
            st += ", " + tr("Underline");
        ui->labelFont2->setText(st);

        ui->groupBoxDisplay2->setChecked(mySettings2.useDisp2settings);
        on_groupBoxDisplay2_toggled(mySettings2.useDisp2settings);
}

void SongSettingWidget::on_buttonSongBackground_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for song wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));
    if(!filename.isNull())
        ui->lineEditSongBackground->setText(filename);
}

void SongSettingWidget::on_buttonTextColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textColor,this));
    if(c.isValid())
        mySettings.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
}

void SongSettingWidget::on_buttonFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.textFont,this);
    if(ok)
        mySettings.textFont = font;

    QString st(QString("%1: %2").arg(mySettings.textFont.rawName()).arg(mySettings.textFont.pointSize()));
    if(mySettings.textFont.bold())
        st += ", " + tr("Bold");
    if(mySettings.textFont.italic())
        st += ", " + tr("Italic");
    if(mySettings.textFont.strikeOut())
        st += ", " + tr("StrikeOut");
    if(mySettings.textFont.underline())
        st += ", " + tr("Underline");
    ui->labelFont->setText(st);
}

void SongSettingWidget::on_pushButtonDefault_clicked()
{
    SongSettings s;
    mySettings = s;
    mySettings2 = s;
    loadSettings();
}

void SongSettingWidget::on_checkBoxUseShadow_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow->setChecked(false);
        ui->checkBoxUseBlurredShadow->setEnabled(false);
    }
}

void SongSettingWidget::on_groupBoxDisplay2_toggled(bool arg1)
{
    ui->groupBoxEffects2->setVisible(arg1);
    ui->groupBoxSongBackground2->setVisible(arg1);
    ui->groupBoxSongEnding2->setVisible(arg1);
    ui->groupBoxSongInfo2->setVisible(arg1);
    ui->groupBoxTextAlingment2->setVisible(arg1);
    ui->groupBoxTextProperties2->setVisible(arg1);
}

void SongSettingWidget::setDispScreen2Visible(bool visible)
{
    ui->groupBoxDisplay2->setVisible(visible);
}

void SongSettingWidget::on_checkBoxUseShadow2_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow2->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow2->setChecked(false);
        ui->checkBoxUseBlurredShadow2->setEnabled(false);
    }
}

void SongSettingWidget::on_buttonSongBackground2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for song wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));
    if(!filename.isNull())
        ui->lineEditSongBackground2->setText(filename);
}

void SongSettingWidget::on_buttonTextColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textColor,this));
    if(c.isValid())
        mySettings2.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);
}

void SongSettingWidget::on_buttonFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.textFont,this);
    if(ok)
        mySettings2.textFont = font;

    QString st(QString("%1: %2").arg(mySettings2.textFont.rawName()).arg(mySettings2.textFont.pointSize()));
    if(mySettings2.textFont.bold())
        st += ", " + tr("Bold");
    if(mySettings2.textFont.italic())
        st += ", " + tr("Italic");
    if(mySettings2.textFont.strikeOut())
        st += ", " + tr("StrikeOut");
    if(mySettings2.textFont.underline())
        st += ", " + tr("Underline");
    ui->labelFont2->setText(st);
}
