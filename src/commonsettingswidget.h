#ifndef COMMONSETTINGSWIDGET_H
#define COMMONSETTINGSWIDGET_H

#include <QWidget>
#include <QColorDialog>
#include <QFontDialog>
#include <QFileDialog>
#include "settings.h"
#include "spfunctions.h"

namespace Ui {
class CommonSettingsWidget;
}

class CommonSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommonSettingsWidget(QWidget *parent = 0);
    ~CommonSettingsWidget();

public slots:
    void setSettings(TextSettingsBase &settings,TextSettingsBase &settings2);
    void getSettings(TextSettingsBase &settings,TextSettingsBase &settings2);
    void setUseSameDisp2Visible(bool visible);

private slots:
    void loadSettings();
    QString getFontText(QFont font);

    void on_toolButtonTextFont_clicked();
    void on_toolButtonTextFont2_clicked();
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
    void on_buttonBrowseBackground_clicked();
    void on_buttonBrowseBackground2_clicked();
    void on_spinBoxMaxScreen_editingFinished();
    void on_spinBoxMaxScreen2_editingFinished();
    void on_comboBoxScreenPosition_currentIndexChanged(int index);
    void on_comboBoxScreenPosition2_currentIndexChanged(int index);
    void on_checkBoxDisp2_stateChanged(int arg1);

    void on_pushButtonDefault_clicked();




private:
    Ui::CommonSettingsWidget *ui;
    TextSettingsBase mySettings,mySettings2;

protected:
    virtual void changeEvent(QEvent *e);
};

#endif // COMMONSETTINGSWIDGET_H
