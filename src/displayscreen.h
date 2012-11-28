#ifndef DISPLAYSCREEN_H
#define DISPLAYSCREEN_H

#include <QtGui>
#include "settings.h"
#include "controlbutton.h"

namespace Ui {
class DisplayScreen;
}

class DisplayScreen : public QWidget
{
    Q_OBJECT
    
public:
    explicit DisplayScreen(QWidget *parent = 0);
    ~DisplayScreen();
    int paintTextToRect(QPainter *painter, QRect origrect, int flags, QString text);
    void setNewWallpaper(QString path, bool isToUse);
    void setNewPassiveWallpaper(QString path, bool isToUse);

public slots:
    void fadeIn();
    void fadeOut();
    void setBlur(bool blur);
    void setUseShadow(bool useShadow);
    void setDisplaySettings(DisplaySettings sets);
    void setFading(bool fade);
    void renderText(bool text_present);
    void setNewFont(QFont newFont);
    void setForegroundColor(QColor new_color);
    void fastbluralpha(QImage &img, int radius);
    void setControlsSettings(DisplayControlsSettings &settings);
    void setControlButtonsVisible(bool visible);

signals:
    void requestTextDrawing(QPainter *painter, int width, int height);
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

private:
    Ui::DisplayScreen *ui;
    DisplayControlsSettings controlsSettings;
    DisplaySettings displaySettings;
    bool use_fading;
    bool use_shadow;
    bool use_blur;
    QFont main_font;
    QString wallpaper_path; // Wallpaper image file path
    QImage wallpaper; // Wallpaper image
    QString passive_wallpaper_path;
    QImage passive_wallpaper;
    QColor foreground_color;

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
};

#endif // DISPLAYSCREEN_H
