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

public slots:
    Song getSongToEdit();

protected:
    virtual void changeEvent(QEvent *e);

signals:
    // To be used ONLY by SongWidget::sendToProjector():
    void sendSong(QStringList songList, QString caption, int currentItem);

private slots:
    void on_listPreview_doubleClicked(QModelIndex index);
    void on_songs_view_clicked(QModelIndex index);
    void on_playlist_view_clicked(QModelIndex index);
    void on_song_num_spinbox_editingFinished();
    void on_playlist_view_doubleClicked(QModelIndex index);
    void on_songs_view_doubleClicked(QModelIndex index);
    void on_match_beginning_box_toggled(bool checked);
    void on_lineEditSearch_textEdited(QString Text);
    void on_btnRemoveFromPlaylist_clicked();
    void on_btnAddToPlaylist_clicked();
    void on_btnLive_clicked();
    void on_song_num_spinbox_valueChanged(int value);
    void on_sbornik_menu_currentIndexChanged(int index);
    void selectMatchingSong(QString title);
    void sendToPreview(Song song);
    void sendToProjector(Song song, int row);
    void loadTitles(QString tSbornik);
    void loadSborniks();
    Song currentSong();
    Song currentPlaylistSong();

private:
    Ui::SongWidget *ui;
    QString sbornik;
    QStringList allTitles;
    QStringList sbornikList;
    int titleType;
    SongDatabase song_database;
    SongsModel *songs_model;
    SongsModel *playlist_model;
    SongProxyModel *proxy_model;
    bool isSpinboxEditing;
    bool focusInPlaylistTable;
    bool allSongs;
public slots:
    void songsViewRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void playlistViewRowChanged(const QModelIndex &current, const QModelIndex &previous);
};

#endif // SONGWIDGET_H
