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

#include "generalsettingwidget.h"
#include "ui_generalsettingwidget.h"

GeneralSettingWidget::GeneralSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GeneralSettingWidget)
{
    ui->setupUi(this);
}

GeneralSettingWidget::~GeneralSettingWidget()
{
    delete ui;
}

void GeneralSettingWidget::changeEvent(QEvent *e)
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

void GeneralSettingWidget::setSettings(GeneralSettings settings)
{
    mySettings = settings;
    loadSettings();
}

void GeneralSettingWidget::loadSettings()
{
    ui->checkBoxDisplayOnTop->setChecked(mySettings.displayIsOnTop);

    // Load Themes
    loadThemes();
    ui->comboBoxTheme->setCurrentIndex(themeIdList.indexOf(mySettings.currentThemeId));

    // Get display screen infomration
    monitors.clear();
    QDesktopWidget d;
    int screen_count = d.screenCount();
    ui->comboBoxDisplayScreen->clear();
    for(int i(0); i<screen_count;++i)
        monitors << QString("%1 - %2x%3").arg(i+1).arg(d.screenGeometry(i).width()).arg(d.screenGeometry(i).height());

    if(screen_count>1)
        ui->groupBoxDisplayScreen->setVisible(true);
    else
        ui->groupBoxDisplayScreen->setVisible(false);

    // Fill display screen comboBoxes
    ui->comboBoxDisplayScreen->clear();
    ui->comboBoxDisplayScreen->addItems(monitors);
    ui->comboBoxDisplayScreen_2->clear();
    ui->comboBoxDisplayScreen_2->addItem(tr("none"));
    ui->comboBoxDisplayScreen_2->addItems(monitors);

    // Set primary display screen
    if(mySettings.displayScreen<0 || mySettings.displayScreen>=screen_count)
        ui->comboBoxDisplayScreen->setCurrentIndex(0);
    else
        ui->comboBoxDisplayScreen->setCurrentIndex(mySettings.displayScreen);

    // Set secondaty display screen
    if(mySettings.displayScreen2<-1 || mySettings.displayScreen2>=screen_count)
        ui->comboBoxDisplayScreen_2->setCurrentIndex(0);
    else
        ui->comboBoxDisplayScreen_2->setCurrentIndex(mySettings.displayScreen2+1);
    updateSecondaryDisplayScreen();

    // Dispalay on Start Up
    ui->checkBoxDisplayOnStartUp->setChecked(mySettings.displayOnStartUp);

    // Set Display Controls
    if(screen_count>1)
        ui->groupBoxDisplayControls->setVisible(false);
    else
        ui->groupBoxDisplayControls->setVisible(true);

    ui->comboBoxIconSize->setCurrentIndex(mySettings.displayControls.buttonSize);
    ui->comboBoxControlsAlignV->setCurrentIndex(mySettings.displayControls.alignmentV);
    ui->comboBoxControlsAlignH->setCurrentIndex(mySettings.displayControls.alignmentH);
    ui->horizontalSliderOpacity->setValue(mySettings.displayControls.opacity*100);
}

void GeneralSettingWidget::loadThemes()
{
    QSqlQuery sq;
    sq.exec("SELECT id, name FROM Themes");
    themeIdList.clear();
    themes.clear();
    while(sq.next())
    {
        themeIdList<< sq.value(0).toInt();
        themes<<sq.value(1).toString();
    }
    ui->comboBoxTheme->clear();
    ui->comboBoxTheme->addItems(themes);
}

GeneralSettings GeneralSettingWidget::getSettings()
{
    mySettings.displayIsOnTop = ui->checkBoxDisplayOnTop->isChecked();

    int tmx = ui->comboBoxTheme->currentIndex();
    if(tmx != -1)
        mySettings.currentThemeId = themeIdList.at(tmx);
    else
        mySettings.currentThemeId = 0;

    mySettings.displayScreen = ui->comboBoxDisplayScreen->currentIndex();
    mySettings.displayScreen2 = monitors.indexOf(ui->comboBoxDisplayScreen_2->currentText());

    mySettings.displayControls.buttonSize = ui->comboBoxIconSize->currentIndex();
    mySettings.displayControls.alignmentV = ui->comboBoxControlsAlignV->currentIndex();
    mySettings.displayControls.alignmentH = ui->comboBoxControlsAlignH->currentIndex();
    qreal r = ui->horizontalSliderOpacity->value();
    r = r/100;
    mySettings.displayControls.opacity = r;

    return mySettings;
}

