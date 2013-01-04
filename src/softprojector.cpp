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

#include <QDesktopWidget>
#include "softprojector.h"
#include "ui_softprojector.h"
#include "aboutdialog.h"
#include "helpdialog.h"

SoftProjector::SoftProjector(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::SoftProjectorClass)
{
    // Load settings
    mySettings.loadSettings();
    theme.setThemeId(mySettings.general.currentThemeId);
    theme.loadTheme();
    // Reset current theme id if initial was 0
    mySettings.general.currentThemeId = theme.getThemeId();

    //Setting up the Display Screen
    desktop = new QDesktopWidget();
    // NOTE: With virtual desktop, desktop->screen() will always return the main screen,
    // so this will initialize the Display1 widget on the main screen:
    displayScreen1 = new DisplayScreen(desktop->screen(0));
    displayScreen2 = new DisplayScreen(desktop->screen(0)); //for future
    // Don't worry, we'll move it later

    bibleWidget = new BibleWidget;
    songWidget = new SongWidget;
    editWidget = new EditWidget;
    announceWidget = new AnnounceWidget;
    manageDialog = new ManageDataDialog(this);
    settingsDialog = new SettingsDialog(this);

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
    applySetting(mySettings.general, theme);

    displayScreen1->setWindowIcon(QIcon(":icons/icons/display.png"));
    displayScreen2->setWindowIcon(QIcon(":icons/icons/display.png"));

    positionDisplayWindow();

    displayScreen1->renderText(false);
    displayScreen2->renderText(false);

    showing = false;

    ui->projectTab->clear();
    ui->projectTab->addTab(bibleWidget,QIcon(":/icons/icons/book.png"), tr("Bible (F6)"));
    ui->projectTab->addTab(songWidget,QIcon(":/icons/icons/song_tab.png"), tr("Songs (F7)"));
    ui->projectTab->addTab(announceWidget,QIcon(":/icons/icons/announce.png"), tr("Announcements (F8)"));

    connect(bibleWidget, SIGNAL(goLive(QStringList, QString, QItemSelection)),
            this, SLOT(setChapterList(QStringList, QString, QItemSelection)));
    connect(bibleWidget, SIGNAL(setArrowCursor()), this, SLOT(setArrowCursor()));
    connect(bibleWidget, SIGNAL(setWaitCursor()), this, SLOT(setWaitCursor()));
    connect(bibleWidget, SIGNAL(historyListChanged(bool)), this, SLOT(setProjectChanged(bool)));
    connect(songWidget, SIGNAL(sendSong(Song, int)), this, SLOT(setSongList(Song, int)));
    connect(songWidget, SIGNAL(setArrowCursor()), this, SLOT(setArrowCursor()));
    connect(songWidget, SIGNAL(setWaitCursor()), this, SLOT(setWaitCursor()));
    connect(songWidget, SIGNAL(sendPlaylistChanged(bool)), this, SLOT(setProjectChanged(bool)));
    connect(announceWidget,SIGNAL(sendText(Announcement)), this, SLOT(setAnnounceText(Announcement)));
    connect(announceWidget, SIGNAL(annouceListChanged(bool)), this, SLOT(setProjectChanged(bool)));
    connect(editWidget, SIGNAL(updateSongFromDatabase(int,int)), songWidget, SLOT(updateSongFromDatabase(int,int)));
    connect(editWidget, SIGNAL(addedNew(Song,int)), songWidget,SLOT(addNewSong(Song,int)));
    connect(manageDialog, SIGNAL(setMainArrowCursor()), this, SLOT(setArrowCursor()));
    connect(manageDialog, SIGNAL(setMainWaitCursor()), this, SLOT(setWaitCursor()));
    connect(languageGroup, SIGNAL(triggered(QAction*)), this, SLOT(switchLanguage(QAction*)));
    connect(displayScreen1,SIGNAL(exitSlide()),this,SLOT(on_clear_button_clicked()));
    connect(displayScreen1,SIGNAL(nextSlide()),this,SLOT(nextSlide()));
    connect(displayScreen1,SIGNAL(prevSlide()),this,SLOT(prevSlide()));
    connect(settingsDialog,SIGNAL(updateSettings(GeneralSettings&,Theme&)),
            this,SLOT(updateSetting(GeneralSettings&,Theme&)));
    connect(settingsDialog,SIGNAL(positionsDisplayWindow()),this,SLOT(positionDisplayWindow()));
    connect(settingsDialog,SIGNAL(updateScreen()),this,SLOT(updateScreen()));

    ui->toolBar->addAction(ui->actionNew_Project);
    ui->toolBar->addAction(ui->actionOpen);
    ui->toolBar->addAction(ui->actionSave_Project);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionPrint);
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

    // Create and connect shortcuts
    shpgUP = new QShortcut(Qt::Key_PageUp,this);
    shpgDwn = new QShortcut(Qt::Key_PageDown,this);
    shSart1 = new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_B),this);
    shSart2 = new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F5),this);
    connect(shpgUP,SIGNAL(activated()),this,SLOT(prevSlide()));
    connect(shpgDwn,SIGNAL(activated()),this,SLOT(nextSlide()));
    connect(shSart1,SIGNAL(activated()),this,SLOT(on_show_button_clicked()));
    connect(shSart2,SIGNAL(activated()),this,SLOT(on_show_button_clicked()));

    // Hide Multi verse selection, only visible to be when showing bible
    ui->rbMultiVerse->setVisible(false);

