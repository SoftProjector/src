#include <QDesktopWidget>
#include "softprojector.h"
#include "ui_softprojector.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "helpdialog.h"

SoftProjector::SoftProjector(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SoftProjectorClass)
{

    //Setting up the Display Screen
    desktop = new QDesktopWidget();
    display = new Display1(desktop->screen(3));
    display->setGeometry(10, 10, 800, 600);
    display->setCursor(Qt::BlankCursor); //Sets a Blank Mouse to the screen
    bibleWidget = new BibleWidget;

    ui->setupUi(this);
    ui->statusBar->showMessage("This software is free and Open Source. If you can help in improving this program please visit sourceforge.net/projects/softprojector/");


    // Apply default settings, in case configuration file does not exist
    // or is out of date:
    applyDefaults();

    // Will replace defaults if setting is saved:
    readXMLConfigurationFile();

    if (display_on_top)
        display->setWindowFlags(Qt::WindowStaysOnTopHint); // Always on top
//    display->setWindowFlags(Qt::ToolTip); // no
    if( desktop->numScreens() > 1 )
    {
        if (desktop->isVirtualDesktop())
            display->setGeometry(desktop->screen(0)->width()-100,200,100,100);
        display->showFullScreen();
    }
    else
        display->show();


    display->renderText(false);

    songWidget = new SongWidget;
    editWidget = new EditWidget;
    announceWidget = new AnnounceWidget;
    manageDialog = new ManageDataDialog(this);

    showing = false;

    ui->tabWidget->clear();
    ui->tabWidget->addTab(bibleWidget, "Bible (F6)");
    ui->tabWidget->addTab(songWidget, "Songs (F7)");
    ui->tabWidget->addTab(announceWidget, "Announcements (F8)");

    editWidget->setWindowTitle("Edit and/or Add New songs");

    connect(songWidget, SIGNAL(sendSong(Song, int)),
            this, SLOT(setSongList(Song, int)));
    connect(bibleWidget, SIGNAL(goLive(QStringList, int, QString)),
            this, SLOT(setChapterList(QStringList, int, QString)));
    connect(announceWidget,SIGNAL(sendText(QString)),
            this, SLOT(setAnnounceText(QString)));
    connect(display, SIGNAL(requestTextDrawing(QPainter*, int, int)),
            this, SLOT(drawText(QPainter*, int, int)));
    connect(editWidget, SIGNAL(updateSongFromDatabase(int)),
            songWidget, SLOT(updateSongFromDatabase(int)));
    connect(editWidget, SIGNAL(addedNew()),
            songWidget,SLOT(updateSborniks()));
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

    ui->show_button->setEnabled(false);
    ui->clear_button->setEnabled(false);

    version_string = "0.9 Alpha 06";
    this->setWindowTitle("softProjector " + version_string);

}

SoftProjector::~SoftProjector()
{
    delete ui;
    delete songWidget;
    delete editWidget;
    delete bibleWidget;
    delete announceWidget;
    delete display;
    delete desktop;
}

void SoftProjector::applyDefaults()
{
    // Apply configuration defaults - in case settings file is missing or out of date
    QFont new_font;
    new_font.fromString("Arial,27,-1,5,75,0,0,0,0,0");
    display->setNewFont(new_font);
    display->setNewWallpaper(QString());
    display->setShowBlack(true);

    // Get fist Bible in the Database
    QSqlQuery sq;
    sq.exec("SELECT id FROM BibleVersions");
    sq.first();
    bibleWidget->loadBibles(QString(sq.value(0).toString()), QString("none"));
    display_on_top = false;
    display->setFading(false);
    display->setBlur(true);
    show_stanza_title = false;
    show_song_number = false;
    show_song_key = false;
}


void SoftProjector::writeXMLConfigurationFile()
{
    // Method for writing the settings to XML format

    QFile fh("settings.xml");
    fh.open(QIODevice::WriteOnly);
    QXmlStreamWriter xml(&fh);
    xml.setAutoFormatting(true);
    xml.setCodec("UTF8");

    xml.writeStartDocument();
    xml.writeStartElement("settings");

    QString font_string = display->getFont().toString();
    xml.writeTextElement("font", font_string);

    QString wallpaper_path = display->getWallpaper();
    xml.writeTextElement("wallpaper", wallpaper_path);

    if(display->getShowBlack())
        xml.writeTextElement("showblack", "true");
    else
        xml.writeTextElement("showblack", "false");

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

    xml.writeEndElement(); // settings
    xml.writeEndDocument();

    fh.close();
}

