#include "commonsettingswidget.h"
#include "ui_commonsettingswidget.h"

CommonSettingsWidget::CommonSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommonSettingsWidget)
{
    ui->setupUi(this);
}

CommonSettingsWidget::~CommonSettingsWidget()
{
    delete ui;
}

void CommonSettingsWidget::changeEvent(QEvent *e)
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

void CommonSettingsWidget::setSettings(TextSettingsBase &settings)
{
    mySettings = settings;
        loadSettings();
    mySettings.resetBaseChangeHandles();
}

void CommonSettingsWidget::getSettings(TextSettingsBase &settings)
{
    settings = mySettings;
    mySettings.resetBaseChangeHandles();
}

void CommonSettingsWidget::loadSettings()
{
    QPalette p;

    // Text Font
    ui->labelFont->setText(getFontText(mySettings.textFont));

    // Text Color
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);

    // TextShadow Color
    p.setColor(QPalette::Base,mySettings.textShadowColor);
    ui->graphicViewShadowColor->setPalette(p);

    // Alingment
    ui->comboBoxVerticalAling->setCurrentIndex(mySettings.textAlingmentV);
    ui->comboBoxHorizontalAling->setCurrentIndex(mySettings.textAlingmentH);

    // Transition Type
    ui->comboBoxTransitionType->setCurrentIndex(mySettings.transitionType);

    // Effect Type
    ui->comboBoxTextEffects->setCurrentIndex(mySettings.effectsType);

    // Backround
    ui->comboBoxBackgoundType->setCurrentIndex(mySettings.backgroundType);
    p.setColor(QPalette::Base,mySettings.backgroundColor);
    ui->graphicsViewBackgroundColor->setPalette(p);
    if(mySettings.backgroundType == 1)
        ui->lineEditBackPath->setText(mySettings.backgroundName);
    else if(mySettings.backgroundType == 2)
        ui->lineEditBackPath->setText(mySettings.backgroundVideoPath);

     // Screen Use
    ui->spinBoxMaxScreen->setValue(mySettings.screenUse);
    ui->comboBoxScreenPosition->setCurrentIndex(mySettings.screenPosition);
}

QString CommonSettingsWidget::getFontText(QFont font)
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

void CommonSettingsWidget::on_toolButtonTextFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.textFont,this);
    if(ok)
    {
        mySettings.textFont = font;
        mySettings.isChangedTextFont = true;
    }

    ui->labelFont->setText(getFontText(mySettings.textFont));
}

void CommonSettingsWidget::on_toolButtonTextColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textColor,this));
    if(c.isValid())
{
        mySettings.textColor = c;
        mySettings.isChangedTextColor = true;
}
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
}

void CommonSettingsWidget::on_toolButtonShadowColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textShadowColor,this));
    if(c.isValid())
{
        mySettings.textShadowColor = c;
        mySettings.isChangedTextShadowColor = true;
}
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textShadowColor);
    ui->graphicViewShadowColor->setPalette(p);
}

void CommonSettingsWidget::on_comboBoxVerticalAling_currentIndexChanged(int index)
{
    mySettings.textAlingmentV = index;
    mySettings.isChangedAlingV = true;
}

void CommonSettingsWidget::on_comboBoxHorizontalAling_currentIndexChanged(int index)
{
    mySettings.textAlingmentH = index;
    mySettings.isChangedAlingH = true;
}

void CommonSettingsWidget::on_comboBoxTransitionType_currentIndexChanged(int index)
{
    mySettings.transitionType = index;
    mySettings.isChangesTranType = true;
}

void CommonSettingsWidget::on_comboBoxTextEffects_currentIndexChanged(int index)
{
    mySettings.effectsType = index;
    mySettings.isChangedEffectType = true;
}

void CommonSettingsWidget::on_comboBoxBackgoundType_currentIndexChanged(int index)
{
    mySettings.backgroundType = index;
    if(index == 0)
    {
        ui->graphicsViewBackgroundColor->setVisible(true);
        ui->lineEditBackPath->setVisible(false);
        ui->buttonBrowseBackground->setText(tr("Select Color..."));
    }
    else
    {
        if(index == 1)
            ui->lineEditBackPath->setText(mySettings.backgroundName);
        else if(index == 2)
            ui->lineEditBackPath->setText(mySettings.backgroundVideoPath);
        ui->graphicsViewBackgroundColor->setVisible(false);
        ui->lineEditBackPath->setVisible(true);
        ui->buttonBrowseBackground->setText(tr("Browse..."));
    }
    mySettings.isChangedBackType = true;
}

void CommonSettingsWidget::on_buttonBrowseBackground_clicked()
{
    QString filename;
    if (mySettings.backgroundType == 0)
    {
        QColor c(QColorDialog::getColor(mySettings.backgroundColor,this));
        if(c.isValid())
        {
            mySettings.backgroundColor = c;
            mySettings.isChangedBackColor = true;
        }
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
            mySettings.isChangedBackPix = true;
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
            mySettings.isChangedBackVid = true;
        }
    }
}

void CommonSettingsWidget::on_spinBoxMaxScreen_editingFinished()
{
    mySettings.screenUse = ui->spinBoxMaxScreen->value();
    mySettings.isChangedScreenUse = true;
}

void CommonSettingsWidget::on_comboBoxScreenPosition_currentIndexChanged(int index)
{
    mySettings.screenPosition = index;
    mySettings.isChangedScreenPos = true;
}

void CommonSettingsWidget::on_pushButtonDefault_clicked()
{
    TextSettingsBase t;
    mySettings = t;
    loadSettings();
}

