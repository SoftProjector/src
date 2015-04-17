#include "commonsettingswidget.h"
#include "ui_commonsettingswidget.h"

CommonSettingsWidget::CommonSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommonSettingsWidget)
{
    ui->setupUi(this);
//    ui->checkBoxDisp2->setChecked(true);
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

void CommonSettingsWidget::setSettings(TextSettingsBase &settings, TextSettingsBase &settings2)
{
    mySettings = settings;
    mySettings2 = settings2;

    loadSettings();
    mySettings.resetBaseChangeHandles();
    mySettings2.resetBaseChangeHandles();
}

void CommonSettingsWidget::getSettings(TextSettingsBase &settings, TextSettingsBase &settings2)
{
    settings = mySettings;
    settings2 = mySettings2;
    mySettings.resetBaseChangeHandles();
    mySettings2.resetBaseChangeHandles();
}

void CommonSettingsWidget::loadSettings()
{
    QPalette p;

    // Text Font
    ui->labelFont->setText(getFontText(mySettings.textFont));
    ui->labelFont2->setText(getFontText(mySettings2.textFont));

    // Text Color
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);

    // TextShadow Color
    p.setColor(QPalette::Base,mySettings.textShadowColor);
    ui->graphicViewShadowColor->setPalette(p);
    p.setColor(QPalette::Base,mySettings2.textShadowColor);
    ui->graphicViewShadowColor2->setPalette(p);

    // Alingment
    ui->comboBoxVerticalAling->setCurrentIndex(mySettings.textAlingmentV);
    ui->comboBoxHorizontalAling->setCurrentIndex(mySettings.textAlingmentH);
    ui->comboBoxVerticalAling2->setCurrentIndex(mySettings2.textAlingmentV);
    ui->comboBoxHorizontalAling2->setCurrentIndex(mySettings2.textAlingmentH);

    // Transition Type
    ui->comboBoxTransitionType->setCurrentIndex(mySettings.transitionType);
    ui->comboBoxTransitionType2->setCurrentIndex(mySettings2.transitionType);

    // Effect Type
    ui->comboBoxTextEffects->setCurrentIndex(mySettings.effectsType);
    ui->comboBoxTextEffects2->setCurrentIndex(mySettings2.effectsType);

    // Backround
    ui->comboBoxBackgoundType->setCurrentIndex(mySettings.backgroundType);
    p.setColor(QPalette::Base,mySettings.backgroundColor);
    ui->graphicsViewBackgroundColor->setPalette(p);
    if(mySettings.backgroundType == 1)
        ui->lineEditBackPath->setText(mySettings.backgroundName);
    else if(mySettings.backgroundType == 2)
        ui->lineEditBackPath->setText(mySettings.backgroundVideoPath);

    ui->comboBoxBackgoundType2->setCurrentIndex(mySettings2.backgroundType);
    p.setColor(QPalette::Base,mySettings2.backgroundColor);
    ui->graphicsViewBackgroundColor2->setPalette(p);
    if(mySettings2.backgroundType == 1)
        ui->lineEditBackPath2->setText(mySettings2.backgroundName);
    else if(mySettings2.backgroundType == 2)
        ui->lineEditBackPath2->setText(mySettings2.backgroundVideoPath);

    // Screen Use
    ui->spinBoxMaxScreen->setValue(mySettings.screenUse);
    ui->comboBoxScreenPosition->setCurrentIndex(mySettings.screenPosition);
    ui->spinBoxMaxScreen2->setValue(mySettings2.screenUse);
    ui->comboBoxScreenPosition2->setCurrentIndex(mySettings2.screenPosition);

    // Secondary Screen
    ui->checkBoxDisp2->setChecked(mySettings.useSameForDisp2);
    on_checkBoxDisp2_stateChanged(mySettings.useSameForDisp2);
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

void CommonSettingsWidget::setUseSameDisp2Visible(bool visible)
{
    ui->checkBoxDisp2->setVisible(visible);
    if(!mySettings.useSameForDisp2)
        ui->groupBoxUseDisp2->setVisible(visible);
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

void CommonSettingsWidget::on_toolButtonTextFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.textFont,this);
    if(ok)
    {
        mySettings2.textFont = font;
        mySettings2.isChangedTextFont = true;
    }

    ui->labelFont2->setText(getFontText(mySettings2.textFont));
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

void CommonSettingsWidget::on_toolButtonTextColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textColor,this));
    if(c.isValid())
{
        mySettings2.textColor = c;
        mySettings2.isChangedTextColor = true;
}
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);
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

void CommonSettingsWidget::on_toolButtonShadowColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textShadowColor,this));
    if(c.isValid())
{
        mySettings2.textShadowColor = c;
        mySettings2.isChangedTextShadowColor = true;
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textShadowColor);
    ui->graphicViewShadowColor2->setPalette(p);
}

void CommonSettingsWidget::on_comboBoxVerticalAling_currentIndexChanged(int index)
{
    mySettings.textAlingmentV = index;
    mySettings.isChangedAlingV = true;
}

void CommonSettingsWidget::on_comboBoxVerticalAling2_currentIndexChanged(int index)
{
    mySettings2.textAlingmentV = index;
    mySettings2.isChangedAlingV = true;
}

