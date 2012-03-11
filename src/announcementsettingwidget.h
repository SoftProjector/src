#ifndef ANNOUNCEMENTSETTINGWIDGET_H
#define ANNOUNCEMENTSETTINGWIDGET_H

#include <QWidget>

namespace Ui {
    class AnnouncementSettingWidget;
}

class AnnouncementSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnnouncementSettingWidget(QWidget *parent = 0);
    ~AnnouncementSettingWidget();

private:
    Ui::AnnouncementSettingWidget *ui;
};

#endif // ANNOUNCEMENTSETTINGWIDGET_H
