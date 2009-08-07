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
    void sendSong(QStringList songList, QString caption, int currentItem);

private slots:
    void on_songs_view_doubleClicked(QModelIndex index);
    void on_songs_view_activated(QModelIndex index);
    void on_sort_box_toggled(bool checked);
    void on_match_beginning_box_toggled(bool checked);
    void on_listPreview_doubleClicked(QModelIndex index);
    void on_lineEditSearch_textEdited(QString Text);
    void on_btnRemoveFromPlaylist_clicked();
    void on_btnAddToPlaylist_clicked();
    void on_btnLive_clicked();
    void on_listPlaylist_itemDoubleClicked(QListWidgetItem* item);
    void on_listPlaylist_currentTextChanged(QString currentText);
    void on_spinBoxPvNumber_valueChanged(int value);
    void on_comboBoxPvNumber_currentIndexChanged(int index);
    void selectMatchingSong(QString title);
    void showPreview(QString title);
    //    void loadSong();
    //    void loadPreview();
    void loadTitles(QString tSbornik);
    QString currentTitle();

private:
    Ui::SongWidget *ui;
    QString sbornik;
    QStringList allTitles;
    int titleType;
    SongDatabase playlist;
    SongsModel *songs_model;
    bool isPlaylistTitle;
    bool allSongs;
    //    SoftProjector *toShow;
};

#endif // SONGWIDGET_H
