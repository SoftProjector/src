/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyleft (Ɔ) 2013  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

#include "displayscreen.h"
#include "ui_displayscreen.h"

#define BLUR_RADIUS 5

DisplayScreen::DisplayScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplayScreen)
{
    ui->setupUi(this);
    setPalette(QPalette(QColor(Qt::black),QColor(Qt::black)));

    timer = new QTimer(this);
    timer_out = new QTimer(this);

    acounter[0]=255;

    // add video player
    videoPlayer = new Phonon::MediaObject;
    videoWidget = new Phonon::VideoWidget(this);
    videoWidget->setVisible(false);
    Phonon::createPath(videoPlayer,videoWidget);

    connect(videoPlayer, SIGNAL(tick(qint64)),this,SLOT(updateTimeText()));
    connect(videoPlayer, SIGNAL(totalTimeChanged(qint64)),this,SLOT(updateTimeText()));
    connect(videoPlayer, SIGNAL(stateChanged(Phonon::State,Phonon::State)),this,SLOT(playerStateChanged(Phonon::State,Phonon::State)));

    // add text render lable
    textRenderLabel = new QLabel(this);

    // Add control buttons
    btnNext = new ControlButton(QIcon(":/icons/icons/controlNext.png"),
                                QIcon(":/icons/icons/controlNextHovered.png"),
                                QIcon(":/icons/icons/controlNextPressed.png"),this);
    btnPrev = new ControlButton(QIcon(":/icons/icons/controlPrev.png"),
                                QIcon(":/icons/icons/controlPrevHovered.png"),
                                QIcon(":/icons/icons/controlPrevPressed.png"),this);
    btnExit = new ControlButton(QIcon(":/icons/icons/controlExit.png"),
                                QIcon(":/icons/icons/controlExitHovered.png"),
                                QIcon(":/icons/icons/controlExitPressed.png"),this);

    connect(timer, SIGNAL(timeout()), this, SLOT(fadeIn()));
    connect(btnNext,SIGNAL(clicked()),this,SLOT(btnNextClicked()));
    connect(btnPrev,SIGNAL(clicked()),this,SLOT(btnPrevClicked()));
    connect(btnExit,SIGNAL(clicked()),this,SLOT(btnExitClicked()));


}

DisplayScreen::~DisplayScreen()
{
    delete timer;
    delete timer_out;
    delete videoPlayer;
    delete videoWidget;
    delete textRenderLabel;
    delete btnPrev;
    delete btnNext;
    delete btnExit;
    delete ui;
}
void DisplayScreen::keyReleaseEvent(QKeyEvent *event)
{
    // Will get called when a key is released
    int key = event->key();
    if(key == Qt::Key_Left)
        prevSlide();
    else if(key == Qt::Key_Up)
        prevSlide();
    else if(key == Qt::Key_PageUp)
        prevSlide();
    else if(key == Qt::Key_Back)
        prevSlide();
    else if(key == Qt::Key_Right)
        nextSlide();
    else if(key == Qt::Key_Down)
        nextSlide();
    else if(key == Qt::Key_PageDown)
        nextSlide();
    else if(key == Qt::Key_Forward)
        nextSlide();
    else if(key == Qt::Key_Enter)
        nextSlide();
    else if(key == Qt::Key_Return)
        nextSlide();
    else if(key == Qt::Key_Escape)
        exitSlide();
    else
        QWidget::keyReleaseEvent(event);
}

void DisplayScreen::positionOpjects()
{
    videoWidget->setGeometry(0,0,width(),height());
    textRenderLabel->setGeometry(0,0,width(),height());
//    videoWidget->setGeometry(200,200,width()/2,height()/2);
//    textRenderLabel->setGeometry(0,0,width()/2,height()/2);
}

void DisplayScreen::setControlsSettings(DisplayControlsSettings &settings)
{
    controlsSettings = settings;
    positionControlButtons();
}

void DisplayScreen::setControlButtonsVisible(bool visible)
{
    btnPrev->setVisible(visible);
    btnNext->setVisible(visible);
    btnExit->setVisible(visible);
}

