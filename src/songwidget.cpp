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

#include <QDebug>
#include "songwidget.h"
#include "ui_songwidget.h"

SongWidget::SongWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongWidget)
{
    ui->setupUi(this);

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
    // IS this needed?

    // Modify the column widths:
    ui->songs_view->setColumnWidth(0, 0);//Category
    ui->songs_view->setColumnWidth(1, 60);//Song Number
    ui->songs_view->setColumnWidth(2, 150);//Song Title
    ui->songs_view->setColumnWidth(3, 70);//Songbook
    ui->playlist_view->setColumnWidth(0, 0);//Category
    ui->playlist_view->setColumnWidth(1, 60);//Song Number
    ui->playlist_view->setColumnWidth(2, 150);//Song Title
    ui->playlist_view->setColumnWidth(3, 70);//Songbook

    proxy_model->setSongbookFilter("ALL");
    proxy_model->setCategoryFilter(-1);
    loadSongbooks();
    loadCategories(false);

    isSpinboxEditing = false;
    playlistSongWasEdited = false;
}

SongWidget::~SongWidget()
{
    delete ui;
    delete songs_model;
}

void SongWidget::songsViewRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if( current.isValid() )
    {
        // Called when a new song is selected in the songs table
        int row = proxy_model->mapToSource(current).row();
        Song song = songs_model->getSong(row);
        sendToPreview(song);
        focusInPlaylistTable = false;
    }
    updateButtonStates();
}

void SongWidget::updateButtonStates()
{
    bool enable_live;
    bool enable_add;
    bool enable_remove;
    bool enable_up;
    bool enable_down;

    if(focusInPlaylistTable)
    {
        enable_live = ui->playlist_view->currentIndex().isValid();
        enable_add = false;
        enable_remove = ( playlist_model->rowCount() > 0 );
        int selrow = ui->playlist_view->currentIndex().row();
        enable_up = (playlist_model->rowCount() > 1) && (selrow != 0);
        enable_down = (playlist_model->rowCount() > 1) && (selrow+1 != playlist_model->rowCount());
    }
    else
    {
        // focus in songs table
        if( proxy_model->rowCount() == 0 )
            enable_live = false;
        else
            enable_live = ui->songs_view->currentIndex().isValid();
        enable_add = enable_live;
        enable_remove = false;
        enable_up = false;
        enable_down = false;
    }

    ui->btnLive->setEnabled(enable_live);
    ui->btnAddToPlaylist->setEnabled(enable_add);
    ui->btnRemoveFromPlaylist->setEnabled(enable_remove);
    ui->btnUpInPlaylist->setEnabled(enable_up);
    ui->btnDownInPlaylist->setEnabled(enable_down);
}

void SongWidget::playlistViewRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if( current.isValid() )
    {
        // Called when a new song is selected in the playlist table
        Song song = playlist_model->getSong(current.row());
        sendToPreview(song);
    }
    else
    {
        //FIXME clear the preview? Or not...
    }
    updateButtonStates();
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

void SongWidget::loadSongbooks()
{
    QSqlQuery sq;
    QStringList sbor;
    sq.exec("SELECT id, name FROM Songbooks");
    while (sq.next())
    {
        songbookList << sq.value(0).toString();
        sbor << sq.value(1).toString();
    }
    ui->songbook_menu->addItem(tr("All songbooks"));
    ui->songbook_menu->addItems(sbor);
    QList<Song> song_list = song_database.getSongs();

    //ui->song_num_spinbox->setEnabled(false);
    songs_model->setSongs(song_list);

    // update the song spin box and redraw the table:
    on_songbook_menu_currentIndexChanged( ui->songbook_menu->currentIndex() );
}

Song SongWidget::currentSong()
{
    // Returns the selected song
    QModelIndex current_index;
    int current_row;
    if (focusInPlaylistTable)
        current_row = ui->playlist_view->currentIndex().row();
    else
    {
        current_index = proxy_model->mapToSource(ui->songs_view->currentIndex());
        current_row = current_index.row();
    }
    Song return_song;
    if(current_row>=0)
    {
        if(focusInPlaylistTable)
            return_song = playlist_model->getSong(current_row);
        else
            return_song = songs_model->getSong(current_row);
    }
    return return_song;
}

