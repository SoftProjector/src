#include <QDesktopWidget>
#include "softprojector.h"
#include "ui_softprojector.h"


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
    settingsDialog = new SettingsDialog;

    ui->tabWidget->clear();
    bibleWidget->setPrimary(0);
    bibleWidget->setSecondary(2);
    ui->tabWidget->addTab(bibleWidget, "Bible");
    ui->tabWidget->addTab(songWidget,"Songs");

    editWidget->setWindowTitle("Edit and/or Add New songs");

    primaryGroup = new QActionGroup(this);
    primaryGroup->addAction(ui->actionRussian);
    primaryGroup->addAction(ui->actionEnglish_Kjv);
    primaryGroup->addAction(ui->actionUkranian);
    ui->actionRussian->setChecked(true);
    secondaryGroup = new QActionGroup(this);
    secondaryGroup->addAction(ui->actionRussian_2);
    secondaryGroup->addAction(ui->actionEnglish_Kjv_2);
    secondaryGroup->addAction(ui->actionUkranian_2);
    ui->actionEnglish_Kjv_2->setChecked(true);

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
    ui->checkBoxLive->setChecked(true);
    ui->btnShow->setText("CLEAR");
    ui->btnShow->setChecked(true);
    ui->labelShow->setText(caption);
    ui->listShow->clear();
    ui->listShow->setSpacing(5);
    ui->listShow->setWordWrap(false);
    ui->listShow->addItems(showList);
    ui->listShow->setCurrentRow(row);
    ui->listShow->setFocus();

}

void SoftProjector::setBibleList(Bible bib, QString bib2, int row)
{
    type = "bible";
    ui->checkBoxLive->setChecked(true);
    ui->btnShow->setText("CLEAR");
    ui->btnShow->setChecked(true);
    bible = bib; bible2 = bib2; cRow = row;
    if( !(bible.primary == bible2) )
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

}

void SoftProjector::on_listShow_currentRowChanged(int currentRow)
{
//    qDebug() << currentRow;
    if( currentRow == -1 )
        return;
    else if(!ui->checkBoxLive->isChecked())
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

void SoftProjector::on_actionRussian_triggered()
{
    bibleWidget->setPrimary(0);
}

void SoftProjector::on_actionUkranian_triggered()
{
    bibleWidget->setPrimary(1);
}

void SoftProjector::on_actionEnglish_Kjv_triggered()
{
    bibleWidget->setPrimary(2);
}

void SoftProjector::on_actionRussian_2_triggered()
{
    bibleWidget->setSecondary(0);
}

void SoftProjector::on_actionUkranian_2_triggered()
{
    bibleWidget->setSecondary(1);
}

void SoftProjector::on_actionEnglish_Kjv_2_triggered()
{
    bibleWidget->setSecondary(2);
}

void SoftProjector::on_btnBlack_clicked()
{
    emit sendDisplay(" "," ");
    ui->btnShow->setText("SHOW");
    ui->btnShow->setChecked(false);
    ui->checkBoxLive->setChecked(false);
}

void SoftProjector::on_checkBoxLive_clicked()
{
    if(ui->checkBoxLive->isChecked()){
        ui->btnShow->setText("CLEAR");
        ui->btnShow->setChecked(true);
        on_listShow_currentRowChanged(ui->listShow->currentRow());
    }
}

void SoftProjector::on_btnShow_clicked()
{
    if(ui->btnShow->text() == "CLEAR"){
        ui->checkBoxLive->setChecked(false);
        ui->btnShow->setText("SHOW");
        ui->btnShow->setChecked(false);
        emit sendDisplay("        "," ");
    }
    else
    {
        ui->checkBoxLive->setChecked(true);
        ui->btnShow->setText("CLEAR");
        ui->btnShow->setChecked(true);
        on_listShow_currentRowChanged(ui->listShow->currentRow());
    }
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
    settingsDialog->exec();
}


