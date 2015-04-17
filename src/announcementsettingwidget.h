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

#ifndef ANNOUNCEMENTSETTINGWIDGET_H
#define ANNOUNCEMENTSETTINGWIDGET_H

#include <QtWidgets>
#include "theme.h"
#include "spfunctions.h"

namespace Ui {
class AnnouncementSettingWidget;
}

class AnnouncementSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnnouncementSettingWidget(QWidget *parent = 0);
    ~AnnouncementSettingWidget();

public slots:
    void setSettings(TextSettings& settings, TextSettings &settings2);
    void getSettings(TextSettings& settings, TextSettings &settings2);
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
    void on_comboBoxTransitionType_currentIndexChanged(int index);
    void on_comboBoxTransitionType2_currentIndexChanged(int index);
    void on_comboBoxTextEffects_currentIndexChanged(int index);
    void on_comboBoxTextEffects2_currentIndexChanged(int index);
    void on_comboBoxBackgoundType_currentIndexChanged(int index);
    void on_comboBoxBackgoundType2_currentIndexChanged(int index);
    void on_toolButtonBrowseBackground_clicked();
    void on_toolButtonBrowseBackground2_clicked();
    void on_checkBoxCommonLayout_stateChanged(int arg1);
    void on_checkBoxCommonLayout2_stateChanged(int arg1);
    void on_spinBoxMaxScreen_editingFinished();
    void on_spinBoxMaxScreen2_editingFinished();
    void on_comboBoxScreenPosition_currentIndexChanged(int index);
    void on_comboBoxScreenPosition2_currentIndexChanged(int index);
    void on_checkBoxDisp2_stateChanged(int arg1);

    void on_pushButtonDefault_clicked();

    QString getFontText(QFont font);

private:
    TextSettings mySettings, mySettings2;
    Ui::AnnouncementSettingWidget *ui;
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // ANNOUNCEMENTSETTINGWIDGET_H
