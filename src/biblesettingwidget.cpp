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

#include "biblesettingwidget.h"
#include "ui_biblesettingwidget.h"

BibleSettingWidget::BibleSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BibleSettingWidget)
{
    ui->setupUi(this);
}

BibleSettingWidget::~BibleSettingWidget()
{
    delete ui;
}

void BibleSettingWidget::setSettings(BibleSettings &settings, BibleSettings &settings2)
{
    mySettings = settings;
    mySettings2 = settings2;
    loadSettings();
}

void BibleSettingWidget::getSettings(BibleSettings &settings, BibleSettings &settings2)
{
    int pbx = ui->comboBoxPrimaryBible->currentIndex();
    int sbx = ui->comboBoxSecondaryBible->currentIndex();
    int tbx = ui->comboBoxTrinaryBible->currentIndex();
    int obx = ui->comboBoxOperatorBible->currentIndex();

    // Get Bible version settings
    if(pbx != -1)
    {
        // Primary
        mySettings.primaryBible = bible_id_list.at(pbx);
        // Secondary
        if(sbx <=0)
            mySettings.secondaryBible = "none";
        else
            mySettings.secondaryBible = secondary_id_list.at(sbx-1);
        // Trinary
        if(tbx <=0)
            mySettings.trinaryBible = "none";
        else
            mySettings.trinaryBible = trinary_id_list.at(tbx-1);
        // Operatror
        if(obx <=0)
            mySettings.operatorBible = "same";
        else
            mySettings.operatorBible = operator_id_list.at(obx-1);

    }
    else
    {
        // There are no bibles in the database
        mySettings.primaryBible = "none";
        mySettings.secondaryBible = "none";
        mySettings.trinaryBible = "none";
        mySettings.operatorBible = "same";
    }

    // Get Bible versions for secondary screen
    pbx = ui->comboBoxPrimaryBible2->currentIndex();
    sbx = ui->comboBoxSecondaryBible2->currentIndex();
    tbx = ui->comboBoxTrinaryBible2->currentIndex();
    if(pbx != -1)
    {
        // Primary
        mySettings2.primaryBible = bible_id_list.at(pbx);
        // Secondary
        if(sbx <=0)
            mySettings2.secondaryBible = "none";
        else
            mySettings2.secondaryBible = secondary_id_list2.at(sbx-1);
        // Trinary
        if(tbx <=0)
            mySettings2.trinaryBible = "none";
        else
            mySettings2.trinaryBible = trinary_id_list2.at(tbx-1);
    }
    else
    {
        // There are no bibles in the database
        mySettings2.primaryBible = "none";
        mySettings2.secondaryBible = "none";
        mySettings2.trinaryBible = "none";
        mySettings2.operatorBible = "same";
    }

    // Effects
    mySettings.useShadow = ui->checkBoxUseShadow->isChecked();
    mySettings.useFading = ui->checkBoxUseFading->isChecked();
    mySettings.useBlurShadow = ui->checkBoxUseBlurredShadow->isChecked();

    mySettings2.useShadow = ui->checkBoxUseShadow2->isChecked();
    mySettings2.useFading = ui->checkBoxUseFading2->isChecked();
    mySettings2.useBlurShadow = ui->checkBoxUseBlurredShadow2->isChecked();

    // Backgroud
    mySettings.useBackground = ui->groupBoxBackground->isChecked();
    mySettings.backgroundPath = ui->lineEditBackPath->text();

    mySettings2.useBackground = ui->groupBoxBackground2->isChecked();
    mySettings2.backgroundPath = ui->lineEditBackPath2->text();

    //Alingment
    mySettings.textAlingmentV = ui->comboBoxVerticalAling->currentIndex();
    mySettings.textAlingmentH = ui->comboBoxHorizontalAling->currentIndex();

    mySettings2.textAlingmentV = ui->comboBoxVerticalAling2->currentIndex();
    mySettings2.textAlingmentH = ui->comboBoxHorizontalAling2->currentIndex();

    // Version Abbreviations
    mySettings.useAbbriviations = ui->checkBoxAbbiviations->isChecked();
    mySettings2.useAbbriviations = ui->checkBoxAbbiviations2->isChecked();

    // Max screen use
    mySettings.maxScreen = ui->spinBoxMaxScreen->value();
    if(ui->radioButtonMaxTop->isChecked())
        mySettings.maxScreenFrom = "top";
    if(ui->radioButtonMaxBottom->isChecked())
        mySettings.maxScreenFrom = "bottom";

    mySettings2.maxScreen = ui->spinBoxMaxScreen2->value();
    if(ui->radioButtonMaxTop2->isChecked())
        mySettings2.maxScreenFrom = "top";
    if(ui->radioButtonMaxBottom2->isChecked())
        mySettings2.maxScreenFrom = "bottom";

    // Get if to use secodary screen settings
    mySettings2.useDisp2settings = ui->groupBoxUseDisp2->isChecked();

    settings = mySettings;
    settings2 = mySettings2;
}

