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
    ui->songs_view->setColumnWidth(0, 55);
    ui->songs_view->setColumnWidth(1, 150);
    ui->songs_view->setColumnWidth(2, 70);
    ui->playlist_view->setColumnWidth(0, 55);
    ui->playlist_view->setColumnWidth(1, 150);
    ui->playlist_view->setColumnWidth(2, 70);

    proxy_model->setSbornikFilter("ALL");
    loadSborniks();

    isSpinboxEditing = false;
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
    if(focusInPlaylistTable)
    {
        enable_live = ui->playlist_view->currentIndex().isValid();
        enable_add = false;
        enable_remove = ( playlist_model->rowCount() > 0 );
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
    }

    ui->btnLive->setEnabled(enable_live);
    ui->btnAddToPlaylist->setEnabled(enable_add);
    ui->btnRemoveFromPlaylist->setEnabled(enable_remove);
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
    QList<Song> song_list = song_database.getSongs("ALL");

    //ui->song_num_spinbox->setEnabled(false);
    songs_model->setSongs(song_list);

    // update the song spin box and redraw the table:
    on_sbornik_menu_currentIndexChanged( ui->sbornik_menu->currentIndex() );
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
    QStringList song_list = song.getSongTextList();
    ui->listPreview->clear();
    ui->listPreview->addItems(song_list);
    ui->listPreview->setCurrentRow(0);
    preview_song = song;
}

void SongWidget::sendToProjector(Song song, int row)
{
    // Display the specified song text in the right-most column of softProjector:
    emit sendSong(song, row);
}


void SongWidget::on_sbornik_menu_currentIndexChanged(int index)
{
    // Called when a different sbornik is selected from the pull-down menu

    if( index == 0 )
    {
        proxy_model->setSbornikFilter("ALL");
        ui->song_num_spinbox->setEnabled(false);
    }
    else
    {
        QString sbornik_name = ui->sbornik_menu->currentText();
        proxy_model->setSbornikFilter(sbornik_name);
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
        if( s.num == value && s.sbornik_name == ui->sbornik_menu->currentText() )
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
    mb.setText("Could not find song with number " + QString::number(value) );
    mb.setWindowTitle("No such song");
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
    updateButtonStates();
}

void SongWidget::on_lineEditSearch_textEdited(QString text)
{
    // These two options are mutually exclusive:
    bool match_beginning = ui->match_beginning_box->isChecked();
    bool exact_match = ui->exact_match_box->isChecked();

    proxy_model->setFilterString(text, match_beginning, exact_match);
    songs_model->emitLayoutChanged(); // forces the view to redraw


    // If 'text' is of numeric value, then select row with exact match
    bool is_int;
    int n = text.toInt(&is_int,10);
    if (is_int)
    {
        // Look for a song with number <value>. Select it and scroll to show it.
        for (int i = 0; i < songs_model->song_list.size(); i++)
        {
            Song s = songs_model->song_list.at(i);
            if( s.num == n )
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
                    sendToPreview(s);
                }
                return;
            }
            //if( s.num > max_num )
            //    max_num = s.num;
        }
    }
    else // Select first row and scroll to top;
    {
        // Filter string is not an integer

        ui->songs_view->selectRow(0);
        ui->songs_view->scrollToTop();
    }

    updateButtonStates();
}

Song SongWidget::getSongToEdit()
{
    return preview_song;
}

void SongWidget::on_match_beginning_box_toggled(bool checked)
{
    // Update the filter:
    if(checked)
        ui->exact_match_box->setChecked(false);
    QString new_text = ui->lineEditSearch->text();
    on_lineEditSearch_textEdited(new_text);
}

void SongWidget::on_exact_match_box_toggled(bool checked)
{
    // Update the filter:
    if(checked)
        ui->match_beginning_box->setChecked(false);
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

void SongWidget::updateSborniks()
{
    // Reload the sbornik and reselect the one that used to be selected
    // if it's still available, otherwise show all sborniks

    QString current_sbornik = ui->sbornik_menu->currentText();
    QString item0 = ui->sbornik_menu->itemText(0);
    ui->sbornik_menu->clear();
    ui->sbornik_menu->addItem(item0);
    loadSborniks();

    int new_index = ui->sbornik_menu->findText(current_sbornik);
    if( new_index == -1 )
        new_index = 0; // All sborniks

    ui->sbornik_menu->setCurrentIndex(new_index);
}

void SongWidget::deleteSong()
{
    song_database.deleteSong(currentSong().songID);
    updateSborniks();
}
