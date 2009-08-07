#include <QDebug>
#include "songwidget.h"
#include "ui_songwidget.h"

SongWidget::SongWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongWidget)
{
    ui->setupUi(this);
    songs_model = new SongsModel;
    playlist_model = new SongsModel;
    ui->songs_view->setModel(songs_model);
    ui->playlist_view->setModel(playlist_model);
    // Decrease the row height:
    ui->songs_view->resizeRowsToContents();
    ui->playlist_view->resizeRowsToContents();
    on_sbornik_menu_currentIndexChanged(0);
}

SongWidget::~SongWidget()
{
    delete ui;
    delete songs_model;
}

void SongWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SongWidget::loadTitles(QString tSbornik)
{
    SongDatabase t;
    songs_model->setSongs(t.getSongs("ALL"));
}

Song SongWidget::currentSong()
{
    // Returns the selected song
    QModelIndex current_index = ui->songs_view->currentIndex();
    int current_row = current_index.row();
    return songs_model->getSong(current_row);
}

Song SongWidget::currentPlaylistSong()
{
    // Returns the selected song of the playlist
    QModelIndex current_index = ui->playlist_view->currentIndex();
    int current_row = current_index.row();
    return playlist_model->getSong(current_row);
}

void SongWidget::selectMatchingSong(QString text)
{
    bool startonly = ui->match_beginning_box->isChecked();
    // Look for a song matching <text>. Select it and scroll to show it.
    for (int i = 0; i < songs_model->song_list.size(); i++)
    {
        QString s = songs_model->song_list.at(i).title;
        bool matches;
        if( startonly )
            matches = s.startsWith(text);
        else
            matches = s.contains(text);

        if( matches )
        {
            // Select the row <i>:
            ui->songs_view->selectRow(i);
            // Scroll the songs table to the row <i>:
            ui->songs_view->scrollTo( songs_model->index(i, 0) );
            return;
        }
    }
}

void SongWidget::showPreview(QString title)
{
    QStringList list = title.split(" - ");
    ui->listPreview->clear();
    if (list.size()==1)
        ui->listPreview->addItems(playlist.getSongList(title));
    else
        ui->listPreview->addItems(playlist.getSongList(list[1]));
    ui->listPreview->setCurrentRow(0);
}

void SongWidget::on_sbornik_menu_currentIndexChanged(int index)
{
    // Called when a different sbornik is selected from the pull-down menu

    titleType=1;
    SongDatabase t;

    QString sbornik;

    switch( index ) {
        case 0:
            sbornik = QString("ALL");
            break;
        case 1:
            sbornik = QString("pv3300");
            break;
        case 2:
            sbornik = QString("pv2001");
            break;
        case 3:
            sbornik = QString("uaEpisni");
            break;
        case 4:
            sbornik = QString("pvUser");
    }

    ui->song_num_spinbox->setEnabled(!(sbornik == QString("ALL")));

    songs_model->setSongs(t.getSongs(sbornik));

    ui->songs_view->selectRow(0);
    // Re-draw the songs table:
    ui->songs_view->viewport()->repaint();
}

void SongWidget::on_song_num_spinbox_valueChanged(int value)
{
    selectMatchingSong(QString::number(value));
    //    selector=1;
//    setSong(value,1);
//    ui->listPreview->clear();
//    ui->listPreview->addItems(playlist.getSongList(value, sbornik));
}






void SongWidget::on_btnLive_clicked()
{
    if(isPlaylistTitle){
        Song song = currentPlaylistSong();
        emit sendSong(playlist.songList,song.title,ui->listPreview->currentRow());;
    }
    else{
        emit sendSong(playlist.songList, currentSong().title, ui->listPreview->currentRow());
    }
}



void SongWidget::on_btnAddToPlaylist_clicked()
{
    playlist_model->addSong(currentSong());
    qDebug("ADDED A SONG TO PLAYLIST");

    ui->playlist_view->selectRow(playlist_model->rowCount()-1);
    ui->playlist_view->setFocus();
    ui->playlist_view->viewport()->repaint();
}

void SongWidget::on_btnRemoveFromPlaylist_clicked()
{
    int row = ui->playlist_view->currentIndex().row();
    playlist_model->removeRow(row);
}

void SongWidget::on_lineEditSearch_textEdited(QString text)
{
    /* FIXME!!!!
    if (ui->sbornik_menu->currentIndex()>0)
        ui->sbornik_menu->setCurrentIndex(0);

    if (!ui->match_beginning_box->isChecked())
    {
        allSongs=false;
        QStringList tlist = text.split(" ");
        QStringList tlist2;
        if(tlist.count()==1){
            QString tx = tlist[0];
            tlist2 = allTitles.filter(tx.trimmed(), Qt::CaseInsensitive);
            songs_model->setSongs(tlist2);
        }
        if(tlist.count()==2)
        {
            QString tx = tlist[0];
            tlist2 = allTitles.filter(tx.trimmed(), Qt::CaseInsensitive);
            tx = tlist[1];
            songs_model->setSongs(tlist2.filter(tx, Qt::CaseInsensitive));
        }

    }
    else
    {
        if (!allSongs)
        {
            allSongs=true;
            songs_model->setTitles(allTitles);
        }
        selectMatchingSong(text);
    }
    */
}


Song SongWidget::getSongToEdit()
{
    // FIXME use the song from the playlist if selected there
    Song song = currentSong();
    return song;
}

void SongWidget::on_match_beginning_box_toggled(bool checked)
{
    QString new_text = ui->lineEditSearch->text();
    on_lineEditSearch_textEdited(new_text);
}



void SongWidget::on_songs_view_activated(QModelIndex index)
{
    // Called when a new song is selected
    isPlaylistTitle = false;
    showPreview(currentSong().title);
}

void SongWidget::on_songs_view_doubleClicked(QModelIndex index)
{
    // Caled when a song is double-clicked
    on_btnAddToPlaylist_clicked();
}

void SongWidget::on_playlist_view_activated(QModelIndex index)
{
    Song song = currentPlaylistSong();
    isPlaylistTitle = true;
    showPreview(song.title);
    //emit sendSong(playlist.songList, song.title, index.row());
}

void SongWidget::on_playlist_view_doubleClicked(QModelIndex index)
{
    Song song = currentPlaylistSong();
    emit sendSong(playlist.songList,song.title,0);
}