//    version_string = "2"; // to be used only for official release
    version_string = "2.Dev.Build: 5"; // to be used between official releases
    this->setWindowTitle("softProjector " + version_string);
}

SoftProjector::~SoftProjector()
{
    saveSettings();
    delete songWidget;
    delete editWidget;
    delete bibleWidget;
    delete announceWidget;
    delete manageDialog;
    delete displayScreen1;
    delete displayScreen2;
    delete desktop;
    delete languageGroup;
    delete settingsDialog;
    delete shpgUP;
    delete shpgDwn;
    delete shSart1;
    delete shSart2;
    delete ui;
}

void SoftProjector::positionDisplayWindow()
{
    // Position the display window as needed (including setting "always on top" flag,
    // showing full screen / normal mode, and positioning it on the right screen)

    if (mySettings.general.displayIsOnTop)
    {
        displayScreen1->setWindowFlags(Qt::WindowStaysOnTopHint);
        displayScreen2->setWindowFlags(Qt::WindowStaysOnTopHint);
    }
    else
    {
        displayScreen1->setWindowFlags(0); // Do not show always on top
        displayScreen2->setWindowFlags(0); // Do not show always on top
    }

    if(desktop->screenCount() > 1)
    {
        if (desktop->isVirtualDesktop())
        {
            // Move the display widget to screen 1 (secondary screen):
            QPoint top_left = desktop->screenGeometry(mySettings.general.displayScreen).topLeft();
            displayScreen1->move(top_left);
        }
        displayScreen1->showFullScreen();
        displayScreen1->setCursor(Qt::BlankCursor); //Sets a Blank Mouse to the screen
        displayScreen1->setControlButtonsVisible(false);

        // check if to display secondary display screen
        if(mySettings.general.displayScreen2>=0)
        {
            hasDisplayScreen2 = true;
            if (desktop->isVirtualDesktop())
            {
                // Move the display widget to screen 1 (secondary screen):
                QPoint top_left = desktop->screenGeometry(mySettings.general.displayScreen2).topLeft();
                displayScreen2->move(top_left);
            }
            displayScreen2->showFullScreen();
            displayScreen2->setCursor(Qt::BlankCursor); //Sets a Blank Mouse to the screen
            displayScreen2->setControlButtonsVisible(false);
        }
        else
        {
            hasDisplayScreen2 = false;
            displayScreen2->hide();
        }

        // specify that there is more than one diplay screen(monitor) availbale
        isSingleScreen = false;
    }
    else
    {
        // Single monitor only: Do not show on strat up.
        // Will be shown only when items were sent to the projector.
        showDisplayScreen(false);
        isSingleScreen = true;
        hasDisplayScreen2 = false;
    }
}

void SoftProjector::showDisplayScreen(bool show)
{
    if(show)
        displayScreen1->showFullScreen();
    else
        displayScreen1->hide();
    displayScreen1->setControlsSettings(mySettings.general.displayControls);
    displayScreen1->setControlButtonsVisible(true);
}

