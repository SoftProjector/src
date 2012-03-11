#ifndef SONGSETTINGWIDGET_H
#define SONGSETTINGWIDGET_H

#include <QWidget>

namespace Ui {
    class SongSettingWidget;
}

class SongSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SongSettingWidget(QWidget *parent = 0);
    ~SongSettingWidget();

private:
    Ui::SongSettingWidget *ui;
};

#endif // SONGSETTINGWIDGET_H