void DisplayScreen::positionControlButtons()
{
    // set icon sise
    int buttonSize(controlsSettings.buttonSize);
    if(buttonSize == 0)
        buttonSize = 16;
    else if(buttonSize == 1)
        buttonSize = 24;
    else if(buttonSize == 2)
        buttonSize = 32;
    else if(buttonSize == 3)
        buttonSize = 48;
    else if(buttonSize == 4)
        buttonSize = 64;
    else if(buttonSize == 5)
        buttonSize = 96;
    else
        buttonSize = 48;
    btnNext->setIconSize(QSize(buttonSize,buttonSize));
    btnPrev->setIconSize(QSize(buttonSize,buttonSize));
    btnExit->setIconSize(QSize(buttonSize,buttonSize));

    // set buttons size to be 2px greater than the icon size
    buttonSize +=2;

    // calculate button position
    int y(this->height()), x(this->width()), margin(5);

    // calculate y position
    if(controlsSettings.alignmentV==0)//top
        y = margin;
    else if(controlsSettings.alignmentV==1)//middle
        y = (y-buttonSize)/2;
    else if(controlsSettings.alignmentV==2)//buttom
        y = y-buttonSize-margin;
    else
        y = y-buttonSize-margin;

    // calculate x position
    int xt((buttonSize*3)+10); //total width of the button group
    if(controlsSettings.alignmentH==0)
        x = margin;
    else if(controlsSettings.alignmentH==1)
        x = (x-xt)/2;
    else if (controlsSettings.alignmentH==2)
        x = x-xt-margin;
    else
        x = (x-xt)/2;

    int x1(x);
    int x2(x1+buttonSize+5);
    int x3(x2+buttonSize+5);

    //set button positon
    btnPrev->setGeometry(x1,y,buttonSize,buttonSize);
    btnNext->setGeometry(x2,y,buttonSize,buttonSize);
    btnExit->setGeometry(x3,y,buttonSize,buttonSize);

    //set button opacity
    btnPrev->setOpacity(controlsSettings.opacity);
    btnNext->setOpacity(controlsSettings.opacity);
    btnExit->setOpacity(controlsSettings.opacity);

    // repaint buttons
    btnPrev->repaint();
    btnNext->repaint();
    btnExit->repaint();
}

void DisplayScreen::btnNextClicked()
{
    emit nextSlide();
}

void DisplayScreen::btnPrevClicked()
{
    emit prevSlide();
}

void DisplayScreen::btnExitClicked()
{
    emit exitSlide();
}

void DisplayScreen::fadeIn()
{
    if (useFading)
    {
        acounter[0]+=64; // fade step increaments
        if (acounter[0]>255)
            acounter[0]=255;

        if (acounter[0]>254)
            timer->stop();
        update();
    }
}

void DisplayScreen::fadeOut() // For future
{
    //    acounter[0]-=24;
    //    if (acounter[0]<0)acounter[0]=0;
    //    if (acounter[0]<1){timer_out->stop();}
    //    update();
}

void DisplayScreen::renderText(bool text_present)
{
    if(displayType=="video")
    {
        if(!videoWidget->isVisible())
            videoWidget->setVisible(true);
        if(textRenderLabel->isVisible())    // Need to remove when text lable on top of video properly works
            textRenderLabel->setVisible(false);
    }
    else
    {
        if(!textRenderLabel->isVisible())   // Need to remove when text lable on top of video properly works
            textRenderLabel->setVisible(true);
        if(videoWidget->isVisible())
        {
            videoPlayer->stop();
            videoWidget->setVisible(false);
        }
    }
    if(!text_present)
        displayType.clear();

    if(useFading)
    {
        acounter[0]=0;
    }
        // Save the previous image for fade-out effect:
        previous_image_pixmap = QPixmap::fromImage(output_image);


    // For later determening which background to draw, and whether to transition to it:
    background_needs_transition = ( use_active_background != text_present );
    use_active_background = text_present;

    // Render the foreground text:
    QImage text_image(width(), height(), QImage::Format_ARGB32);//_Premultiplied);
    // Fill transparent background instead of initial garbage (fixes issues on MacOSX):
    text_image.fill(qRgba(0, 0, 0, 0)); //transparent background

    QPainter text_painter(&text_image);
    //text_painter.setRenderHint(QPainter::TextAntialiasing);
    //text_painter.setRenderHint(QPainter::Antialiasing);

    text_painter.setPen(foregroundColor);
    text_painter.setFont(mainFont);

    // Request to write its text to the QPainter:
    if(displayType == "bible")
        drawBibleText(&text_painter, width(), height());
    else if(displayType == "song")
        drawSongText(&text_painter, width(), height());
    else if(displayType == "announce")
        drawAnnounceText(&text_painter, width(), height());
    text_painter.end();

    // Draw the shadow image:
    QImage shadow_image(width(), height(), QImage::Format_ARGB32);//_Premultiplied);
    shadow_image.fill(qRgba(0, 0, 0, 0)); //transparent background
    QPainter shadow_painter(&shadow_image);
    shadow_painter.setPen(QColor(Qt::black));
    shadow_painter.setFont(mainFont);
    if(useShadow)
    {
        if(displayType == "bible")
            drawBibleText(&shadow_painter, width(), height());
        else if(displayType == "song")
            drawSongText(&shadow_painter, width(), height());
        else if(displayType == "announce")
            drawAnnounceText(&shadow_painter, width(), height());
        shadow_painter.end();
    }

    // Set the blured image to the produced text image:
    if(useBluredShadow)
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

    QImage temp_image(width(), height(), QImage::Format_ARGB32);//_Premultiplied);
    output_image = temp_image;
    output_image.fill(qRgba(0, 0, 0, 0)); //transparent background
    // Painter for drawing the final image:
    QPainter output_painter(&output_image);
    //output_painter.setRenderHint(QPainter::TextAntialiasing);
    //output_painter.setRenderHint(QPainter::Antialiasing);

    // Offset the shadow by a fraction of the font size:
    int shadow_offset = (mainFont.pointSize() / 15 );

    // Draw the shadow:
    output_painter.drawImage(shadow_offset, shadow_offset, shadow_image);

    // Draw the text:
    output_painter.drawImage(0, 0, text_image);
    output_painter.end();

    if(useFading)
        timer->start(32); // time beween fade steps in milliseconds
    else
        update();
}