void SongWidget::selectMatchingSong(QString text)
{
    bool startonly = ui->begins_rbutton->isChecked();
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
    QStringList song_list = song.getSongTextList();
    ui->listPreview->clear();
    ui->listPreview->addItems(song_list);
    ui->listPreview->setCurrentRow(0);
    ui->preview_label->setText(tr("Song Preview: ") + song.title);
    preview_song = song;
}

void SongWidget::sendToProjector(Song song, int row)
{
    // Display the specified song text in the right-most column of softProjector:
    emit sendSong(song, row);

    // Add a count to a song
    counter.addSongCount(song);
}

void SongWidget::on_songbook_menu_currentIndexChanged(int index)
{
    // Called when a different songbook is selected from the pull-down menu

    songs_model->emitLayoutAboutToBeChanged(); //prepeare to chage layout
    if( index == 0 )
    {
        proxy_model->setSongbookFilter("ALL");
        ui->song_num_spinbox->setEnabled(false);
    }
    else
    {
        QString songbook_name = ui->songbook_menu->currentText();
        proxy_model->setSongbookFilter(songbook_name);
        ui->song_num_spinbox->setEnabled(true);
    }

    updateButtonStates();

    songs_model->emitLayoutChanged(); // forces the view to redraw

}

void SongWidget::on_song_num_spinbox_valueChanged(int value)
{
    // checks if spinbox just got into eding mode, it yes, then it reset searchbox and sorts song table view
    if (!isSpinboxEditing)
    {
        isSpinboxEditing = true;
        ui->lineEditSearch->clear();
        on_lineEditSearch_textEdited("");
        ui->songs_view->sortByColumn(0,Qt::AscendingOrder);
    }

    //int max_num = 0;
    // Look for a song with number <value>. Select it and scroll to show it.
    for (int i = 0; i < songs_model->song_list.size(); i++)
    {
        Song s = songs_model->song_list.at(i);
        if( s.num == value && s.songbook_name == ui->songbook_menu->currentText() )
        {
            // Found a song with this song number
            QModelIndex source_index = songs_model->index(i, 0);
            if( proxy_model->filterAcceptsRow(source_index.row(), source_index) )
            {
                // If this row is visible
                QModelIndex proxy_index = proxy_model->mapFromSource(source_index);

                // Select the row <i>:
                ui->songs_view->selectRow(proxy_index.row());
                // Scroll the songs table to the row <i>:
                ui->songs_view->scrollTo(proxy_index);
            }
            else
            {
                // This song is filtered out using text filter, so can't select
                // it in the table. Just show it:
                sendToPreview(s);
            }
            return;
        }
        //if( s.num > max_num )
        //    max_num = s.num;
    }
    // If got here, then no such song
    //if( value > max_num )
    //    ui->song_num_spinbox->setMaximum();

    QMessageBox mb;
    mb.setText(tr("Could not find song with number ") + QString::number(value) );
    mb.setWindowTitle(tr("No such song"));
    mb.setIcon(QMessageBox::Warning);
    mb.exec();

}

void SongWidget::on_song_num_spinbox_editingFinished()
{
    // Called when the user presses enter after editing the song number
    // At this point, the song is already selected in the songs table
    //on_btnAddToPlaylist_clicked();

    // Resets spin box to non eding mode
    isSpinboxEditing = false;
}

void SongWidget::on_btnLive_clicked()
{
    sendToProjector(preview_song, ui->listPreview->currentRow()); // Send current selected
}

void SongWidget::on_btnAddToPlaylist_clicked()
{
    Song song;
    if( false ) //ui->song_num_spinbox->hasFocus() )
    {
        int value = ui->song_num_spinbox->text().toInt();
        for (int i = 0; i < songs_model->song_list.size(); i++)
        {
            Song s = songs_model->song_list.at(i);
            if( s.num == value )
            {
                song = s;
                break;
            }
        }
    }
    else
    {
        song = currentSong();
    }

    playlist_model->addSong(song);
    emit sendPlaylistChanged(true);

    // Select the row that was just added:
    ui->playlist_view->selectRow(playlist_model->rowCount()-1);
    //ui->playlist_view->setFocus();
    //focusInPlaylistTable = true;
    //ui->btnRemoveFromPlaylist->setEnabled(true);
    sendToPreview(song);
    updateButtonStates();
}

void SongWidget::on_btnRemoveFromPlaylist_clicked()
{
    int row = ui->playlist_view->currentIndex().row();
    playlist_model->removeRow(row);
    emit sendPlaylistChanged(true);
    updateButtonStates();
}

