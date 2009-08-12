#include <QDebug>
#include "songwidget.h"
#include "ui_songwidget.h"

SongWidget::SongWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongWidget)
{
    ui->setupUi(this);
    loadSborniks();
    songs_model = new SongsModel;

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(songs_model);
    proxyModel->setDynamicSortFilter(true);
    //ui->songs_view->setModel(proxyModel);
    ui->songs_view->setModel(songs_model);

    playlist_model = new SongsModel;
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

void SongWidget::loadSborniks()
{
    QSqlQuery sq;
    QStringList sbor;
    sq.exec("SELECT id, name FROM Sborniks");
    while (sq.next())
    {
        sbornikList << sq.value(0).toString();
        sbor << sq.value(1).toString();
    }

    ui->sbornik_menu->addItems(sbor);
}

void SongWidget::loadTitles(QString tSbornik)
{
   songs_model->setSongs(song_database.getSongs("ALL"));
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

void SongWidget::showPreview(Song song)
{
    ui->listPreview->clear();
    QStringList song_list = song_database.getSongList(song);
    ui->listPreview->addItems(song_list);
    ui->listPreview->setCurrentRow(0);
}

void SongWidget::on_sbornik_menu_currentIndexChanged(int index)
{
    // Called when a different sbornik is selected from the pull-down menu

    QList<Song> song_list;

    if (index==0)
    {
        song_list = song_database.getSongs("ALL");
        ui->song_num_spinbox->setEnabled(false);
    }
    else
    {
        QString sbornik = sbornikList[index-1];
        song_list = song_database.getSongs(sbornik);
        ui->song_num_spinbox->setEnabled(true);
        ui->song_num_spinbox->setMaximum(song_list.count());
    }
    songs_model->setSongs(song_list);

    ui->songs_view->selectRow(0);
    // Re-draw the songs table:
    ui->songs_view->viewport()->repaint();
}

void SongWidget::on_song_num_spinbox_valueChanged(int value)
{
    //int max_num = 0;
    // Look for a song with number <value>. Select it and scroll to show it.
    for (int i = 0; i < songs_model->song_list.size(); i++)
    {
        Song s = songs_model->song_list.at(i);
        if( s.num == value )
        {
            // Select the row <i>:
            ui->songs_view->selectRow(i);
            // Scroll the songs table to the row <i>:
            ui->songs_view->scrollTo( songs_model->index(i, 0) );
            return;
        }
        //if( s.num > max_num )
        //    max_num = s.num;
    }
    // If got here, then no such song
    //if( value > max_num )
    //    ui->song_num_spinbox->setMaximum();
}





void SongWidget::on_btnLive_clicked()
{
    Song song;
    if(isPlaylistTitle)
        song = currentPlaylistSong();
    else
        song = currentSong();

    QStringList song_list = song_database.getSongList(song);
    emit sendSong(song_list, song.title, ui->listPreview->currentRow());
}



void SongWidget::on_btnAddToPlaylist_clicked()
{
    playlist_model->addSong(currentSong());

    ui->playlist_view->selectRow(playlist_model->rowCount()-1);
    ui->playlist_view->setFocus();
    ui->playlist_view->viewport()->repaint();
    if( playlist_model->rowCount() > 0 )
        ui->btnRemoveFromPlaylist->setEnabled(true);
}

void SongWidget::on_btnRemoveFromPlaylist_clicked()
{
    int row = ui->playlist_view->currentIndex().row();
    playlist_model->removeRow(row);

    if( playlist_model->rowCount() == 0 )
        ui->btnRemoveFromPlaylist->setEnabled(false);
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
    showPreview(currentSong());
}

void SongWidget::on_songs_view_doubleClicked(QModelIndex index)
{
    // Called when a song is double-clicked
    on_btnAddToPlaylist_clicked();
}

void SongWidget::on_playlist_view_activated(QModelIndex index)
{
    Song song = currentPlaylistSong();
    isPlaylistTitle = true;
    showPreview(song);
    //emit sendSong(song_database.songList, song.title, index.row());
}

void SongWidget::on_playlist_view_doubleClicked(QModelIndex index)
{
    Song song = currentPlaylistSong();
    QStringList song_list = song_database.getSongList(song);
    emit sendSong(song_list, song.title, 0);
}

