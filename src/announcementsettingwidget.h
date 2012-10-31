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
    void setSettings(AnnounceSettings& settings);
    AnnounceSettings getSettings();

private slots:
    void loadSettings();
    void on_button_Background_clicked();
    void on_button_textColor_clicked();
    void on_button_font_clicked();

    void on_pushButton_default_clicked();

private:
    AnnounceSettings mySettings;
    Ui::AnnouncementSettingWidget *ui;
};

#endif // ANNOUNCEMENTSETTINGWIDGET_H