void GeneralSettingWidget::on_pushButtonDefault_clicked()
{
    GeneralSettings g;
    mySettings = g;
    mySettings.settingsChangedAll = true;
    mySettings.settingsChangedMulti = true;
    mySettings.settingsChangedSingle = true;
    loadSettings();
}

void GeneralSettingWidget::updateSecondaryDisplayScreen()
{
    QString ds1 = ui->comboBoxDisplayScreen->currentText();
    QString ds2 = ui->comboBoxDisplayScreen_2->currentText();
    QStringList monitors2 = monitors;
    monitors2.removeOne(ds1);

    ui->comboBoxDisplayScreen_2->clear();
    ui->comboBoxDisplayScreen_2->addItem(tr("None"));
    ui->comboBoxDisplayScreen_2->addItems(monitors2);

    int i = ui->comboBoxDisplayScreen_2->findText(ds2);
    if(i != -1)
        ui->comboBoxDisplayScreen_2->setCurrentIndex(i);
    else
    {
        ui->comboBoxDisplayScreen_2->setCurrentIndex(0);
        emit setDisp2Use(false);
    }
}

void GeneralSettingWidget::on_comboBoxDisplayScreen_activated(const QString &arg1)
{
    updateSecondaryDisplayScreen();
    mySettings.settingsChangedMulti = true;
}

void GeneralSettingWidget::on_comboBoxDisplayScreen_2_activated(int index)
{
    if(index<=0)
        emit setDisp2Use(false);
    else
        emit setDisp2Use(true);
    mySettings.settingsChangedMulti = true;
}

void GeneralSettingWidget::on_pushButtonAddTheme_clicked()
{
    Theme tm;
    ThemeInfo tmi;
    int nId;

    AddSongbookDialog theme_dia;
    theme_dia.setWindowTitle(tr("Edit Theme"));
    theme_dia.setWindowText(tr("Theme Name:"),tr("Comments:"));
    theme_dia.setSongbook(tr("Default"),tr("This theme will contain program default settings."));
    int ret = theme_dia.exec();
    switch(ret)
    {
    case AddSongbookDialog::Accepted:
        tmi.name = theme_dia.title;
        tmi.comments = theme_dia.info;
        tm.setThemeInfo(tmi);
        tm.saveThemeNew();
        nId = tm.getThemeId();

        loadThemes();

        ui->comboBoxTheme->setCurrentIndex(themeIdList.indexOf(nId));
        emit themeChanged(nId);
        mySettings.settingsChangedAll = true;

        break;
    case AddSongbookDialog::Rejected:
        break;
    }
}

void GeneralSettingWidget::on_comboBoxTheme_activated(int index)
{
    emit themeChanged(themeIdList.at(index));
    mySettings.settingsChangedAll = true;
}

void GeneralSettingWidget::on_checkBoxDisplayOnStartUp_toggled(bool checked)
{
    mySettings.displayOnStartUp = checked;
    mySettings.settingsChangedMulti = true;
}

void GeneralSettingWidget::on_comboBoxIconSize_activated(const QString &arg1)
{
    mySettings.settingsChangedSingle = true;
}

void GeneralSettingWidget::on_comboBoxControlsAlignV_activated(const QString &arg1)
{
    mySettings.settingsChangedSingle = true;
}

void GeneralSettingWidget::on_comboBoxControlsAlignH_activated(const QString &arg1)
{
    mySettings.settingsChangedSingle = true;
}

void GeneralSettingWidget::on_horizontalSliderOpacity_actionTriggered(int action)
{
    mySettings.settingsChangedSingle = true;
}

void GeneralSettingWidget::on_checkBoxDisplayOnTop_toggled(bool checked)
{
    mySettings.settingsChangedAll = true;
}