void SoftProjector::saveSettings()
{
    // Save splitter states
    mySettings.spMain.spSplitter = ui->splitter->saveState();
    mySettings.spMain.bibleHiddenSplitter = bibleWidget->getHiddenSplitterState();
    mySettings.spMain.bibleShowSplitter = bibleWidget->getShownSplitterState();
    mySettings.spMain.songSplitter = songWidget->getSplitterState();

    // Save window maximized state
    mySettings.spMain.isWindowMaximized = this->isMaximized();

    // save translation settings
    QList<QAction*> languageActions = ui->menuLanguage->actions();

    for(int i(0);i < languageActions.count();++i)
    {
        if(languageActions.at(i)->isChecked())
        {
            if(i < languageActions.count())
                mySettings.spMain.uiTranslation = languageActions.at(i)->data().toString();
            else
                mySettings.spMain.uiTranslation = "en";
        }
    }

    // save settings
//    mySettings.general.currentThemeId = theme.getThemeId();
    mySettings.saveSettings();
    theme.saveTheme();
}

void SoftProjector::updateSetting(GeneralSettings &g, Theme &t)
{
    mySettings.general = g;
    theme = t;
    bibleWidget->setSettings(theme.bible);
    announceWidget->setAlingment(theme.announce.textAlingmentV,theme.announce.textAlingmentH);
    
    // Apply display settings;
    displayScreen1->setNewPassiveWallpaper(theme.passive.backgroundPath,theme.passive.useBackground);
    if(theme.passive2.useDisp2settings)
        displayScreen2->setNewPassiveWallpaper(theme.passive2.backgroundPath,theme.passive2.useBackground);
    else
        displayScreen2->setNewPassiveWallpaper(theme.passive.backgroundPath,theme.passive.useBackground);
}

