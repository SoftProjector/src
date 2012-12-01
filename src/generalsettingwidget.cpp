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
    ui->checkBoxDisplayOnTop->setChecked(mySettings.displayIsOnTop);
    ui->groupBoxBackground->setChecked(mySettings.useBackground);
    ui->lineEditBackgroundPath->setText(mySettings.backgroundPath);

    // Get display screen infomration
    monitors.clear();
    QDesktopWidget d;
    int screen_count = d.screenCount();
    ui->comboBoxDisplayScreen->clear();
    for(int i(0); i<screen_count;++i)
    {
        monitors << QString("%1 - %2x%3").arg(i+1).arg(d.screenGeometry(i).width()).arg(d.screenGeometry(i).height());
    }

    if(screen_count>1)
        ui->groupBoxDisplayScreen->setEnabled(true);
    else
        ui->groupBoxDisplayScreen->setEnabled(false);

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

    // Set Display Controls
    if(screen_count>1)
        ui->groupBoxDisplayControls->setEnabled(false);
    else
        ui->groupBoxDisplayControls->setEnabled(true);
    ui->comboBoxIconSize->setCurrentIndex(mySettings.displayControls.buttonSize);
    ui->comboBoxControlsAlignV->setCurrentIndex(mySettings.displayControls.alignmentV);
    ui->comboBoxControlsAlignH->setCurrentIndex(mySettings.displayControls.alignmentH);
    ui->horizontalSliderOpacity->setValue(mySettings.displayControls.opacity*100);

    // Set display 2 settings
    ui->groupBoxDisp2Sets->setChecked(mySettings.useDisplaySettings2);
    ui->groupBoxBackground2->setChecked(mySettings.useBackground2);
    ui->lineEditBackgroundPath2->setText(mySettings.backgroundPath2);
}

GeneralSettings GeneralSettingWidget::getSettings()
{
    mySettings.displayIsOnTop = ui->checkBoxDisplayOnTop->isChecked();
    mySettings.useBackground = ui->groupBoxBackground->isChecked();
    mySettings.backgroundPath = ui->lineEditBackgroundPath->text();

    mySettings.displayScreen = ui->comboBoxDisplayScreen->currentIndex();
    mySettings.displayScreen2 = monitors.indexOf(ui->comboBoxDisplayScreen_2->currentText());

    mySettings.displayControls.buttonSize = ui->comboBoxIconSize->currentIndex();
    mySettings.displayControls.alignmentV = ui->comboBoxControlsAlignV->currentIndex();
    mySettings.displayControls.alignmentH = ui->comboBoxControlsAlignH->currentIndex();
    qreal r = ui->horizontalSliderOpacity->value();
    r = r/100;
    mySettings.displayControls.opacity = r;

    // get dispaly 2 settings
    mySettings.useDisplaySettings2 = ui->groupBoxDisp2Sets->isChecked();
    on_groupBoxDisp2Sets_toggled(ui->groupBoxDisp2Sets->isChecked());
    mySettings.useBackground2 = ui->groupBoxBackground2->isChecked();
    mySettings.backgroundPath2 = ui->lineEditBackgroundPath2->text();

    return mySettings;
}

void GeneralSettingWidget::on_pushButtonDefault_clicked()
{
    GeneralSettings g;
    mySettings = g;
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
        ui->comboBoxDisplayScreen_2->setCurrentIndex(0);

    if(ui->comboBoxDisplayScreen_2->currentIndex()<=0)
        ui->groupBoxDisp2Sets->setVisible(false);
    else
        ui->groupBoxDisp2Sets->setVisible(true);
}

void GeneralSettingWidget::on_comboBoxDisplayScreen_activated(const QString &arg1)
{
    updateSecondaryDisplayScreen();
}

void GeneralSettingWidget::on_buttonBrowseBackgound_clicked()
{
    // Change background
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for main wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));

    if( !filename.isNull() )
        ui->lineEditBackgroundPath->setText(filename);
}

void GeneralSettingWidget::on_groupBoxDisp2Sets_toggled(bool arg1)
{
    ui->groupBoxBackground2->setVisible(arg1);
}

void GeneralSettingWidget::on_buttonBrowseBackgound2_clicked()
{
    // Change background
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for main wallpaper"),
                                                    ".", tr("Images (*.png *.jpg *.jpeg)"));

    if( !filename.isNull() )
        ui->lineEditBackgroundPath2->setText(filename);
}

void GeneralSettingWidget::on_comboBoxDisplayScreen_2_activated(int index)
{
    if(index<=0)
        ui->groupBoxDisp2Sets->setVisible(false);
    else
        ui->groupBoxDisp2Sets->setVisible(true);
}
