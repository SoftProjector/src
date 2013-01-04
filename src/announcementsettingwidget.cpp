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

void AnnouncementSettingWidget::setSettings(AnnounceSettings &settings, AnnounceSettings &settings2)
{
    mySettings = settings;
    mySettings2 = settings2;
    loadSettings();
}

void AnnouncementSettingWidget::loadSettings()
{
    // Set Effects
    ui->checkBoxUseShadow->setChecked(mySettings.useShadow);
    ui->checkBoxUseFading->setChecked(mySettings.useFading);
    ui->checkBoxUseBlurredShadow->setChecked(mySettings.useBlurShadow);

    ui->checkBoxUseShadow2->setChecked(mySettings2.useShadow);
    ui->checkBoxUseFading2->setChecked(mySettings2.useFading);
    ui->checkBoxUseBlurredShadow2->setChecked(mySettings2.useBlurShadow);

    // Set background
    ui->groupBoxBackground->setChecked(mySettings.useBackground);
    ui->lineEditBackground->setText(mySettings.backgroundPath);

    ui->groupBoxBackground2->setChecked(mySettings2.useBackground);
    ui->lineEditBackground2->setText(mySettings2.backgroundPath);

    // Set Alingment
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

    ui->groupBoxUseDisp2->setChecked(mySettings2.useDisp2settings);
    on_groupBoxUseDisp2_toggled(mySettings2.useDisp2settings);
}

void AnnouncementSettingWidget::getSettings(AnnounceSettings &settings, AnnounceSettings &settings2)
{
    // Effects
    mySettings.useShadow = ui->checkBoxUseShadow->isChecked();
    mySettings.useFading = ui->checkBoxUseFading->isChecked();
    mySettings.useBlurShadow = ui->checkBoxUseBlurredShadow->isChecked();

    mySettings2.useShadow = ui->checkBoxUseShadow2->isChecked();
    mySettings2.useFading = ui->checkBoxUseFading2->isChecked();
    mySettings2.useBlurShadow = ui->checkBoxUseBlurredShadow2->isChecked();

    // Get Background
    mySettings.useBackground = ui->groupBoxBackground->isChecked();
    mySettings.backgroundPath = ui->lineEditBackground->text();

    mySettings2.useBackground = ui->groupBoxBackground2->isChecked();
    mySettings2.backgroundPath = ui->lineEditBackground2->text();

    // Alingmet
    mySettings.textAlingmentV = ui->comboBoxVerticalAling->currentIndex();
    mySettings.textAlingmentH = ui->comboBoxHorizontalAling->currentIndex();

    mySettings2.textAlingmentV = ui->comboBoxVerticalAling2->currentIndex();
    mySettings2.textAlingmentH = ui->comboBoxHorizontalAling2->currentIndex();

    mySettings2.useDisp2settings = ui->groupBoxUseDisp2->isChecked();

    settings = mySettings;
    settings2 = mySettings2;
}

void AnnouncementSettingWidget::on_buttonBackground_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for announcement wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));
    if(!filename.isNull())
        ui->lineEditBackground->setText(filename);
}

void AnnouncementSettingWidget::on_buttonTextColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textColor,this));
    if(c.isValid())
        mySettings.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
}

void AnnouncementSettingWidget::on_buttonFont_clicked()
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

void AnnouncementSettingWidget::on_pushButtonDefault_clicked()
{
    AnnounceSettings a;
    mySettings = a;
    mySettings2 = a;
    loadSettings();
}

void AnnouncementSettingWidget::on_checkBoxUseShadow_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow->setChecked(false);
        ui->checkBoxUseBlurredShadow->setEnabled(false);
    }
}

void AnnouncementSettingWidget::on_groupBoxUseDisp2_toggled(bool arg1)
{
    ui->groupBoxBackground2->setVisible(arg1);
    ui->groupBoxEffects2->setVisible(arg1);
    ui->groupBoxTextAlingment2->setVisible(arg1);
    ui->groupBoxTextProperties2->setVisible(arg1);
}

void AnnouncementSettingWidget::on_checkBoxUseShadow2_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow2->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow2->setChecked(false);
        ui->checkBoxUseBlurredShadow2->setEnabled(false);
    }
}

void AnnouncementSettingWidget::on_buttonTextColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textColor,this));
    if(c.isValid())
        mySettings2.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);
}

void AnnouncementSettingWidget::on_buttonFont2_clicked()
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

void AnnouncementSettingWidget::on_buttonBackground2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for announcement wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));
    if(!filename.isNull())
        ui->lineEditBackground2->setText(filename);
}

void AnnouncementSettingWidget::setDispScreen2Visible(bool visible)
{
    ui->groupBoxUseDisp2->setVisible(visible);
}
