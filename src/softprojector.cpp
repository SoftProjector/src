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

/*
void drawText34(QPainter *painter, QFont font, int flags, QString text)
{
    // Draw the text to the specified Painter at 3/4 of the size of the rest of the text.
    QFont font = painter->font();
    int backup_size = font.pointSize();
    font.setPointSize(backup_size*3/4);
    painter->drawText(crect, cflags, v.primary_caption);
    font.setPointSize(backup_size);
}
*/

SoftProjector::SoftProjector(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SoftProjectorClass)
{

    //Setting up the Display Screen
    desktop = new QDesktopWidget();

    // NOTE: With virtual desktop, desktop->screen() will always return the main screen,
    // so this will initialize the Display1 widget on the main screen:
    display = new Display1(desktop->screen(0));
    // Don't worry, we'll move it later

    display->setCursor(Qt::BlankCursor); //Sets a Blank Mouse to the screen

    bibleWidget = new BibleWidget;
    songWidget = new SongWidget;
    editWidget = new EditWidget;
    announceWidget = new AnnounceWidget;
    manageDialog = new ManageDataDialog(this);

    ui->setupUi(this);

    // Create action group for language slections
    languageGroup = new QActionGroup(this);
    languageGroup->addAction(ui->actionRussian);
    languageGroup->addAction(ui->actionEnglish);
    languageGroup->addAction(ui->actionGerman);
    languageGroup->addAction(ui->actionCzech);

    // Always place the "Settings" menu item under the application
    // menu, even if the item is translated (Mac OS X):
    ui->actionSettings->setMenuRole(QAction::PreferencesRole);
    // FIXME Make the Preferences menu appear in the menu bar even for the
    // display window (Mac OS X)

    // Apply default settings, in case configuration file does not exist
    // or is out of date:
    applyDefaults();

    // Will replace defaults if setting is saved:
    readXMLConfigurationFile();

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

//    version_string = "1.06"; // to be used only for official release
    version_string = "1.06_Alpha"; // to be used between official releases
    this->setWindowTitle("softProjector " + version_string);

    // Initialize bool variables
    setSongEditStatus(false);
}

SoftProjector::~SoftProjector()
{
    writeXMLConfigurationFile();
    delete ui;
    delete songWidget;
    delete editWidget;
    delete bibleWidget;
    delete announceWidget;
    delete display;
    delete desktop;
}

