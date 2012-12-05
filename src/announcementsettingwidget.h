#ifndef ANNOUNCEMENTSETTINGWIDGET_H
#define ANNOUNCEMENTSETTINGWIDGET_H

#include <QWidget>
#include "settings.h"

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
    void setSettings(AnnounceSettings& settings, AnnounceSettings &settings2);
    void getSettings(AnnounceSettings& settings, AnnounceSettings &settings2);
    void setDispScreen2Visible(bool visible);

private slots:
    void loadSettings();
    void on_buttonBackground_clicked();
    void on_buttonTextColor_clicked();
    void on_buttonFont_clicked();
    void on_pushButtonDefault_clicked();
    void on_checkBoxUseShadow_stateChanged(int arg1);
    void on_groupBoxUseDisp2_toggled(bool arg1);
    void on_checkBoxUseShadow2_stateChanged(int arg1);
    void on_buttonTextColor2_clicked();
    void on_buttonFont2_clicked();
    void on_buttonBackground2_clicked();

private:
    AnnounceSettings mySettings, mySettings2;
    Ui::AnnouncementSettingWidget *ui;
};

#endif // ANNOUNCEMENTSETTINGWIDGET_H