void SoftProjector::applySetting(QString name, QString value)
{
    // Apply the specified setting to the program

    if( name == "font" )
    {
        QFont new_font;
        new_font.fromString(value);
        display->setNewFont(new_font);
    }
    else if( name == "wallpaper" )
        display->setNewWallpaper(value);
    else if( name == "showblack" )
        display->setShowBlack( value == "true" );
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

}

void SoftProjector::readXMLConfigurationFile()
{
    // Method for reading XML settings format

    QFile fh("settings.xml");
    if( ! fh.exists() )
    {
        // Defaults were already applied ealier
        return;
    }


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
    QCoreApplication::exit(0);
    event->accept();
}

void SoftProjector::keyPressEvent( QKeyEvent * event )
{
    // Will get called when a key is pressed
    int key = event->key();
    if( key == Qt::Key_F6 )
        ui->tabWidget->setCurrentWidget(bibleWidget);
    else if( key == Qt::Key_F7 )
        ui->tabWidget->setCurrentWidget(songWidget);
    else if( key == Qt::Key_F8 )
        ui->tabWidget->setCurrentWidget(announceWidget);
    else
        QMainWindow::keyPressEvent(event);
}

void SoftProjector::on_actionClose_triggered()
{
    close();
}

void SoftProjector::setAnnounceText(QString text)
{
    announce_text = text;
    type = "announce";
    showing = true;
    new_list = true;
    ui->labelShow->setText("Announcement");
    ui->listShow->clear();
    ui->listShow->setSpacing(5); // ?
    ui->listShow->setWordWrap(true);
    ui->listShow->addItem(text);
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
       main_flags = Qt::AlignHCenter | Qt::AlignVCenter;
    else
       main_flags = Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap;

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
    QString sbornik_str = current_song.sbornik_name;
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


    if( textemp.startsWith(QString::fromUtf8("Вставка"))
        || textemp.startsWith(QString::fromUtf8("Куплет"))
        || textemp.startsWith(QString::fromUtf8("Припев")) )
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
    for (int i = 0; i < lines_list.size(); ++i)
    {
        if( i > 0 )
            main_text += "\n";
        main_text += lines_list.at(i);
    }

    if( last_verse )
        main_text += "\n*   *   *";

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
        out_rect = drawSongTextToRect(painter, rect, false, false, main_text, top_caption_str, song_num_str);
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
            out_rect = drawSongTextToRect(painter, rect, false, true, main_text, top_caption_str, song_num_str);
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
    drawSongTextToRect(painter, rect, true, wrap, main_text, top_caption_str, song_num_str);
}



void SoftProjector::drawCurrentBibleText(QPainter *painter, int width, int height)
{
    // Margins:
    int left = 30;
    int top = 20;
    int w = width - left - left;
    int h = height - top - top;
    QFont font = painter->font();
    Verse v = current_verse; // for convenience
    QRect rect;
    int flags;
    int original_font_size = font.pointSize();
    int used_font_size;

    if( !v.secondary_text.isNull() )
    {
        // Two bible versions are selected

        int caption_height = 80;
        h -= caption_height;
        int middle = (height / 2);

        // Draw verse text:
        flags = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap;
        h = middle-top-(caption_height/3);

        rect = QRect(left, top, w, h);
        display->paintTextToRect(painter, rect, flags, v.primary_text);

        rect = QRect(left, middle, w, h);
        used_font_size = display->paintTextToRect(painter, rect, flags, v.secondary_text);

        // Draw citations:
        h = middle-top-caption_height;
        if( used_font_size < original_font_size*3/4 )
            // Shriked down the bible verse too much
            font.setPointSize(used_font_size);
        else
            font.setPointSize(original_font_size*3/4);
        painter->setFont(font);

        flags = Qt::AlignHCenter | Qt::AlignVCenter;

        rect = QRect(width/2, middle-caption_height, width/2, caption_height);
        display->paintTextToRect(painter, rect, flags, v.primary_caption);

        rect = QRect(width/2, middle+h, width/2, caption_height);
        display->paintTextToRect(painter, rect, flags, v.secondary_caption);

    }
    else
    {
        // Only one bible used

        int caption_height = 80;
        h -= caption_height;
        int caption_top = top + h;

        // Draw verse text:
        flags = Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap;
        rect = QRect(left, top, w, h);
        used_font_size = display->paintTextToRect(painter, rect, flags, v.primary_text);

        // Draw citation:
        if( used_font_size < original_font_size*3/4 )
            // Shriked down the bible verse too much
            font.setPointSize(used_font_size);
        else
            font.setPointSize(original_font_size*3/4);
        painter->setFont(font);

        flags = Qt::AlignHCenter | Qt::AlignVCenter;
        rect = QRect(width/2, caption_top, width/2, caption_height);
        display->paintTextToRect(painter, rect, flags, v.primary_caption);
    }
}




