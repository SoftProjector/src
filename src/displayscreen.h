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

#ifndef DISPLAYSCREEN_H
#define DISPLAYSCREEN_H

#include <QtGui>
#include <phonon>
#include "settings.h"
#include "theme.h"
#include "controlbutton.h"
#include "bible.h"
#include "song.h"
#include "announcewidget.h"
#include "videoinfo.h"

namespace Ui {
class DisplayScreen;
}

class DisplayScreen : public QWidget
{
    Q_OBJECT
    
public:
    explicit DisplayScreen(QWidget *parent = 0);
    ~DisplayScreen();
    Phonon::MediaObject *videoPlayer;

public slots:
    void setNewWallpaper(QString path, bool isToUse);
    void setNewWallpaper(QPixmap wallPix, bool isToUse);
    void setNewPassiveWallpaper(QPixmap wallPix, bool isToUse);

    void fadeIn();
    void fadeOut();
    void setDisplaySettings(DisplaySettings sets);
    void setFading(bool fade);

    void fastbluralpha(QImage &img, int radius);
    void setControlsSettings(DisplayControlsSettings &settings);
    void setControlButtonsVisible(bool visible);
    void positionOpjects();

    void renderText(bool text_present);
    void renderBibleText(Verse verse, BibleSettings &bibleSetings);
    void renderSongText(Stanza stanza, SongSettings &songSettings);
    void renderAnnounceText(AnnounceSlide announce, AnnounceSettings &announceSettings);
    void renderPicture(QPixmap image);
    void renderVideo(VideoInfo &vid);



signals:
    void exitSlide();
    void nextSlide();
    void prevSlide();
    void sendTimeText(QString cTime);
    void updatePlayButton(bool isPlaying);

protected:
    void paintEvent(QPaintEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    
private slots:
    void positionControlButtons();
    void btnNextClicked();
    void btnPrevClicked();
    void btnExitClicked();

    void drawBibleText(QPainter *painter, int width, int height);
    void drawBibleTextToRect(QPainter *painter, QRect& trect, QRect& crect, QString ttext, QString ctext, int tflags, int cflags, int top, int left, int width, int height, int font_size);
    void drawSongText(QPainter *painter, int width, int height);
    QRect drawSongTextToRect(QPainter *painter, QRect bound_rect, bool draw, bool wrap, QString main_text, QString caption_str, QString song_num_str, QString ending_str);
    QRect boundRectOrDrawText(QPainter *painter, bool draw, int left, int top, int width, int height, int flags, QString text);
    void drawAnnounceText(QPainter *painter, int width, int height);

    void updateTimeText();
    void playerStateChanged(Phonon::State newstate, Phonon::State oldstate);

private:
    Ui::DisplayScreen *ui;
    DisplayControlsSettings controlsSettings;
//    DisplaySettings displaySettings;
    bool useFading;
    bool useShadow;
    bool useBluredShadow;
    QFont mainFont;
    QString wallpaperPath; // Wallpaper image file path
//    QImage wallpaper; // Wallpaper image
    QPixmap wallpaper;
    QString passiveWallpaperPath;
    QImage passiveWallpaper;
    QColor foregroundColor;

    QPixmap previous_image_pixmap;
    QImage output_image;
    bool use_active_background;
    bool background_needs_transition;
    int acounter[2];
    QTimer *timer;
    QTimer *timer_out;

    ControlButton *btnNext;
    ControlButton *btnPrev;
    ControlButton *btnExit;

    SongSettings songSets;
    BibleSettings bibleSets;
    AnnounceSettings annouceSets;

    Verse bibleVerse;
    Stanza songStanza;
    AnnounceSlide announcement;

    QString displayType;

    QLabel *textRenderLabel;

    Phonon::VideoWidget *videoWidget;
//    Phonon::VideoPlayer *vidP;
};

#endif // DISPLAYSCREEN_H
