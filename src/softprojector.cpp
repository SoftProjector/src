#include <QDesktopWidget>
#include "softprojector.h"
#include "ui_softprojector.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "managedatadialog.h"


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

    showing = false;

    ui->tabWidget->clear();
    ui->tabWidget->addTab(bibleWidget, "Bible (F6)");
    ui->tabWidget->addTab(songWidget, "Songs (F7)");

    editWidget->setWindowTitle("Edit and/or Add New songs");

    connect(songWidget, SIGNAL(sendSong(Song, int)),
            this, SLOT(setSongList(Song, int)));
    connect(bibleWidget, SIGNAL(goLive(QStringList, int, QString)),
            this, SLOT(setChapterList(QStringList, int, QString)));
    connect(display, SIGNAL(requestTextDrawing(QPainter*, int, int)),
            this, SLOT(drawText(QPainter*, int, int)));

    ui->show_button->setEnabled(false);
    ui->clear_button->setEnabled(false);

}

SoftProjector::~SoftProjector()
{
    delete ui;
    delete songWidget;
    delete editWidget;
    delete bibleWidget;
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
    else
        QMainWindow::keyPressEvent(event);
}





void SoftProjector::on_actionClose_triggered()
{
    close();
}

void SoftProjector::setSongList(Song song, int row)
{
    QStringList song_list = song.getSongTextList();
    current_song = song;
    current_song_verse = row;

    // Display the specified song text in the right-most column of softProjector
    type = "song";
    ui->labelShow->setText(song.title);
    ui->listShow->clear();
    ui->listShow->setSpacing(5);
    ui->listShow->setWordWrap(false);
    ui->listShow->addItems(song_list);
    showing = true;
    ui->listShow->setCurrentRow(row);
    ui->listShow->setFocus();
//    updateScreen();
}


QRect SoftProjector::drawSongTextToRect(QPainter *painter, QRect bound_rect, bool draw, bool wrap, QString main_text, QString caption_str, QString song_num_str)
{
    QRect caption_rect, num_rect, main_rect, out_rect;
    int left = bound_rect.left();
    int top = bound_rect.top();
    int bottom = bound_rect.bottom();
    int right = bound_rect.right();
    int width = bound_rect.width();
    int height = bound_rect.height();
    int main_flags;

    if(wrap)
       main_flags = Qt::AlignHCenter | Qt::AlignVCenter;
    else
       main_flags = Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap;

    if(draw)
    {
        painter->drawText(left, top, width, height, Qt::AlignLeft | Qt::AlignTop, caption_str, &caption_rect);
        painter->drawText(left, top, width, height, Qt::AlignRight | Qt::AlignTop, song_num_str, &num_rect);

        int cheight = caption_rect.height(); // Height of the caption text
        painter->drawText(left, top+cheight, width, height-cheight, main_flags, main_text, &main_rect);
    }
    else
    {  
        caption_rect = painter->boundingRect(left, top, width, height, Qt::AlignLeft | Qt::AlignTop, caption_str);
        num_rect = painter->boundingRect(left, top, width, height, Qt::AlignRight | Qt::AlignTop, song_num_str);

        int cheight = caption_rect.height(); // Height of the caption text
        main_rect = painter->boundingRect(left, top+cheight, width, height-cheight, main_flags, main_text);
    }

    top = top;
    left = main_rect.left();
    height = main_rect.bottom() - top;
    width = main_rect.right() - left;
    qDebug() << "OUT top, left, height, width:" << top << left << height << width;
    out_rect.setRect(left, top, width, height);
    return out_rect;
}