void SoftProjector::positionDisplayWindow()
{
    // Position the display window as needed (including setting "always on top" flag,
    // showing full screen / normal mode, and positioning it on the right screen)

    if (display_on_top)
        display->setWindowFlags(Qt::WindowStaysOnTopHint);
    else
        display->setWindowFlags(0); // Do not show always on top

    if( desktop->numScreens() > 1 )
    {
        if (desktop->isVirtualDesktop())
        {
            // Move the display widget to screen 1 (secondary screen):
            // FIXME what if more than 2 screens are available??
            QPoint top_left = desktop->screenGeometry(1).topLeft();
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

void SoftProjector::applyDefaults()
{
    // Apply configuration defaults - in case settings file is missing or out of date
    QFont new_font;
    new_font.fromString("Arial,27,-1,5,75,0,0,0,0,0");
    display->setNewFont(new_font);
    display->setNewWallpaper(QString());
    display->setNewPassiveWallpaper(QString());

    // Get fist Bible in the Database
    QSqlQuery sq;
    sq.exec("SELECT id FROM BibleVersions");
    sq.first();

    bibleWidget->loadBibles(QString(sq.value(0).toString()), QString("none"));
    //bibleWidget->loadBibles(value, bibleWidget->bible.secondaryId);
    display_on_top = false;
    display->setFading(false);
    display->setBlur(false);
    display->setForegroundColor( QColor("WHITE") );
    show_stanza_title = false;
    show_song_number = false;
    show_song_key = false;
    ui->actionEnglish->setChecked(true);
}

void SoftProjector::writeXMLConfigurationFile()
{
    // Method for writing the settings to XML format

    QString settings_file = QApplication::instance()->applicationDirPath() + QDir::separator() + "settings.xml";
    QFile fh(settings_file);
    fh.open(QIODevice::WriteOnly);
    QXmlStreamWriter xml(&fh);
    xml.setAutoFormatting(true);
    xml.setCodec("UTF8");

    xml.writeStartDocument();
    xml.writeStartElement("settings");

    QString font_string = display->getFont().toString();
    xml.writeTextElement("font", font_string);

    unsigned int foreground_color_int = (unsigned int)(display->getForegroundColor().rgb());
    xml.writeTextElement("foreground_color", QString::number(foreground_color_int) );

    QString wallpaper_path = display->getWallpaper();
    QString passive_wallpaper_path = display->getPassiveWallpaper();

    xml.writeTextElement("wallpaper", wallpaper_path);
    xml.writeTextElement("passive_wallpaper", passive_wallpaper_path);

    xml.writeTextElement("primary_bible", bibleWidget->getPrimary());
    xml.writeTextElement("secondary_bible", bibleWidget->getSecondary());

    if (display_on_top)
        xml.writeTextElement("displayontop", "true");
    else
        xml.writeTextElement("displayontop", "false");

    if (display->useFading())
        xml.writeTextElement("usefading", "true");
    else
        xml.writeTextElement("usefading", "false");

    if (display->useBlur())
        xml.writeTextElement("useblur", "true");
    else
        xml.writeTextElement("useblur", "false");

    // Save song settings
    if (show_stanza_title)
        xml.writeTextElement("showstanzatitle", "true");
    else
        xml.writeTextElement("showstanzatitle", "false");

    if (show_song_number)
        xml.writeTextElement("showsongnumber", "true");
    else
        xml.writeTextElement("showsongnumber", "false");

    if (show_song_key)
        xml.writeTextElement("showsongkey", "true");
    else
        xml.writeTextElement("showsongkey", "false");

    // save softProjectors window settings
    if (this->isMaximized())
        xml.writeTextElement("maximizedscreen", "true");
    else
        xml.writeTextElement("maximizedscreen", "false");

    // save splitter states
    xml.writeTextElement("softprojectorsplitterstate", ui->splitter->saveState().toHex());
    xml.writeTextElement("biblehiddensplitterstate", bibleWidget->getHiddenSplitterState().toHex());
    xml.writeTextElement("bibleshownsplitterstate", bibleWidget->getShownSplitterState().toHex());
    xml.writeTextElement("songsplitterstate", songWidget->getSplitterState().toHex());

    // save translation settings
    if (ui->actionRussian->isChecked())
        xml.writeTextElement("translate", "ru");
    else if (ui->actionEnglish->isChecked())
        xml.writeTextElement("translate", "en");
    else if (ui->actionGerman->isChecked())
        xml.writeTextElement("translate", "de");
    else if (ui->actionCzech->isChecked())
        xml.writeTextElement("translate", "ua");

    xml.writeEndElement(); // settings
    xml.writeEndDocument();

    fh.close();
}

void SoftProjector::applySetting(QString name, QString value)
{
    // Apply the specified setting to the program

    QByteArray b;
    if( name == "font" )
    {
        QFont new_font;
        new_font.fromString(value);
        display->setNewFont(new_font);
    }
    else if( name == "foreground_color" )
        display->setForegroundColor( QColor::fromRgb(value.toUInt()));
    else if( name == "wallpaper" )
        display->setNewWallpaper(value);
    else if( name == "passive_wallpaper" )
        display->setNewPassiveWallpaper(value);
    else if( name == "primary_bible" )
        bibleWidget->loadBibles(value, bibleWidget->bible.secondaryId);
    else if( name == "secondary_bible" )
        bibleWidget->loadBibles(bibleWidget->bible.primaryId, value);
    else if( name == "displayontop" )
        display_on_top = (value == "true");
    else if( name == "usefading")
        display->setFading(value == "true");
    else if( name == "useblur")
        display->setBlur(value == "true");
    else if(name == "showstanzatitle")
        show_stanza_title = (value == "true");
    else if(name == "showsongnumber")
        show_song_number = (value == "true");
    else if(name == "showsongkey")
        show_song_key = (value == "true");
    else if(name == "maximizedscreen")
    {
        if(value == "true")
            this->setWindowState(Qt::WindowMaximized);
    }
    else if(name == "softprojectorsplitterstate")
    {
        QByteArray b;
        b.insert(0,value);
        b = b.fromHex(b);
        ui->splitter->restoreState(b);
    }
    // If this code is run, then if the GUI is closed when the results section is visible,
    // then when it is opened, the top bible splitter will take up a lot of space
    else if(name == "biblehiddensplitterstate")
        bibleWidget->setHiddenSplitterState(value);
    else if(name == "bibleshownsplitterstate")
        bibleWidget->setShownSplitterState(value);
    else if(name == "songsplitterstate")
        songWidget->setSplitterState(value);
    else if(name == "translate")
    {
        if(value == "en")
            ui->actionEnglish->setChecked(true);
        else if(value == "ru")
            ui->actionRussian->setChecked(true);
        else if(value == "de")
            ui->actionGerman->setChecked(true);
        else if(value == "ua")
            ui->actionCzech->setChecked(true);

        retranslateUis();
    }

}

void SoftProjector::readXMLConfigurationFile()
{
    // Method for reading XML settings format

    QString settings_file = QApplication::instance()->applicationDirPath() + QDir::separator() + "settings.xml";
    if( ! QFile::exists(settings_file) )
    {
        // Defaults were already applied ealier
        return;
    }
    QFile fh(settings_file);

    fh.open(QIODevice::ReadOnly);
    QXmlStreamReader xml(&fh);
    QXmlStreamReader::TokenType token;

    while( !xml.atEnd() )
    {
        token = xml.readNext();
        if( token == QXmlStreamReader::StartElement && xml.name() == "settings" )
            while( token != QXmlStreamReader::EndElement ) // While in <settings>
            {
                token = xml.readNext();
                if( token == QXmlStreamReader::StartElement )
                    applySetting( xml.name().toString(), xml.readElementText() );
            }

    }
    if( xml.hasError() )
    {
        // FIXME Show a friendly error dialog
        qDebug() << "ERROR reading file: settings.xml";
        //print "ERROR reading file: settings.xml"
        //print "ERROR TYPE:", xml.errorString()
    }
    fh.close();
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

QRect SoftProjector::drawSongTextToRect(QPainter *painter, QRect bound_rect, bool draw, bool wrap, QString main_text, QString caption_str, QString song_num_str)
{
    // Figure out how to draw the specified song verse to the specified rect.
    // Return the rectangle of the output - may be bigger than input rect.
    // draw - whether to actually draw or to simply calculate the rect.

    QRect caption_rect, num_rect, main_rect, out_rect;
    int left = bound_rect.left();
    int top = bound_rect.top();
    int width = bound_rect.width();
    int height = bound_rect.height();
    int main_flags;

    if(wrap)
        main_flags = Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap;
    else
        main_flags = Qt::AlignHCenter | Qt::AlignVCenter;

    QFont orig_font = painter->font();
    QFont caption_font = orig_font;
    caption_font.setPointSize( orig_font.pointSize() *4/5 );

    painter->setFont(caption_font);
    caption_rect = boundRectOrDrawText(painter, draw, left, top, width, height, Qt::AlignLeft | Qt::AlignTop, caption_str);
    num_rect = boundRectOrDrawText(painter, draw, left, top, width, height, Qt::AlignRight | Qt::AlignTop, song_num_str);

    int cheight = caption_rect.height(); // Height of the caption text
    painter->setFont(orig_font);
    main_rect = boundRectOrDrawText(painter, draw, left, top+cheight, width, height-cheight, main_flags, main_text);

    top = top;
    left = main_rect.left();
    height = main_rect.bottom() - top;
    width = main_rect.right() - left;
    //qDebug() << "OUT top, left, height, width:" << top << left << height << width;
    out_rect.setRect(left, top, width, height);
    return out_rect;
}

void SoftProjector::drawCurrentSongText(QPainter *painter, int width, int height)
{
    // Draw the text of the current song verse to the specified painter; making
    // sure that the output rect is narrower than <width> and shorter than <height>.
    SongDatabase song_database;
    QStringList song_list = current_song.getSongTextList();

    QString main_text;
    QString top_caption_str;

    bool last_verse = ( current_song_verse == song_list.count()-1 );

    QStringList lines_list = song_list.at(current_song_verse).split("\n");
    QString songbook_str = current_song.songbook_name;
    QString song_num_str = QString::number(current_song.num);
    QString song_key_str = current_song.tune;

    // Remove the first line if it starts with "Kuplet" or "Pripev" or "Vstavka":
    QString textemp = lines_list[0];
//    textemp.remove(6,10);

    // Check whether to display song numbers
    if (show_song_number)
        song_num_str = song_num_str;
    else
        song_num_str = "";

    // Check whether to display song key
    if (show_song_key)
        song_num_str = song_key_str + "  " + song_num_str;
    else
        song_num_str = song_num_str;

    // Remove Stanza Title from the stanza and send it to top caption
    if(isStanzaTitle(textemp))
    {
        if (show_stanza_title)
            top_caption_str = lines_list.at(0);
        else
            top_caption_str = "";
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

    if( last_verse )
    {
        // If the last vers is very large, do not put extra blank lines:
        // FIXME use better logic here. Instead of number of lines, analyze the
        // height vs width ratio.
        if( num_lines > 8 )
            main_text += "\n*   *   *";
        else
            // The extra blank lines make the display cleaner:
            main_text = "\n" + main_text + "\n\n*   *   *";
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
        //qDebug() << "Trying to draw song using font " << font.pointSize();
        out_rect = drawSongTextToRect(painter, rect, false, false, main_text, top_caption_str, song_num_str);
        exit = ( out_rect.width() <= w && out_rect.height() <= h );
        if( !exit )
        {
            //qDebug() << "  not fitting, decreasing font size";
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
        //qDebug() << "Font size is less than 4/5 of the original; attempting to wrap";
        font.setPointSize(orig_font_size);
        painter->setFont(font);
        exit = false;
        wrap = true;
        while( !exit )
        {
            //qDebug() << "WRAP: Trying to draw song using font " << font.pointSize();
            out_rect = drawSongTextToRect(painter, rect, false, true, main_text, top_caption_str, song_num_str);
            exit = ( out_rect.width() <= w && out_rect.height() <= h );
            if( !exit )
            {
                //qDebug() << "  WRAP: not fitting, decreasing font size";
                // Decrease font size by one point and try again
                font.setPointSize( font.pointSize()-1 );
                painter->setFont(font);
            }
        }
    }
    //qDebug() << "Drawing with wrap set to " << wrap;
    // At this point we picked correct font size and flags; so it's safe to draw:
    drawSongTextToRect(painter, rect, true, wrap, main_text, top_caption_str, song_num_str);
}

void SoftProjector::drawCurrentBibleText(QPainter *painter, int width, int height)
{
    // Margins:
       int left = 30;
       int top = 20;
       int bottom = height - top;
       //int right = width - left;
       int w = width - left - left;
       int h = height - top - top;

       QFont font = painter->font();
       int original_size = font.pointSize();
       int current_size = original_size;
       Verse v = current_verse; // for convenience


       // Keep decreasing the font size until the text fits into the allocated space:

       // Rects for storing the position of the text and caption drawing:
       QRect trect1, crect1, trect2, crect2;
       // Flags to be used for drawing verse text and caption:
       int tflags = Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap;
       int cflags = Qt::AlignRight | Qt::AlignTop;

       bool exit = false;
       while( !exit )
       {
           if( !v.secondary_text.isNull() )
           {
               // Secondary bible present:

               // Figure out how much space the drawing will take at the current font size:
               // Determine rect of primary verse text:
               font.setPointSize(current_size);
               painter->setFont(font);
               trect1 = painter->boundingRect(left, top, w, 10, tflags, v.primary_text);
               QStringList lines = v.primary_text.split("\n");
               //qDebug() << "Num primary lines:" << lines.count();

               // Determine rect of primary caption:
               font.setPointSize(current_size*3/4);
               painter->setFont(font);
               crect1 = painter->boundingRect(left, trect1.bottom(), w, 10, cflags, v.primary_caption);

               // Calculate the top of the secondary verse text:
               int v2_bottom = crect1.bottom();
               if( v2_bottom < height/2 )
                   v2_bottom = height/2;

               // Determine rect of secondary verse text:
               font.setPointSize(current_size);
               painter->setFont(font);
               trect2 = painter->boundingRect(left, v2_bottom, w, 10, tflags, v.secondary_text);

               // Determine rect of secondary caption:
               font.setPointSize(current_size*3/4);
               painter->setFont(font);
               crect2 = painter->boundingRect(left, trect2.bottom(), w, 10, cflags, v.secondary_caption);

               exit = ( crect2.bottom() <= bottom );
           }
           else
           {
               // No secondary bible:
               // Figure out how much space the drawing will take at the current font size:
               font.setPointSize(current_size);
               painter->setFont(font);
               trect1 = painter->boundingRect(left, top, w, h, tflags, v.primary_text);

               font.setPointSize(current_size*3/4);
               painter->setFont(font);
               crect1 = painter->boundingRect(left, trect1.bottom(), w, bottom-trect1.bottom(), cflags, v.primary_caption);

               exit = ( crect1.bottom() <= bottom );
           }

           if( !exit )
           {
               // The current font is too large, decrease and try again:
               --current_size;
           }
       }


       // Draw the bible text verse(s) at the final size:
       font.setPointSize(current_size);
       painter->setFont(font);
       painter->drawText(trect1, tflags, v.primary_text);
       if( !v.secondary_text.isNull() )
           painter->drawText(trect2, tflags, v.secondary_text);

       // Draw the verse caption(s) at the final size:
       font.setPointSize(current_size*3/4);
       painter->setFont(font);
       painter->drawText(crect1, cflags, v.primary_caption);
       if( !v.secondary_text.isNull() )
           painter->drawText(crect2, cflags, v.secondary_caption);

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

    if(selectedItems.count()>1)
        ui->rbMultiVerse->setChecked(true);
    else
        ui->rbMultiVerse->setChecked(false);
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
            current_song_verse = currentRow;
        else if(type=="bible")
        {
            int srows(ui->listShow->count());
            QList<int> currentRows;
            for(int i(0); i<srows; ++i)
            {
                if(ui->listShow->item(i)->isSelected())
                    currentRows.append(i);
            }
            current_verse = bibleWidget->bible.getCurrentVerseAndCaption(currentRows);
        }
        else if( type == "announce" )
            announcement_text.text = announceWidget->getText();
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
    QString primaryBible = bibleWidget->getPrimary();
    QString secondaryBible = bibleWidget->getSecondary();
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
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + primaryBible);
        if (!sq.first())
        {
            // If original primary bible has been removed, set first bible in the list to be primary
            sq.clear();
            sq.exec("SELECT id FROM BibleVersions");
            sq.first();
            primaryBible = sq.value(0).toString();
        }
        sq.clear();

        // check if secondary bible has been removed, if yes, set secondary to "none"
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + secondaryBible);
        if (!sq.first())
            secondaryBible = "none";
        bibleWidget->loadBibles(primaryBible, secondaryBible);
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

void SoftProjector::on_actionRussian_triggered()
{
    retranslateUis();
}

void SoftProjector::on_actionEnglish_triggered()
{
    retranslateUis();
}

void SoftProjector::on_actionGerman_triggered()
{
    retranslateUis();
}

void SoftProjector::on_actionCzech_triggered()
{
    retranslateUis();
}

void SoftProjector::retranslateUis()
{
    if(ui->actionEnglish->isChecked()) // Set English language
    {
        qApp->removeTranslator(&translator);
    }
    else if(ui->actionRussian->isChecked()) // Set Russian language
    {
        translator.load(":/language/softpro_ru");
        qApp->installTranslator(&translator);
    }
    else if(ui->actionGerman->isChecked()) // Set German language
    {
        translator.load(":/language/softpro_de");
        qApp->installTranslator(&translator);
    }
    else if(ui->actionCzech->isChecked()) // Set Czech language
    {
        translator.load(":/language/softpro_cs");
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
    songCounter = new SongCounter(this);
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
        project_file_path = path;
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
    xml.writeAttribute("version","1.0");

    //Write Bible History
    xml.writeStartElement("BibleHistory");
    for(int i(0); i<histories.count(); ++i)
    {
        h = histories.at(i);
        xml.writeStartElement("verse");
        xml.writeCharacters(h.verse_id);
        xml.writeEndElement();
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
            if(xml.attributes().value("version").toString() == "1.0")
            {
                xml.readNext();
                while(xml.tokenString() != "EndElement" && xml.name() != "spProject")
                {
                    xml.readNext();
                    if(xml.StartElement && xml.name() == "BibleHistory")
                    {
                        // Read saved bible history
                        readBibleHistoryFromSavedProject(&xml);
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
    QStringList histories;
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
    bibleWidget->loadHistoriesFromFile(histories);
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
