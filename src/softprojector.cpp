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
//    display->setWindowFlags(Qt::WindowStaysOnTopHint); // Always on top
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


    ui->setupUi(this);
    ui->statusBar->showMessage("This software is free and Open Source. If you can help in improving this program please visit www.softprojector.com");

    bibleWidget = new BibleWidget;
    // Will modify display's font and wallpaper:
    readConfigurationFile();
    display->RenderText();

    songWidget = new SongWidget;
    editWidget = new EditWidget;
    importSongs = new ImportDialog;
    exportSongs = new ExportDialog;

    showing = false;

    ui->tabWidget->clear();
    ui->tabWidget->addTab(bibleWidget, "Bible");
    ui->tabWidget->addTab(songWidget, "Songs");

    editWidget->setWindowTitle("Edit and/or Add New songs");

    connect(songWidget, SIGNAL(sendSong(QStringList, QString, int)),
            this, SLOT(setSongList(QStringList, QString, int)));
    connect(bibleWidget, SIGNAL(goLive(Bible,QString,int)),
            this, SLOT(setBibleList(Bible,QString,int)));

    ui->show_button->setEnabled(false);
    ui->clear_button->setEnabled(false);

}

SoftProjector::~SoftProjector()
{
    delete ui;
}



void SoftProjector::readConfigurationFile()
{
    // Read the font from the font configuration file:
    QFile fh("settings.cfg");

    if( ! fh.exists() )
    {
        // Set default settings:

        // display->setNewFont(...
        display->setNewWallpaper(QString());
        display->setShowBlack(true);
        bibleWidget->setPrimary(QString("bible_ru"));
        bibleWidget->setSecondary(QString("none"));
        by_chapter= false;
        return;
    }
    // Read the settings file:

    fh.open(QIODevice::Text | QIODevice::ReadOnly);

    QFont font;
    font.fromString(fh.readLine());
    display->setNewFont(font);

    // Read the path of the wallpaper from the configuration file:
    QString new_wallpaper_path = fh.readLine();
    new_wallpaper_path.chop(1); // Remove the trailing return character
    display->setNewWallpaper(new_wallpaper_path);

    display->setShowBlack( fh.readLine() == "Show black: true" );

    QString primary_bible = fh.readLine();
    primary_bible.chop(1);
    QString secondary_bible = fh.readLine();
    secondary_bible.chop(1);

    // Needs code to read from config file
    by_chapter= false;

    bibleWidget->setPrimary(primary_bible);
    bibleWidget->setSecondary(secondary_bible);

    fh.close();
}


void SoftProjector::writeConfigurationFile()
{
    QFile fh("settings.cfg");
    QString fontString = display->getFont().toString();
    QString wallpaper_path = display->getWallpaper();
    fh.open(QIODevice::Text | QIODevice::WriteOnly);

    fh.write(qPrintable(fontString));
    fh.write("\n");
    fh.write(qPrintable(wallpaper_path));
    qDebug("Writing wallpaper to file:");
    qDebug(qPrintable(wallpaper_path));
    fh.write("\n");
    if( display->getShowBlack() )
        fh.write("Show black: true\n");
    else
        fh.write("Show black: false\n");

    fh.write(qPrintable(bibleWidget->getPrimary()));
    fh.write("\n");
    fh.write(qPrintable(bibleWidget->getSecondary()));
    fh.write("\n");

    fh.close();

    // FIXME also save black, verse, primarybible, secondarybible
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
    this->on_show_button_clicked();

    ui->show_button->setEnabled(false);
    ui->clear_button->setEnabled(true);
}

void SoftProjector::setBibleList(Bible bib, QString bib2, int row)
{
    // Called to show a bible verse

    type = "bible";
    bible = bib; bible2 = bib2; cRow = row;
    if( (bible.primary != bible2) && (by_chapter) )
        bible.setSecondary(bible2);
    QString temp = bible.captionList1[0];
    QStringList templ = temp.split(":");
    ui->labelShow->setText(templ[0]);
    ui->listShow->clear();
    ui->listShow->setSpacing(0);
    ui->listShow->setWordWrap(true);
    ui->listShow->addItems(bible.verseList);
    ui->listShow->setCurrentRow(cRow);
    ui->listShow->setFocus();
    this->on_show_button_clicked();

}


void SoftProjector::on_listShow_currentRowChanged(int currentRow)
{
    // Called when the user selects a different row in the preview table.
//    qDebug() << currentRow;
    if( currentRow == -1 )
        return;

    if( showing == false )
        return;

    if(type=="song")
        display->SetAllText(ui->listShow->currentItem()->text(),"");
        //emit sendDisplay(ui->listShow->currentItem()->text(),"");
    else if(type=="bible")
    {
        if( (bible.primary==bible2) || (bible2=="none") )
            display->SetAllText(bible.verseList1[currentRow],bible.captionList1[currentRow]);
        else
	{
	    QString verse = bible.verseList1[currentRow] + "\n";
            if (by_chapter)
                verse += bible.verseList2[currentRow];
            else
                verse += bible.getSecondaryVerse(bible.idList.at(currentRow),bible2);
	    QString caption = bible.captionList1[currentRow] + "    ";
            if (by_chapter)
                caption += bible.captionList2[currentRow];
            else
                caption += bible.getSecondaryCaption(bible.idList.at(currentRow),bible2);

            display->SetAllText(verse, caption);
	}
    }
}

void SoftProjector::on_clear_button_clicked()
{
    showing = false;

    // Do not display any text:
    display->SetAllText(" ", " ");

    ui->show_button->setEnabled(true);
    ui->clear_button->setEnabled(false);
}

void SoftProjector::on_show_button_clicked()
{
    showing = true;
    on_listShow_currentRowChanged(ui->listShow->currentRow());
    ui->show_button->setEnabled(false);
    ui->clear_button->setEnabled(true);
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
    exportSongs->exec();
}

void SoftProjector::on_actionImportSongs_triggered()
{
    bool ok=false;
    importSongs->load();
    while (!ok)
    {
        int ret = importSongs->exec();
        switch (ret)
        {
        case ImportDialog::Accepted:
            ok = importSongs->isNewSbornik;
            if (!ok)
            {
                QMessageBox ms;
                ms.setWindowTitle("Error");
                ms.setText("The Sbornik code that you have entered already exists.\nPlease enter a diffirent unique Sbornik code.");
                ms.exec();
            }
            break;
        case ImportDialog::Rejected:
            ok = true;
            break;
        }
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
    this->on_show_button_clicked();
}

void SoftProjector::on_actionAbout_triggered()
{
    AboutDialog *aboutDialog;
    aboutDialog = new AboutDialog(this);
    aboutDialog->setWindowTitle("About softProjecor");
    aboutDialog->exec();
//    QString text = QString("softProjector.com");
//    QMessageBox::information(this, "Info", text);
}

void SoftProjector::on_tabWidget_currentChanged(int index)
{
    if (index ==0)
    {
        ui->actionEditSong->setVisible(false);
        ui->actionNewSong->setVisible(false);
    }
    else if (index==1)
    {
        ui->actionEditSong->setVisible(true);
        ui->actionNewSong->setVisible(true);
    }
}
