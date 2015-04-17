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

#ifndef SONGSETTINGWIDGET_H
#define SONGSETTINGWIDGET_H

//#include <QWidget>
#include <QtWidgets>
#include "theme.h"

namespace Ui {
class SongSettingWidget;
}

class SongSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SongSettingWidget(QWidget *parent = 0);
    ~SongSettingWidget();

public slots:
    void getSettings(SongSettings &settings, SongSettings &settings2);
    void setSettings(SongSettings &settings, SongSettings &settings2);
    void setDispScreen2Visible(bool visible);

private slots:
    void loadSettings();

    void on_checkBoxCommonFont_stateChanged(int arg1);
    void on_checkBoxCommonFont2_stateChanged(int arg1);
    void on_toolButtonTextFont_clicked();
    void on_toolButtonTextFont2_clicked();
    void on_checkBoxCommonColor_stateChanged(int arg1);
    void on_checkBoxCommonColor2_stateChanged(int arg1);
    void on_toolButtonTextColor_clicked();
    void on_toolButtonTextColor2_clicked();
    void on_toolButtonShadowColor_clicked();
    void on_toolButtonShadowColor2_clicked();
    void on_comboBoxVerticalAling_currentIndexChanged(int index);
    void on_comboBoxVerticalAling2_currentIndexChanged(int index);
    void on_comboBoxHorizontalAling_currentIndexChanged(int index);
    void on_comboBoxHorizontalAling2_currentIndexChanged(int index);
    void on_checkBoxStanzaTitle_stateChanged(int arg1);
    void on_checkBoxStanzaTitle2_stateChanged(int arg1);
    void on_checkBoxSongKey_stateChanged(int arg1);
    void on_checkBoxSongKey2_stateChanged(int arg1);
    void on_checkBoxSongNumber_stateChanged(int arg1);
    void on_checkBoxSongNumber2_stateChanged(int arg1);
    void on_checkBoxSameInfoFont_stateChanged(int arg1);
    void on_checkBoxSameInfoFont2_stateChanged(int arg1);
    void on_toolButtonInfoFont_clicked();
    void on_toolButtonInfoFont2_clicked();
    void on_checkBoxSameInfoColor_stateChanged(int arg1);
    void on_checkBoxSameInfoColor2_stateChanged(int arg1);
    void on_toolButtonInfoColor_clicked();
    void on_toolButtonInfoColor2_clicked();
    void on_toolButtonInfoShadowColor_clicked();
    void on_toolButtonInfoShadowColor2_clicked();
    void on_comboBoxInfoAlign_currentIndexChanged(int index);
    void on_comboBoxInfoAlign2_currentIndexChanged(int index);
    void on_groupBoxSongEnding_toggled(bool arg1);
    void on_groupBoxSongEnding2_toggled(bool arg1);
    void on_checkBoxSameEndingFont_stateChanged(int arg1);
    void on_checkBoxSameEndingFont2_stateChanged(int arg1);
    void on_toolButtonEndingFont_clicked();
    void on_toolButtonEndingFont2_clicked();
    void on_checkBoxSameEndingColor_stateChanged(int arg1);
    void on_checkBoxSameEndingColor2_stateChanged(int arg1);
    void on_toolButtonEndingColor_clicked();
    void on_toolButtonEndingColor2_clicked();
    void on_toolButtonEndingShadowColor_clicked();
    void on_toolButtonEndingShadowColor2_clicked();
    void on_comboBoxEndingType_currentIndexChanged(int index);
    void on_comboBoxEndingType2_currentIndexChanged(int index);
    void on_comboBoxEndingPosition_currentIndexChanged(int index);
    void on_comboBoxEndingPosition2_currentIndexChanged(int index);
    void on_comboBoxTransitionType_currentIndexChanged(int index);
    void on_comboBoxTransitionType2_currentIndexChanged(int index);
    void on_comboBoxTextEffects_currentIndexChanged(int index);
    void on_comboBoxTextEffects2_currentIndexChanged(int index);
    void on_comboBoxBackgoundType_currentIndexChanged(int index);
    void on_comboBoxBackgoundType2_currentIndexChanged(int index);
    void on_toolButtonBrowseBackround_clicked();
    void on_toolButtonBrowseBackround2_clicked();
    void on_checkBoxCommonLayout_stateChanged(int arg1);
    void on_checkBoxCommonLayout2_stateChanged(int arg1);
    void on_spinBoxMaxScreen_editingFinished();
    void on_spinBoxMaxScreen2_editingFinished();
    void on_comboBoxScreenPosition_currentIndexChanged(int index);
    void on_comboBoxScreenPosition2_currentIndexChanged(int index);
    void on_checkBoxDisp2_stateChanged(int arg1);
    void on_pushButtonDefault_clicked();

    QString getFontText(QFont font);
    void updateInfoButtons();
    void updateInfoButtons2();

private:
    SongSettings mySettings;
    SongSettings mySettings2;

    Ui::SongSettingWidget *ui;
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // SONGSETTINGWIDGET_H
