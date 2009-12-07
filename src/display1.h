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
    int getWallState();
    QString getWallpaper();
    QString getFillWallpaper();
    void setWallpaper(QString name, QString value);
    void setWallpaper(int state, QString display_path, QString fill_path);
    void fastbluralpha(QImage &img, int radius);
    void alphaImage(QImage &img, int alpha);

signals:
    void requestTextDrawing(QPainter *painter, int width, int height);

protected:
    void paintEvent(QPaintEvent *event);


private:
    int wallpaper_state; // 0 for black, 1 for display, 2 for fill
    bool use_fading;
    bool use_blur;
    QFont main_font;
    QString wallpaper_path; // Wallpaper image file path
    QString fill_wallpaper_path; // Fill wallpaper file path for when not displaying text
    QImage wallpaper, fill_wallpaper; // Wallpaper image
    QPixmap sharp0;
    QImage sharp1;
    //	QImage blured1;
    int acounter[2];
    QImage m_blurred;
    QTimer *timer;
    QTimer *timer_out;
};

#endif // DISPLAY1_H
