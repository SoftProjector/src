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

void BibleSettingWidget::setSettings(BibleSettings settings)
{
    mySettings = settings;
    loadSettings();
}

BibleSettings BibleSettingWidget::getSettings()
{
    int pbx = ui->comboBox_primaryBible->currentIndex();
    int sbx = ui->comboBox_secondaryBible->currentIndex();
    int tbx = ui->comboBox_trinaryBible->currentIndex();
    int obx = ui->comboBox_operatorBible->currentIndex();
//    QString pb,sb,tb,ob;

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

    return mySettings;
}

void BibleSettingWidget::loadSettings()
{
    // Get Bibles that exist in the database
    QSqlQuery sq;
    sq.exec("SELECT bible_name, id FROM BibleVersions");
    while(sq.next()){
        bibles << sq.value(0).toString();
        bible_id_list << sq.value(1).toString();
    }
    sq.clear();

    // Fill bibles comboboxes
    ui->comboBox_primaryBible->addItems(bibles);
    ui->comboBox_secondaryBible->addItem(tr("None"));
    ui->comboBox_secondaryBible->addItems(bibles);
    ui->comboBox_trinaryBible->addItem(tr("None"));
    ui->comboBox_trinaryBible->addItems(bibles);
    ui->comboBox_operatorBible->addItem(tr("Same as primary Bible"));
    ui->comboBox_operatorBible->addItems(bibles);

    // Set current primary bible
    if(mySettings.primaryBible == "none")
        ui->comboBox_primaryBible->setCurrentIndex(0);
    else
        ui->comboBox_primaryBible->setCurrentIndex(bible_id_list.indexOf(mySettings.primaryBible));

    // Set current secondary bible
    if(mySettings.secondaryBible == "none")
        ui->comboBox_secondaryBible->setCurrentIndex(0);
    else
        ui->comboBox_secondaryBible->setCurrentIndex(bible_id_list.indexOf(mySettings.secondaryBible)+1);
//    updateSecondaryBibleMenu();

    // Set current trinaty bibile
    if(mySettings.trinaryBible == "none")
        ui->comboBox_trinaryBible->setCurrentIndex(0);
    else
        ui->comboBox_trinaryBible->setCurrentIndex(bible_id_list.indexOf(mySettings.trinaryBible)+1);
//    updateTrinaryBibleMenu();
    updateSecondaryBibleMenu();

    // Set current operator bibile
    if(mySettings.operatorBible == "same")
        ui->comboBox_operatorBible->setCurrentIndex(0);
    else
        ui->comboBox_operatorBible->setCurrentIndex(bible_id_list.indexOf(mySettings.operatorBible)+1);
    updateOperatorBibleMenu();

}

void BibleSettingWidget::updateSecondaryBibleMenu()
{
    QString pbible = ui->comboBox_primaryBible->currentText();
    QString sbible = ui->comboBox_secondaryBible->currentText();
    secondary_bibles = bibles;
    secondary_bibles.removeOne(pbible);

    secondary_id_list = bible_id_list;
    secondary_id_list.removeAt(ui->comboBox_primaryBible->currentIndex());
    ui->comboBox_secondaryBible->clear();
    ui->comboBox_secondaryBible->addItem(tr("None"));
    ui->comboBox_secondaryBible->addItems(secondary_bibles);

    int i = ui->comboBox_secondaryBible->findText(sbible);
    if( i != -1 )
        // The same secondary bible is still available
        ui->comboBox_secondaryBible->setCurrentIndex(i);

    updateTrinaryBibleMenu();
}

void BibleSettingWidget::updateTrinaryBibleMenu()
{

    if (/*mySettings.secondaryBible == "none" ||*/ ui->comboBox_secondaryBible->currentIndex() == 0)
    {
        ui->comboBox_trinaryBible->setCurrentIndex(0);
        ui->comboBox_trinaryBible->setEnabled(false);
    }
    else
    {
        ui->comboBox_trinaryBible->setEnabled(true);
        QString sbible = ui->comboBox_secondaryBible->currentText();
        QString tbible = ui->comboBox_trinaryBible->currentText();
        QStringList trinary_bibles = secondary_bibles;
        trinary_bibles.removeOne(sbible);


        trinary_id_list = secondary_id_list;
        trinary_id_list.removeAt(ui->comboBox_secondaryBible->currentIndex()-1);
        ui->comboBox_trinaryBible->clear();
        ui->comboBox_trinaryBible->addItem(tr("None"));
        ui->comboBox_trinaryBible->addItems(trinary_bibles);

        int i = ui->comboBox_trinaryBible->findText(tbible);
        if( i != -1 )
            // The same secondary bible is still available
            ui->comboBox_trinaryBible->setCurrentIndex(i);
    }
}

void BibleSettingWidget::updateOperatorBibleMenu()
{
    QString pbible = ui->comboBox_primaryBible->currentText();
    QString obible = ui->comboBox_operatorBible->currentText();
    QStringList operator_bibles = bibles;
    operator_bibles.removeOne(pbible);

    operator_id_list = bible_id_list;
    operator_id_list.removeAt(ui->comboBox_primaryBible->currentIndex());
    ui->comboBox_operatorBible->clear();
    ui->comboBox_operatorBible->addItem(tr("Same as primary bible"));
    ui->comboBox_operatorBible->addItems(operator_bibles);

    int i = ui->comboBox_operatorBible->findText(obible);
    if( i != -1 )
        // The same operaotr bible is still available
        ui->comboBox_operatorBible->setCurrentIndex(i);

}

void BibleSettingWidget::on_comboBox_primaryBible_activated(const QString &arg1)
{
    updateSecondaryBibleMenu();
    updateOperatorBibleMenu();
}

void BibleSettingWidget::on_comboBox_secondaryBible_activated(const QString &arg1)
{
    updateTrinaryBibleMenu();
}

void BibleSettingWidget::on_comboBox_trinaryBible_activated(const QString &arg1)
{
//    qDebug()<< trinary_id_list.at(ui->comboBox_trinaryBible->currentIndex()-1);
}
