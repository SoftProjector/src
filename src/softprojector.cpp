#include <QDesktopWidget>
#include "softprojector.h"
#include "ui_softprojector.h"
#include "settingsdialog.h"
#include "aboutdialog.h"


SoftProjector::SoftProjector(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SoftProjectorClass)
{

    //Setting up the Display Screen
    QDesktopWidget *desktop;
    desktop = new QDesktopWidget();
    display = new Display1(desktop->screen(3));



    display->setGeometry(10, 10, 800, 600);
    display->setCursor(Qt::BlankCursor); //Sets a Blank Mouse to the screen
    bibleWidget = new BibleWidget;

    ui->setupUi(this);
    ui->statusBar->showMessage("This software is free and Open Source. If you can help in improving this program please visit www.softprojector.com");

    readXMLConfigurationFile();

    if (display_on_top)
        display->setWindowFlags(Qt::WindowStaysOnTopHint); // Always on top
//    display->setWindowFlags(Qt::ToolTip); // no
    if( desktop->numScreens() > 1 )
    {
        if (desktop->isVirtualDesktop())
        {
            display->setGeometry(desktop->screen(0)->width()-100,200,100,100);
        }
        display->showFullScreen();
    }
    else
    {
        display->show();
    }




    display->renderText();

    songWidget = new SongWidget;
    editWidget = new EditWidget;

    showing = false;

    ui->tabWidget->clear();
    ui->tabWidget->addTab(bibleWidget, "Bible");
    ui->tabWidget->addTab(songWidget, "Songs");

    editWidget->setWindowTitle("Edit and/or Add New songs");

    connect(songWidget, SIGNAL(sendSong(QStringList, QString, int)),
            this, SLOT(setSongList(QStringList, QString, int)));
    connect(bibleWidget, SIGNAL(goLive(QStringList, int, QString)),
            this, SLOT(setChapterList(QStringList, int, QString)));

    ui->show_button->setEnabled(false);
    ui->clear_button->setEnabled(false);

}

SoftProjector::~SoftProjector()
{
    delete ui;
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

    if( display->getShowBlack() )
        xml.writeTextElement("showblack", "true");
    else
        xml.writeTextElement("showblack", "false");

    xml.writeTextElement("primary_bible", bibleWidget->getPrimary());
    xml.writeTextElement("secondary_bible", bibleWidget->getSecondary());

    if (display_on_top)
        xml.writeTextElement("displayontop", "true");
    else
        xml.writeTextElement("displayontop", "false");

    xml.writeEndElement(); // settings
    xml.writeEndDocument();

    fh.close();
}

void SoftProjector::applySetting(QString name, QString value)
{
    // Apply the specified setting to the program

    if( name == "font" )
        display->setNewFont(value);
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

}

void SoftProjector::readXMLConfigurationFile()
{
    // Method for reading XML settings format

    QFile fh("settings.xml");
    if( ! fh.exists() )
    {
        // Apply default settings
        display->setNewWallpaper(QString());
        display->setShowBlack(true);
        bibleWidget->loadBibles(QString("bible_kjv"), QString("none"));
        display_on_top = false;
        writeXMLConfigurationFile();
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

void SoftProjector::on_actionClose_triggered()
{
    close();
}

void SoftProjector::setSongList(QStringList showList, QString caption, int row)
{
    // Display the specified song text in the right-most column of softProjector
    type = "song";
    ui->labelShow->setText(caption);
    ui->listShow->clear();
    ui->listShow->setSpacing(5);
    ui->listShow->setWordWrap(false);
    ui->listShow->addItems(showList);
    ui->listShow->setCurrentRow(row);
    ui->listShow->setFocus();
    updateScreen();
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
    ui->listShow->setCurrentRow(verse);
    ui->listShow->setFocus();
    showing = true;
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

    if( currentRow == -1 || showing == false )
    {
        // Do not display any text:
        display->setAllText(" ", " ");
        ui->show_button->setEnabled(true);
        ui->clear_button->setEnabled(false);
    }
    else
    {
        if(type=="song")
        {
            display->setAllText(ui->listShow->currentItem()->text(),"");
        }
        else if(type=="bible")
        {
            Verse verse = bibleWidget->bible.getCurrentVerseAndCaption(currentRow);
            QString text = verse.primary_text;
            QString caption = verse.primary_caption;
            if( !verse.secondary_text.isNull() )
            {
                text.append("\n");
                text += verse.secondary_text;
                caption.append("    ");
                caption += verse.secondary_caption;
            }
            // FIXME we need code here that will display the verse so that the
            // primary caption is right below the primary text, and secondary caption
            // is right bellow the secondary text
            display->setAllText(text, caption);
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

void SoftProjector::on_actionExportSongs_triggered()
{
    ExportDialog exportDialog;
    int ret = exportDialog.exec();
    switch (ret)
    {
    case ExportDialog::Accepted:
        // code needed to reload sborniks in SongWidget sbornik combobox
        break;
    case ExportDialog::Rejected:
        // No code needed
        break;
    }
}

void SoftProjector::on_actionImportSongs_triggered()
{
    ImportDialog importSborniks;
    importSborniks.load();
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
