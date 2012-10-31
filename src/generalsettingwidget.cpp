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

void GeneralSettingWidget::setSettings(GeneralSettings settings)
{
    mySettings = settings;
    loadSettings();
}

void GeneralSettingWidget::loadSettings()
{
    ui->checkBox_displayOnTop->setChecked(mySettings.displayIsOnTop);
    ui->checkBox_useShadow->setChecked(mySettings.useShadow);
    ui->checkBox_useFading->setChecked(mySettings.useFading);
    ui->checkBox_useBlurredShadow->setChecked(mySettings.useBlurShadow);
    ui->groupBox_Back->setChecked(mySettings.useBackground);
    ui->lineEdit_BackPath->setText(mySettings.backgroundPath);

    // Set display screen selection
    QDesktopWidget d;
    int screen_count = d.screenCount();
    ui->comboBox_displayScreen->clear();
    for(int i(0); i<screen_count;++i)
    {
        ui->comboBox_displayScreen->addItem(QString::number(i+1));
    }
    ui->comboBox_displayScreen->setCurrentIndex(mySettings.displayScreen);
}

GeneralSettings GeneralSettingWidget::getSettings()
{
    mySettings.displayIsOnTop = ui->checkBox_displayOnTop->isChecked();
    mySettings.useShadow = ui->checkBox_useShadow->isChecked();
    mySettings.useFading = ui->checkBox_useFading->isChecked();
    mySettings.useBlurShadow = ui->checkBox_useBlurredShadow->isChecked();
    mySettings.useBackground = ui->groupBox_Back->isChecked();
    mySettings.backgroundPath = ui->lineEdit_BackPath->text();

    mySettings.displayScreen = ui->comboBox_displayScreen->currentIndex();

    return mySettings;
}

void GeneralSettingWidget::on_button_BrowseBack_clicked()
{
    // Change background
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for main wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));

    if( !filename.isNull() )
        ui->lineEdit_BackPath->setText(filename);

}

void GeneralSettingWidget::on_pushButton_default_clicked()
{
//    mySettings.displayIsOnTop = false;
//    mySettings.useShadow = true;
//    mySettings.useFading = true;
//    mySettings.useBlurShadow = false;
//    mySettings.useBackground = false;
//    mySettings.backgroundPath.clear();
    GeneralSettings g;
    mySettings = g;
    loadSettings();
}

void GeneralSettingWidget::on_checkBox_useShadow_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBox_useBlurredShadow->setEnabled(true);
    else
    {
        ui->checkBox_useBlurredShadow->setChecked(false);
        ui->checkBox_useBlurredShadow->setEnabled(false);
    }
}