void BibleSettingWidget::loadSettings()
{
    // Clear items if exitst
    bibles.clear();
    bible_id_list.clear();

    // Get Bibles that exist in the database
    QSqlQuery sq;
    sq.exec("SELECT bible_name, id FROM BibleVersions");
    while(sq.next()){
        bibles << sq.value(0).toString();
        bible_id_list << sq.value(1).toString();
    }
    sq.clear();

    // Fill bibles comboboxes
    ui->comboBoxPrimaryBible->clear();
    ui->comboBoxPrimaryBible->addItems(bibles);
    ui->comboBoxSecondaryBible->addItem(tr("None"));
    ui->comboBoxSecondaryBible->addItems(bibles);
    ui->comboBoxTrinaryBible->addItem(tr("None"));
    ui->comboBoxTrinaryBible->addItems(bibles);
    ui->comboBoxOperatorBible->addItem(tr("Same as primary Bible"));
    ui->comboBoxOperatorBible->addItems(bibles);

    ui->comboBoxPrimaryBible2->clear();
    ui->comboBoxPrimaryBible2->addItems(bibles);
    ui->comboBoxSecondaryBible2->addItem(tr("None"));
    ui->comboBoxSecondaryBible2->addItems(bibles);
    ui->comboBoxTrinaryBible2->addItem(tr("None"));
    ui->comboBoxTrinaryBible2->addItems(bibles);

    // Set current primary bible
    if(mySettings.primaryBible == "none")
        ui->comboBoxPrimaryBible->setCurrentIndex(0);
    else
        ui->comboBoxPrimaryBible->setCurrentIndex(bible_id_list.indexOf(mySettings.primaryBible));

    if(mySettings2.primaryBible == "none")
        ui->comboBoxPrimaryBible2->setCurrentIndex(0);
    else
        ui->comboBoxPrimaryBible2->setCurrentIndex(bible_id_list.indexOf(mySettings2.primaryBible));

    // Set current secondary bible
    if(mySettings.secondaryBible == "none")
        ui->comboBoxSecondaryBible->setCurrentIndex(0);
    else
        ui->comboBoxSecondaryBible->setCurrentIndex(bible_id_list.indexOf(mySettings.secondaryBible)+1);

    if(mySettings2.secondaryBible == "none")
        ui->comboBoxSecondaryBible2->setCurrentIndex(0);
    else
        ui->comboBoxSecondaryBible2->setCurrentIndex(bible_id_list.indexOf(mySettings2.secondaryBible)+1);

    // Set current trinaty bibile
    if(mySettings.trinaryBible == "none")
        ui->comboBoxTrinaryBible->setCurrentIndex(0);
    else
        ui->comboBoxTrinaryBible->setCurrentIndex(bible_id_list.indexOf(mySettings.trinaryBible)+1);
    updateSecondaryBibleMenu();

    if(mySettings2.trinaryBible == "none")
        ui->comboBoxTrinaryBible2->setCurrentIndex(0);
    else
        ui->comboBoxTrinaryBible2->setCurrentIndex(bible_id_list.indexOf(mySettings2.trinaryBible)+1);
    updateSecondaryBibleMenu2();

    // Set current operator bibile
    if(mySettings.operatorBible == "same")
        ui->comboBoxOperatorBible->setCurrentIndex(0);
    else
        ui->comboBoxOperatorBible->setCurrentIndex(bible_id_list.indexOf(mySettings.operatorBible)+1);
    updateOperatorBibleMenu();

    // Set Effects
    ui->checkBoxUseShadow->setChecked(mySettings.useShadow);
    ui->checkBoxUseFading->setChecked(mySettings.useFading);
    ui->checkBoxUseBlurredShadow->setChecked(mySettings.useBlurShadow);

    ui->checkBoxUseShadow2->setChecked(mySettings2.useShadow);
    ui->checkBoxUseFading2->setChecked(mySettings2.useFading);
    ui->checkBoxUseBlurredShadow2->setChecked(mySettings2.useBlurShadow);

    // Set background use
    ui->groupBoxBackground->setChecked(mySettings.useBackground);
    ui->lineEditBackPath->setText(mySettings.backgroundPath);

    ui->groupBoxBackground2->setChecked(mySettings2.useBackground);
    ui->lineEditBackPath2->setText(mySettings2.backgroundPath);

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

    // Set text font
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

    // Set abbriviations use
    ui->checkBoxAbbiviations->setChecked(mySettings.useAbbriviations);

    ui->checkBoxAbbiviations2->setChecked(mySettings2.useAbbriviations);

    // Set max screen use
    ui->spinBoxMaxScreen->setValue(mySettings.maxScreen);
    if(mySettings.maxScreenFrom == "top")
        ui->radioButtonMaxTop->setChecked(true);
    else if(mySettings.maxScreenFrom == "bottom")
        ui->radioButtonMaxBottom->setChecked(true);

    ui->spinBoxMaxScreen2->setValue(mySettings2.maxScreen);
    if(mySettings2.maxScreenFrom == "top")
        ui->radioButtonMaxTop2->setChecked(true);
    else if(mySettings2.maxScreenFrom == "bottom")
        ui->radioButtonMaxBottom2->setChecked(true);

    // Set if to use secondary screen settings
    ui->groupBoxUseDisp2->setChecked(mySettings2.useDisp2settings);
    on_groupBoxUseDisp2_toggled(mySettings2.useDisp2settings);
}

