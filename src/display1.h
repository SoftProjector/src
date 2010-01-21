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
    void alphaImage(QImage &img, int alpha);

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

    QPixmap sharp0;
    QImage sharp1;
    //	QImage blured1;
    int acounter[2];
    QImage m_blurred;
    QTimer *timer;
    QTimer *timer_out;
};

#endif // DISPLAY1_H