void SoftProjector::drawAnnounceText(QPainter *painter, int width, int height)
{
    // Margins:
    int left = 30;
    int top = 20;
    int w = width - left - left;
    int h = height - top - top;

    int flags = Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap;
    QRect rect = QRect(left, top, w, h);
    display->paintTextToRect(painter, rect, flags, announce_text);
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

void SoftProjector::setChapterList(QStringList chapter_list, int verse, QString caption)
{
    // Called to show a bible verse from a chapter in the preview list

    type = "bible";
    showing = true;
    new_list = true;
    ui->labelShow->setText(caption);
    ui->listShow->clear();
    ui->listShow->setSpacing(0);
    ui->listShow->setWordWrap(true);
    ui->listShow->addItems(chapter_list);
    ui->listShow->setCurrentRow(verse);
    ui->listShow->setFocus();
    new_list = false;
    updateScreen();
}

void SoftProjector::on_listShow_currentRowChanged(int currentRow)
{
    // Called when the user selects a different row in the show (right-most) list.
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
        if(type=="song")
            current_song_verse = currentRow;
        else if(type=="bible")
            current_verse = bibleWidget->bible.getCurrentVerseAndCaption(currentRow);
        else if( type == "announce" )
            announce_text = announceWidget->getText();
        display->renderText(true);
        ui->show_button->setEnabled(false);
        ui->clear_button->setEnabled(true);
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
    if (ui->tabWidget->currentIndex()==1){
        editWidget->setEdit(songWidget->getSongToEdit());
        editWidget->show();
        editWidget->activateWindow();
    }
}

void SoftProjector::on_actionNewSong_triggered()
{
    if (ui->tabWidget->currentIndex()==1){
        editWidget->show();
        editWidget->setNew();
        editWidget->activateWindow();

    }
}

void SoftProjector::on_actionDeleteSong_triggered()
{
    QMessageBox ms;
    ms.setWindowTitle("Delete Song?");
    ms.setText("Are you sure that you want to delete a song?");
    ms.setInformativeText("This action will permanentrly delete this song");
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
    aboutDialog->setWindowTitle("About softProjecor");
    aboutDialog->exec();
}

void SoftProjector::on_tabWidget_currentChanged(int index)
{
    if (index == 1)
    {
        // Songs tab is activated
        ui->actionEditSong->setEnabled(true);
        ui->actionNewSong->setEnabled(true);
        ui->actionDeleteSong->setEnabled(true);
    }
    else
    {
        // Other tabs are activated
        ui->actionEditSong->setEnabled(false);
        ui->actionNewSong->setEnabled(false);
        ui->actionDeleteSong->setEnabled(false);
    }
}

void SoftProjector::on_actionManage_Database_triggered()
{
    QString primaryBible = bibleWidget->getPrimary();
    QString secondaryBible = bibleWidget->getSecondary();
    QSqlQuery sq;



    manageDialog->exec();

    // Reload sborniks if Sbornik has been added, edited, or deleted
    if (manageDialog->reload_sbornik)
        songWidget->updateSborniks();

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

