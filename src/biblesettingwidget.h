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
    void setSettings(BibleSettings settings);
    BibleSettings getSettings();

private slots:
    void loadSettings();
    void updateSecondaryBibleMenu();
    void updateTrinaryBibleMenu();
    void updateOperatorBibleMenu();

    void on_comboBox_primaryBible_activated(const QString &arg1);

    void on_comboBox_secondaryBible_activated(const QString &arg1);

    void on_comboBox_trinaryBible_activated(const QString &arg1);

private:
    QStringList bibles, secondary_bibles;
    QStringList bible_id_list,secondary_id_list,trinary_id_list,operator_id_list;
    BibleSettings mySettings;
    Ui::BibleSettingWidget *ui;
};

#endif // BIBLESETTINGWIDGET_H
