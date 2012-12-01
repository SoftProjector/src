#ifndef DISPLAYSCREEN_H
#define DISPLAYSCREEN_H

#include <QtGui>
#include "settings.h"
#include "controlbutton.h"
#include "bible.h"
#include "song.h"
#include "announcewidget.h"

namespace Ui {
class DisplayScreen;
}

class DisplayScreen : public QWidget
{
    Q_OBJECT
    
public:
    explicit DisplayScreen(QWidget *parent = 0);
    ~DisplayScreen();

public slots:
    void setNewWallpaper(QString path, bool isToUse);
    void setNewPassiveWallpaper(QString path, bool isToUse);

    void fadeIn();
    void fadeOut();
    void setDisplaySettings(DisplaySettings sets);
    void setFading(bool fade);

    void fastbluralpha(QImage &img, int radius);
    void setControlsSettings(DisplayControlsSettings &settings);
    void setControlButtonsVisible(bool visible);

    void renderText(bool text_present);
    void renderBibleText(Verse verse, BibleSettings &bibleSetings);
    void renderSongText(Stanza stanza, SongSettings &songSettings);
    void renderAnnounceText(Announcement announce, AnnounceSettings &announceSettings);

signals:
    void exitSlide();
    void nextSlide();
    void prevSlide();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    
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

private:
    Ui::DisplayScreen *ui;
    DisplayControlsSettings controlsSettings;
//    DisplaySettings displaySettings;
    bool useFading;
    bool useShadow;
    bool useBluredShadow;
    QFont mainFont;
    QString wallpaperPath; // Wallpaper image file path
    QImage wallpaper; // Wallpaper image
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
    Announcement announcement;

    QString textType;
};

#endif // DISPLAYSCREEN_H