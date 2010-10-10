/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2010  Vladislav Kobzar and Matvey Adzhigirey
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

#include <QPainter>
#include <QDebug>

#include "display1.h"
#ifdef Q_WS_MAC
#include <CoreFoundation/CoreFoundation.h>
#endif
#define MARGIN 20
#define	BORDER 3
#define TEXT_COLOR "BLACK" //inverse?
#define SHADOW_COLOR "BLACK"
#define BLUR_RADIUS 15



Display1::Display1(QWidget *parent)
        : QWidget(parent)
{
    setPalette(QPalette(QColor("BLACK"),QColor("BLACK")));
    //root_path="./";
#ifdef Q_WS_MAC
    CFURLRef pluginRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(pluginRef,
                          kCFURLPOSIXPathStyle);
    const char *pathPtr = CFStringGetCStringPtr(macPath,
                          CFStringGetSystemEncoding());
    CFRelease(pluginRef);
    CFRelease(macPath);

    //root_path=pathPtr;
#endif
    timer = new QTimer(this);
    timer_out = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(fadeIn()));
//    connect(timer_out, SIGNAL(timeout()), this, SLOT(fadeOut()));

    acounter[0]=255;

}

void Display1::fadeIn()
{
    if (use_fading)
    {
        acounter[0]+=64;
        if (acounter[0]>255)acounter[0]=255;
        if (acounter[0]>254){timer->stop();}
        update();
    }
}


void Display1::fadeOut() // For future
{
//    acounter[0]-=24;
//    if (acounter[0]<0)acounter[0]=0;
//    if (acounter[0]<1){timer_out->stop();}
//    update();
}

int Display1::paintTextToRect(QPainter *painter, QRect origrect, int flags, QString text)
{

    int left = origrect.left();
    int top = origrect.top();
    int w = origrect.width();
    int h = origrect.height();

    QFont font = painter->font();
    int orig_font_size = font.pointSize();
    // Keep decreasing the font size until the text fits into the allocated space:
    QRect rect;

    bool exit = false;
    while( !exit )
    {
        rect = painter->boundingRect(left, top, w, h, flags, text);
        exit = ( rect.width() <= w && rect.height() <= h );
        if( !exit )
        {
            font.setPointSize( font.pointSize()-1 );
            painter->setFont(font);
        }
    }

    // Force wrapping of songs that have really wide lines:
    // (Do not allow font to be shrinked less than a 4/5 of the desired font)
    if( font.pointSize() < (orig_font_size*4/5) )
    {
        font.setPointSize(orig_font_size);
        painter->setFont(font);
        flags = (flags | Qt::TextWordWrap);
        //qDebug() << "DRAWING WITH WRAP";
        exit = false;
        while( !exit )
        {
            rect = painter->boundingRect(left, top, w, h, flags, text);
            exit = ( rect.width() <= w && rect.height() <= h );
            if( !exit )
            {
                font.setPointSize( font.pointSize()-1 );
                //qDebug() << "SETTING SIZE:" << font.pointSize();
                painter->setFont(font);
            }
        }
    }
    painter->drawText(rect, flags, text);
    return font.pointSize();

}


void Display1::renderText(bool text_present)
{
    if(use_fading)
    {
        acounter[0]=0;
        sharp0=QPixmap::fromImage (sharp1);
    }

    // Render the text
    QImage image;
    image=QImage::QImage (width(),height(),QImage::Format_ARGB32);//_Premultiplied);
    sharp1=QImage::QImage (width(),height(),QImage::Format_ARGB32);//_Premultiplied);
    QPainter text_painter(&image);
    // text_painter.setRenderHint(QPainter::TextAntialiasing);
    //text_painter.setRenderHint( QPainter::Antialiasing);
    QPainter blur_painter(&sharp1);

    setFont(main_font);


    if( text_present )
    {
        // Draw the active wallpaper if there is text to display
        //qDebug() << "text present, active path:" << wallpaper_path;

        if (wallpaper.width()!=width() || wallpaper.isNull())
        {
            wallpaper.load(wallpaper_path);
            if( !wallpaper.isNull() )
                wallpaper = wallpaper.scaled(width(),height());
        }
        if( ! wallpaper.isNull() )
            blur_painter.drawImage(0,0,wallpaper);
    }
    else
    {
        // Draw the passive wallpaper if set:
        //qDebug() << "no text present, passive path:" << passive_wallpaper_path;

        if (passive_wallpaper.width()!=width() || passive_wallpaper.isNull())
        {
            passive_wallpaper.load(passive_wallpaper_path);
            if( !passive_wallpaper.isNull() )
                passive_wallpaper = passive_wallpaper.scaled(width(),height());
        }
        if( ! passive_wallpaper.isNull() )
            blur_painter.drawImage(0,0, passive_wallpaper);
    }

    text_painter.setPen(QColor(TEXT_COLOR));
    text_painter.setFont(font());

    // Request SoftProjector to write its text to the QPainter:
    emit requestTextDrawing(&text_painter, width(), height());

    m_blurred=image;

    if(use_blur)
        fastbluralpha(m_blurred,BLUR_RADIUS);

    blur_painter.drawImage(BORDER,BORDER,m_blurred);
    image.invertPixels();
    blur_painter.drawImage(0,0,image);

    if(use_fading)
        timer->start(33); // 1/24 sec = ~42miliseconds
    else
        update();
}

