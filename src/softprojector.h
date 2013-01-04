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

#ifndef SOFTPROJECTOR_H
#define SOFTPROJECTOR_H

#include <QtGui/QMainWindow>
#include "songwidget.h"
#include "biblewidget.h"
#include "announcewidget.h"
#include "displayscreen.h"
#include "editwidget.h"
#include "bible.h"
#include "managedatadialog.h"
#include "settingsdialog.h"
#include "songcounter.h"
#include "settings.h"
#include "printpreviewdialog.h"
#include "helpdialog.h"
#include "picturewidget.h"

class QActionGroup;

namespace Ui
{
    class SoftProjectorClass;
}

class SoftProjector : public QMainWindow
{
    Q_OBJECT

public:
    SoftProjector(QWidget *parent = 0);
    ~SoftProjector();
    SongWidget *songWidget;
    BibleWidget *bibleWidget;
    AnnounceWidget *announceWidget;
    ManageDataDialog *manageDialog;
    QDesktopWidget *desktop;
    EditWidget *editWidget;
    DisplayScreen *displayScreen1;
    DisplayScreen *displayScreen2;
    PictureWidget *pictureWidget;

    bool showing; // whether we are currently showing to the projector
    Song current_song;
    int current_song_verse;
    Verse current_verse;
    Announcement announcement_text;
    QString version_string;
    Theme theme;
    Settings mySettings;

    SoftProjector *softProjector;

public slots:
    void updateSetting(GeneralSettings &g,Theme &t);
    void saveSettings();
    void positionDisplayWindow();
    void updateScreen();

    void setWaitCursor();
    void setArrowCursor();
    void setProjectChanged(bool isChanged);

private:
    Ui::SoftProjectorClass *ui;
    SettingsDialog *settingsDialog;
    HelpDialog *helpDialog;
    QString type;
    bool new_list;
    QActionGroup *languageGroup;
    QString languagePath;
    QTranslator translator;

    //For saving and opening softProjector project files
    QString project_file_path;
    void updateWindowTest();
    bool is_project_saved;
    QString cur_locale;

    bool isSingleScreen;
    bool hasDisplayScreen2;

    // shortcuts
    QShortcut *shpgUP;
    QShortcut *shpgDwn;
    QShortcut *shSart1;
    QShortcut *shSart2;

    // Pictures
    QList<QPixmap> pictureShowList;

private slots:
    void showDisplayScreen(bool show);
    //For saving and opening softProjector project files
    void on_actionClose_Project_triggered();
    void on_actionNew_Project_triggered();
    void saveProject();
    void openProject();
    void clearProject();
    void readBibleHistoryFromSavedProject(QXmlStreamReader *xml);
    void readBibleHistoryFromSavedProject1_0(QXmlStreamReader *xml);
    void readSongsFromSavedProject(QXmlStreamReader *xml);
    void readAnnouncementsFromSavedProject(QXmlStreamReader *xml);

    void applySetting(GeneralSettings &g, Theme &t);
    void on_actionSave_Project_As_triggered();
    void on_actionSave_Project_triggered();
    void on_actionOpen_triggered();
    void on_actionSong_Counter_triggered();
    void on_projectTab_currentChanged(int index);
    void on_actionCopy_Song_triggered();
    void retranslateUis();
    void createLanguageActions();
    void switchLanguage(QAction *action);

    void on_action_Help_triggered();
    void on_actionDeleteSong_triggered();
    void on_actionManage_Database_triggered();
    void on_actionAbout_triggered();
    void on_listShow_doubleClicked(QModelIndex index);
    void on_actionSettings_triggered();
    void on_actionNewSong_triggered();
    void on_actionEditSong_triggered();
    void on_show_button_clicked();
    void on_clear_button_clicked();
    void on_listShow_currentRowChanged(int currentRow);
    void on_actionClose_triggered();
    void setSongList(Song song, int row);
    void setAnnounceText(Announcement text);
    void setChapterList(QStringList chapter_list, QString caption, QItemSelection selectedItems);
    void setPictureList(QList<QPixmap> &image_list, int row);

    void on_listShow_itemSelectionChanged();
    void on_rbMultiVerse_toggled(bool checked);
    void on_actionPrint_triggered();
    void on_actionPrint_Project_triggered();

    void nextSlide();
    void prevSlide();

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // SOFTPROJECTOR_H