void BibleSettingWidget::updateSecondaryBibleMenu()
{
    QString pbible = ui->comboBoxPrimaryBible->currentText();
    QString sbible = ui->comboBoxSecondaryBible->currentText();
    secondary_bibles = bibles;
    secondary_bibles.removeOne(pbible);

    secondary_id_list = bible_id_list;
    secondary_id_list.removeAt(ui->comboBoxPrimaryBible->currentIndex());
    ui->comboBoxSecondaryBible->clear();
    ui->comboBoxSecondaryBible->addItem(tr("None"));
    ui->comboBoxSecondaryBible->addItems(secondary_bibles);

    int i = ui->comboBoxSecondaryBible->findText(sbible);
    if( i != -1 )
        // The same secondary bible is still available
        ui->comboBoxSecondaryBible->setCurrentIndex(i);

    updateTrinaryBibleMenu();
}

void BibleSettingWidget::updateSecondaryBibleMenu2()
{
    QString pbible = ui->comboBoxPrimaryBible2->currentText();
    QString sbible = ui->comboBoxSecondaryBible2->currentText();
    secondary_bibles2 = bibles;
    secondary_bibles2.removeOne(pbible);

    secondary_id_list2 = bible_id_list;
    secondary_id_list2.removeAt(ui->comboBoxPrimaryBible2->currentIndex());
    ui->comboBoxSecondaryBible2->clear();
    ui->comboBoxSecondaryBible2->addItem(tr("None"));
    ui->comboBoxSecondaryBible2->addItems(secondary_bibles2);

    int i = ui->comboBoxSecondaryBible2->findText(sbible);
    if( i != -1 )
        // The same secondary bible is still available
        ui->comboBoxSecondaryBible2->setCurrentIndex(i);

    updateTrinaryBibleMenu2();
}

void BibleSettingWidget::updateTrinaryBibleMenu()
{

    if (ui->comboBoxSecondaryBible->currentIndex() == 0)
    {
        ui->comboBoxTrinaryBible->setCurrentIndex(0);
        ui->comboBoxTrinaryBible->setEnabled(false);
    }
    else
    {
        ui->comboBoxTrinaryBible->setEnabled(true);
        QString sbible = ui->comboBoxSecondaryBible->currentText();
        QString tbible = ui->comboBoxTrinaryBible->currentText();
        QStringList trinary_bibles = secondary_bibles;
        trinary_bibles.removeOne(sbible);


        trinary_id_list = secondary_id_list;
        trinary_id_list.removeAt(ui->comboBoxSecondaryBible->currentIndex()-1);
        ui->comboBoxTrinaryBible->clear();
        ui->comboBoxTrinaryBible->addItem(tr("None"));
        ui->comboBoxTrinaryBible->addItems(trinary_bibles);

        int i = ui->comboBoxTrinaryBible->findText(tbible);
        if( i != -1 )
            // The same secondary bible is still available
            ui->comboBoxTrinaryBible->setCurrentIndex(i);
    }
}