void SoftProjector::drawCurrentSongText(QPainter *painter, int width, int height)
{
    SongDatabase song_database;
    QStringList song_list = current_song.getSongTextList();

    QString main_text;
    QString top_caption_str;

    bool last_verse = ( current_song_verse == song_list.count()-1 );

    QStringList lines_list = song_list.at(current_song_verse).split("\n");

    // Remove the first line if it starts with "Kuplet" or "Pripev":
    QString textemp = lines_list[0];
    textemp.remove(6,10);

    if( textemp.startsWith(QString::fromUtf8("Припев")) || textemp.startsWith(QString::fromUtf8("Куплет")) )
    {
        top_caption_str = lines_list.at(0);
        qDebug() << "CAPTION:";
        qDebug() << top_caption_str;
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
    QString song_num_str = QString::number(current_song.num);

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
        int caption_top = top + h;
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


void SoftProjector::drawText(QPainter *painter, int width, int height)
{
    if( !showing )
        return;

    if(type=="song")
        drawCurrentSongText(painter, width, height);
    else if(type=="bible")
        drawCurrentBibleText(painter, width, height);
}

void SoftProjector::setChapterList(QStringList chapter_list, int verse, QString caption)
{
    // Called to show a bible verse from a chapter in the preview list

    type = "bible";
    ui->labelShow->setText(caption);
    ui->listShow->clear();
    ui->listShow->setSpacing(0);
    ui->listShow->setWordWrap(true);
    ui->listShow->addItems(chapter_list);
    showing = true;
    ui->listShow->setCurrentRow(verse);
    ui->listShow->setFocus();
//    updateScreen();
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

    if( currentRow == -1 || showing == false )
    {
        // Do not display any text:
        display->renderText(false);
        ui->show_button->setEnabled(true);
        ui->clear_button->setEnabled(false);
    }
    else
    {
        if(type=="song")
        {
//            Song song = songWidget->currentSong();
//            int row = ui->listShow->currentRow();
//            current_song = song;
            current_song_verse = currentRow;
            display->renderText(true);
        }
        else if(type=="bible")
        {
            current_verse = bibleWidget->bible.getCurrentVerseAndCaption(currentRow);
            display->renderText(true);
        }
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

void SoftProjector::on_actionEditDialog_triggered()
{
    editWidget->show();
    editWidget->activateWindow();
}

//void SoftProjector::on_actionExportSongs_triggered()
//{
//    ExportDialog exportDialog;
//    int ret = exportDialog.exec();
//    switch (ret)
//    {
//    case ExportDialog::Accepted:
//        // code needed to reload sborniks in SongWidget sbornik combobox
//        break;
//    case ExportDialog::Rejected:
//        // No code needed
//        break;
//    }
//}
//
//void SoftProjector::on_actionImportSongs_triggered()
//{
//    ImportDialog importSborniks;
//    importSborniks.load();
//}
//
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
    aboutDialog = new AboutDialog(this);
    aboutDialog->setWindowTitle("About softProjecor");
    aboutDialog->exec();
}

void SoftProjector::on_tabWidget_currentChanged(int index)
{
    if (index ==0)
    {
        // Bible tab activated
        ui->actionEditSong->setEnabled(false);
        ui->actionNewSong->setEnabled(false);
    }
    else if (index==1)
    {
        // Songs tab activated
        ui->actionEditSong->setEnabled(true);
        ui->actionNewSong->setEnabled(true);
    }
}

void SoftProjector::on_actionManage_Database_triggered()
{
    QString primaryBible = bibleWidget->getPrimary();
    QString secondaryBible = bibleWidget->getSecondary();
    QSqlQuery sq;

    ManageDataDialog *manage;
    manage = new ManageDataDialog(this);
    manage->exec();

    // Reload sborniks if Sbornik has been added, edited, or deleted
    if (manage->reload_sbornik)
        songWidget->updateSborniks();

    // Reload Bibles if Bile has been deleted
    if (manage->reload_bible)
    {
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + primaryBible);
        if (!sq.first())
        {
            sq.clear();
            sq.exec("SELECT id FROM BibleVersions");
            sq.first();
            primaryBible = sq.value(0).toString();
        }
        sq.clear();
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + secondaryBible);
        if (!sq.first())
            secondaryBible = "none";
        bibleWidget->loadBibles(primaryBible, secondaryBible);
    }
}
