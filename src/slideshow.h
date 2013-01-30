#ifndef SLIDESHOW_H
#define SLIDESHOW_H

#include <QtSql>
//#include <QtCore>
#include <QString>
#include <QPixmap>
#include <QDebug>
#include <QProgressDialog>

#include "spfunctions.h"

class SlideShowItem
{
public:
    SlideShowItem();
    int slideId;
    int order;
    QString name;
    QString path;
    QPixmap image;
    QPixmap imageSmall;
    QPixmap imagePreview;
};

class SlideShowInfo
{
public:
    SlideShowInfo();
    int slideSwId;
    QString name;
    QString info;
};

class SlideShow
{
//Q_OBJECT
public:
    SlideShow();
    int slideShowId;
    QString name;
    QString info;
    QList<SlideShowItem> slides;

public slots:
    void loadSlideShow(int id);
    void saveSideShow(QString savelbl, QWidget *ptW, QList<int> delList);
};

#endif // SLIDESHOW_H