void DisplayScreen::renderBibleText(Verse verse, BibleSettings &bibleSetings)
{
    // Render bible verse text
    displayType = "bible";
    bibleVerse = verse;
    bibleSets = bibleSetings;

    useFading = bibleSets.useFading;
    useShadow = bibleSets.useShadow;
    useBluredShadow = bibleSets.useBlurShadow;
    setNewWallpaper(bibleSets.backgroundPath,bibleSets.useBackground);
    mainFont = bibleSets.textFont;
    foregroundColor = bibleSets.textColor;

    renderText(true);
}

void DisplayScreen::renderSongText(Stanza stanza, SongSettings &songSettings)
{
    // Render song stanza text
    displayType = "song";
    songStanza = stanza;
    songSets = songSettings;

    useFading = songSets.useFading;
    useShadow = songSets.useShadow;
    useBluredShadow = songSets.useBlurShadow;
    if(songStanza.usePrivateSettings)
    {
        // Set song specific settings
        setNewWallpaper(songStanza.backgroundPath,(!songStanza.backgroundPath.simplified().isEmpty()));
        mainFont = songStanza.font;
        foregroundColor = songStanza.color;
    }
    else
    {
        setNewWallpaper(songSets.backgroundPath,songSets.useBackground);
        mainFont = songSets.textFont;
        foregroundColor = songSets.textColor;
    }

    renderText(true);
}

void DisplayScreen::renderAnnounceText(Announcement announce, AnnounceSettings &announceSettings)
{
    // Render aanouncement text
    displayType = "announce";
    announcement = announce;
    annouceSets = announceSettings;

    useFading = annouceSets.useFading;
    useShadow = annouceSets.useShadow;
    useBluredShadow = annouceSets.useBlurShadow;
    setNewWallpaper(annouceSets.backgroundPath,annouceSets.useBackground);
    mainFont = annouceSets.textFont;
    foregroundColor = annouceSets.textColor;

    renderText(true);
}

void DisplayScreen::renderPicture(QPixmap image)
{
    displayType = "pix";
    wallpaper = image.scaled(width(),height(),Qt::KeepAspectRatio);
//    wallpaper = image.scaled(width(),height(),Qt::KeepAspectRatioByExpanding);
//    wallpaper = image.scaled(width(),height());
//    wallpaper = image;

    renderText(true);
    useBluredShadow = false;
    useShadow = false;
}

void DisplayScreen::renderVideo(VideoInfo &vid)
{
    displayType = "video";
    renderText(false);
    videoPlayer->stop();
    videoPlayer->setCurrentSource(Phonon::MediaSource(vid.filePath));
    videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatio(vid.aspectRatio));
    videoPlayer->play();
}

void DisplayScreen::updateTimeText()
{
    long len = videoPlayer->totalTime();
    long pos = videoPlayer->currentTime();
    QString timeString;
    if (pos || len)
    {
        int sec = pos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = pos;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = len / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = len;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";
        timeString = playTime.toString(timeFormat);
        if (len)
            timeString += " / " + stopTime.toString(timeFormat);
    }

    emit sendTimeText(timeString);

}