void SongWidget::on_btnDownInPlaylist_clicked()
{
    playlist_model->emitLayoutAboutToBeChanged();
    int row = ui->playlist_view->currentIndex().row();
    Song song = playlist_model->song_list.takeAt(row);
    playlist_model->song_list.insert(row+1, song);
    playlist_model->emitLayoutChanged();

    ui->playlist_view->selectRow(row+1);
    emit sendPlaylistChanged(true);

    updateButtonStates();
}

void SongWidget::on_btnUpInPlaylist_clicked()
{
    playlist_model->emitLayoutAboutToBeChanged();
    int row = ui->playlist_view->currentIndex().row();
    Song song = playlist_model->song_list.takeAt(row);
    playlist_model->song_list.insert(row-1, song);
    playlist_model->emitLayoutChanged();

    ui->playlist_view->selectRow(row-1);
    emit sendPlaylistChanged(true);

    updateButtonStates();
}

void SongWidget::on_lineEditSearch_textEdited(QString text)
{
    // These two options are mutually exclusive:
    bool match_beginning = ui->begins_rbutton->isChecked();
    bool exact_match = ui->exact_match_rbutton->isChecked();

    songs_model->emitLayoutAboutToBeChanged(); // prepares view to be redrawn
    proxy_model->setFilterString(text, match_beginning, exact_match);
    songs_model->emitLayoutChanged(); // forces the view to redraw

    // Select the first row that matches the new filter:
    ui->songs_view->selectRow(0);
    ui->songs_view->scrollToTop();

    // Load Preview on song changes
    int row = proxy_model->mapToSource(ui->songs_view->currentIndex()).row();
    if( row>=0)
    {
        Song song = songs_model->getSong(row);
        sendToPreview(song);
        focusInPlaylistTable = false;
    }
    updateButtonStates();
}

Song SongWidget::getSongToEdit()
{
    playlistSongWasEdited = focusInPlaylistTable;
    return preview_song;
}

void SongWidget::on_songs_view_doubleClicked(QModelIndex index)
{
    // Called when a song is double-clicked
    int row = proxy_model->mapToSource(index).row();
    Song song = songs_model->getSong(row);

    playlist_model->addSong(song);
    emit sendPlaylistChanged(true);
    ui->playlist_view->selectRow(playlist_model->rowCount()-1);
    //ui->playlist_view->setFocus();
    //focusInPlaylistTable = true;
    sendToPreview(song);
}

void SongWidget::on_playlist_view_doubleClicked(QModelIndex index)
{
    Song song = playlist_model->getSong(index.row());
    //sendToProjector(song, ui->listPreview->currentRow());
    sendToProjector(song, 0); // Send the first verse
}

void SongWidget::on_playlist_view_clicked(QModelIndex index)
{
    // This method is implemented for the case where the use clicks
    // in the playlist table without changing the previous selection.
    Song song = playlist_model->getSong(index);
    focusInPlaylistTable = true;
    sendToPreview(song);
    updateButtonStates();
}

void SongWidget::on_songs_view_clicked(QModelIndex index)
{
    // This method is implemented for the case where the use clicks
    // in the playlist table without changing the previous selection.
    Song song = songs_model->getSong(proxy_model->mapToSource(index));
    focusInPlaylistTable = false;
    sendToPreview(song);
    updateButtonStates();
}

void SongWidget::on_listPreview_doubleClicked(QModelIndex index)
{
    sendToProjector(preview_song, index.row());
}

void SongWidget::updateSongbooks()
{
    emit setWaitCursor();
    // Reload the songbook and reselect the one that used to be selected
    // if it's still available, otherwise show all songbooks

    QString current_songbook = ui->songbook_menu->currentText();
    QString item0 = ui->songbook_menu->itemText(0);
    ui->songbook_menu->clear();
    loadSongbooks();

    int new_index = ui->songbook_menu->findText(current_songbook);
    if( new_index == -1 )
        new_index = 0; // All songbooks

    ui->songbook_menu->setCurrentIndex(new_index);
    emit setArrowCursor();
}

void SongWidget::updateSongFromDatabase(int songid, int initial_sid)
{
    songs_model->updateSongFromDatabase(songid);

    // Updated playlist song if song was edited comes from playlist table
    if (playlistSongWasEdited)
    {
        playlist_model->updateSongFromDatabase(songid, initial_sid);
        emit sendPlaylistChanged(true);
    }

    // Update the preview table:
    sendToPreview( currentSong() );
}

