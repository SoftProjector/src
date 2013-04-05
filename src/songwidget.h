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

#ifndef SONGWIDGET_H
#define SONGWIDGET_H

#include <QtGui/QWidget>
#include "song.h"
#include "songcounter.h"
#include "editwidget.h"
//#include "softprojector.h"

namespace Ui {
    class SongWidget;
}

class SongWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(SongWidget)
public:
    explicit SongWidget(QWidget *parent = 0);
    virtual ~SongWidget();
    Song currentSong();

public slots:
    void loadPlaylistFromFile(QList<Song> savedSongs);
    QList<Song> getPlaylistSongs();
    void retranslateUis();
    void deleteSong();
    Song getSongToEdit();
    void updateSongbooks();
    bool isSongSelected();
    // will be called by the edit widget:
    void updateSongFromDatabase(int songid, int initial_sid);
    void addNewSong(Song song, int initial_sid);
    QByteArray getSplitterState();
    void setSplitterState(QByteArray& state);

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void setWaitCursor();
    void setArrowCursor();
    // To be used ONLY by SongWidget::sendToProjector():
    void sendPlaylistChanged(bool changed);
    void sendSong(Song song, int currentItem);
    void addToSchedule(Song &song);

private slots:
    void on_comboBoxCategory_currentIndexChanged(int index);
    void on_btnDownInPlaylist_clicked();
    void on_btnUpInPlaylist_clicked();
    void on_exact_match_rbutton_clicked();
    void on_begins_rbutton_clicked();
    void on_contains_rbutton_clicked();
    void on_listPreview_doubleClicked(QModelIndex index);
    void on_songs_view_clicked(QModelIndex index);
    void on_playlist_view_clicked(QModelIndex index);
    void on_song_num_spinbox_editingFinished();
    void on_playlist_view_doubleClicked(QModelIndex index);
    void on_songs_view_doubleClicked(QModelIndex index);
    void on_lineEditSearch_textEdited(QString Text);
    void on_btnRemoveFromPlaylist_clicked();
    void on_btnAddToPlaylist_clicked();
    void on_btnLive_clicked();
    void on_song_num_spinbox_valueChanged(int value);
    void on_songbook_menu_currentIndexChanged(int index);
    void selectMatchingSong(QString title);
    void sendToPreview(Song song);
    void sendToProjector(Song song, int row);
    void loadSongbooks();
    void updateButtonStates();
    void filterModeChanged();
    void loadCategories(bool ui_update);

    void on_pushButtonSearch_clicked();

    void on_pushButtonClearResults_clicked();

private:
    Ui::SongWidget *ui;
    QString songbook;
    QStringList allTitles;
    QStringList songbookList;
    int titleType;
    SongDatabase song_database;
    SongsModel *playlist_model;
    SongProxyModel *proxy_model;
    bool isSpinboxEditing;
    bool focusInPlaylistTable;
    bool playlistSongWasEdited;
//    bool allSongs;
    Song preview_song;
    SongCounter counter;
    QList<int> cat_ids;
//    QStringList searchResults;
    QList<Song> allSongs;
    HighlighterDelegate *highlight;
public:
    SongsModel *songs_model;

public slots:
    void songsViewRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void playlistViewRowChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // SONGWIDGET_H