void SoftProjector::applySetting(GeneralSettings &g, Theme &t)
{
    updateSetting(g,t);

    // Apply splitter states
    ui->splitter->restoreState(mySettings.spMain.spSplitter);
    bibleWidget->setHiddenSplitterState(mySettings.spMain.bibleHiddenSplitter);
    bibleWidget->setShownSplitterState(mySettings.spMain.bibleShowSplitter);
    songWidget->setSplitterState(mySettings.spMain.songSplitter);

    // Apply window maximized
    if(mySettings.spMain.isWindowMaximized)
        this->setWindowState(Qt::WindowMaximized);

    // Apply current translation
    QList<QAction*> la = ui->menuLanguage->actions();
    QString splocale;
    for(int i(0);i < la.count(); ++i)
    {
        if(la.at(i)->data().toString() == mySettings.spMain.uiTranslation)
        {
            if(i < la.count())
            {
                ui->menuLanguage->actions().at(i)->setChecked(true);
                splocale = mySettings.spMain.uiTranslation;
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

void SoftProjector::keyPressEvent(QKeyEvent *event)
{
    // Will get called when a key is pressed
    int key = event->key();
    if(key == Qt::Key_F6)
        ui->projectTab->setCurrentWidget(bibleWidget);
    else if(key == Qt::Key_F7)
        ui->projectTab->setCurrentWidget(songWidget);
    else if(key == Qt::Key_F8)
        ui->projectTab->setCurrentWidget(announceWidget);
    else if(key == Qt::Key_Left)
        prevSlide();
    else if(key == Qt::Key_Back)
        prevSlide();
    else if(key == Qt::Key_Right)
        nextSlide();
    else if(key == Qt::Key_Forward)
        nextSlide();
    else if(key == Qt::Key_Return)
        nextSlide();
    else if(key == Qt::Key_Enter)
        nextSlide();
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

    if(!showing)
    {
        if(isSingleScreen)
            showDisplayScreen(false);

        // Do not display any text:
        displayScreen1->renderText(false);
        if(hasDisplayScreen2)
            displayScreen2->renderText(false);
        ui->show_button->setEnabled(true);
        ui->clear_button->setEnabled(false);
    }
    else if (currentRow >=0 && !new_list)
    {
        if(isSingleScreen)
        {
            if(displayScreen1->isHidden())
                showDisplayScreen(true);
        }

        ui->show_button->setEnabled(false);
        ui->clear_button->setEnabled(true);

        if(type=="bible")
        {
            int srows(ui->listShow->count());
            QList<int> currentRows;
            for(int i(0); i<srows; ++i)
            {
                if(ui->listShow->item(i)->isSelected())
                    currentRows.append(i);
            }
            displayScreen1->renderBibleText(bibleWidget->bible.getCurrentVerseAndCaption(currentRows,theme.bible),theme.bible);
            if(hasDisplayScreen2)
            {
                if(theme.bible2.useDisp2settings)
                    displayScreen2->renderBibleText(bibleWidget->bible.getCurrentVerseAndCaption(currentRows,theme.bible2),theme.bible2);
                else
                    displayScreen2->renderBibleText(bibleWidget->bible.getCurrentVerseAndCaption(currentRows,theme.bible),theme.bible);
            }
        }
        else if(type=="song")
        {
            displayScreen1->renderSongText(current_song.getStanza(currentRow),theme.song);
            if(hasDisplayScreen2)
            {
                if(theme.song2.useDisp2settings)
                    displayScreen2->renderSongText(current_song.getStanza(currentRow),theme.song2);
                else
                    displayScreen2->renderSongText(current_song.getStanza(currentRow),theme.song);
            }
        }
        else if(type == "announce")
        {
            displayScreen1->renderAnnounceText(announcement_text,theme.announce);
            if(hasDisplayScreen2)
            {
                if(theme.announce2.useDisp2settings)
                    displayScreen2->renderAnnounceText(announcement_text,theme.announce2);
                else
                    displayScreen2->renderAnnounceText(announcement_text,theme.announce);
            }
        }
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
        if(!editWidget->isHidden()) //Prohibits editing a song when a different song already been edited.
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

void SoftProjector::on_actionNewSong_triggered()
{
    if (!editWidget->isHidden()) //Prohibits editing a song when a different song already been edited.
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
    {if (!editWidget->isHidden()) //Prohibits editing a song when a different song already been edited.
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
    settingsDialog->loadSettings(mySettings.general,theme);
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
    delete aboutDialog;
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
//        ui->actionPrint->setEnabled(true);
    }
    else
    {
        // Other tabs are activated
        ui->actionEditSong->setEnabled(false);
        ui->actionNewSong->setEnabled(false);
        ui->actionCopy_Song->setEnabled(false);
        ui->actionDeleteSong->setEnabled(false);
//        ui->actionPrint->setEnabled(false);
    }
}

void SoftProjector::on_actionManage_Database_triggered()
{
    QSqlQuery sq;

    manageDialog->loadThemes();
    manageDialog->load_songbooks();
    manageDialog->exec();

    // Reload songbooks if Songbook has been added, edited, or deleted
    if (manageDialog->reload_songbook)
        songWidget->updateSongbooks();

    // Relaod themes if a theme has been deleted
    if (manageDialog->reloadThemes)
    {
        // Check if current theme has been deleted
        sq.exec("SELECT * FROM Themes WHERE is = " + theme.getThemeId());
        if(!sq.first())
        {
            GeneralSettings g = mySettings.general;
            Theme t;
            sq.exec("SELECT id FROM Themes");
            sq.first();
            t.setThemeId(sq.value(0).toString());
            t.loadTheme();
            g.currentThemeId = t.getThemeId();
            updateSetting(g,t);
            updateScreen();
        }
    }

    // Reload Bibles if Bible has been deleted
    if (manageDialog->reload_bible)
    {
        // check if Primary bible has been removed
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + theme.bible.primaryBible);
        if (!sq.first())
        {
            // If original primary bible has been removed, set first bible in the list to be primary
            sq.clear();
            sq.exec("SELECT id FROM BibleVersions");
            sq.first();
            theme.bible.primaryBible = sq.value(0).toString();
        }
        sq.clear();

        // check if secondary bible has been removed, if yes, set secondary to "none"
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + theme.bible.secondaryBible);
        if (!sq.first())
            theme.bible.secondaryBible = "none";
        sq.clear();

        // check if trinary bible has been removed, if yes, set secondary to "none"
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + theme.bible.trinaryBible);
        if (!sq.first())
            theme.bible.trinaryBible = "none";
        sq.clear();

        // check if operator bible has been removed, if yes, set secondary to "same"
        sq.exec("SELECT * FROM BibleVersions WHERE id = " + theme.bible.operatorBible);
        if (!sq.first())
            theme.bible.operatorBible = "same";
        bibleWidget->setSettings(theme.bible);
    }
}

void SoftProjector::on_action_Help_triggered()
{
    HelpDialog *help_dialog;
    help_dialog = new HelpDialog();
    help_dialog->show();
    delete help_dialog;
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
    delete songCounter;
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
    xml.writeAttribute("version","2.0");

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
        atrs.append("num",QString::number(s.number));
        atrs.append("category",QString::number(s.category));
        xml.writeAttributes(atrs);

        // write song items
        xml.writeTextElement("title",s.title);
        xml.writeTextElement("text",s.songText);
        xml.writeTextElement("songbook",s.songbook_name);
        xml.writeTextElement("tune",s.tune);
        xml.writeTextElement("words",s.wordsBy);
        xml.writeTextElement("music",s.musicBy);
        xml.writeTextElement("notes",s.notes);
        if(s.usePrivateSettings)
            xml.writeTextElement("use_private","true");
        else
            xml.writeTextElement("use_private","false");
        xml.writeTextElement("align",QString("%1,%2").arg(QString::number(s.alignmentV)).arg(QString::number(s.alignmentH)));
        unsigned int textColorInt = (unsigned int)(s.color.rgb());
        xml.writeTextElement("color",QString::number(textColorInt));
        xml.writeTextElement("font",s.font.toString());
        xml.writeTextElement("back",s.backgroundPath.trimmed());

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
            if(1.0 < p_version && p_version <= 2.0)
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
            s.number = a.toInt();
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
                else if(xml->StartElement && (xml->name() == "notes" || xml->name() == "comments"))
                {
                    s.notes = xml->readElementText();
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "use_private")
                {
                    s.usePrivateSettings = (xml->readElementText() == "true");
                    xml->readNext();
                }
                else if(xml->StartElement && (xml->name() == "aling" || xml->name() == "align"))
                {
                    QString str = xml->readElementText();
                    QStringList l = str.split(",");
                    if(l.count()>1)
                    {
                    s.alignmentV = l.at(0).toInt();
                    s.alignmentH = l.at(1).toInt();
                    }
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "color")
                {
                    s.color = QColor::fromRgb(xml->readElementText().toUInt());
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "font")
                {
                    s.font.fromString(xml->readElementText());
                    xml->readNext();
                }
                else if(xml->StartElement && xml->name() == "back")
                {
                    s.backgroundPath = xml->readElementText();
                    xml->readNext();
                }
//                else if(xml->StartElement)
//                {
//                   xml->readNext();
//                }
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

void SoftProjector::on_actionPrint_triggered()
{
    PrintPreviewDialog* p;
    p = new PrintPreviewDialog(this);
    if(ui->projectTab->currentIndex() == 0)
    {
        p->setText(theme.bible.operatorBible + "," + theme.bible.primaryBible,
                   bibleWidget->getCurrentBook(),bibleWidget->getCurrentChapter());
        p->exec();
    }
    else if (ui->projectTab->currentIndex() == 1)
    {
        if (songWidget->isSongSelected())
        {
            p->setText(songWidget->getSongToEdit());
            p->exec();

        }
        else
        {
            QMessageBox ms;
            ms.setWindowTitle(tr("No song selected"));
            ms.setText(tr("No song has been selected to be printed."));
            ms.setInformativeText(tr("Please select a song to be printed."));
            ms.setIcon(QMessageBox::Information);
            ms.exec();
        }

    }
    else if (ui->projectTab->currentIndex() == 2)
    {
        p->setText(announceWidget->getAnnouncements());
        p->exec();
    }

    delete p;
}

void SoftProjector::on_actionPrint_Project_triggered()
{
    PrintPreviewDialog* p;
    p = new PrintPreviewDialog(this);
    p->setText(project_file_path,bibleWidget->getHistoryList(),songWidget->getPlaylistSongs(),announceWidget->getAnnouncements());
    p->exec();
    delete p;
}

void SoftProjector::nextSlide()
{
    // selects next item in the show list
    int current = ui->listShow->currentRow();
    if(ui->rbMultiVerse->isChecked())
    {
        // if multiple is selected, select last one
        for (int i(0);i<ui->listShow->count();++i)
        {
            if(ui->listShow->item(i)->isSelected())
                current = i;
        }
        if(current < ui->listShow->count()-1)
            ui->rbMultiVerse->setChecked(false);
    }
    if(current < ui->listShow->count()-1)
        ui->listShow->setCurrentRow(current+1);
}

void SoftProjector::prevSlide()
{
    // selects previous item in the show list
    int current = ui->listShow->currentRow();
    if(ui->rbMultiVerse->isChecked())
    {
        // if multiple is selected, select first one
        for (int i(0);i<ui->listShow->count();++i)
        {
            if(ui->listShow->item(i)->isSelected())
            {
                current = i;
                break;
            }
        }
        if(current>0)
            ui->rbMultiVerse->setChecked(false);
    }
    if(current>0)
        ui->listShow->setCurrentRow(current-1);
}
