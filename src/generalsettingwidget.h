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
    QStringList monitors;

public slots:
    void setSettings(GeneralSettings settings);
    void updateSecondaryDisplayScreen();
    GeneralSettings getSettings();

private slots:
    void on_pushButtonDefault_clicked();

    void loadSettings();
    void on_comboBoxDisplayScreen_activated(const QString &arg1);
    void on_buttonBrowseBackgound_clicked();
    void on_groupBoxDisp2Sets_toggled(bool arg1);
    void on_buttonBrowseBackgound2_clicked();
    void on_comboBoxDisplayScreen_2_activated(int index);
};

#endif // GENERALSETTINGWIDGET_H
