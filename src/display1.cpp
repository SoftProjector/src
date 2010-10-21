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

#define BLUR_RADIUS 5



/*
shadow_effect.setBlurRadius(15.0);

r = self.boundingRect()
sz = QSize(r.width(), r.height())
tmp = QImage(sz, QImage.Format_ARGB32_Premultiplied)
self.tmp = tmp
new_painter = QPainter(tmp)
QGraphicsDropShadowEffect.drawSource(self, new_painter)

pixmap, offset = self.sourcePixmap()
painter.setCompositionMode(QPainter.CompositionMode_SourceOver)
painter.drawPixmap(0,0, QPixmap.fromImage(tmp))
new_painter.end()
del self.tmp




class ShadowAndAlphaEffect(QGraphicsDropShadowEffect):
    def __init__(self, blur, transparency, parent=None):
        QGraphicsEffect.__init__(self, parent)
        self.opacity = QGraphicsOpacityEffect(self)
        self.setBlurRadius(15.0)
        self.opacity.setOpacity(0.9)

    def draw(self, painter):
        r = self.boundingRect()
        sz = QSize(r.width(), r.height())
        tmp = QImage(sz, QImage.Format_ARGB32_Premultiplied)
        self.tmp = tmp
        new_painter = QPainter(tmp)
        QGraphicsDropShadowEffect.drawSource(self, new_painter)

        painter.setOpacity(self.opacity.opacity())
        pixmap, offset = self.sourcePixmap()
        painter.setCompositionMode(QPainter.CompositionMode_SourceOver)
        painter.drawPixmap(0,0, QPixmap.fromImage(tmp))
        new_painter.end()
        del self.tmp


*/

Display1::Display1(QWidget *parent)
        : QWidget(parent)
{
    setPalette(QPalette(QColor("BLACK"),QColor("BLACK")));

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
        // Save the previous image for fade-out effect:
        previous_image_pixmap = QPixmap::fromImage(output_image);
    }


    // For later determening which background to draw, and whether to transition to it:
    background_needs_transition = ( use_active_background != text_present );
    use_active_background = text_present;

    setFont(main_font);


    // Render the foreground text:
    QImage text_image;
    text_image = QImage::QImage(width(), height(), QImage::Format_ARGB32);//_Premultiplied);
    // Fill transparent background instead of initial garbage (fixes issues on MacOSX):
    text_image.fill(qRgba(0, 0, 0, 0)); //transparent background

    QPainter text_painter(&text_image);
    //text_painter.setRenderHint(QPainter::TextAntialiasing);
    //text_painter.setRenderHint( QPainter::Antialiasing);

    text_painter.setPen(foreground_color);
    text_painter.setFont(font());

    // Request SoftProjector to write its text to the QPainter:
    emit requestTextDrawing(&text_painter, width(), height());
    text_painter.end();


    // Draw the shadow image:
    QImage shadow_image;
    shadow_image = QImage::QImage(width(), height(), QImage::Format_ARGB32);//_Premultiplied);
    shadow_image.fill(qRgba(0, 0, 0, 0)); //transparent background
    QPainter shadow_painter(&shadow_image);
    shadow_painter.setPen(QColor(Qt::black));
    shadow_painter.setFont(font());
    emit requestTextDrawing(&shadow_painter, width(), height());
    shadow_painter.end();

    // Set the blured image to the produced text image:
    if(use_blur)
    {
        /*
        Produces a text with a blurred drop shadow using QGraphicsView.
        This experiment failed, though, as the quality was a little unsatisfactory.

        QPixmap shadow_pixmap = QPixmap::fromImage(text_image);

        QGraphicsPixmapItem *shadow_pixmap_item;
        shadow_pixmap_item = new QGraphicsPixmapItem(shadow_pixmap);

        QGraphicsDropShadowEffect *shadow_effect;
        shadow_effect = new QGraphicsDropShadowEffect();
        shadow_effect->setColor(QColor(20, 20, 20, 240));//qRgba(0, 0, 0, 90));//Qt::black);
        shadow_effect->setOffset(4.0);
        shadow_effect->setBlurRadius(10.0);
        shadow_pixmap_item->setGraphicsEffect( shadow_effect );

        QGraphicsScene scene;
        // The scene will take owndership of the item:
        scene.addItem(shadow_pixmap_item);

        QGraphicsView view(&scene);
        view.show();
        view.render(&output_painter);
        */

        // Blur the shadow:
        fastbluralpha(shadow_image, BLUR_RADIUS);
    }


    output_image = QImage::QImage(width(), height(), QImage::Format_ARGB32);//_Premultiplied);
    output_image.fill(qRgba(0, 0, 0, 0)); //transparent background
    // Painter for drawing the final image:
    QPainter output_painter(&output_image);
    //output_painter.setRenderHint(QPainter::TextAntialiasing);
    //output_painter.setRenderHint( QPainter::Antialiasing);

    // Offset the shadow by a fraction of the font size:
    int shadow_offset = ( font().pointSize() / 15 );

    // Draw the shadow:
    output_painter.drawImage(shadow_offset, shadow_offset, shadow_image);

    // Draw the text:
    output_painter.drawImage(0, 0, text_image);

    output_painter.end();

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
        wallpaper = QImage();
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
        passive_wallpaper = QImage();
    }
    else
    {
        passive_wallpaper.load(passive_wallpaper_path);
        passive_wallpaper = passive_wallpaper.scaled(width(), height());
    }
}

QColor Display1::getForegroundColor()
{
    return foreground_color;
}
void Display1::setForegroundColor(QColor new_color)
{
    foreground_color = new_color;
}

void Display1::paintEvent(QPaintEvent *event )
{
    QPainter painter(this);

    // This code will, with each iteraction, draw the previous image with increasing transparency, and draw
    // the current image with increasing opacity; making a smooth transition:
    double curr_opacity = acounter[0] / 255.0;
    double prev_opacity = 1.0 - curr_opacity;

    // FIXME transition out of the previous background as well
    // Draw the background at the current opacity:
    if(background_needs_transition)
        painter.setOpacity(curr_opacity);

    if( use_active_background )
    {
        // Draw the active wallpaper if there is text to display
        if (wallpaper.width()!=width() || wallpaper.isNull())
        {
            wallpaper.load(wallpaper_path);
            if( !wallpaper.isNull() )
                wallpaper = wallpaper.scaled(width(),height());
        }
        if( ! wallpaper.isNull() )
            painter.drawImage(0,0,wallpaper);
        else
        {
            // Use black for the background:
            painter.setPen(QColor(Qt::black));
            painter.drawRect( 0, 0, width(), height() );
        }
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
            painter.drawImage(0,0, passive_wallpaper);
        else
        {
            // Use black for the background:
            painter.setPen(QColor(Qt::black));
            painter.drawRect( 0, 0, width(), height() );
        }
    }


    // Draw the previous image into the window, at decreasing opacity:
    painter.setOpacity(prev_opacity);
    painter.drawPixmap(0, 0, previous_image_pixmap);

    // Draw the output_image into the window, at increasing opacity:
    painter.setOpacity(curr_opacity);
    painter.drawImage(0, 0, output_image);

    // Reset the opacity to default opaque:
    painter.setOpacity(1.0);
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





