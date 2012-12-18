#ifndef GENERALSETTINGWIDGET_H
#define GENERALSETTINGWIDGET_H

#include <QtGui>
#include "settings.h"
#include "addsongbookdialog.h"
#include "theme.h"

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
    QStringList themes;
    QStringList themeIdList;

public slots:
    void setSettings(GeneralSettings settings);
    void updateSecondaryDisplayScreen();
    GeneralSettings getSettings();

signals:
    void setDisp2Use(bool toUse);
    void themeChanged(QString theme_id);

private slots:
    void on_pushButtonDefault_clicked();
    void loadThemes();
    void loadSettings();
    void on_comboBoxDisplayScreen_activated(const QString &arg1);
    void on_comboBoxDisplayScreen_2_activated(int index);
    void on_pushButtonAddTheme_clicked();
    void on_comboBoxTheme_activated(int index);
};

#endif // GENERALSETTINGWIDGET_H
