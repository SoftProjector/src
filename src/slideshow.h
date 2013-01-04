#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <QString>
#include <QPixmap>

class SlideShowItem
{
public:
    SlideShowItem();
    QString name;
    QPixmap image;
    QPixmap imageSmall;
    QPixmap imagePreview;
};

class SlideShow
{
public:
    SlideShow();
};

#endif // SLIDESHOW_H
