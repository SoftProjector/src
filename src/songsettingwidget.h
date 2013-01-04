/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyleft (Ɔ) 2013  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

#ifndef SONGSETTINGWIDGET_H
#define SONGSETTINGWIDGET_H

//#include <QWidget>
#include <QtGui>
#include "theme.h"

namespace Ui {
    class SongSettingWidget;
}

class SongSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SongSettingWidget(QWidget *parent = 0);
    ~SongSettingWidget();

public slots:
    void getSettings(SongSettings &settings, SongSettings &settings2);
    void setSettings(SongSettings &settings, SongSettings &settings2);
    void setDispScreen2Visible(bool visible);

private slots:
    void loadSettings();
    void on_buttonSongBackground_clicked();
    void on_buttonTextColor_clicked();
    void on_buttonFont_clicked();
    void on_pushButtonDefault_clicked();
    void on_checkBoxUseShadow_stateChanged(int arg1);
    void on_groupBoxDisplay2_toggled(bool arg1);

    void on_checkBoxUseShadow2_stateChanged(int arg1);

    void on_buttonSongBackground2_clicked();

    void on_buttonTextColor2_clicked();

    void on_buttonFont2_clicked();

private:
    SongSettings mySettings;
    SongSettings mySettings2;

    Ui::SongSettingWidget *ui;
};

#endif // SONGSETTINGWIDGET_H
