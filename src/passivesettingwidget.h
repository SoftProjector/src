#ifndef PASSIVESETTINGWIDGET_H
#define PASSIVESETTINGWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include "theme.h"

namespace Ui {
class PassiveSettingWidget;
}

class PassiveSettingWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PassiveSettingWidget(QWidget *parent = 0);
    ~PassiveSettingWidget();

public slots:
    void setSetings(PassiveSettings &settings, PassiveSettings &settings2);
    void getSettings(PassiveSettings &settings, PassiveSettings &settings2);
    void setDispScreen2Visible(bool visible);

private slots:
    void loadSettings();
    void on_buttonBrowseBackgound_clicked();
    void on_groupBoxDisp2Sets_toggled(bool arg1);
    void on_buttonBrowseBackgound2_clicked();
    void on_pushButtonDefault_clicked();

private:
    Ui::PassiveSettingWidget *ui;
    PassiveSettings mySettings,mySettings2;
};

#endif // PASSIVESETTINGWIDGET_H
