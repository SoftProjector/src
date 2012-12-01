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
    SongSettings getSettings();
    void setSettings(SongSettings settings);

private slots:
    void loadSettings();

    void on_button_SongBackground_clicked();

    void on_button_textColor_clicked();

    void on_button_font_clicked();

    void on_pushButton_default_clicked();

    void on_checkBox_useShadow_stateChanged(int arg1);

    void on_groupBoxDisplay2_toggled(bool arg1);

private:
    SongSettings mySettings;
    Ui::SongSettingWidget *ui;
};

#endif // SONGSETTINGWIDGET_H
