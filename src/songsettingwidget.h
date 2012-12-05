#ifndef SONGSETTINGWIDGET_H
#define SONGSETTINGWIDGET_H

#include <QWidget>
#include "settings.h"

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
    void on_buttonSongBackground_clicked();
    void on_buttonTextColor_clicked();
    void on_buttonFont_clicked();
    void on_pushButtonDefault_clicked();
    void on_checkBoxUseShadow_stateChanged(int arg1);
    void on_groupBoxDisplay2_toggled(bool arg1);

    void on_checkBoxUseShadow2_stateChanged(int arg1);

    void on_buttonSongBackground2_clicked();

    void on_buttonTextColor2_clicked();

    void on_buttonFont2_clicked();

private:
    SongSettings mySettings;
    SongSettings mySettings2;

    Ui::SongSettingWidget *ui;
};

#endif // SONGSETTINGWIDGET_H
