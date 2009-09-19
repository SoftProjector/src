#ifndef DISPLAY1_H
#define DISPLAY1_H

#include <QtGui>

class Display1 : public QWidget
{
    Q_OBJECT
public:
    Display1(QWidget *parent = 0);


    int paintTextToRect(QPainter *painter, QRect origrect, int flags, QString text);


public slots:
    bool useFading();
    void setFading(bool fade);
    void renderText(bool text_present);
    QFont getFont();
    void setNewFont(QFont newFont);
    QString getWallpaper();
    void setNewWallpaper(QString path);
    bool getShowBlack();
    void setShowBlack(bool show_black);
    void fastbluralpha(QImage &img, int radius);
    void alphaImage(QImage &img, int alpha);

signals:
    void requestTextDrawing(QPainter *painter, int width, int height);

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void fadeIn();
    void fadeOut();


private:
    bool use_fading;
    QFont main_font;
    QString wallpaper_path; // Wallpaper image file path
    QImage wallpaper; // Wallpaper image
    bool show_black; // Whether to show black instead of wallpaper on clear
    QPixmap sharp0;
    QImage sharp1;
    //	QImage blured1;
    int acounter[2];
    QImage m_blurred;
    QTimer *timer;
    QTimer *timer_out;
};

#endif // DISPLAY1_H
