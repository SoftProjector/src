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

#ifndef BIBLESETTINGWIDGET_H
#define BIBLESETTINGWIDGET_H

#include <QtWidgets>
#include <QtSql>
#include "theme.h"
#include "settings.h"
#include "spfunctions.h"

namespace Ui {
class BibleSettingWidget;
}

class BibleSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BibleSettingWidget(QWidget *parent = 0);
    ~BibleSettingWidget();

public slots:
    void setSettings(BibleSettings &settings, BibleSettings &settings2);
    void getSettings(BibleSettings &settings, BibleSettings &settings2);
    void setBibleVersions(BibleVersionSettings &bver, BibleVersionSettings &bver2);
    void getBibleVersions(BibleVersionSettings &bver, BibleVersionSettings &bver2);
    void setDispScreen2Visible(bool visible);

private slots:
    void loadSettings();
    void loadBibleVersions();
    void updateSecondaryBibleMenu();
    void updateSecondaryBibleMenu2();
    void updateTrinaryBibleMenu();
    void updateTrinaryBibleMenu2();
    void updateOperatorBibleMenu();

    void on_comboBoxPrimaryBible_activated(const QString &arg1);
    void on_comboBoxPrimaryBible2_activated(const QString &arg1);
    void on_comboBoxSecondaryBible_activated(const QString &arg1);
    void on_comboBoxSecondaryBible2_activated(const QString &arg1);
    void on_comboBoxTrinaryBible_activated(const QString &arg1);
    void on_comboBoxTrinaryBible2_activated(const QString &arg1);
    void on_comboBoxOperatorBible_activated(const QString &arg1);
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
    void on_checkBoxSameFont_stateChanged(int arg1);
    void on_checkBoxSameFont2_stateChanged(int arg1);
    void on_toolButtonCaptionFont_clicked();
    void on_toolButtonCaptionFont2_clicked();
    void on_checkBoxSameColor_stateChanged(int arg1);
    void on_checkBoxSameColor2_stateChanged(int arg1);
    void on_toolButtonCaptionColor_clicked();
    void on_toolButtonCaptionColor2_clicked();
    void on_toolButtonCaptionShadowColor_clicked();
    void on_toolButtonCaptionShadowColor2_clicked();
    void on_comboBoxCaptionPosition_currentIndexChanged(int index);
    void on_comboBoxCaptionPosition2_currentIndexChanged(int index);
    void on_comboBoxCaptionAlign_currentIndexChanged(int index);
    void on_comboBoxCaptionAlign2_currentIndexChanged(int index);
    void on_comboBoxTransitionType_currentIndexChanged(int index);
    void on_comboBoxTransitionType2_currentIndexChanged(int index);
    void on_comboBoxTextEffects_currentIndexChanged(int index);
    void on_comboBoxTextEffects2_currentIndexChanged(int index);
    void on_comboBoxBackgoundType_currentIndexChanged(int index);
    void on_comboBoxBackgoundType2_currentIndexChanged(int index);
    void on_toolButtonBrowseBackground_clicked();
    void on_toolButtonBrowseBackground2_clicked();
    void on_checkBoxAbbiviations_stateChanged(int arg1);
    void on_checkBoxAbbiviations2_stateChanged(int arg1);
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
    QStringList bibles, secondary_bibles;
    QStringList bible_id_list, secondary_id_list, trinary_id_list, operator_id_list;
    QStringList secondary_bibles2, secondary_id_list2, trinary_id_list2;
    BibleSettings mySettings, mySettings2;
    BibleVersionSettings bversion,bversion2;
    bool isLoading;

    Ui::BibleSettingWidget *ui;
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // BIBLESETTINGWIDGET_H
