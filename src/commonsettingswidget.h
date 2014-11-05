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
    void setSettings(TextSettings &settings,TextSettings &settings2);
    void getSettings(TextSettings &settings,TextSettings &settings2);
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
    void on_comboBoxBackgoundType_currentIndexChanged(int index);
    void on_comboBoxBackgoundType2_currentIndexChanged(int index);
    void on_buttonBrowseBackground_clicked();
    void on_buttonBrowseBackground2_clicked();
    void on_checkBoxDisp2_stateChanged(int arg1);

    void on_pushButtonDefault_clicked();

private:
    Ui::CommonSettingsWidget *ui;
    TextSettings mySettings,mySettings2;

protected:
    virtual void changeEvent(QEvent *e);
};

#endif // COMMONSETTINGSWIDGET_H
