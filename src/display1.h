#ifndef DISPLAY1_H
#define DISPLAY1_H

#include <QWidget>
#include <QString>
#include <QFont>
#include <QStringList>
#include <QImage>
#include <QTimer>
#include <QPaintEngine>
#include <QPixmap>
#include <cmath>
#include <QFile>
#include <QTextCodec>

class Display1 : public QWidget
{
    Q_OBJECT
public:
    Display1(QWidget *parent = 0);
    QTextCodec *codec;
    int paintTextToRect(QPainter *painter, QRect origrect, int flags, QString text);


public slots:
    void renderText(bool text_present);
    void CrossFade();
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

private:
    QFont main_font;
    QString wallpaper_path; // Wallpaper image file path
    QImage wallpaper; // Wallpaper image
    bool show_black; // Whether to show black instead of wallpaper on clear
    QImage sharp1;
    //	QImage blured1;
    int acounter[2];
    QImage m_blurred;
    QTimer *timer;
};

#endif // DISPLAY1_H