void CommonSettingsWidget::on_comboBoxHorizontalAling_currentIndexChanged(int index)
{
    mySettings.textAlingmentH = index;
    mySettings.isChangedAlingH = true;
}

void CommonSettingsWidget::on_comboBoxHorizontalAling2_currentIndexChanged(int index)
{
    mySettings2.textAlingmentH = index;
    mySettings2.isChangedAlingH = true;
}

void CommonSettingsWidget::on_comboBoxTransitionType_currentIndexChanged(int index)
{
    mySettings.transitionType = index;
    mySettings.isChangesTranType = true;
}

void CommonSettingsWidget::on_comboBoxTransitionType2_currentIndexChanged(int index)
{
    mySettings2.transitionType = index;
    mySettings2.isChangesTranType = true;
}

void CommonSettingsWidget::on_comboBoxTextEffects_currentIndexChanged(int index)
{
    mySettings.effectsType = index;
    mySettings.isChangedEffectType = true;
}

void CommonSettingsWidget::on_comboBoxTextEffects2_currentIndexChanged(int index)
{
    mySettings2.effectsType = index;
    mySettings2.isChangedEffectType = true;
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

void CommonSettingsWidget::on_comboBoxBackgoundType2_currentIndexChanged(int index)
{
    mySettings2.backgroundType = index;
    if(index == 0)
    {
        ui->graphicsViewBackgroundColor2->setVisible(true);
        ui->lineEditBackPath2->setVisible(false);
        ui->buttonBrowseBackground2->setText(tr("Select Color..."));
    }
    else
    {
        if(index == 1)
            ui->lineEditBackPath2->setText(mySettings2.backgroundName);
        else if(index == 2)
            ui->lineEditBackPath2->setText(mySettings2.backgroundVideoPath);
        ui->graphicsViewBackgroundColor2->setVisible(false);
        ui->lineEditBackPath2->setVisible(true);
        ui->buttonBrowseBackground2->setText(tr("Browse..."));
    }
     mySettings2.isChangedBackType = true;
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

void CommonSettingsWidget::on_buttonBrowseBackground2_clicked()
{
    QString filename;
    if (mySettings2.backgroundType == 0)
    {
        QColor c(QColorDialog::getColor(mySettings2.backgroundColor,this));
        if(c.isValid())
        {
            mySettings2.backgroundColor = c;
            mySettings2.isChangedBackColor = true;
        }
        QPalette p;
        p.setColor(QPalette::Base,mySettings2.backgroundColor);
        ui->graphicsViewBackgroundColor2->setPalette(p);
    }
    else if (mySettings2.backgroundType == 1)
    {
        filename = QFileDialog::getOpenFileName(this, tr("Select a image for Bible wallpaper"),
                                                ".", tr("Images(%1)").arg(getSupportedImageFormats()));
        if(!filename.isNull())
        {
            QPixmap pix(filename);
            if(pix.width()>1280 || pix.height()>1280)
                mySettings2.backgroundPix = pix.scaled(1280,1280,Qt::KeepAspectRatio);
            else
                mySettings.backgroundPix = pix;
            QFileInfo fip(filename);
            filename = fip.fileName();
            mySettings2.backgroundName = filename;
            ui->lineEditBackPath2->setText(filename);
            mySettings2.isChangedBackPix = true;
        }
    }
    else if (mySettings2.backgroundType == 2)
    {
        filename = QFileDialog::getOpenFileName(this, tr("Select a video for Bible wallpaper"),
                                                ".", "*");
        if(!filename.isNull())
        {
            mySettings2.backgroundVideoPath = filename;
            QFileInfo fiv(filename);
            filename = fiv.fileName();
            ui->lineEditBackPath2->setText(filename);
            mySettings2.isChangedBackVid = true;
        }
    }
}

void CommonSettingsWidget::on_spinBoxMaxScreen_editingFinished()
{
    mySettings.screenUse = ui->spinBoxMaxScreen->value();
    mySettings.isChangedScreenUse = true;
}

void CommonSettingsWidget::on_spinBoxMaxScreen2_editingFinished()
{
    mySettings2.screenUse = ui->spinBoxMaxScreen2->value();
    mySettings2.isChangedScreenUse = true;
}

void CommonSettingsWidget::on_comboBoxScreenPosition_currentIndexChanged(int index)
{
    mySettings.screenPosition = index;
    mySettings.isChangedScreenPos = true;
}

void CommonSettingsWidget::on_comboBoxScreenPosition2_currentIndexChanged(int index)
{
    mySettings2.screenPosition = index;
    mySettings2.isChangedScreenPos = true;
}

void CommonSettingsWidget::on_checkBoxDisp2_stateChanged(int arg1)
{
    mySettings.useSameForDisp2 = arg1;
    mySettings.isChangedSameDisp2 = true;
    ui->groupBoxUseDisp2->setVisible(!arg1);
}

void CommonSettingsWidget::on_pushButtonDefault_clicked()
{
    TextSettingsBase t;
    mySettings = t;
    mySettings2 = t;
    loadSettings();
}