void DisplayScreen::playerStateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    Q_UNUSED(oldstate);
    switch (newstate)
    {
    case Phonon::ErrorState:
        videoPlayer->pause();
        QMessageBox::warning(this,tr("Video Player Error"),videoPlayer->errorString(),QMessageBox::Close);
        break;
    case Phonon::StoppedState:
    case Phonon::PausedState:
        emit updatePlayButton(false);
        break;
    case Phonon::PlayingState:
        emit updatePlayButton(true);
    case Phonon::BufferingState:
        break;
    case Phonon::LoadingState:
        break;
    }
}

void DisplayScreen::drawBibleText(QPainter *painter, int width, int height)
{
    // Margins:
    int left = 30;
    int top = 20;

    //int right = width - left;
    int w = width - left - left;
    int h = height - top - top;

    // set maximum screen size - For primary bibile  only
    int maxh = h * bibleSets.maxScreen/100; // maximun screen height
    int maxtop; // top of max screen
    if(bibleSets.maxScreenFrom=="top")
        maxtop  = top;
    if(bibleSets.maxScreenFrom=="bottom")
        maxtop = top+h-maxh;

    // apply max screen use settings
    h=maxh;
    top=maxtop;

    QFont font = painter->font();
    int original_size = font.pointSize();
    int current_size = original_size;

    // Keep decreasing the font size until the text fits into the allocated space:

    // Rects for storing the position of the text and caption drawing:
    QRect trect1, crect1, trect2, crect2, trect3, crect3;
    // Flags to be used for drawing verse text and caption:
    int tflags = Qt::TextWordWrap;
    tflags = Qt::TextWordWrap;
    if(bibleSets.textAlingmentV==0)
        tflags += Qt::AlignTop;
    else if(bibleSets.textAlingmentV==1)
        tflags += Qt::AlignVCenter;
    else if(bibleSets.textAlingmentV==2)
        tflags += Qt::AlignBottom;
    if(bibleSets.textAlingmentH==0)
        tflags += Qt::AlignLeft;
    else if(bibleSets.textAlingmentH==1)
        tflags += Qt::AlignHCenter;
    else if(bibleSets.textAlingmentH==2)
        tflags += Qt::AlignRight;
    int cflags = Qt::AlignRight | Qt::AlignTop ;

    bool exit = false;
    while( !exit )
    {
        if(bibleVerse.secondary_text.isEmpty() && bibleVerse.trinary_text.isEmpty())
        {
            // Prepare primary version only, 2nd and 3rd do not exist
            // Figure out how much space the drawing will take at the current font size:
            drawBibleTextToRect(painter,trect1,crect1,bibleVerse.primary_text,bibleVerse.primary_caption,tflags,cflags,top,left,w,h, current_size);

            // Make sure that all fits into the screen
            int th = trect1.height()+crect1.height();
            exit = (th<=h);
        }
        else if(!bibleVerse.secondary_text.isEmpty() && bibleVerse.trinary_text.isEmpty())
        {
            // Prepare primary and secondary versions, trinary does not exist
            // Figure out how much space the drawing will take at the current font size for primary:
            drawBibleTextToRect(painter,trect1,crect1,bibleVerse.primary_text,bibleVerse.primary_caption,tflags,cflags,top,left,w,h/2,current_size);

            // set new top for secondary
            int top2 = crect1.bottom();
            if(top2<h/2+top)
                top2=h/2+top;

            // Figure out how much space the drawing will take at the current font size for secondary:
            drawBibleTextToRect(painter,trect2,crect2,bibleVerse.secondary_text,bibleVerse.secondary_caption,tflags,cflags,top2,left,w,h/2,current_size);

            int th1 = trect1.height()+crect1.height();
            int th2 = trect2.height()+crect2.height();

            // Make sure that primary fits
            exit = (th1<=h/2);
            if (exit)
                // If primary fits, make sure secondary fits
                exit = (th2<=h/2);
        }
        else if(!bibleVerse.secondary_text.isEmpty() && !bibleVerse.trinary_text.isEmpty())
        {
            // Prepare primary and secondary and trinary versions
            // Figure out how much space the drawing will take at the current font size for primary:
            drawBibleTextToRect(painter,trect1,crect1,bibleVerse.primary_text,bibleVerse.primary_caption,tflags,cflags,top,left,w,h*1/3,current_size);

            // set new top for secondary
            int top2 = crect1.bottom();
            if(top2<h*1/3+top)
                top2=h*1/3+top;

            // Figure out how much space the drawing will take at the current font size for secondary:
            drawBibleTextToRect(painter,trect2,crect2,bibleVerse.secondary_text,bibleVerse.secondary_caption,tflags,cflags,top2,left,w,h*1/3,current_size);

            // set new top for trinaty
            top2 = crect2.bottom();
            if(top2<h*2/3+top)
                top2 = h*2/3+top;

            // Figure out how much space the drawing will take at the current font size for trinary:
            drawBibleTextToRect(painter,trect3,crect3,bibleVerse.trinary_text,bibleVerse.trinary_caption,tflags,cflags,top2,left,w,h*1/3,current_size);

            int th1 = trect1.height()+crect1.height();
            int th2 = trect2.height()+crect2.height();
            int th3 = trect3.height()+crect3.height();

            // Make sure that primary fits
            exit = (th1<=h*1/3);
            if(exit)
                // If primary fits, make sure secondary fits
                exit = (th2<=h*1/3);
            if(exit)
                // If also secondary fits, make sure trinary fits
                exit = (th3<=h*1/3);
        }

        if( !exit ) // The current font is too large, decrease and try again:
            current_size--;
    }

    // Draw the bible text verse(s) at the final size:
    font.setPointSize(current_size);
    painter->setFont(font);
    painter->drawText(trect1, tflags, bibleVerse.primary_text);
    if(!bibleVerse.secondary_text.isNull())
        painter->drawText(trect2, tflags, bibleVerse.secondary_text);
    if(!bibleVerse.trinary_text.isNull())
        painter->drawText(trect3, tflags, bibleVerse.trinary_text);

    // Draw the verse caption(s) at the final size:
    font.setPointSize(current_size*3/4);
    painter->setFont(font);
    painter->drawText(crect1, cflags, bibleVerse.primary_caption);
    if(!bibleVerse.secondary_text.isNull())
        painter->drawText(crect2, cflags, bibleVerse.secondary_caption);
    if(!bibleVerse.trinary_caption.isNull())
        painter->drawText(crect3, cflags, bibleVerse.trinary_caption);

    // Restore the original font:
    font.setPointSize(original_size);
    painter->setFont(font);

}

