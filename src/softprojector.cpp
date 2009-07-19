#include <QDesktopWidget>
#include "softprojector.h"
#include "ui_softprojector.h"
#include "settingsdialog.h"


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
    songWidget = new SongWidget;
    editWidget = new EditWidget;
    importSongs = new ImportDialog;
    exportSongs = new ExportDialog;

    showing = false;

    // Will modify's display's font and wallpaper:
    readConfigurationFile();



    ui->tabWidget->clear();
    ui->tabWidget->addTab(bibleWidget, "Bible");
    ui->tabWidget->addTab(songWidget,"Songs");

    editWidget->setWindowTitle("Edit and/or Add New songs");

    connect(songWidget, SIGNAL(sendSong(QStringList, QString, int)),
            this, SLOT(setSongList(QStringList, QString, int)));
    connect(bibleWidget, SIGNAL(goLive(Bible,QString,int)),
            this, SLOT(setBibleList(Bible,QString,int)));
    connect(this, SIGNAL(sendDisplay(QString,QString)),
            display, SLOT(SetAllText(QString,QString)));

}

SoftProjector::~SoftProjector()
{
    delete ui;
}



void SoftProjector::readConfigurationFile()
{
    // Read the font from the font configuration file:
    QFile cfgFile("settings.cfg");
    cfgFile.open(QIODevice::Text | QIODevice::ReadOnly);

    QFont font;
    font.fromString(cfgFile.readLine());
    display->setNewFont(font);

    //    qDebug() << MainFont.toString();

    // Read the path of the wallpaper from the configuration file:
    display->setNewWallpaper(cfgFile.readLine());


    // FIXME
    ui->show_button->setEnabled(true);
    ui->clear_button->setEnabled(false);

    // FIXME
    bibleWidget->setPrimary("Russian");
    bibleWidget->setSecondary("English (KJV)");

    cfgFile.close();
}


void SoftProjector::writeConfigurationFile()
{
    QFile cfgFile("settings.cfg");
    QString fontString = display->getFont().toString();
    QString wallpaperPath = display->getWallpaper();
    cfgFile.open(QIODevice::Text | QIODevice::WriteOnly);

    cfgFile.write(qPrintable(fontString));
    cfgFile.write("\n");
    cfgFile.write(qPrintable(wallpaperPath));
    cfgFile.close();


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
    type = "song";
    ui->labelShow->setText(caption);
    ui->listShow->clear();
    ui->listShow->setSpacing(5);
    ui->listShow->setWordWrap(false);
    ui->listShow->addItems(showList);
    ui->listShow->setCurrentRow(row);
    ui->listShow->setFocus();
    this->on_show_button_clicked();

}

void SoftProjector::setBibleList(Bible bib, QString bib2, int row)
{
    // Called to show a bible verse

    type = "bible";
    bible = bib; bible2 = bib2; cRow = row;
    if( bible.primary != bible2 )
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
        emit sendDisplay(ui->listShow->currentItem()->text(),"");
    else if(type=="bible")
    {
	if( bible.primary==bible2 )
	    emit sendDisplay(bible.verseList1[currentRow],bible.captionList1[currentRow]);
	else
	{
	    QString verse = bible.verseList1[currentRow] + "\n";
	    verse += bible.verseList2[currentRow];
	    QString caption = bible.captionList1[currentRow] + "    ";
	    caption += bible.captionList2[currentRow];
	    emit sendDisplay(verse,caption);
	}
    }
}

void SoftProjector::on_clear_button_clicked()
{
    showing = false;
    emit sendDisplay(" "," ");
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
        editWidget->setEdit(songWidget->sendToEdit());
        editWidget->show();
        editWidget->activateWindow();
    }
}

void SoftProjector::on_actionNewSong_triggered()
{
    if (ui->tabWidget->currentIndex()==1){
        editWidget->setNew();
        editWidget->show();
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
    importSongs->exec();
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
