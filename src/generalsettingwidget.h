#ifndef GENERALSETTINGWIDGET_H
#define GENERALSETTINGWIDGET_H

#include <QWidget>
#include "settings.h"

namespace Ui {
    class GeneralSettingWidget;
}

class GeneralSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralSettingWidget(QWidget *parent = 0);
    ~GeneralSettingWidget();

private:
    Ui::GeneralSettingWidget *ui;
    GeneralSettings mySettings;
    Settings allSetings;

public slots:
    void setSettings(GeneralSettings settings);
    GeneralSettings getSettings();

private slots:
    void on_button_BrowseBack_clicked();
    void on_pushButton_default_clicked();

    void loadSettings();
};

#endif // GENERALSETTINGWIDGET_H
