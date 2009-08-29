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
    proxy_model = new SongProxyModel(this);
    proxy_model->setSourceModel(songs_model);
    proxy_model->setDynamicSortFilter(true);
    ui->songs_view->setModel(proxy_model);
    //ui->songs_view->sortByColumn(1, Qt::AscendingOrder);
    connect(ui->songs_view->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
        this, SLOT(songsViewRowChanged(const QModelIndex&, const QModelIndex&)));

    playlist_model = new SongsModel;
    ui->playlist_view->setModel(playlist_model);
    connect(ui->playlist_view->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
        this, SLOT(playlistViewRowChanged(const QModelIndex&, const QModelIndex&)));

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

void SongWidget::songsViewRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    // Called when a new song is selected in the songs table
    int row = proxy_model->mapToSource(current).row();
    Song song = songs_model->getSong(row);
    sendToPreview(song);
    focusInPlaylistTable = false;
}

void SongWidget::playlistViewRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    // Called when a new song is selected in the playlist table
    Song song = playlist_model->getSong(current.row());
    sendToPreview(song);
    focusInPlaylistTable = true;
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
    QModelIndex current_index = proxy_model->mapToSource(ui->songs_view->currentIndex());
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

void SongWidget::sendToPreview(Song song)
{
    ui->listPreview->clear();
    QStringList song_list = song_database.getSongList(song);
    ui->listPreview->addItems(song_list);
    ui->listPreview->setCurrentRow(0);
}

void SongWidget::sendToProjector(Song song)
{
    // Display the specified song text in the right-most column of softProjector:
    QStringList song_list = song_database.getSongList(song);
    emit sendSong(song_list, song.title, 0);
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
        // FIXME Instead of using count(), find the song with highest num.
        ui->song_num_spinbox->setMaximum(song_list.count());
    }
    songs_model->setSongs(song_list);

    ui->songs_view->selectRow(0);
    // Re-draw the songs table:
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


void SongWidget::on_song_num_spinbox_editingFinished()
{
    // Called when the user presses enter after editing the song number
    // At this point, the song is already selected in the songs table
    on_btnAddToPlaylist_clicked();
}



void SongWidget::on_btnLive_clicked()
{
    Song song;
    if(focusInPlaylistTable)
        song = currentPlaylistSong();
    else
        song = currentSong();

    sendToProjector(song);
}



void SongWidget::on_btnAddToPlaylist_clicked()
{
    playlist_model->addSong(currentSong());

    ui->playlist_view->selectRow(playlist_model->rowCount()-1);
    ui->playlist_view->setFocus();
    //ui->playlist_view->viewport()->repaint();
    if( playlist_model->rowCount() > 0 )
        ui->btnRemoveFromPlaylist->setEnabled(true);
    sendToPreview(currentSong());
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
    bool match_beginning = ui->match_beginning_box->isChecked();
    proxy_model->setFilterString(text, match_beginning);
    songs_model->emitLayoutChanged(); // forces the view to redraw
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


void SongWidget::on_songs_view_doubleClicked(QModelIndex index)
{
    // Called when a song is double-clicked
    int row = proxy_model->mapToSource(index).row();
    Song song = songs_model->getSong(row);

    playlist_model->addSong(song);
    ui->playlist_view->selectRow(playlist_model->rowCount()-1);
    ui->playlist_view->setFocus();
    //ui->playlist_view->viewport()->repaint();
    ui->btnRemoveFromPlaylist->setEnabled(true);
    sendToPreview(song);
}


void SongWidget::on_playlist_view_doubleClicked(QModelIndex index)
{
    Song song = playlist_model->getSong(index.row());
    sendToProjector(song);
}



void SongWidget::on_playlist_view_clicked(QModelIndex index)
{
    // This method is implemented for the case where the use clicks
    // in the playlist table without changing the previous selection.
    Song song = playlist_model->getSong(index);
    sendToPreview(song);
}

void SongWidget::on_songs_view_clicked(QModelIndex index)
{
    // This method is implemented for the case where the use clicks
    // in the playlist table without changing the previous selection.
    Song song = songs_model->getSong(proxy_model->mapToSource(index));
    sendToPreview(song);
}
