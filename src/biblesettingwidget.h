#ifndef BIBLESETTINGWIDGET_H
#define BIBLESETTINGWIDGET_H

#include <QtCore>
#include <QtSql>
#include "settings.h"

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
    void setDefaults();
    BibleSettings getSettings();

private slots:
    void loadSettings();

private:
    BibleSettings settings;
    Ui::BibleSettingWidget *ui;
};

#endif // BIBLESETTINGWIDGET_H
