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

#include "picturesettingwidget.h"
#include "ui_picturesettingwidget.h"

PictureSettingWidget::PictureSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureSettingWidget)
{
    ui->setupUi(this);
}

PictureSettingWidget::~PictureSettingWidget()
{
    delete ui;
}

void PictureSettingWidget::changeEvent(QEvent *e)
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

void PictureSettingWidget::setSettings(SlideShowSettings &settings)
{
    mySettings = settings;
    ui->checkBoxExpand->setChecked(mySettings.expandSmall);

    if(mySettings.fitType == 0)
        ui->radioButtonFit->setChecked(true);
    else if(mySettings.fitType == 1)
        ui->radioButtonFitExpand->setChecked(true);

    ui->groupBoxResize->setChecked(mySettings.resize);
    ui->comboBoxBoundAmount->setCurrentIndex(mySettings.boundType);
    if(mySettings.boundType == 7)
        ui->lineEditBound->setText(QString::number(mySettings.boundWidth));
    else
        ui->lineEditBound->clear();
    ui->comboBoxTransitionType->setCurrentIndex(mySettings.transitionType);

    mySettings.settingsChanged = false;
}

void PictureSettingWidget::getSettings(SlideShowSettings &settings)
{
    settings = mySettings;
}

void PictureSettingWidget::on_comboBoxBoundAmount_currentIndexChanged(int index)
{
    mySettings.boundType = index;
    if(index == 7)
    {
        ui->lineEditBound->setEnabled(true);
        ui->lineEditBound->setFocus();
    }
    else
    {
        ui->lineEditBound->setEnabled(false);
        QList<int> bts;
        bts<<800<<1024<<1280<<1366<<1440<<1660<<1920;
        mySettings.boundWidth = bts.at(index);
    }

    mySettings.settingsChanged = true;
}

void PictureSettingWidget::on_lineEditBound_textChanged(const QString &arg1)
{
    ui->labelBoundBy->setText("x " + arg1);
}

void PictureSettingWidget::on_lineEditBound_editingFinished()
{
    bool ok;
    QString t = ui->lineEditBound->text();
    if(!t.isEmpty())
    {
        int bw;
        bw = t.toInt(&ok);
        if(!ok)
        {
            QMessageBox mb(this);
            mb.setWindowTitle(tr("Inalid Numeric Value"));
            mb.setText(tr("Entered '%1' custom width is not numeric. ").arg(t));
            mb.exec();
            ui->lineEditBound->setFocus();
            ui->lineEditBound->selectAll();
        }
        else
        {
            mySettings.boundWidth = bw;
            mySettings.settingsChanged = true;
        }
    }
}

void PictureSettingWidget::on_checkBoxExpand_toggled(bool checked)
{
    mySettings.expandSmall = checked;
    mySettings.settingsChanged = true;
}

void PictureSettingWidget::on_radioButtonFit_toggled(bool checked)
{
    if(checked)
        mySettings.fitType = 0;
    else
        mySettings.fitType = 1;
    mySettings.settingsChanged = true;
}

void PictureSettingWidget::on_radioButtonFitExpand_toggled(bool checked)
{
    if(checked)
        mySettings.fitType = 1;
    else
        mySettings.fitType = 0;
    mySettings.settingsChanged = true;
}

void PictureSettingWidget::on_groupBoxResize_toggled(bool arg1)
{
    mySettings.resize = arg1;
}

void PictureSettingWidget::on_comboBoxTransitionType_currentIndexChanged(int index)
{
    mySettings.transitionType = index;
    mySettings.settingsChanged = true;
}