bool Display1::useBlur()
{
    return use_blur;
}

void Display1::setBlur(bool blur)
{
    use_blur = blur;
}

bool Display1::useFading()
{
    return use_fading;
}

void Display1::setFading(bool fade)
{
    use_fading = fade;
}

QFont Display1::getFont()
{
    return main_font;
}

void Display1::setNewFont(QFont new_font)
{
    main_font = new_font;
}

QString Display1::getWallpaper()
{
    return wallpaper_path;
}

QString Display1::getPassiveWallpaper()
{
    return passive_wallpaper_path;
}

void Display1::setNewWallpaper(QString path)
{
    wallpaper_path = path;
    if( path.isEmpty() ) {
        QImage null_wallpaper;
        wallpaper = null_wallpaper;
    }
    else
    {
        wallpaper.load(wallpaper_path);
        wallpaper = wallpaper.scaled(width(),height());
    }
}

void Display1::setNewPassiveWallpaper(QString path)
{
    passive_wallpaper_path = path;
    if( path.isEmpty() ) {
        QImage null_wallpaper;
        passive_wallpaper = null_wallpaper;
    }
    else
    {
        passive_wallpaper.load(passive_wallpaper_path);
        passive_wallpaper = passive_wallpaper.scaled(width(), height());
    }
}


void Display1::paintEvent(QPaintEvent *event )
{
    QPainter painter(this);
    painter.drawPixmap(0,0,sharp0);
    alphaImage(sharp1,acounter[0]);
    painter.drawImage(0,0,sharp1);
}

void Display1::alphaImage(QImage &img, int alpha1)
{
    QRgb *pix = (QRgb*)img.bits();
    int w   = img.width();
    int h   = img.height();
    int wh = w*h;
    QRgb p;
    for (int i = 0; i < wh; ++i)
    {
        p = pix[i];
        pix[i] = qRgba(qRed(p), qGreen(p),qBlue(p),alpha1);///dv[asum]);
    }

}

