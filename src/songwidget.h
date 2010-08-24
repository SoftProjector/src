#ifndef SONGWIDGET_H
#define SONGWIDGET_H

#include <QtGui/QWidget>
#include "song.h"
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
    void retranslateUis();
    void deleteSong();
    Song getSongToEdit();
    void updateSongbooks();
    bool isSongSelected();
    // will be called by the edit widget:
    void updateSongFromDatabase(int songid);
    QByteArray getSplitterState();
    void setSplitterState(QString state);

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void setWaitCursor();
    void setArrowCursor();
    // To be used ONLY by SongWidget::sendToProjector():
    void sendSong(Song song, int currentItem);

private slots:
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
    bool allSongs;
    Song preview_song;
public:
    SongsModel *songs_model;

public slots:
    void songsViewRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void playlistViewRowChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // SONGWIDGET_H