void DisplayScreen::drawBibleTextToRect(QPainter *painter, QRect& trect, QRect& crect, QString ttext, QString ctext, int tflags, int cflags, int top, int left, int width, int height, int font_size)
{
    QFont font = painter->font();

    // prepare caption
    font.setPointSize(font_size * 3/4);
    painter->setFont(font);
    crect = painter->boundingRect(left, top, width, height, cflags, ctext);

    // prepare text
    font.setPointSize(font_size);
    painter->setFont(font);
    trect = painter->boundingRect(left, top, width, height-crect.height(), tflags, ttext);

    // reset capion location
    int ch = crect.height();
    crect.setTop(trect.bottom());
    crect.setHeight(ch);
}

void DisplayScreen::drawSongText(QPainter *painter, int width, int height)
{
    // Draw the text of the current song verse to the specified painter; making
    // sure that the output rect is narrower than <width> and shorter than <height>.

    QString main_text = songStanza.stanza;
    QString top_caption_str;
    QString song_ending = " ";

    //QStringList lines_list = song_list.at(current_song_verse).split("\n");
    QString song_num_str = QString::number(songStanza.number);
    QString song_key_str = songStanza.tune;

    // Check whether to display song numbers
    if (songSets.showSongNumber)
        song_num_str = song_num_str;
    else
        song_num_str = " ";

    // Check whether to display song key
    if (songSets.showSongKey)
        song_num_str = song_key_str + "  " + song_num_str;
    else
        song_num_str = song_num_str;

    // Check wheter to display stanza tiles
    if (songSets.showStanzaTitle)
        top_caption_str = songStanza.stanzaTitle;
    else
        top_caption_str = " ";

    // Prepare Song ending string
    if(songStanza.isLast)
    {
        // first check if to show ending
        if(songSets.showSongEnding)
        {
            if(songSets.songEndingType == 0)
                song_ending = "*    *    *";
            else if(songSets.songEndingType == 1)
            {
                // First check if copyrigth info exist. If it does show it.
                // If some exist, then show what exist. If nothing exist, then show '* * *'
                if(!songStanza.wordsBy.isEmpty() && !songStanza.musicBy.isEmpty())
                    song_ending = QString(tr("Words by: %1, Music by: %2")).arg(songStanza.wordsBy).arg(songStanza.musicBy);
                else if(!songStanza.wordsBy.isEmpty() && songStanza.musicBy.isEmpty())
                    song_ending = QString(tr("Words by: %1")).arg(songStanza.wordsBy);
                else if(songStanza.wordsBy.isEmpty() && !songStanza.musicBy.isEmpty())
                    song_ending = QString(tr("Music by: %1")).arg(songStanza.musicBy);
                else if(songStanza.wordsBy.isEmpty() && songStanza.musicBy.isEmpty())
                    song_ending = "*    *    *";
            }
        }
    }

    // Margins:
    int left = 30;
    int top = 20;
    int w = width - left - left;
    int h = height - top - top;
    QRect rect = QRect(left, top, w, h);

    QRect out_rect;

    QFont font = painter->font();

    int orig_font_size = font.pointSize();

    // Keep decreasing the font size until the text fits into the allocated space:
    bool exit = false;
    while( !exit )
    {
        out_rect = drawSongTextToRect(painter, rect, false, false, main_text, top_caption_str, song_num_str, song_ending);
        exit = ( out_rect.width() <= w && out_rect.height() <= h );
        if( !exit )
        {
            // Decrease font size by one point and try again
            font.setPointSize( font.pointSize()-1 );
            painter->setFont(font);
        }
    }

    bool wrap = false;
    // Force wrapping of songs that have really wide lines:
    // (Do not allow font to be shrinked less than a 4/5 of the desired font)
    if( font.pointSize() < (orig_font_size*4/5) )
    {
        font.setPointSize(orig_font_size);
        painter->setFont(font);
        exit = false;
        wrap = true;
        while( !exit )
        {
            out_rect = drawSongTextToRect(painter, rect, false, wrap, main_text, top_caption_str, song_num_str, song_ending);
            exit = ( out_rect.width() <= w && out_rect.height() <= h );
            if( !exit )
            {
                // Decrease font size by one point and try again
                font.setPointSize( font.pointSize()-1 );
                painter->setFont(font);
            }
        }
    }
    // At this point we picked correct font size and flags; so it's safe to draw:
    drawSongTextToRect(painter, rect, true, wrap, main_text, top_caption_str, song_num_str, song_ending);
}