// Stack Blur Algorithm by Mario Klingemann <mario@quasimondo.com>
void Display1::fastbluralpha(QImage &img, int radius)
{
    if (radius < 1) {
        return;
    }

    QRgb *pix = (QRgb*)img.bits();
    int w   = img.width();
    int h   = img.height();
    int wm  = w-1;
    int hm  = h-1;
    int wh  = w*h;
    int div = radius+radius+1;
    double junk;

    int *r = new int[wh];
    int *g = new int[wh];
    int *b = new int[wh];
    int *a = new int[wh];
    int rsum, gsum, bsum, asum, x, y, i, yp, yi, yw;
    QRgb p;
    int *vmin = new int[qMax(w,h)];

    int divsum = (div+1)>>1;
    divsum *= divsum;
    int *dv = new int[256*divsum];
    for (i=0; i < 256*divsum; ++i) {
        dv[i] = (i/divsum);
    }

    yw = yi = 0;

    int **stack = new int*[div];
    for (int i = 0; i < div; ++i) {
        stack[i] = new int[4];
    }


    int stackpointer;
    int stackstart;
    int *sir;
    int rbs;
    int r1 = radius+1;
    int routsum, goutsum, boutsum, aoutsum;
    int rinsum, ginsum, binsum, ainsum;

    for (y = 0; y < h; ++y){
        rinsum = ginsum = binsum = ainsum
                                   = routsum = goutsum = boutsum = aoutsum
                                                                   = rsum = gsum = bsum = asum = 0;
        for (i =- radius; i <= radius; ++i) {
            p = pix[yi+qMin(wm,qMax(i,0))];
            sir = stack[i+radius];
            sir[0] = qRed(p);
            sir[1] = qGreen(p);
            sir[2] = qBlue(p);
            sir[3] = qAlpha(p);

            rbs = r1-abs(i);
            rsum += sir[0]*rbs;
            gsum += sir[1]*rbs;
            bsum += sir[2]*rbs;
            asum += sir[3]*rbs;

            if (i > 0){
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
                ainsum += sir[3];
            } else {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
                aoutsum += sir[3];
            }
        }
        stackpointer = radius;

        for (x=0; x < w; ++x) {

            r[yi] = dv[rsum];
            g[yi] = dv[gsum];
            b[yi] = dv[bsum];
            a[yi] = dv[asum];

            rsum -= routsum;
            gsum -= goutsum;
            bsum -= boutsum;
            asum -= aoutsum;

            stackstart = stackpointer-radius+div;
            sir = stack[stackstart%div];

            routsum -= sir[0];
            goutsum -= sir[1];
            boutsum -= sir[2];
            aoutsum -= sir[3];

            if (y == 0) {
                vmin[x] = qMin(x+radius+1,wm);
            }
            p = pix[yw+vmin[x]];

            sir[0] = qRed(p);
            sir[1] = qGreen(p);
            sir[2] = qBlue(p);
            sir[3] = qAlpha(p);

            rinsum += sir[0];
            ginsum += sir[1];
            binsum += sir[2];
            ainsum += sir[3];

            rsum += rinsum;
            gsum += ginsum;
            bsum += binsum;
            asum += ainsum;

            stackpointer = (stackpointer+1)%div;
            sir = stack[(stackpointer)%div];

            routsum += sir[0];
            goutsum += sir[1];
            boutsum += sir[2];
            aoutsum += sir[3];

            rinsum -= sir[0];
            ginsum -= sir[1];
            binsum -= sir[2];
            ainsum -= sir[3];

            ++yi;
        }
        yw += w;
    }
    for (x=0; x < w; ++x){
        rinsum = ginsum = binsum = ainsum
                                   = routsum = goutsum = boutsum = aoutsum
                                                                   = rsum = gsum = bsum = asum = 0;

        yp =- radius * w;

        for (i=-radius; i <= radius; ++i) {
            yi=qMax(0,yp)+x;

            sir = stack[i+radius];

            sir[0] = r[yi];
            sir[1] = g[yi];
            sir[2] = b[yi];
            sir[3] = a[yi];

            rbs = r1-abs(i);

            rsum += r[yi]*rbs;
            gsum += g[yi]*rbs;
            bsum += b[yi]*rbs;
            asum += a[yi]*rbs;

            if (i > 0) {
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
                ainsum += sir[3];
            } else {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
                aoutsum += sir[3];
            }

            if (i < hm){
                yp += w;
            }
        }

        yi = x;
        stackpointer = radius;

        for (y=0; y < h; ++y){
            junk=dv[asum];
            junk=junk*2.4;
            if (junk>255)junk=255;
            pix[yi] = qRgba(dv[rsum], dv[gsum], dv[bsum], int(junk));///dv[asum]);

            rsum -= routsum;
            gsum -= goutsum;
            bsum -= boutsum;
            asum -= aoutsum;

            stackstart = stackpointer-radius+div;
            sir = stack[stackstart%div];

            routsum -= sir[0];
            goutsum -= sir[1];
            boutsum -= sir[2];
            aoutsum -= sir[3];

            if (x==0){
                vmin[y] = qMin(y+r1,hm)*w;
            }
            p = x+vmin[y];

            sir[0] = r[p];
            sir[1] = g[p];
            sir[2] = b[p];
            sir[3] = a[p];

            rinsum += sir[0];
            ginsum += sir[1];
            binsum += sir[2];
            ainsum += sir[3];

            rsum += rinsum;
            gsum += ginsum;
            bsum += binsum;
            asum += ainsum;

            stackpointer = (stackpointer+1)%div;
            sir = stack[stackpointer];

            routsum += sir[0];
            goutsum += sir[1];
            boutsum += sir[2];
            aoutsum += sir[3];

            rinsum -= sir[0];
            ginsum -= sir[1];
            binsum -= sir[2];
            ainsum -= sir[3];

            yi += w;
        }
    }
    delete [] r;
    delete [] g;
    delete [] b;
    delete [] a;
    delete [] vmin;
    delete [] dv;
}