void BibleSettingWidget::updateTrinaryBibleMenu2()
{

    if (ui->comboBoxSecondaryBible2->currentIndex() == 0)
    {
        ui->comboBoxTrinaryBible2->setCurrentIndex(0);
        ui->comboBoxTrinaryBible2->setEnabled(false);
    }
    else
    {
        ui->comboBoxTrinaryBible2->setEnabled(true);
        QString sbible = ui->comboBoxSecondaryBible2->currentText();
        QString tbible = ui->comboBoxTrinaryBible2->currentText();
        QStringList trinary_bibles = secondary_bibles2;
        trinary_bibles.removeOne(sbible);


        trinary_id_list2 = secondary_id_list2;
        trinary_id_list2.removeAt(ui->comboBoxSecondaryBible2->currentIndex()-1);
        ui->comboBoxTrinaryBible2->clear();
        ui->comboBoxTrinaryBible2->addItem(tr("None"));
        ui->comboBoxTrinaryBible2->addItems(trinary_bibles);

        int i = ui->comboBoxTrinaryBible2->findText(tbible);
        if( i != -1 )
            // The same secondary bible is still available
            ui->comboBoxTrinaryBible2->setCurrentIndex(i);
    }
}

void BibleSettingWidget::updateOperatorBibleMenu()
{
    QString pbible = ui->comboBoxPrimaryBible->currentText();
    QString obible = ui->comboBoxOperatorBible->currentText();
    QStringList operator_bibles = bibles;
    operator_bibles.removeOne(pbible);

    operator_id_list = bible_id_list;
    operator_id_list.removeAt(ui->comboBoxPrimaryBible->currentIndex());
    ui->comboBoxOperatorBible->clear();
    ui->comboBoxOperatorBible->addItem(tr("Same as primary Bible"));
    ui->comboBoxOperatorBible->addItems(operator_bibles);

    int i = ui->comboBoxOperatorBible->findText(obible);
    if( i != -1 )
        // The same operaotr bible is still available
        ui->comboBoxOperatorBible->setCurrentIndex(i);

}

void BibleSettingWidget::on_comboBoxPrimaryBible_activated(const QString &arg1)
{
    updateSecondaryBibleMenu();
    updateOperatorBibleMenu();
}

void BibleSettingWidget::on_comboBoxSecondaryBible_activated(const QString &arg1)
{
    updateTrinaryBibleMenu();
}

void BibleSettingWidget::on_buttonBrowseBackground_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for Bible wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));
    if(!filename.isNull())
        ui->lineEditBackPath->setText(filename);
}

void BibleSettingWidget::on_buttonTextColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textColor,this));
    if(c.isValid())
        mySettings.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
}

void BibleSettingWidget::on_buttonFont_clicked()
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

void BibleSettingWidget::on_pushButtonDefault_clicked()
{
    BibleSettings b;
    mySettings = b;
    mySettings2 = b;
    loadSettings();
}

void BibleSettingWidget::on_checkBoxUseShadow_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow->setChecked(false);
        ui->checkBoxUseBlurredShadow->setEnabled(false);
    }
}

void BibleSettingWidget::on_groupBoxUseDisp2_toggled(bool arg1)
{
    ui->widgetBibles2->setVisible(arg1);
    ui->groupBoxEffects2->setVisible(arg1);
    ui->groupBoxBackground2->setVisible(arg1);
    ui->groupBoxMaxScreen2->setVisible(arg1);
    ui->groupBoxTextAlingment2->setVisible(arg1);
    ui->groupBoxTextProperties2->setVisible(arg1);
    ui->checkBoxAbbiviations2->setVisible(arg1);
}

void BibleSettingWidget::setDispScreen2Visible(bool visible)
{
    ui->groupBoxUseDisp2->setVisible(visible);
}

void BibleSettingWidget::on_comboBoxPrimaryBible2_activated(const QString &arg1)
{
    updateSecondaryBibleMenu2();
    updateTrinaryBibleMenu2();
}

void BibleSettingWidget::on_comboBoxSecondaryBible2_activated(const QString &arg1)
{
    updateTrinaryBibleMenu2();
}

void BibleSettingWidget::on_buttonBrowseBackground2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for Bible wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));
    if(!filename.isNull())
        ui->lineEditBackPath2->setText(filename);
}

void BibleSettingWidget::on_buttonTextColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textColor,this));
    if(c.isValid())
        mySettings2.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);
}

void BibleSettingWidget::on_buttonFont2_clicked()
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

void BibleSettingWidget::on_checkBoxUseShadow2_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow2->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow2->setChecked(false);
        ui->checkBoxUseBlurredShadow2->setEnabled(false);
    }
}
