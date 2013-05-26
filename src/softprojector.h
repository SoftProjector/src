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
#include <phonon>
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
#include "slideshow.h"
#include "mediawidget.h"
#include "videoinfo.h"
#include "slideshoweditor.h"
#include "schedule.h"

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
    MediaWidget *mediaPlayer;

    bool showing; // whether we are currently showing to the projector
    Song current_song;
    int current_song_verse;
    Verse current_verse;
    Announcement currentAnnounce;
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

private:
    Ui::SoftProjectorClass *ui;
    SettingsDialog *settingsDialog;
    HelpDialog *helpDialog;
    QString type;
    bool new_list;
    QActionGroup *languageGroup;
    QString languagePath;
    QTranslator translator;

    //For saving and opening schedule files
    //QString project_file_path;
    QString schedule_file_path;
    bool is_schedule_saved;
    //bool is_project_saved;

    QString cur_locale;

    bool isSingleScreen;
    bool hasDisplayScreen2;

    // shortcuts
    QShortcut *shpgUP;
    QShortcut *shpgDwn;
    QShortcut *shSart1;
    QShortcut *shSart2;

    // Pictures
    QList<SlideShowItem> pictureShowList;

    // video items
    Phonon::SeekSlider *playerSlider;
    Phonon::VolumeSlider *volumeSlider;
    VideoInfo currentVideo;
    Phonon::AudioOutput *playerAudioOutput;
//    Phonon::Path playerAudioOutputPath;

    // Schelude list
    QList<Schedule> schedule;

private slots:
    void showDisplayScreen(bool show);

    void applySetting(GeneralSettings &g, Theme &t);
    void on_actionSong_Counter_triggered();
    void on_projectTab_currentChanged(int index);
    void updateEditActions();
    void on_actionNew_triggered();
    void on_actionEdit_triggered();
    void on_actionCopy_triggered();
    void on_actionDelete_triggered();
    void updateWindowTest();

    void retranslateUis();
    void createLanguageActions();
    void switchLanguage(QAction *action);
    void on_actionDonate_triggered();
    void on_action_Help_triggered();
    void on_actionManage_Database_triggered();
    void on_actionAbout_triggered();
    void on_listShow_doubleClicked(QModelIndex index);
    void on_actionSettings_triggered();
    void newSong();
    void copySong();
    void editSong();
    void deleteSong();
    void newSlideShow();
    void editSlideShow();
    void deleteSlideShow();
    void addMediaToLibrary();
    void removeMediaFromLibrary();
    void newAnnouncement();
    void editAnnouncement();
    void copyAnnouncement();
    void deleteAnnoucement();
    void on_show_button_clicked();
    void on_clear_button_clicked();
    void on_listShow_currentRowChanged(int currentRow);
    void on_actionClose_triggered();
    void setSongList(Song song, int row);
    void setAnnounceText(Announcement announce, int row);
    void setChapterList(QStringList chapter_list, QString caption, QItemSelection selectedItems);
    void setPictureList(QList<SlideShowItem> &image_list, int row);
    void setVideo(VideoInfo &video);

    void on_listShow_itemSelectionChanged();
    void on_rbMultiVerse_toggled(bool checked);
    void on_actionPrint_triggered();
    void on_actionPrintSchedule_triggered();

    void nextSlide();
    void prevSlide();

    // video playback functions
    void on_pushButtonPlay_clicked();
    void setButtonPlayIcon(bool isPlaying);
    void setTimeText(QString cTime);

    // schedule functions
    void on_actionScheduleAdd_triggered();
    void on_actionScheduleRemove_triggered();
    void on_actionScheduleClear_triggered();
    void addToShcedule(BibleHistory &b);
    void addToShcedule(Song &s);
    void addToShcedule(SlideShow &s);
    void addToShcedule(VideoInfo &v);
    void addToShcedule(Announcement &a);
    void reloadShceduleList();
    void on_listWidgetSchedule_doubleClicked(const QModelIndex &index);
    void on_listWidgetSchedule_itemSelectionChanged();
    void on_actionMoveScheduleTop_triggered();
    void on_actionMoveScheduleUp_triggered();
    void on_actionMoveScheduleDown_triggered();
    void on_actionMoveScheduleBottom_triggered();
    void on_actionNewSchedule_triggered();
    void on_actionOpenSchedule_triggered();
    void on_actionSaveSchedule_triggered();
    void on_actionSaveScheduleAs_triggered();
    void on_actionCloseSchedule_triggered();
    void openSchedule();
    void saveSchedule();
    void saveScheduleNew(QSqlQuery &q);
    void saveScheduleItemNew(QSqlQuery &q, int scid, const BibleHistory &b);
    void saveScheduleItemNew(QSqlQuery &q, int scid, const Song &s);
    void saveScheduleItemNew(QSqlQuery &q, int scid, const SlideShow &s);
    void saveScheduleItemNew(QSqlQuery &q, int scid, const VideoInfo &v);
    void saveScheduleItemNew(QSqlQuery &q, int scid, const Announcement &a);
    void saveScheduleUpdate(QSqlQuery &q);
    void saveScheduleItemUpdate(QSqlQuery &q, int scid, const BibleHistory &b);
    void saveScheduleItemUpdate(QSqlQuery &q, int scid, const Song &s);
    void saveScheduleItemUpdate(QSqlQuery &q, int scid, const SlideShow &s);
    void saveScheduleItemUpdate(QSqlQuery &q, int scid, const VideoInfo &v);
    void saveScheduleItemUpdate(QSqlQuery &q, int scid, const Announcement &a);
protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // SOFTPROJECTOR_H
