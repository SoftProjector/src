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
        QFile cfgFile("font.cfg");
    cfgFile.open(QIODevice::Text | QIODevice::ReadOnly);
    MainFont.fromString(cfgFile.readLine());
    cfgFile.close();
//    qDebug() << MainFont.toString();

    QFile cfgFile2("wallpaper.cfg");
    cfgFile2.open(QIODevice::Text | QIODevice::ReadOnly);
    wallpaperPath=cfgFile2.readLine();
    cfgFile2.close();

    codec = QTextCodec::codecForName("UTF8");


        //MainFont=font();
//    MainFont.setWeight(99);
   // MainFont.setFamily("Charcoal CY");
    setPalette(QPalette(QColor("BLACK"),QColor("BLACK")));
    root_path="./";
#ifdef Q_WS_MAC
    CFURLRef pluginRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    CFStringRef macPath = CFURLCopyFileSystemPath(pluginRef,
                          kCFURLPOSIXPathStyle);
    const char *pathPtr = CFStringGetCStringPtr(macPath,
                          CFStringGetSystemEncoding());
    CFRelease(pluginRef);
    CFRelease(macPath);

    root_path=pathPtr;
#endif
    //wallpaper.load(root_path.append("/background"));
    //wallpaper = wallpaper.scaled(width(),height());
    timer = new QTimer(this);



    connect(timer, SIGNAL(timeout()), this, SLOT(CrossFade()));
//    acounter[0]=255;
//    acounter[0]=0;
    //acounter[1]=255;
    //FaderPixmap=QPixmap::QPixmap(width(),height());

    RenderText();
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

void Display1::SetMainText(QString text)
{

    //if(!(4&&windowState()))showFullScreen();
    int FontSize=45,text_width=0;
//    FontSize = MainFont.pointSize();
    int width_of_screen=width();
    max_width=0;
    QFontMetrics fm(MainFont);
    QStringList list1 = text.split("\n");
    QString textemp = list1[0];
    textemp.remove(6,10);
    if (textemp.startsWith(codec->toUnicode("Припев")) || textemp.startsWith(codec->toUnicode("Куплет")))list1[0] ="";
    if (list1[0]=="")
        list1.removeFirst();
    int listlen = list1.size()-1;
    if (list1[listlen]=="* * *"){
        hasCaption=1;
        CaptionText="*\t*\t*";
        list1[listlen]="";
    }
    if (list1[listlen]=="")
        list1.removeLast(); // removes last line if nothing is there
    if ( text.length() < 3 )
    {
        DisplayList.clear();
        list1.clear();
        update();
        return;
    }
    int width_of_space;
    do
    {
        MainFont.setPointSize(FontSize);
        setFont(MainFont);
        fm=QFontMetrics(font());
        //qDebug() << MainFont;
        width_of_space=fm.width(" ",-1);
        MainText.clear();
        DisplayList.clear();
        QString tempText;

        for (int j=0;j<list1.size();++j){
            QString text2="";
            text2=list1.at(j);
            QStringList list2=text2.split(" ");
            //text2.chop(2);

            if (fm.width(text2,-1)>(width_of_screen-width_of_space-MARGIN))
            {
                MainText.clear();
                for (int i = 0; i < list2.size(); ++i)
                {
                    //if (list2[i]==" ") ++i;
                    //if (list2[i]=="") ++i;
                    if (fm.width(tempText+" "+list2[i],-1)<(width_of_screen-width_of_space-MARGIN))                    {
                        MainText += list2[i]+" ";
                        tempText=MainText;

                    } else {
                        if (MainText==""); // Next 3 statements check that empty lines do not print on the screen
                        else if (MainText==" ");
                        else if (MainText=="  ");
                        else DisplayList << MainText;
                        text_width = fm.width(MainText,-1);
                        if (text_width>max_width)
                            max_width=text_width+width_of_space+width_of_space;
                        tempText=list2[i] + " ";
                        MainText=tempText;

                    }

                }
                if (MainText=="");// Next 3 statements check that empty lines do not print on the screen
                else if (MainText==" ");
                else if (MainText=="  ");
                else DisplayList<<MainText;


            } else{  DisplayList<<text2;}

        }

        text_width=fm.width(MainText,-1);
        if (text_width>max_width)
            max_width=text_width;


        //DisplayList<<MainText;
        //DisplayList<<text;
        FontSize-=4;
    }
    while ((DisplayList.size() * fm.height())>(height()-2*MARGIN-(fm.height()*hasCaption))) ;
    //CaptionText.clear();
    max_width=max_width-width_of_space;
    //update();

}