QRect DisplayScreen::drawSongTextToRect(QPainter *painter, QRect bound_rect, bool draw, bool wrap, QString main_text, QString caption_str, QString song_num_str, QString ending_str)
{
    // Figure out how to draw the specified song verse to the specified rect.
    // Return the rectangle of the output - may be bigger than input rect.
    // draw - whether to actually draw or to simply calculate the rect.

    QRect caption_rect, num_rect, main_rect, out_rect, ending_rect;
    int left = bound_rect.left();
    int top = bound_rect.top();
    int width = bound_rect.width();
    int height = bound_rect.height();
    int main_flags(0);

    if(wrap)
        main_flags += Qt::TextWordWrap;

    if(songStanza.usePrivateSettings)
    {
        // Apply song specific alignment
        if(songStanza.alignmentV==0)
            main_flags += Qt::AlignTop;
        else if(songStanza.alignmentV==1)
            main_flags += Qt::AlignVCenter;
        else if(songStanza.alignmentV==2)
            main_flags += Qt::AlignBottom;
        if(songStanza.alignmentH==0)
            main_flags += Qt::AlignLeft;
        else if(songStanza.alignmentH==1)
            main_flags += Qt::AlignHCenter;
        else if(songStanza.alignmentH==2)
            main_flags += Qt::AlignRight;
    }
    else
    {
        if(songSets.textAlingmentV==0)
            main_flags += Qt::AlignTop;
        else if(songSets.textAlingmentV==1)
            main_flags += Qt::AlignVCenter;
        else if(songSets.textAlingmentV==2)
            main_flags += Qt::AlignBottom;
        if(songSets.textAlingmentH==0)
            main_flags += Qt::AlignLeft;
        else if(songSets.textAlingmentH==1)
            main_flags += Qt::AlignHCenter;
        else if(songSets.textAlingmentH==2)
            main_flags += Qt::AlignRight;
    }


    QFont orig_font = painter->font();
    QFont caption_font = orig_font;
    caption_font.setPointSize( orig_font.pointSize() *4/5 );
    QFont ending_font = orig_font;
    ending_font.setPointSize(orig_font.pointSize() *2/3);

    // Paint caption, key and number
    painter->setFont(caption_font);
    caption_rect = boundRectOrDrawText(painter, draw, left, top, width, height, Qt::AlignLeft | Qt::AlignTop, caption_str);
    num_rect = boundRectOrDrawText(painter, draw, left, top, width, height, Qt::AlignRight | Qt::AlignTop, song_num_str);

    // Paint song ending
    painter->setFont(ending_font);
    ending_rect = boundRectOrDrawText(painter, false, left, top, width, height, Qt::AlignBottom | Qt::AlignCenter, ending_str);
    if(ending_rect.width()> width)
    {
        // decrease songe ending font size so that it would fit in the screen
        while (ending_rect.width()> width)
        {
            ending_font.setPointSize(ending_font.pointSize()-1);
            painter->setFont(ending_font);
            ending_rect = boundRectOrDrawText(painter, false, left, top, width, height, Qt::AlignBottom | Qt::AlignCenter, ending_str);
        }
        ending_rect = boundRectOrDrawText(painter, draw, left, top, width, height, Qt::AlignBottom | Qt::AlignCenter, ending_str);
    }
    else
        ending_rect = boundRectOrDrawText(painter, draw, left, top, width, height, Qt::AlignBottom | Qt::AlignCenter, ending_str);

    int cheight = caption_rect.height(); // Height of the caption text
    int eheight = ending_rect.height(); // hieght of song ending text
    painter->setFont(orig_font);
    main_rect = boundRectOrDrawText(painter, draw, left, top+cheight, width, height-cheight-eheight, main_flags, main_text);

    top = top-eheight;
    left = main_rect.left();
    height = main_rect.bottom() - top ;
    width = main_rect.right() - left;

    out_rect.setRect(left, top, width, height);
    return out_rect;
}

