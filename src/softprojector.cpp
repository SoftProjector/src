/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2011  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

#include <QDesktopWidget>
#include "softprojector.h"
#include "ui_softprojector.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "helpdialog.h"

SoftProjector::SoftProjector(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SoftProjectorClass)
{
    // Load settings
    allsettings.loadSettings("0");
    //Setting up the Display Screen
    desktop = new QDesktopWidget();

    // NOTE: With virtual desktop, desktop->screen() will always return the main screen,
    // so this will initialize the Display1 widget on the main screen:
    display = new Display1(desktop->screen(0));
    // Don't worry, we'll move it later

    bibleWidget = new BibleWidget;
    songWidget = new SongWidget;
    editWidget = new EditWidget;
    announceWidget = new AnnounceWidget;
    manageDialog = new ManageDataDialog(this);

//    this->setLayoutDirection(Qt::RightToLeft);
    ui->setupUi(this);


    // Create action group for language slections
    languagePath = "."+QString(QDir::separator())+"translations"+QString(QDir::separator());
    createLanguageActions();

    // Always place the "Settings" menu item under the application
    // menu, even if the item is translated (Mac OS X):
    ui->actionSettings->setMenuRole(QAction::PreferencesRole);
    // FIXME Make the Preferences menu appear in the menu bar even for the
    // display window (Mac OS X)

    // Apply Settings
    applySetting(allsettings);

    display->setCursor(Qt::BlankCursor); //Sets a Blank Mouse to the screen
    display->setWindowIcon(QIcon(":icons/icons/display.png"));

    positionDisplayWindow();

    display->renderText(false);

    showing = false;

    ui->projectTab->clear();
    ui->projectTab->addTab(bibleWidget,QIcon(":/icons/icons/book.png"), tr("Bible (F6)"));
    ui->projectTab->addTab(songWidget,QIcon(":/icons/icons/song_tab.png"), tr("Songs (F7)"));
    ui->projectTab->addTab(announceWidget,QIcon(":/icons/icons/announce.png"), tr("Announcements (F8)"));

    connect(songWidget, SIGNAL(sendSong(Song, int)),
            this, SLOT(setSongList(Song, int)));
    connect(bibleWidget, SIGNAL(goLive(QStringList, QString, QItemSelection)),
            this, SLOT(setChapterList(QStringList, QString, QItemSelection)));
    connect(announceWidget,SIGNAL(sendText(Announcement)),
            this, SLOT(setAnnounceText(Announcement)));
    connect(display, SIGNAL(requestTextDrawing(QPainter*, int, int)),
            this, SLOT(drawText(QPainter*, int, int)));
    connect(editWidget, SIGNAL(updateSongFromDatabase(int,int)),
            songWidget, SLOT(updateSongFromDatabase(int,int)));
    connect(editWidget, SIGNAL(addedNew(Song,int)),
            songWidget,SLOT(addNewSong(Song,int)));
    connect(editWidget, SIGNAL(setSongEditStatus(bool)),
            this, SLOT(setSongEditStatus(bool)));
    connect(manageDialog, SIGNAL(setMainArrowCursor()),
            this, SLOT(setArrowCursor()));
    connect(manageDialog, SIGNAL(setMainWaitCursor()),
            this, SLOT(setWaitCursor()));
    connect(bibleWidget, SIGNAL(setArrowCursor()),
            this, SLOT(setArrowCursor()));
    connect(bibleWidget, SIGNAL(setWaitCursor()),
            this, SLOT(setWaitCursor()));
    connect(songWidget, SIGNAL(setArrowCursor()),
            this, SLOT(setArrowCursor()));
    connect(songWidget, SIGNAL(setWaitCursor()),
            this, SLOT(setWaitCursor()));
    connect(songWidget, SIGNAL(sendPlaylistChanged(bool)),
            this, SLOT(setProjectChanged(bool)));
    connect(announceWidget, SIGNAL(annouceListChanged(bool)),
            this, SLOT(setProjectChanged(bool)));
    connect(bibleWidget, SIGNAL(historyListChanged(bool)),
            this, SLOT(setProjectChanged(bool)));
    connect(languageGroup, SIGNAL(triggered(QAction*)), this, SLOT(switchLanguage(QAction*)));

    ui->toolBar->addAction(ui->actionNew_Project);
    ui->toolBar->addAction(ui->actionOpen);
    ui->toolBar->addAction(ui->actionSave_Project);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionSettings);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionNewSong);
    ui->toolBar->addAction(ui->actionEditSong);
    ui->toolBar->addAction(ui->actionCopy_Song);
    ui->toolBar->addAction(ui->actionDeleteSong);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionSong_Counter);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->action_Help);

    ui->show_button->setEnabled(false);
    ui->clear_button->setEnabled(false);

    // Hide Multi verse selection, only visibel to be when showing bible
    ui->rbMultiVerse->setVisible(false);

        version_string = "1.07"; // to be used only for official release
//    version_string = "1.07_Beta2"; // to be used between official releases
    this->setWindowTitle("softProjector " + version_string);

    // Initialize bool variables
    setSongEditStatus(false);
}

SoftProjector::~SoftProjector()
{
    saveSettings();
    delete songWidget;
    delete editWidget;
    delete bibleWidget;
    delete announceWidget;
    delete display;
    delete desktop;
    delete ui;
}

void SoftProjector::positionDisplayWindow()
{
    // Position the display window as needed (including setting "always on top" flag,
    // showing full screen / normal mode, and positioning it on the right screen)

    if (allsettings.general.displayIsOnTop)
        display->setWindowFlags(Qt::WindowStaysOnTopHint);
    else
        display->setWindowFlags(0); // Do not show always on top

    if( desktop->numScreens() > 1 )
    {
        if (desktop->isVirtualDesktop())
        {
            // Move the display widget to screen 1 (secondary screen):
            // FIXME what if more than 2 screens are available??
            QPoint top_left = desktop->screenGeometry(allsettings.general.displayScreen).topLeft();
            display->move(top_left);
        }
        display->showFullScreen();
    }
    else
    {
        // Single monitor only, show normal (not full screen):
        display->showNormal();
        display->setGeometry(10, 50, 800, 600);
        // NOTE the y-coordinate of 50 is so that the window does not overwrite the menu bar on Mac OS X
    }
}

