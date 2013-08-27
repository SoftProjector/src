#ifndef PICTURESETTINGWIDGET_H
#define PICTURESETTINGWIDGET_H

#include <QWidget>
#include <QMessageBox>
#include "settings.h"

namespace Ui {
class PictureSettingWidget;
}

class PictureSettingWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PictureSettingWidget(QWidget *parent = 0);
    ~PictureSettingWidget();
    void setSettings(SlideShowSettings &settings);
    void getSettings(SlideShowSettings &settings);
    
private slots:
    void on_comboBoxBoundAmount_currentIndexChanged(int index);
    void on_lineEditBound_textChanged(const QString &arg1);

    void on_lineEditBound_editingFinished();

private:
    SlideShowSettings mySettings;
    Ui::PictureSettingWidget *ui;
};

#endif // PICTURESETTINGWIDGET_H
