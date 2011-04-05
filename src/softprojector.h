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

#ifndef SOFTPROJECTOR_H
#define SOFTPROJECTOR_H

#include <QtGui/QMainWindow>
#include "songwidget.h"
#include "biblewidget.h"
#include "announcewidget.h"
#include "display1.h"
#include "editwidget.h"
#include "bible.h"
#include "managedatadialog.h"
#include "songcounter.h"

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
    Display1 *display;
    EditWidget *editWidget;
    bool showing; // whether we are currently showing to the projector
    bool display_on_top; // whether display screen is alway on top on not
    bool show_stanza_title; // whether to display song stanza title "Verse, Chorus, Insert"
    bool show_song_number; // whether to show song numbers
    bool show_song_key; // whether to show song key
    Song current_song;
    int current_song_verse;
    Verse current_verse;
    Announcement announcement_text;
    QString version_string;

    SoftProjector *softProjector;

    void applySetting(QString name, QString value);
    void applyDefaults();
    void readXMLConfigurationFile();
    void writeXMLConfigurationFile();
    void positionDisplayWindow();

public slots:
    void setWaitCursor();
    void setArrowCursor();
    void screenCountChanged(int);

private:
    Ui::SoftProjectorClass *ui;
    QString type;
    bool new_list;
    QActionGroup *languageGroup;
    QRect boundRectOrDrawText(QPainter *painter, bool draw, int left, int top, int width, int height, int flags, QString text);
    QRect drawSongTextToRect(QPainter *painter, QRect rect, bool draw, bool wrap, QString main_text, QString caption_str, QString song_num_str);
    void drawCurrentSongText(QPainter *painter, int width, int height);
    void drawCurrentBibleText(QPainter *painter, int width, int height);
    void drawAnnounceText(QPainter *painter, int width, int height);
    QTranslator translator;

private slots:
    void on_actionSong_Counter_triggered();
    void on_projectTab_currentChanged(int index);
    void on_actionCopy_Song_triggered();
    void on_actionCzech_triggered();
    void on_actionGerman_triggered();
    void retranslateUis();
    void on_actionEnglish_triggered();
    void on_actionRussian_triggered();
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
    void setChapterList(QStringList chapter_list, int verse, QString caption);
    void drawText(QPainter *painter, int width, int height);

public:
    void updateScreen();

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent( QKeyEvent * event );
};

#endif // SOFTPROJECTOR_H