void SoftProjector::saveSettings()
{
    // Save splitter states
    allsettings.spmain.spSplitter = ui->splitter->saveState();
    allsettings.spmain.bibleHiddenSplitter = bibleWidget->getHiddenSplitterState();
    allsettings.spmain.bibleShowSplitter = bibleWidget->getShownSplitterState();
    allsettings.spmain.songSplitter = songWidget->getSplitterState();

    // Save window maximized state
    allsettings.spmain.isWindowMaximized = this->isMaximized();

    // save translation settings
    QList<QAction*> languageActions = ui->menuLanguage->actions();

    for(int i(0);i < languageActions.count();++i)
    {
        if(languageActions.at(i)->isChecked())
        {
            if(i < languageActions.count())
                allsettings.spmain.uiTranslation = languageActions.at(i)->data().toString();
            else
                allsettings.spmain.uiTranslation = "en";
        }
    }

    // save settings
    allsettings.saveSettings("0");
}

void SoftProjector::updateSetting(Settings &allsets)
{
    allsettings = allsets;
    bibleWidget->setSettings(allsettings.bible);
    announceWidget->setAlingment(allsettings.announce.textAlingmentV,allsettings.announce.textAlingmentH);

    // Apply display settings;
    display->setNewPassiveWallpaper(allsettings.general.backgroundPath,allsettings.general.useBackground);
    display->setFading(allsettings.general.useFading);
    display->setUseShadow(allsettings.general.useShadow);
    display->setBlur(allsettings.general.useBlurShadow);
}

void SoftProjector::applySetting(Settings &allsets)
{
    allsettings = allsets;
    bibleWidget->setSettings(allsettings.bible);
    announceWidget->setAlingment(allsettings.announce.textAlingmentV,allsettings.announce.textAlingmentH);

    // Apply display settings;
    display->setNewPassiveWallpaper(allsettings.general.backgroundPath,allsettings.general.useBackground);
    display->setFading(allsettings.general.useFading);
    display->setUseShadow(allsettings.general.useShadow);
    display->setBlur(allsettings.general.useBlurShadow);

    // Apply splitter states
    ui->splitter->restoreState(allsettings.spmain.spSplitter);
    bibleWidget->setHiddenSplitterState(allsettings.spmain.bibleHiddenSplitter);
    bibleWidget->setShownSplitterState(allsettings.spmain.bibleShowSplitter);
    songWidget->setSplitterState(allsettings.spmain.songSplitter);

    // Apply window maximized
    if(allsettings.spmain.isWindowMaximized)
        this->setWindowState(Qt::WindowMaximized);

    // Apply current translation
    QList<QAction*> la = ui->menuLanguage->actions();
    QString splocale;
    for(int i(0);i < la.count(); ++i)
    {
        if(la.at(i)->data().toString() == allsettings.spmain.uiTranslation)
        {
            if(i < la.count())
            {
                ui->menuLanguage->actions().at(i)->setChecked(true);
                splocale = allsettings.spmain.uiTranslation;
            }
            else
            {
                ui->menuLanguage->actions().at(0)->setChecked(true);//default
                splocale = "en";
            }
        }
    }
    cur_locale = splocale;
    retranslateUis();
}

void SoftProjector::closeEvent(QCloseEvent *event)
{
    if(is_project_saved || project_file_path.isEmpty())
    {
        QCoreApplication::exit(0);
        event->accept();
    }
    else
    {
        QMessageBox mb;
        mb.setWindowTitle(tr("Project not saved","project as in document file"));
        mb.setText(tr("Do you want to save current project?","project as in document file"));
        mb.setIcon(QMessageBox::Question);
        mb.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard);
        mb.setDefaultButton(QMessageBox::Save);
        int ret = mb.exec();

        switch (ret)
        {
        case QMessageBox::Save:
            // Save Project and close
            on_actionSave_Project_triggered();
            QCoreApplication::exit(0);
            event->accept();
            break;
        case QMessageBox::Cancel:
            // Cancel was clicked, do nothing
            event->ignore();
            break;
        case QMessageBox::Discard:
            // Close without saving
            QCoreApplication::exit(0);
            event->accept();
            break;
        default:
            // should never be reached
            break;
        }
    }
}

void SoftProjector::keyPressEvent( QKeyEvent * event )
{
    // Will get called when a key is pressed
    int key = event->key();
    if( key == Qt::Key_F6 )
        ui->projectTab->setCurrentWidget(bibleWidget);
    else if( key == Qt::Key_F7 )
        ui->projectTab->setCurrentWidget(songWidget);
    else if( key == Qt::Key_F8 )
        ui->projectTab->setCurrentWidget(announceWidget);
    else
        QMainWindow::keyPressEvent(event);
}

void SoftProjector::on_actionClose_triggered()
{
    close();
}

void SoftProjector::setAnnounceText(Announcement text)
{
    announcement_text = text;
    type = "announce";
    ui->rbMultiVerse->setVisible(false);
    ui->rbMultiVerse->setChecked(false);
    showing = true;
    new_list = true;
    ui->labelShow->setText(tr("Announcement"));
    ui->listShow->clear();
    ui->listShow->setSpacing(5); // ?
    ui->listShow->setWordWrap(true);
    ui->listShow->addItem(announcement_text.text);
    ui->listShow->setCurrentRow(0);
    ui->listShow->setFocus();
    new_list = false;
    updateScreen();
}

void SoftProjector::setSongList(Song song, int row)
{
    QStringList song_list = song.getSongTextList();
    current_song = song;
    current_song_verse = row;

    // Display the specified song text in the right-most column of softProjector
    type = "song";
    ui->rbMultiVerse->setVisible(false);
    ui->rbMultiVerse->setChecked(false);
    showing = true;
    new_list = true;
    ui->labelShow->setText(song.title);
    ui->listShow->setSpacing(5);
    ui->listShow->setWordWrap(false);
    ui->listShow->clear();
    ui->listShow->addItems(song_list);
    ui->listShow->setCurrentRow(row);
    ui->listShow->setFocus();
    new_list = false;
    updateScreen();
}