QRect DisplayScreen::boundRectOrDrawText(QPainter *painter, bool draw, int left, int top, int width, int height, int flags, QString text)
{
    // If draw is false, calculate the rectangle that the specified text would be
    // drawn into if it was draw. If draw is true, draw as well.
    // Output rect is returned.
    QRect out_rect;
    if(draw)
        painter->drawText(left, top, width, height, flags, text, &out_rect);
    else
        out_rect = painter->boundingRect(left, top, width, height, flags, text);
    return out_rect;
}

void DisplayScreen::drawAnnounceText(QPainter *painter, int width, int height)
{
    // Margins:
    int left = 30;
    int top = 20;
    int w = width - left - left;
    int h = height - top - top;

    int flags = Qt::TextWordWrap;
    if(annouceSets.textAlingmentV==0)
        flags += Qt::AlignTop;
    else if(annouceSets.textAlingmentV==1)
        flags += Qt::AlignVCenter;
    else if(annouceSets.textAlingmentV==2)
        flags += Qt::AlignBottom;
    if(annouceSets.textAlingmentH==0)
        flags += Qt::AlignLeft;
    else if(annouceSets.textAlingmentH==1)
        flags += Qt::AlignHCenter;
    else if(annouceSets.textAlingmentH==2)
        flags += Qt::AlignRight;

    QFont font = painter->font();
    int orig_font_size = font.pointSize();

    // Keep decreasing the font size until the text fits into the allocated space:
    QRect rect;

    bool exit = false;
    while( !exit )
    {
        rect = painter->boundingRect(left, top, w, h, flags, announcement.text);
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
            rect = painter->boundingRect(left, top, w, h, flags, announcement.text);
            exit = ( rect.width() <= w && rect.height() <= h );
            if( !exit )
            {
                font.setPointSize( font.pointSize()-1 );
                //qDebug() << "SETTING SIZE:" << font.pointSize();
                painter->setFont(font);
            }
        }
    }
    painter->drawText(rect, flags, announcement.text);

}

void DisplayScreen::setFading(bool fade)
{
    useFading = fade;
}

void DisplayScreen::setDisplaySettings(DisplaySettings sets)
{
    //    mySettings = sets;
}

void DisplayScreen::setNewWallpaper(QString path, bool isToUse)
{
    if(isToUse)
        wallpaperPath = path;
    else
        wallpaperPath.clear();

    if(wallpaperPath.simplified().isEmpty() )
        wallpaper = QPixmap();
    else
    {
        wallpaper.load(wallpaperPath);
        wallpaper = wallpaper.scaled(width(),height());
    }
}

void DisplayScreen::setNewPassiveWallpaper(QString path, bool isToUse)
{
    if(isToUse)
        passiveWallpaperPath = path;
    else
        passiveWallpaperPath.clear();

    if(passiveWallpaperPath.simplified().isEmpty() )
        passiveWallpaper = QImage();
    else
    {
        passiveWallpaper.load(passiveWallpaperPath);
        passiveWallpaper = passiveWallpaper.scaled(width(), height());
    }
}

