/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2010  Vladislav Kobzar and Matvey Adzhigirey
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

#ifndef DISPLAY1_H
#define DISPLAY1_H

#include <QtGui>

class Display1 : public QWidget
{
    Q_OBJECT
public:
    Display1(QWidget *parent = 0);
//    QTextCodec *codec;
    int paintTextToRect(QPainter *painter, QRect origrect, int flags, QString text);
    void setNewWallpaper(QString path);
    void setNewPassiveWallpaper(QString path);

public slots:
    void fadeIn();
    void fadeOut();
    bool useBlur();
    void setBlur(bool blur);
    bool useFading();
    void setFading(bool fade);
    void renderText(bool text_present);
    QFont getFont();
    void setNewFont(QFont newFont);
    QString getWallpaper();
    QString getPassiveWallpaper();

    void fastbluralpha(QImage &img, int radius);

signals:
    void requestTextDrawing(QPainter *painter, int width, int height);

protected:
    void paintEvent(QPaintEvent *event);


private:
    bool use_fading;
    bool use_blur;
    QFont main_font;
    QString wallpaper_path; // Wallpaper image file path
    QImage wallpaper; // Wallpaper image
    QString passive_wallpaper_path;
    QImage passive_wallpaper;

    QPixmap previous_image_pixmap;
    QImage output_image;
    bool use_active_background;
    bool background_needs_transition;
    int acounter[2];
    QTimer *timer;
    QTimer *timer_out;
};

#endif // DISPLAY1_H
