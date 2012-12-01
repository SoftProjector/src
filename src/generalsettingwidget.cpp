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

    // Set Display Controls
    if(screen_count>1)
        ui->groupBoxDisplayControls->setEnabled(false);
    else
        ui->groupBoxDisplayControls->setEnabled(true);
    ui->comboBoxIconSize->setCurrentIndex(mySettings.displayControls.buttonSize);
    ui->comboBoxControlsAlignV->setCurrentIndex(mySettings.displayControls.alignmentV);
    ui->comboBoxControlsAlignH->setCurrentIndex(mySettings.displayControls.alignmentH);
    ui->horizontalSliderOpacity->setValue(mySettings.displayControls.opacity*100);
}

GeneralSettings GeneralSettingWidget::getSettings()
{
    mySettings.displayIsOnTop = ui->checkBox_displayOnTop->isChecked();
    mySettings.useBackground = ui->groupBox_Back->isChecked();
    mySettings.backgroundPath = ui->lineEdit_BackPath->text();

    mySettings.displayScreen = ui->comboBox_displayScreen->currentIndex();

    mySettings.displayControls.buttonSize = ui->comboBoxIconSize->currentIndex();
    mySettings.displayControls.alignmentV = ui->comboBoxControlsAlignV->currentIndex();
    mySettings.displayControls.alignmentH = ui->comboBoxControlsAlignH->currentIndex();
    qreal r = ui->horizontalSliderOpacity->value();
    r = r/100;
    mySettings.displayControls.opacity = r;

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
    GeneralSettings g;
    mySettings = g;
    loadSettings();
}
