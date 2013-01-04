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

#include "passivesettingwidget.h"
#include "ui_passivesettingwidget.h"

PassiveSettingWidget::PassiveSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PassiveSettingWidget)
{
    ui->setupUi(this);
}

PassiveSettingWidget::~PassiveSettingWidget()
{
    delete ui;
}

void PassiveSettingWidget::setSetings(PassiveSettings &settings, PassiveSettings &settings2)
{
    mySettings = settings;
    mySettings2 = settings2;
    loadSettings();
}

void PassiveSettingWidget::getSettings(PassiveSettings &settings, PassiveSettings &settings2)
{
    mySettings.useBackground = ui->groupBoxBackground->isChecked();
    mySettings.backgroundPath = ui->lineEditBackgroundPath->text();

    // Get display two settings
    mySettings2.useDisp2settings = ui->groupBoxDisp2Sets->isChecked();
    mySettings2.useBackground = ui->groupBoxBackground2->isChecked();
    mySettings2.backgroundPath = ui->lineEditBackgroundPath2->text();

    settings = mySettings;
    settings2 = mySettings2;
}

void PassiveSettingWidget::loadSettings()
{
    ui->groupBoxBackground->setChecked(mySettings.useBackground);
    ui->lineEditBackgroundPath->setText(mySettings.backgroundPath);

    // Displpay screen two
    ui->groupBoxDisp2Sets->setChecked(mySettings2.useDisp2settings);
    ui->groupBoxBackground2->setChecked(mySettings.useBackground);
    ui->lineEditBackgroundPath2->setText(mySettings.backgroundPath);
}

void PassiveSettingWidget::setDispScreen2Visible(bool visible)
{
    ui->groupBoxDisp2Sets->setVisible(visible);
}

void PassiveSettingWidget::on_buttonBrowseBackgound_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for passive wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));
    if(!filename.isNull())
        ui->lineEditBackgroundPath->setText(filename);
}

void PassiveSettingWidget::on_groupBoxDisp2Sets_toggled(bool arg1)
{
    ui->groupBoxBackground2->setVisible(arg1);
}

void PassiveSettingWidget::on_buttonBrowseBackgound2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for passive wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));
    if(!filename.isNull())
        ui->lineEditBackgroundPath2->setText(filename);
}

void PassiveSettingWidget::on_pushButtonDefault_clicked()
{
    PassiveSettings p;
    mySettings = p;
    mySettings2 = p;
    loadSettings();
}
