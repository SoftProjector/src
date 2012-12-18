#ifndef BIBLESETTINGWIDGET_H
#define BIBLESETTINGWIDGET_H

#include <QtGui>
#include <QtSql>
#include "theme.h"

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
    void setDispScreen2Visible(bool visible);

private slots:
    void loadSettings();
    void updateSecondaryBibleMenu();
    void updateSecondaryBibleMenu2();
    void updateTrinaryBibleMenu();
    void updateTrinaryBibleMenu2();
    void updateOperatorBibleMenu();

    void on_comboBoxPrimaryBible_activated(const QString &arg1);
    void on_comboBoxSecondaryBible_activated(const QString &arg1);
    void on_buttonBrowseBackground_clicked();
    void on_buttonTextColor_clicked();
    void on_buttonFont_clicked();
    void on_pushButtonDefault_clicked();
    void on_checkBoxUseShadow_stateChanged(int arg1);
    void on_groupBoxUseDisp2_toggled(bool arg1);
    void on_comboBoxPrimaryBible2_activated(const QString &arg1);
    void on_comboBoxSecondaryBible2_activated(const QString &arg1);
    void on_buttonBrowseBackground2_clicked();
    void on_buttonTextColor2_clicked();
    void on_buttonFont2_clicked();
    void on_checkBoxUseShadow2_stateChanged(int arg1);

private:
    QStringList bibles, secondary_bibles;
    QStringList bible_id_list, secondary_id_list, trinary_id_list, operator_id_list;
    QStringList secondary_bibles2, secondary_id_list2, trinary_id_list2;
    BibleSettings mySettings, mySettings2;
    Ui::BibleSettingWidget *ui;
};

#endif // BIBLESETTINGWIDGET_H
