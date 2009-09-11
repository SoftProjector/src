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

    codec = QTextCodec::codecForName("UTF8");


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



    connect(timer, SIGNAL(timeout()), this, SLOT(CrossFade()));
//    acounter[0]=255;
//    acounter[0]=0;
    //acounter[1]=255;
    //FaderPixmap=QPixmap::QPixmap(width(),height());


    //timer->start(1000/15);
    //setUpdatesEnabled(0);

}



void Display1::CrossFade()
{
//    acounter[0]+=50;
//    if (acounter[0]>255)acounter[0]=255;
    //acounter[1]--;
    //qDebug("a");
//    if (acounter[0]>254){timer->stop();}
    //setUpdatesEnabled(1);
    //setUpdatesEnabled(0);
    timer->stop();
    update();

}



void Display1::setAllText(QString text, QString caption)
{

    if( text.isEmpty() )
    {
        DisplayList.clear();
        update();
        return;
    }


    CaptionText = caption;

    QStringList lines_list = text.split("\n");

    // Remove the first line if it starts with "Kuplet" or "Pripev":
    QString textemp = lines_list[0];
    textemp.remove(6,10);
    if( textemp.startsWith(codec->toUnicode("Припев")) || textemp.startsWith(codec->toUnicode("Куплет")) )
        lines_list.removeFirst();


    // The displayed text should take less space than this:
    int allowed_width;
    int allowed_height = height() - 2*MARGIN;

    // Set the initial font size:
    int font_size = main_font.pointSize();


    bool exit = false;
    do
    {
        // Go through this loop until the font size is small enough to fit the screen
        // Descrease the font size in each iteration.
        QFont current_font = main_font;
        current_font.setPointSize(font_size);
        setFont(current_font);
        QFontMetrics fm = QFontMetrics(current_font);

        // How many pixels we can use at this font size and sreen width:
        allowed_width = width() - MARGIN - fm.width(" ",-1);

        DisplayList.clear();

        for (int j=0;j<lines_list.size();++j)
        {
            QString curr_line = lines_list.at(j);
            QStringList words_list = curr_line.split(" ");

            if (fm.width(curr_line,-1)>allowed_width)
            {
                // If this line is too wide for the screen at this font size;
                // then try to split it into 2 or more lines:
                QString tempText;
                QString part_of_line;
                for (int i = 0; i < words_list.size(); ++i)
                {
                    if (fm.width(tempText+" "+words_list[i],-1) < allowed_width)
                    {
                        part_of_line += words_list[i]+" ";
                        tempText = part_of_line;
                    }
                    else
                    {
                        DisplayList << part_of_line;
                        tempText = words_list[i] + " ";
                        part_of_line = tempText;
                    }
                }
                DisplayList << part_of_line;
            }
            else
                DisplayList << curr_line;

        }

        font_size -= 4;
        //qDebug() << "FM HEIGHT:" << fm.height();
        int text_height = DisplayList.size() * fm.height();
        if( !CaptionText.isEmpty() )
            text_height += fm.height();
        exit = (text_height <= allowed_height);
    }
    while( !exit );


    //acounter[0]=0;
    //acounter[1]=255;
    //timer->stop();
    renderText();
    timer->start(0);
}


void Display1::renderText()
{
    // Render the text
    QImage image;
    image=QImage::QImage (width(),height(),QImage::Format_ARGB32);//_Premultiplied);
    sharp1=QImage::QImage (width(),height(),QImage::Format_ARGB32);//_Premultiplied);
    QPainter text_painter(&image);
    // text_painter.setRenderHint(QPainter::TextAntialiasing);
    //text_painter.setRenderHint( QPainter::Antialiasing);
    QPainter blur_painter(&sharp1);

    if( !show_black || !DisplayList.isEmpty() )
    {
        // Draw the background picture if there is text to display
        // If show_black is False, always draw the wallpaper

        if (wallpaper.width()!=width() || wallpaper.isNull())
        {
            //qDebug("Loading wallpaper from:");
            //qDebug(qPrintable(wallpaper_path));
            wallpaper.load(wallpaper_path);
            wallpaper = wallpaper.scaled(width(),height());
        }
        blur_painter.drawImage(0,0,wallpaper);
    }

    QFontMetrics fm(font());
    //int start_y;
    text_painter.setPen(QColor(TEXT_COLOR));

    //if( CaptionText.isEmpty() )
    //    start_y=(height()-(fm.height()*(DisplayList.size()+1)))/2;
    //else
    //    start_y=(height()-(fm.height()*(DisplayList.size()+2)))/2;
    //int start_y = (height()-(fm.height()*(DisplayList.size()+1)))/2;

    int left = fm.width(" ",-1) + MARGIN - 5;
    int top = MARGIN; //start_y+(i+1)*fm.height();
    int right = width() - left;
    int bottom = height() - top;
    if( !CaptionText.isEmpty() )
        bottom -= fm.height();

    text_painter.setFont(font());
    QString all_text;

    for (int i = 0; i < DisplayList.size(); ++i)
    {
        if( i > 0 )
            all_text += "\n";
        all_text += DisplayList.at(i);
    }
    // FIXME Try Qt::TextWordWrap
    text_painter.drawText(left, top, right-left, bottom-top, Qt::AlignHCenter | Qt::AlignVCenter, all_text);

    if (!CaptionText.isEmpty())
    {

        QFont caption_font = font();
        caption_font.setPointSize(20);
        text_painter.setFont(caption_font);
        fm = QFontMetrics(caption_font);
        int start_x=(width()-fm.width(CaptionText))/2;
        int start_y=height()-20;//fm.height();
        text_painter.drawText(start_x,start_y,CaptionText);
    }
    m_blurred=image;
//    fastbluralpha(m_blurred,BLUR_RADIUS);
    blur_painter.drawImage(BORDER,BORDER,m_blurred);
    image.invertPixels();
    blur_painter.drawImage(0,0,image);

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

bool Display1::getShowBlack()
{
    return show_black;
}

void Display1::setShowBlack(bool new_show_black)
{
    // Whether to show black instead of the wallpaper when not
    // displaying anything to the screen
    show_black = new_show_black;
}



void Display1::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    /*    if (painter.paintEngine()->hasFeature(QPaintEngine::ConstantOpacity))
    {
        double alpha;
        alpha=acounter[0]/255;
        painter.setOpacity(alpha);
        qDebug("opacity\a");

    }else
    { */
//            alphaImage(sharp1,acounter[0]);
        //}
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
        pix[i] = qRgba(qRed(p), qGreen(p),qBlue(p),255);///dv[asum]);
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





