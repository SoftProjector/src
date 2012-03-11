#ifndef BIBLESETTINGWIDGET_H
#define BIBLESETTINGWIDGET_H

#include <QWidget>

namespace Ui {
    class BibleSettingWidget;
}

class BibleSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BibleSettingWidget(QWidget *parent = 0);
    ~BibleSettingWidget();

private:
    Ui::BibleSettingWidget *ui;
};

#endif // BIBLESETTINGWIDGET_H