void DisplayScreen::paintEvent(QPaintEvent *event )
{
    QPainter painter(this);
//    QSiza sz();
    QImage txtPix(width(), height(), QImage::Format_ARGB32);//_Premultiplied);
    txtPix.fill(qRgba(0, 0, 0, 0)); //transparent background
    QPainter txtPainter(&txtPix);

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
        if (displayType != "pix" && (wallpaper.width()!=width() || wallpaper.isNull()))
        {
            wallpaper.load(wallpaperPath);
            if( !wallpaper.isNull() )
                wallpaper = wallpaper.scaled(width(),height());
        }
        if( ! wallpaper.isNull() )
        {
            int ww = wallpaper.width();
            int wh = wallpaper.height();
            if(displayType == "pix" && ww!=width() && wh!=height())
                painter.drawPixmap((width()-ww)/2,(height()-wh)/2,wallpaper);
            else if(displayType == "pix" && ww!=width())
                painter.drawPixmap((width()-ww)/2,0,wallpaper);
            else if(displayType == "pix" && wh!=height())
                painter.drawPixmap(0,(height()-wh)/2,wallpaper);
            else
                painter.drawPixmap(0,0,wallpaper);

        }
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
        //qDebug() << "no text present, passive path:" << passiveWallpaperPath;

        if (passiveWallpaper.width()!=width() || passiveWallpaper.isNull())
        {
            passiveWallpaper.load(passiveWallpaperPath);
            if( !passiveWallpaper.isNull() )
                passiveWallpaper = passiveWallpaper.scaled(width(),height());
        }
        if( ! passiveWallpaper.isNull() )
            painter.drawImage(0,0, passiveWallpaper);
        else
        {
            // Use black for the background:
            painter.setPen(QColor(Qt::black));
            painter.drawRect( 0, 0, width(), height() );
        }
    }

    // Draw the previous image into the window, at decreasing opacity:
    txtPainter.setOpacity(prev_opacity);
    txtPainter.drawPixmap(0, 0, previous_image_pixmap);
//    painter.setOpacity(prev_opacity);
//    painter.drawPixmap(0, 0, previous_image_pixmap);

    // Draw the output_image into the window, at increasing opacity:
//    painter.setOpacity(curr_opacity);
//    painter.drawImage(0, 0, output_image);
    txtPainter.setOpacity(curr_opacity);
    txtPainter.drawImage(0, 0, output_image);

//    textRenderLabel->setPixmap(QPixmap::fromImage(output_image.scaled(width()/2,height()/2)));
    textRenderLabel->setPixmap(QPixmap::fromImage(txtPix));

    // Reset the opacity to default opaque:
    painter.setOpacity(1.0);
    txtPainter.setOpacity(1.0);
//    txtPainter.end();
}

// Stack Blur Algorithm by Mario Klingemann <mario@quasimondo.com>
void DisplayScreen::fastbluralpha(QImage &img, int radius)
{
    if (radius < 1)
        return;

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
    for (i=0; i < 256*divsum; ++i)
    {
        dv[i] = (i/divsum);
    }

    yw = yi = 0;

    int **stack = new int*[div];
    for (int i = 0; i < div; ++i)
    {
        stack[i] = new int[4];
    }

    int stackpointer;
    int stackstart;
    int *sir;
    int rbs;
    int r1 = radius+1;
    int routsum, goutsum, boutsum, aoutsum;
    int rinsum, ginsum, binsum, ainsum;

    for (y = 0; y < h; ++y)
    {
        rinsum = ginsum = binsum = ainsum
                = routsum = goutsum = boutsum = aoutsum
                = rsum = gsum = bsum = asum = 0;
        for (i =- radius; i <= radius; ++i)
        {
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

            if (i > 0)
            {
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
                ainsum += sir[3];
            }
            else
            {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
                aoutsum += sir[3];
            }
        }
        stackpointer = radius;

        for (x=0; x < w; ++x)
        {
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

            if (y == 0)
            {
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
    for (x=0; x < w; ++x)
    {
        rinsum = ginsum = binsum = ainsum
                = routsum = goutsum = boutsum = aoutsum
                = rsum = gsum = bsum = asum = 0;

        yp =- radius * w;

        for (i=-radius; i <= radius; ++i)
        {
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

            if (i > 0)
            {
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
                ainsum += sir[3];
            }
            else
            {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
                aoutsum += sir[3];
            }

            if (i < hm)
            {
                yp += w;
            }
        }

        yi = x;
        stackpointer = radius;

        for (y=0; y < h; ++y)
        {
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

            if (x==0)
            {
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