QRect SoftProjector::boundRectOrDrawText(QPainter *painter, bool draw, int left, int top, int width, int height, int flags, QString text)
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

QRect SoftProjector::drawSongTextToRect(QPainter *painter, QRect bound_rect, bool draw, bool wrap, QString main_text, QString caption_str, QString song_num_str, QString ending_str)
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
    {
        main_flags = Qt::TextWordWrap;
        if(allsettings.song.textAlingmentV==0)
            main_flags += Qt::AlignTop;
        else if(allsettings.song.textAlingmentV==1)
            main_flags += Qt::AlignVCenter;
        else if(allsettings.song.textAlingmentV==2)
            main_flags += Qt::AlignBottom;
        if(allsettings.song.textAlingmentH==0)
            main_flags += Qt::AlignLeft;
        else if(allsettings.song.textAlingmentH==1)
            main_flags += Qt::AlignHCenter;
        else if(allsettings.song.textAlingmentH==2)
            main_flags += Qt::AlignRight;

    }
    else
    {
        if(allsettings.song.textAlingmentV==0)
            main_flags = Qt::AlignTop;
        else if(allsettings.song.textAlingmentV==1)
            main_flags = Qt::AlignVCenter;
        else if(allsettings.song.textAlingmentV==2)
            main_flags = Qt::AlignBottom;
        if(allsettings.song.textAlingmentH==0)
            main_flags += Qt::AlignLeft;
        else if(allsettings.song.textAlingmentH==1)
            main_flags += Qt::AlignHCenter;
        else if(allsettings.song.textAlingmentH==2)
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

    //    qDebug() << "OUT top, left, height, width:" << top << left << height << width;
    out_rect.setRect(left, top, width, height);
    return out_rect;
}