void Display1::quit_kill()
{
    // qDebug("hello");
    exit(0);
}

void Display1::SetCaptionText(QString text)
{
    CaptionText=text;
    //update();

}

void Display1::SetAllText(QString text,QString caption)
{
    hasCaption=1;
    if (caption.size()<3)hasCaption=0;
    SetCaptionText(caption);
    SetMainText(text);
    //SetCaptionText(caption);
//	RenderText();
    //update();
    acounter[0]=0;
    //acounter[1]=255;
//    sharp0=QPixmap::fromImage (sharp1);
    //timer->stop();
    RenderText();
    timer->start(0);
}


void Display1::RenderText()
{
    QImage temp1;
    temp1=QImage::QImage (width(),height(),QImage::Format_ARGB32);//_Premultiplied);
    sharp1=QImage::QImage (width(),height(),QImage::Format_ARGB32);//_Premultiplied);
    QPainter painter2(&temp1);
    // painter2.setRenderHint(QPainter::TextAntialiasing);
    //painter2.setRenderHint( QPainter::Antialiasing);
    QPainter painter3(&sharp1);
    QString path;
    if (!DisplayList.isEmpty()){

        if (wallpaper.width()!=width() || wallpaper.isNull())
        {//path.append(root_path);
           // path.append("png/background.png");//+QString::number(rand()%33)+".png");
           wallpaper.load(wallpaperPath);
           wallpaper=wallpaper.scaled(width(),height());
           //wallpaper = QImage((wallpaperPath)).scaled(width(),height());
//            wallpaper2 = QImage((path)).scaled(width(),height());
        }
        painter3.drawImage(0,0,wallpaper);

    }/*else{

                path.append(root_path);
                path.append("png/background.png");//+QString::number(rand()%33)+".png");
                wallpaper = QImage((path)).scaled(width(),height());
                //fastbluralpha(wallpaper,2);
            }//
            if(wallpaper.width()!=width())
            {path.append(root_path);
            path.append("png/background.png");//+QString::number(rand()%33)+".png");
            wallpaper = QImage((path)).scaled(width(),height());
            painter3.drawImage(0,0,wallpaper);
        }*/
    QFontMetrics fm(font());
    int start_y;//=MARGIN;//(height()-(fm.height()*(DisplayList.size()+hasCaption)))/2;
    painter2.setPen(QColor(TEXT_COLOR));
    start_y=(height()-(fm.height()*(DisplayList.size()+1+hasCaption)))/2;
    int start_x=MARGIN;
    painter2.setFont(MainFont);
    for (int i = 0; i < DisplayList.size(); ++i)
    {
        painter2.drawText(fm.width(" ",-1)+start_x, start_y+(i+1)*fm.height(), DisplayList.at(i));
    }
    if (!CaptionText.isEmpty())
    {
        MainFont.setPointSize(20);
        painter2.setFont(MainFont);
        fm = QFontMetrics (MainFont);
        start_x=(width()-fm.width(CaptionText))/2;
        start_y=height()-20;//fm.height();
        painter2.drawText(start_x,start_y,CaptionText);
    }
    m_blurred=temp1;
//    fastbluralpha(m_blurred,BLUR_RADIUS);
    painter3.drawImage(BORDER,BORDER,m_blurred);
    temp1.invertPixels();
    painter3.drawImage(0,0,temp1);

}


void Display1::NewFont(QFont newFont)
{
    MainFont=newFont;
}

void Display1::NewWallpaper(QString path)
{
    wallpaperPath=path;
        wallpaper.load(wallpaperPath);
    wallpaper = wallpaper.scaled(width(),height());
}

void Display1::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,sharp0);
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