void SongWidget::deleteSong()
{
    song_database.deleteSong(currentSong().songID);
    int row = ui->songs_view->currentIndex().row();
    proxy_model->removeRow(row);
}

void SongWidget::addNewSong(Song song, int initial_sid)
{
    songs_model->addSong(song);
    ui->songs_view->selectRow(songs_model->rowCount()-1);

    // Updated playlist song if song was edited comes from playlist table
    if (playlistSongWasEdited)
    {
        playlist_model->updateSongFromDatabase(song.songID,initial_sid);
        emit sendPlaylistChanged(true);
    }

    sendToPreview(song);
}

void SongWidget::filterModeChanged()
{
    // Re-apply the filter:
    QString new_text = ui->lineEditSearch->text();
    on_lineEditSearch_textEdited(new_text);
}

void SongWidget::on_contains_rbutton_clicked()
{
    filterModeChanged();
}

void SongWidget::on_begins_rbutton_clicked()
{
    filterModeChanged();
}

void SongWidget::on_exact_match_rbutton_clicked()
{
    filterModeChanged();
}

QByteArray SongWidget::getSplitterState()
{
    return ui->splitter->saveState();
}

void SongWidget::setSplitterState(QString state)
{
    QByteArray b;
    b.insert(0,state);
    b = b.fromHex(b);
    ui->splitter->restoreState(b);
}

void SongWidget::retranslateUis()
{
    ui->songbook_menu->setItemText(0,tr("All songbooks"));
    loadCategories(true);
}

bool SongWidget::isSongSelected()
{
    preview_song.isValid();
//    return ui->songs_view->currentIndex().isValid();
}

void SongWidget::loadCategories(bool ui_update)
{
    EditWidget e;

    // retrieve current category id
    int cur_cat_id(-1);
    int cur_index = ui->comboBoxCategory->currentIndex();
    if(cur_index>0)
        cur_cat_id = cat_ids.at(cur_index-1);

    // get categories
    QStringList cat_list;
    cat_list = e.categories();

    // create sorting by name and refrance categories id
    QMap<QString,int> cmap;
    for(int i(0); i< cat_list.count(); ++i)
        cmap.insert(cat_list.at(i),i);
    cat_ids.clear();
    cat_list.clear();
    cat_ids.append(cmap.values());
    cat_list.append(cmap.keys());

    if(ui_update) // update categories to retranslate
    {
        ui->comboBoxCategory->setItemText(0,tr("All song categories"));
        for(int i(1); i <= ui->comboBoxCategory->count()-1;++i)
        {
            ui->comboBoxCategory->setItemText(i,cat_list.at(i-1));
        }

        // reset to selected category
        if(cur_cat_id==-1)
            cur_index=0;
        else
            cur_index= cat_ids.indexOf(cur_cat_id)+1;
        ui->comboBoxCategory->setCurrentIndex(cur_index);
    }
    else if(!ui_update) // initialize categories
    {
        ui->comboBoxCategory->addItem(tr("All song categories"));
        ui->comboBoxCategory->addItems(cat_list);
    }
}

void SongWidget::on_comboBoxCategory_currentIndexChanged(int index)
{
    if(index!=-1)
    {
        songs_model->emitLayoutAboutToBeChanged(); //prepeare to chage layout
        if(index==0)
            proxy_model->setCategoryFilter(index-1);
        else
            proxy_model->setCategoryFilter(cat_ids.at(index-1));
        songs_model->emitLayoutChanged();
    }
}

QList<Song> SongWidget::getPlaylistSongs()
{
    QList<Song> playlist_songs;

    for(int i(0); i<playlist_model->rowCount(); ++i)
    {
        playlist_songs.append(playlist_model->getSong(i));
    }

    return playlist_songs;
}

void SongWidget::loadPlaylistFromFile(QList<Song> songs)
{
    // remove all songs in playlist
//    for(int i(0); i<playlist_model->rowCount();++i)
//        playlist_model->removeRow(0);

    playlist_model->removeRows(0,playlist_model->rowCount());

    for(int i(0); i<songs.count(); ++i)
        playlist_model->addSong(songs.at(i));

    updateButtonStates();
}