void SoftProjector::drawCurrentSongText(QPainter *painter, int width, int height)
{
    // Draw the text of the current song verse to the specified painter; making
    // sure that the output rect is narrower than <width> and shorter than <height>.
    QStringList song_list = current_song.getSongTextList();

    QString main_text;
    QString top_caption_str;
    QString song_ending = " ";

    bool last_verse = ( current_song_verse == song_list.count()-1 );

    QStringList lines_list = song_list.at(current_song_verse).split("\n");
    QString song_num_str = QString::number(current_song.num);
    QString song_key_str = current_song.tune;

    // Remove the first line if it starts with "Kuplet" or "Pripev" or "Vstavka":
    QString textemp = lines_list[0];
    //    textemp.remove(6,10);

    // Check whether to display song numbers
    if (allsettings.song.showSongNumber)
        song_num_str = song_num_str;
    else
        song_num_str = " ";

    // Check whether to display song key
    if (allsettings.song.showSongKey)
        song_num_str = song_key_str + "  " + song_num_str;
    else
        song_num_str = song_num_str;

    // Remove Stanza Title from the stanza and send it to top caption
    if(isStanzaTitle(textemp))
    {
        if (allsettings.song.showStanzaTitle)
            top_caption_str = lines_list.at(0);
        else
            top_caption_str = " ";
        //qDebug() << "CAPTION:";
        //qDebug() << top_caption_str;
        lines_list.removeFirst();
    }


    // Convert lines_list to a string:
    main_text = "";
    int num_lines = 0;
    for (int i = 0; i < lines_list.size(); ++i)
    {
        if( i > 0 )
            main_text += "\n";
        main_text += lines_list.at(i);
        num_lines++;
    }

    // Prepare Song ending string
    if( last_verse )
    {
        // first check if to show ending
        if(allsettings.song.showSongEnding)
        {
            if(allsettings.song.songEndingType == 0)
                song_ending = "*    *    *";
            else if(allsettings.song.songEndingType == 1)
            {
                // First check if copyrigth info exist. If it does show it.
                // If some exist, then show what exist. If nothing exist, then show '* * *'
                if(!current_song.wordsBy.isEmpty() && !current_song.musicBy.isEmpty())
                    song_ending = QString(tr("Words by: %1, Music by: %2")).arg(current_song.wordsBy).arg(current_song.musicBy);
                else if(!current_song.wordsBy.isEmpty() && current_song.musicBy.isEmpty())
                    song_ending = QString(tr("Words by: %1")).arg(current_song.wordsBy);
                else if(current_song.wordsBy.isEmpty() && !current_song.musicBy.isEmpty())
                    song_ending = QString(tr("Music by: %1")).arg(current_song.musicBy);
                else if(current_song.wordsBy.isEmpty() && current_song.musicBy.isEmpty())
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
            out_rect = drawSongTextToRect(painter, rect, false, true, main_text, top_caption_str, song_num_str, song_ending);
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

void SoftProjector::drawBibleTextToRect(QPainter *painter, QRect& trect, QRect& crect, QString ttext, QString ctext, int tflags, int cflags, int top, int left, int width, int height, int font_size)
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

void SoftProjector::drawCurrentBibleText(QPainter *painter, int width, int height)
{
    // Margins:
    int left = 30;
    int top = 20;

    //int right = width - left;
    int w = width - left - left;
    int h = height - top - top;

    // set maximum screen size - For primary bibile  only
    int maxh = h * allsettings.bible.maxScreen/100; // maximun screen height
    int maxtop; // top of max screen
    if(allsettings.bible.maxScreenFrom=="top")
        maxtop  = top;
    if(allsettings.bible.maxScreenFrom=="bottom")
        maxtop = top+h-maxh;

    // apply max screen use settings
    h=maxh;
    top=maxtop;

    //       QFont font = painter->font();
    QFont font = painter->font();
    //       qDebug()<< "font 1:" + font.toString();
    int original_size = font.pointSize();
    int current_size = original_size;
    Verse& v = current_verse; // for convenience


    // Keep decreasing the font size until the text fits into the allocated space:

    // Rects for storing the position of the text and caption drawing:
    QRect trect1, crect1, trect2, crect2, trect3, crect3;
    // Flags to be used for drawing verse text and caption:
    int tflags = Qt::TextWordWrap;
    tflags = Qt::TextWordWrap;
    if(allsettings.bible.textAlingmentV==0)
        tflags += Qt::AlignTop;
    else if(allsettings.bible.textAlingmentV==1)
        tflags += Qt::AlignVCenter;
    else if(allsettings.bible.textAlingmentV==2)
        tflags += Qt::AlignBottom;
    if(allsettings.bible.textAlingmentH==0)
        tflags += Qt::AlignLeft;
    else if(allsettings.bible.textAlingmentH==1)
        tflags += Qt::AlignHCenter;
    else if(allsettings.bible.textAlingmentH==2)
        tflags += Qt::AlignRight;
    int cflags = Qt::AlignRight | Qt::AlignTop ;

    bool exit = false;
    while( !exit )
    {
        if(v.secondary_text.isEmpty() && v.trinary_text.isEmpty())
        {
            // Prepare primary version only, 2nd and 3rd do not exist
            // Figure out how much space the drawing will take at the current font size:
            drawBibleTextToRect(painter,trect1,crect1,v.primary_text,v.primary_caption,tflags,cflags,top,left,w,h, current_size);

            // Make sure that all fits into the screen
            int th = trect1.height()+crect1.height();
            exit = (th<=h);
        }
        else if(!v.secondary_text.isEmpty() && v.trinary_text.isEmpty())
        {
            // Prepare primary and secondary versions, trinary does not exist

            // Figure out how much space the drawing will take at the current font size for primary:
            drawBibleTextToRect(painter,trect1,crect1,v.primary_text,v.primary_caption,tflags,cflags,top,left,w,h/2,current_size);

            // set new top for secondary
            int top2 = crect1.bottom();
            if(top2<h/2+top)
                top2=h/2+top;

            // Figure out how much space the drawing will take at the current font size for secondary:
            drawBibleTextToRect(painter,trect2,crect2,v.secondary_text,v.secondary_caption,tflags,cflags,top2,left,w,h/2,current_size);

            int th1 = trect1.height()+crect1.height();
            int th2 = trect2.height()+crect2.height();

            // Make sure that primary fits
            exit = (th1<=h/2);
            if (exit)
                // If primary fits, make sure secondary fits
                exit = (th2<=h/2);
        }
        else if(!v.secondary_text.isEmpty() && !v.trinary_text.isEmpty())
        {
            // Prepare primary and secondary and trinary versions

            // Figure out how much space the drawing will take at the current font size for primary:
            drawBibleTextToRect(painter,trect1,crect1,v.primary_text,v.primary_caption,tflags,cflags,top,left,w,h*1/3,current_size);

            // set new top for secondary
            int top2 = crect1.bottom();
            if(top2<h*1/3+top)
                top2=h*1/3+top;

            // Figure out how much space the drawing will take at the current font size for secondary:
            drawBibleTextToRect(painter,trect2,crect2,v.secondary_text,v.secondary_caption,tflags,cflags,top2,left,w,h*1/3,current_size);

            // set new top for trinaty
            top2 = crect2.bottom();
            if(top2<h*2/3+top)
                top2 = h*2/3+top;

            // Figure out how much space the drawing will take at the current font size for trinary:
            drawBibleTextToRect(painter,trect3,crect3,v.trinary_text,v.trinary_caption,tflags,cflags,top2,left,w,h*1/3,current_size);

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
    painter->drawText(trect1, tflags, v.primary_text);
//    tflags += Qt::RightToLeft;
    if(!v.secondary_text.isNull())
        painter->drawText(trect2, tflags, v.secondary_text);
    if(!v.trinary_text.isNull())
        painter->drawText(trect3, tflags, v.trinary_text);

    // Draw the verse caption(s) at the final size:
    font.setPointSize(current_size*3/4);
    painter->setFont(font);
    painter->drawText(crect1, cflags, v.primary_caption);
//    cflags += Qt::RightToLeft;
    if(!v.secondary_text.isNull())
        painter->drawText(crect2, cflags, v.secondary_caption);
    if(!v.trinary_caption.isNull())
        painter->drawText(crect3, cflags, v.trinary_caption);

    // Restore the original font:
    font.setPointSize(original_size);
    painter->setFont(font);

}

void SoftProjector::drawAnnounceText(QPainter *painter, int width, int height)
{
    // Margins:
    int left = 30;
    int top = 20;
    int w = width - left - left;
    int h = height - top - top;

    int flags = announcement_text.align_flags;

    QRect rect = QRect(left, top, w, h);
    QString announce_text = announcement_text.text;
    display->paintTextToRect(painter, rect, flags, announce_text);

    //    announceWidget->drawToPainter(painter, width, height);
}

void SoftProjector::drawText(QPainter *painter, int width, int height)
{
    if( !showing )
        return;

    if(type=="song")
        drawCurrentSongText(painter, width, height);
    else if(type=="bible")
        drawCurrentBibleText(painter, width, height);
    else if( type == "announce" )
        drawAnnounceText(painter, width, height);
}

void SoftProjector::setChapterList(QStringList chapter_list, QString caption, QItemSelection selectedItems)
{
    // Called to show a bible verse from a chapter in the preview list

    type = "bible";
    ui->rbMultiVerse->setVisible(true);
    showing = true;
    new_list = true;
    ui->labelShow->setText(caption);
    ui->listShow->clear();
    ui->listShow->setSpacing(0);
    ui->listShow->setWordWrap(true);
    ui->listShow->addItems(chapter_list);

    if(selectedItems.indexes().count()>1)
        ui->rbMultiVerse->setChecked(true);
    else
        ui->rbMultiVerse->setChecked(false);

    ui->listShow->setCurrentRow(selectedItems.first().top());
    ui->listShow->selectionModel()->select(selectedItems,QItemSelectionModel::Select);
    ui->listShow->setFocus();
    new_list = false;
    updateScreen();
}

void SoftProjector::on_listShow_currentRowChanged(int currentRow)
{
    // Called when the user selects a different row in the show (right-most) list.
    //    updateScreen();
}
void SoftProjector::on_listShow_itemSelectionChanged()
{
    // Called when the user selects a different row in the show (right-most) list.
    // First check if ratio button "Multi Verse" is check. If so, make button "Show"
    // enable and update screen only after show_botton is clicked.
    if(ui->rbMultiVerse->isChecked())
        ui->show_button->setEnabled(true);
    else
        updateScreen();
}

void SoftProjector::updateScreen()
{
    // Display the specified row of the show (rightmost) table to
    // the display
    int currentRow = ui->listShow->currentRow();

    if( showing == false )
    {
        // Do not display any text:
        display->renderText(false);
        ui->show_button->setEnabled(true);
        ui->clear_button->setEnabled(false);
    }
    else if (currentRow >=0 && !new_list)
    {
        ui->show_button->setEnabled(false);
        ui->clear_button->setEnabled(true);

        if(type=="song")
        {
            display->setNewWallpaper(allsettings.song.backgroundPath,allsettings.song.useBackground);
            display->setForegroundColor(allsettings.song.textColor);
            display->setNewFont(allsettings.song.textFont);
            current_song_verse = currentRow;
        }
        else if(type=="bible")
        {
            display->setNewWallpaper(allsettings.bible.backgroundPath,allsettings.bible.useBackground);
            display->setForegroundColor(allsettings.bible.textColor);
            display->setNewFont(allsettings.bible.textFont);
            int srows(ui->listShow->count());
            QList<int> currentRows;
            for(int i(0); i<srows; ++i)
            {
                if(ui->listShow->item(i)->isSelected())
                    currentRows.append(i);
            }
            current_verse = bibleWidget->bible.getCurrentVerseAndCaption(currentRows,allsettings.bible);
        }
        else if( type == "announce" )
        {
            display->setNewWallpaper(allsettings.announce.backgroundPath,allsettings.announce.useBackground);
            display->setForegroundColor(allsettings.announce.textColor);
            display->setNewFont(allsettings.announce.textFont);
            announcement_text.text = announceWidget->getText();
        }
        display->renderText(true);
    }
}

void SoftProjector::on_clear_button_clicked()
{
    showing = false;
    updateScreen();
}

void SoftProjector::on_show_button_clicked()
{
    showing = true;
    updateScreen();
}

void SoftProjector::on_actionEditSong_triggered()
{
    if (songWidget->isSongSelected())
    {
        if (is_song_in_edit) //Prohibits editing a song when a different song already been edited.
        {
            QMessageBox ms;
            ms.setWindowTitle(tr("Cannot start new edit"));
            ms.setText(tr("Another song is already been edited."));
            ms.setInformativeText(tr("Please save and/or close current edited song before edited a different song."));
            ms.setIcon(QMessageBox::Information);
            ms.exec();
        }
        else
        {
            editWidget->show();
            editWidget->setEdit(songWidget->getSongToEdit());
            editWidget->activateWindow();
            setSongEditStatus(true);
        }
    }
    else
    {
        QMessageBox ms;
        ms.setWindowTitle(tr("No song selected"));
        ms.setText(tr("No song has been selected to be edited."));
        ms.setInformativeText(tr("Please select a song to be edited."));
        ms.setIcon(QMessageBox::Information);
        ms.exec();
    }
}

void SoftProjector::setSongEditStatus(bool isEdited)
{
    is_song_in_edit = isEdited;
}

void SoftProjector::on_actionNewSong_triggered()
{
    if (is_song_in_edit) //Prohibits editing a song when a different song already been edited.
    {
        QMessageBox ms;
        ms.setWindowTitle(tr("Cannot create a new song"));
        ms.setText(tr("Another song is already been edited."));
        ms.setInformativeText(tr("Please save and/or close current edited song before edited a different song."));
        ms.setIcon(QMessageBox::Information);
        ms.exec();
    }
    else
    {
        editWidget->show();
        editWidget->setNew();
        editWidget->activateWindow();
    }
}

void SoftProjector::on_actionCopy_Song_triggered()
{
    if (songWidget->isSongSelected())
    {if (is_song_in_edit) //Prohibits editing a song when a different song already been edited.
        {
            QMessageBox ms;
            ms.setWindowTitle(tr("Cannot copy this song"));
            ms.setText(tr("Another song is already been edited."));
            ms.setInformativeText(tr("Please save and/or close current edited song before edited a different song."));
            ms.setIcon(QMessageBox::Information);
            ms.exec();
        }
        else
        {
            editWidget->show();
            editWidget->setCopy(songWidget->getSongToEdit());
            editWidget->activateWindow();
        }
    }
    else
    {
        QMessageBox ms;
        ms.setWindowTitle(tr("No song selected"));
        ms.setText(tr("No song has been selected to be copied"));
        ms.setInformativeText(tr("Please select a song to be copied"));
        ms.setIcon(QMessageBox::Information);
        ms.exec();
    }
}

void SoftProjector::on_actionDeleteSong_triggered()
{
    if (songWidget->isSongSelected())
    {
        QString song_title = songWidget->currentSong().title;
        QMessageBox ms;
        ms.setWindowTitle(tr("Delete song?"));
        ms.setText(tr("Delete song \"") + song_title + "\"?");
        ms.setInformativeText(tr("This action will permanentrly delete this song"));
        ms.setIcon(QMessageBox::Question);
        ms.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        ms.setDefaultButton(QMessageBox::Yes);
        int ret = ms.exec();

        switch (ret) {
        case QMessageBox::Yes:
            // Delete a song
            //        close();
            songWidget->deleteSong();
            break;
        case QMessageBox::No:
            // Cancel was clicked
            break;
        default:
            // should never be reached
            break;
        }
    }
    else
    {
        QMessageBox ms;
        ms.setWindowTitle(tr("No song selected"));
        ms.setText(tr("No song has been selected to be deleted"));
        ms.setInformativeText(tr("Please select a song to be deleted"));
        ms.setIcon(QMessageBox::Information);
        ms.exec();
    }
}

void SoftProjector::on_actionSettings_triggered()
{
    SettingsDialog *settingsDialog;
    settingsDialog = new SettingsDialog(this);
    settingsDialog->loadSettings(allsettings);
    settingsDialog->exec();
}

void SoftProjector::on_listShow_doubleClicked(QModelIndex index)
{
    // Called when the user double clicks on a row in the preview table.
    showing = true;
    updateScreen();
}

void SoftProjector::on_actionAbout_triggered()
{
    AboutDialog *aboutDialog;
    aboutDialog = new AboutDialog(this, version_string);
    aboutDialog->exec();
}

void SoftProjector::on_projectTab_currentChanged(int index)
{
    if (index == 1)
    {
        // Songs tab is activated
        ui->actionEditSong->setEnabled(true);
        ui->actionNewSong->setEnabled(true);
        ui->actionCopy_Song->setEnabled(true);
        ui->actionDeleteSong->setEnabled(true);
    }
    else
    {
        // Other tabs are activated
        ui->actionEditSong->setEnabled(false);
        ui->actionNewSong->setEnabled(false);
        ui->actionCopy_Song->setEnabled(false);
        ui->actionDeleteSong->setEnabled(false);
    }
}

void SoftProjector::on_actionManage_Database_triggered()
{
    QSqlQuery sq;

    manageDialog->load_songbooks();
    manageDialog->exec();

    // Reload songbooks if Songbook has been added, edited, or deleted
    if (manageDialog->reload_songbook)
        songWidget->updateSongbooks();

    // Reload Bibles if Bible has been deleted
    if (manageDialog->reload_bible)
    {
        // check if Primary bible has been removed
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + allsettings.bible.primaryBible);
        if (!sq.first())
        {
            // If original primary bible has been removed, set first bible in the list to be primary
            sq.clear();
            sq.exec("SELECT id FROM BibleVersions");
            sq.first();
            allsettings.bible.primaryBible = sq.value(0).toString();
        }
        sq.clear();

        // check if secondary bible has been removed, if yes, set secondary to "none"
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + allsettings.bible.secondaryBible);
        if (!sq.first())
            allsettings.bible.secondaryBible = "none";
        sq.clear();

        // check if trinary bible has been removed, if yes, set secondary to "none"
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + allsettings.bible.trinaryBible);
        if (!sq.first())
            allsettings.bible.trinaryBible = "none";
        sq.clear();

        // check if operator bible has been removed, if yes, set secondary to "same"
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + allsettings.bible.operatorBible);
        if (!sq.first())
            allsettings.bible.operatorBible = "same";
        bibleWidget->setSettings(allsettings.bible);
    }
}

void SoftProjector::on_action_Help_triggered()
{
    HelpDialog *help_dialog;
    help_dialog = new HelpDialog();
    help_dialog->show();
}

void SoftProjector::setArrowCursor()
{
    this->setCursor(Qt::ArrowCursor);
}

void SoftProjector::setWaitCursor()
{
    this->setCursor(Qt::WaitCursor);
}

void SoftProjector::createLanguageActions()
{
    // find all *.qm files at language folder
     // and create coresponding action in language menu

        languageGroup = new QActionGroup(this);
        //default language and flag
        QAction *englishAction = new QAction(QIcon(":/icons/icons/flag_uk.png"), "English", this);
        englishAction->setCheckable(true);
        englishAction->setChecked(true);
        englishAction->setIconVisibleInMenu(true);
        languageGroup->addAction(englishAction);
        ui->menuLanguage->addAction(englishAction);

        QDir languageDir(languagePath);
        //all available languages
        QStringList languagesList = languageDir.entryList(QStringList("softpro_*.qm"), QDir::Files);
        //all available flags
        QStringList flagsList = languageDir.entryList(QStringList("flag_*.png"), QDir::Files);

        foreach(QString agent, languagesList)
        {
            // local translator for taken original language's name
            QTranslator tmpTranslator;
            tmpTranslator.load(agent, languageDir.absolutePath());
            // this string are used for detection language' name
            // this is one of translated strings

            QString fullLanguageName = tmpTranslator.translate("Native language name", "English","Do not change");
            QAction *tmpAction = new QAction(fullLanguageName, this);

            QString splocale = agent.remove(0, agent.indexOf('_')+1);
            splocale.chop(3);
            // flag's file name
            QString flagFileName = "flag_"+splocale+".png";
            if(flagsList.contains(flagFileName))//  if flag is available
            {
                tmpAction->setIcon(QIcon(languageDir.absolutePath() + QDir::separator() + flagFileName));
                tmpAction->setIconVisibleInMenu(true);
            }

            tmpAction->setData(splocale);// information abount localization
            tmpAction->setCheckable(true);
            languageGroup->addAction(tmpAction);
            ui->menuLanguage->addAction(tmpAction);
        }

//        connect(languageGroup, SIGNAL(triggered(QAction*)), this, SLOT(switchLanguage(QAction*)));
}
void SoftProjector::switchLanguage(QAction *action)
{
    cur_locale = action->data().toString();
    retranslateUis();
}

void SoftProjector::retranslateUis()
{
    qApp->removeTranslator(&translator);
    if(cur_locale != "en")
    {
        translator.load("softpro_"+cur_locale+".qm", QDir(languagePath).absolutePath());
        // qt libs translator must be add there,
        // but where are qt_locale.qm files?
        qApp->installTranslator(&translator);
    }

    ui->retranslateUi(this);
    ui->projectTab->setTabText(0, tr("Bible (F6)"));
    ui->projectTab->setTabText(1, tr("Songs (F7)"));
    ui->projectTab->setTabText(2, tr("Announcements (F8)"));
    songWidget->retranslateUis();
    editWidget->retranslateUis();
}

void SoftProjector::on_actionSong_Counter_triggered()
{
    SongCounter *songCounter;
    songCounter = new SongCounter(this, cur_locale);
    songCounter->exec();
}

void SoftProjector::on_actionOpen_triggered()
{
    if(!is_project_saved && !project_file_path.isEmpty())
    {
        QMessageBox mb;
        mb.setWindowTitle(tr("Save Project?","project as in document file"));
        mb.setText(tr("Do you want to save current project before opening other?","project as in document file"));
        mb.setIcon(QMessageBox::Question);
        mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        mb.setDefaultButton(QMessageBox::Yes);
        int ret = mb.exec();

        switch (ret)
        {
        case QMessageBox::Yes:
            // Save Project and continuew opening next
            on_actionSave_Project_triggered();
            break;
        case QMessageBox::No:
            // Cancel was clicked, do nothing
            break;
        default:
            // should never be reached
            break;
        }
    }
    QString path = QFileDialog::getOpenFileName(this,tr("Open softProjector project"),".",
                                                tr("softProjector project file ") + "(*.spp)");
    if(!path.isEmpty())
    {
        // reset project file path only when path is valid and then open

        project_file_path = path;
        openProject();
    }
    updateWindowTest();
}

void SoftProjector::on_actionSave_Project_triggered()
{
    if(project_file_path.isEmpty() || project_file_path.startsWith("untitled"))
        on_actionSave_Project_As_triggered();
    else
        saveProject();

    updateWindowTest();
}

void SoftProjector::on_actionSave_Project_As_triggered()
{
    QString path = QFileDialog::getSaveFileName(this,tr("Save softProjector project as:"),".",
                                                tr("softProjector project file ") + "(*.spp)");
    if(!path.isEmpty())
    {
//        qDebug()<<"path:"<<path;
        if(path.endsWith(".spp"))
            project_file_path = path;
        else
//            qDebug()<<"file with no .spp";
            project_file_path = path + ".spp";
//        qDebug()<<"project path:"<<project_file_path;
        saveProject();
    }

    updateWindowTest();
}

void SoftProjector::on_actionNew_Project_triggered()
{
    QMessageBox mb;
    if(!is_project_saved && !project_file_path.isEmpty())
    {
        mb.setWindowTitle(tr("Save Project?"));
        mb.setText(tr("Do you want to save current project before creating a new project?"));
        mb.setIcon(QMessageBox::Question);
        mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        mb.setDefaultButton(QMessageBox::Yes);
        int ret = mb.exec();

        switch (ret)
        {
        case QMessageBox::Yes:
            // Save Project and continuew opening next
            on_actionSave_Project_triggered();
            break;
        case QMessageBox::No:
            // Cancel was clicked, do nothing
            break;
        default:
            // should never be reached
            break;
        }
    }

    project_file_path = "untitled.spp";
    clearProject();
    updateWindowTest();
}

void SoftProjector::on_actionClose_Project_triggered()
{
    QMessageBox mb;
    if(!is_project_saved && !project_file_path.isEmpty())
    {
        mb.setWindowTitle(tr("Save Project?"));
        mb.setText(tr("Do you want to save current project before closing it?"));
        mb.setIcon(QMessageBox::Question);
        mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        mb.setDefaultButton(QMessageBox::Yes);
        int ret = mb.exec();

        switch (ret)
        {
        case QMessageBox::Yes:
            // Save Project and continuew opening next
            on_actionSave_Project_triggered();
            break;
        case QMessageBox::No:
            // Cancel was clicked, do nothing
            break;
        default:
            // should never be reached
            break;
        }
    }
    project_file_path.clear();
    clearProject();
    updateWindowTest();
}

void SoftProjector::saveProject()
{
    QList<Song> songs = songWidget->getPlaylistSongs();
    Song s;
    QList<BibleSearch> histories = bibleWidget->getHistoryList();
    BibleSearch h;
    QList<Announcement> announcements = announceWidget->getAnnouncements();
    Announcement a;

    QFile file(project_file_path);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xml(&file);
    xml.setAutoFormatting(true);
    xml.setCodec("UTF8");

    xml.writeStartDocument();
    xml.writeStartElement("spProject");
    xml.writeAttribute("version","1.7");

    //Write Bible History
    xml.writeStartElement("BibleHistory");
    for(int i(0); i<histories.count(); ++i)
    {
        QXmlStreamAttributes atrs;
        h = histories.at(i);
        atrs.append("first",QString::number(h.first_v));
        atrs.append("last",QString::number(h.last_v));
        xml.writeStartElement("history"); // Start verse
        xml.writeAttributes(atrs);
        xml.writeTextElement("ids",h.verse_id);
        xml.writeTextElement("book",h.book);
        xml.writeTextElement("chapter",h.chapter);
        xml.writeTextElement("display_txt",h.display_text);
        xml.writeTextElement("verse",h.verse);
        xml.writeTextElement("verse_txt",h.verse_text);
        xml.writeEndElement(); //End verse
    }
    xml.writeEndElement(); // End Bible Histories

    //Write songs
    xml.writeStartElement("Songs");
    for(int i(0); i<songs.count(); ++i)
    {
        xml.writeStartElement("song"); // start song
        s = songs.at(i);
        QXmlStreamAttributes atrs;
        atrs.append("id",QString::number(s.songID));
        atrs.append("num",QString::number(s.num));
        atrs.append("category",QString::number(s.category));
        xml.writeAttributes(atrs);

        // write song items
        xml.writeTextElement("title",s.title);
        xml.writeTextElement("text",s.songText);
        xml.writeTextElement("songbook",s.songbook_name);
        xml.writeTextElement("tune",s.tune);
        xml.writeTextElement("words",s.wordsBy);
        xml.writeTextElement("music",s.musicBy);
        xml.writeTextElement("font",s.font);
        xml.writeTextElement("aling",s.alingment);
        xml.writeTextElement("back",s.background.trimmed());
        xml.writeTextElement("comments",s.notes);

        xml.writeEndElement(); // enst song
    }
    xml.writeEndElement(); // End Songs

    //Write Announcements
    xml.writeStartElement("Announcements");
    for(int i(0); i<announcements.count();++i)
    {
        a = announcements.at(i);
        xml.writeStartElement("announce");
        xml.writeAttribute("aling_flag",QString::number(a.align_flags));
        xml.writeCharacters(a.text);
        xml.writeEndElement();
    }
    xml.writeEndElement(); // End Announcements

    xml.writeEndElement(); // softProjector Project
    xml.writeEndDocument();

    file.close();
    is_project_saved = true;
}

void SoftProjector::openProject()
{
    QFile file(project_file_path);
    file.open(QIODevice::ReadOnly);
    QXmlStreamReader xml(&file);

    while(!xml.atEnd())
    {
        xml.readNext();
        if(xml.StartElement && xml.name() == "spProject")
        {
            double p_version = xml.attributes().value("version").toString().toDouble();
            if(1.0 < p_version && p_version <= 1.7)
            {
                xml.readNext();
                while(xml.tokenString() != "EndElement" && xml.name() != "spProject")
                {
                    xml.readNext();
                    if(xml.StartElement && xml.name() == "BibleHistory" && p_version >= 1.5)
                    {
                        // Read saved bible history
                        readBibleHistoryFromSavedProject(&xml);
                        xml.readNext();
                    }
                    else if(xml.StartElement && xml.name() == "BibleHistory" && p_version == 1.0)
                    {
                        // Read saved bible history
                        readBibleHistoryFromSavedProject1_0(&xml);
                        xml.readNext();
                    }
                    else if(xml.StartElement && xml.name() == "Songs")
                    {
                        // Read save versionsongs
                        readSongsFromSavedProject(&xml);
                        xml.readNext();
                    }
                    else if(xml.StartElement && xml.name() == "Announcements")
                    {
                        // Read saved announcements
                        readAnnouncementsFromSavedProject(&xml);
                        xml.readNext();
                    }
                }
            }
            else
            {
                //User friednly box for incorrect file version
                QMessageBox mb;
                mb.setWindowTitle(tr("Incorrect project file format"));
                mb.setText(tr("The softProjector project file you are opening,\n"
                              "is not supported by your version of softProjector.\n"
                              "You may try upgrading your version of softProjector."));
                mb.setIcon(QMessageBox::Information);
                mb.exec();

                // remove any file paths
                project_file_path.clear();
                updateWindowTest();
                return;
            }
        }
    }
    file.close();
    is_project_saved = true;
}

void SoftProjector::clearProject()
{
    QList<Announcement> announcements;
    QList<BibleSearch> histories;
    QList<Song> songs;

    announceWidget->loadFromFile(announcements);
    bibleWidget->loadHistoriesFromFile(histories);
    songWidget->loadPlaylistFromFile(songs);
}

void SoftProjector::readAnnouncementsFromSavedProject(QXmlStreamReader *xml)
{
    QList<Announcement> announcements;
    Announcement a;
    QXmlStreamAttributes atrs;
    while(xml->tokenString() != "EndElement")
    {
        xml->readNext();
        if(xml->StartElement && xml->name() == "announce")
        {
            atrs = xml->attributes();
            QString f = atrs.value("aling_flag").toString();
            a.align_flags = f.toInt();
            a.text = xml->readElementText();
            announcements.append(a);
            xml->readNext();
        }
    }
    announceWidget->loadFromFile(announcements);
}

void SoftProjector::readBibleHistoryFromSavedProject(QXmlStreamReader *xml)
{
    BibleSearch h;
    QList<BibleSearch> histories;
    QXmlStreamAttributes atrs;
    while(xml->tokenString() != "EndElement")
    {
        xml->readNext();
        if(xml->StartElement && xml->name() == "history")
        {
            atrs = xml->attributes();
            QString a = atrs.value("first").toString();
            h.first_v = a.toInt();
            a = atrs.value("last").toString();
            h.last_v = a.toInt();

            xml->readNext();
            while(xml->tokenString() != "EndElement")
            {
                xml->readNext();
                if(xml->StartElement && xml->name() == "ids")
                {
                    h.verse_id = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "book")
                {
                    h.book = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "chapter")
                {
                    h.chapter = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "display_txt")
                {
                    h.display_text = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "verse")
                {
                    h.verse = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "verse_txt")
                {
                    h.verse_text = xml->readElementText();
                    xml->readNext();
                }
            }
            histories.append(h);
            xml->readNext();
        }
    }
    bibleWidget->loadHistoriesFromFile(histories);
}

void SoftProjector::readBibleHistoryFromSavedProject1_0(QXmlStreamReader *xml)
{
    QStringList histories;
    while(xml->tokenString() != "EndElement")
    {
        xml->readNext();
        if(xml->StartElement && xml->name() == "verse")
        {
            histories.append(xml->readElementText());
            xml->readNext();
        }
    }
    bibleWidget->loadHistoriesFromFile1_0(histories);
}

void SoftProjector::readSongsFromSavedProject(QXmlStreamReader *xml)
{
    QList<Song> songs;
    Song s;
    QXmlStreamAttributes atrs;
    while(xml->tokenString() != "EndElement")
    {
        xml->readNext();
        if(xml->StartElement && xml->name() == "song")
        {
            atrs = xml->attributes();
            QString a = atrs.value("id").toString();
            s.songID = a.toInt();
            a = atrs.value("num").toString();
            s.num = a.toInt();
            a = atrs.value("category").toString();
            s.category = a.toInt();

            xml->readNext();
            while(xml->tokenString() != "EndElement")
            {
                xml->readNext();
                if(xml->StartElement && xml->name() == "title")
                {
                    s.title = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "text")
                {
                    s.songText = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "songbook")
                {
                    s.songbook_name = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "tune")
                {
                    s.tune = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "words")
                {
                    s.wordsBy = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "music")
                {
                    s.musicBy = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "font")
                {
                    s.font = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "aling")
                {
                    s.alingment = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "back")
                {
                    s.background = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "comments")
                {
                    s.notes = xml->readElementText();
                    xml->readNext();
                }
            }
            songs.append(s);
            xml->readNext();
        }
    }
    songWidget->loadPlaylistFromFile(songs);
}

void SoftProjector::updateWindowTest()
{
    QFileInfo fi(project_file_path);
    QString file = fi.fileName();

    if(!file.isEmpty())
    {
        file.remove(".spp");
        if(is_project_saved)
            this->setWindowTitle(file + " - softProjector " + version_string);
        else
            this->setWindowTitle(file + "* - softProjector " + version_string);
    }
    else
    {
        this->setWindowTitle("softProjector " + version_string);
    }
}

void SoftProjector::setProjectChanged(bool changed)
{
    if(changed)
        is_project_saved = false;
    else
        is_project_saved = true;

    updateWindowTest();
}

void SoftProjector::on_rbMultiVerse_toggled(bool checked)
{
    if(checked)
        ui->listShow->setSelectionMode(QAbstractItemView::ContiguousSelection);
    else
        ui->listShow->setSelectionMode(QAbstractItemView::SingleSelection);
}
