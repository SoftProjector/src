#ifndef DISPLAY1_H
#define DISPLAY1_H

#include <QWidget>
#include <QString>
#include <QFont>
#include <QStringList>
#include <QFontMetrics>
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
    QStringList DisplayList;
    int hasCaption;
public:
    Display1(QWidget *parent = 0);
    QTextCodec *codec;


public slots:
    void SetMainText(QString text);
    void SetCaptionText(QString text);
    void setAllText(QString text,QString caption);
    void RenderText();
    void CrossFade();
    QFont getFont();
    void setNewFont(QFont newFont);
    QString getWallpaper();
    void setNewWallpaper(QString path);
    bool getShowBlack();
    void setShowBlack(bool show_black);
    void quit_kill();
    void fastbluralpha(QImage &img, int radius);
    void alphaImage(QImage &img, int alpha);


protected:
    void paintEvent(QPaintEvent *event);

private:
    QString MainText;
    QString CaptionText;
    QString root_path;
    QFont main_font;
    QString wallpaper_path; // Wallpaper image file path
    QImage wallpaper; // Wallpaper image
    bool show_black; // Whether to show black instead of wallpaper on clear
    //	QPixmap FaderPixmap;
    QPixmap sharp0;
    QImage sharp1;
    //	QImage blured1;
    int max_width;
    int acounter[2];
    QImage m_blurred;
    QTimer *timer;
};

#endif // DISPLAY1_H
