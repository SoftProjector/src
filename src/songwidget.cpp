#include <QDebug>
#include "songwidget.h"
#include "ui_songwidget.h"

SongWidget::SongWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongWidget)
{
    ui->setupUi(this);
    songs_model = new SongsModel;
    ui->songs_view->setModel(songs_model);
    // Decrease the row height:
    ui->songs_view->resizeRowsToContents();
    on_comboBoxPvNumber_currentIndexChanged(0);
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
    songs_model->setSongs(t.getSongs("ALL", false));
}

QString SongWidget::currentTitle()
{
    // Returns the title of the selected song
    QModelIndex current_index = ui->songs_view->currentIndex();
    int current_row = current_index.row();
    QString title = songs_model->getSong(current_row).title;
    return title;
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

void SongWidget::on_comboBoxPvNumber_currentIndexChanged(int index)
{
    // Called when a different sbornik is selected from the pull-down menu

    titleType=1;
    SongDatabase t;
    bool sort = ui->sort_box->isChecked();

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

    ui->spinBoxPvNumber->setEnabled(sbornik == QString("ALL"));

    songs_model->setSongs(t.getSongs(sbornik, sort));

    ui->songs_view->selectRow(0);
    // Re-draw the songs table:
    ui->songs_view->viewport()->repaint();
}

void SongWidget::on_spinBoxPvNumber_valueChanged(int value)
{
    if (ui->sort_box->isChecked()){
        ui->sort_box->setChecked(false);
    }
    selectMatchingSong(QString::number(value));
    //    selector=1;
//    setSong(value,1);
//    ui->listPreview->clear();
//    ui->listPreview->addItems(playlist.getSongList(value, sbornik));
}




void SongWidget::on_listPlaylist_currentTextChanged(QString currentText)
{
    isPlaylistTitle = true;
    showPreview(currentText);
}



void SongWidget::on_listPlaylist_itemDoubleClicked(QListWidgetItem* item)
{
    emit sendSong(playlist.songList,item->text(),0);
}

void SongWidget::on_btnLive_clicked()
{
    if(isPlaylistTitle){
        emit sendSong(playlist.songList,ui->listPlaylist->currentItem()->text(),ui->listPreview->currentRow());;
    }
    else{
        emit sendSong(playlist.songList, currentTitle(), ui->listPreview->currentRow());
    }
}



void SongWidget::on_btnAddToPlaylist_clicked()
{
    ui->listPlaylist->addItem(currentTitle());
    ui->listPlaylist->setCurrentRow(ui->listPlaylist->count()-1);
    ui->listPlaylist->setFocus();
}

void SongWidget::on_btnRemoveFromPlaylist_clicked()
{
    ui->listPlaylist->takeItem(ui->listPlaylist->currentRow());
}

void SongWidget::on_lineEditSearch_textEdited(QString text)
{
    /* FIXME!!!!
    if (ui->comboBoxPvNumber->currentIndex()>0)
        ui->comboBoxPvNumber->setCurrentIndex(0);

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


void SongWidget::on_listPreview_doubleClicked(QModelIndex index)
{
    QString title = currentTitle();
    emit sendSong(playlist.songList, title, index.row());
}

Song SongWidget::getSongToEdit()
{
    // FIXME
    Song song(0, "", "");
    return song;
    //return playlist;
}

void SongWidget::on_match_beginning_box_toggled(bool checked)
{
    QString new_text = ui->lineEditSearch->text();
    on_lineEditSearch_textEdited(new_text);
}

void SongWidget::on_sort_box_toggled(bool checked)
{
//    QString new_text = ui->lineEditSearch->text();
//    SongWidget::on_lineEditSearch_textEdited(new_text);
    on_comboBoxPvNumber_currentIndexChanged(ui->comboBoxPvNumber->currentIndex());
}


void SongWidget::on_songs_view_activated(QModelIndex index)
{
    // Called when a new song is selected
    isPlaylistTitle = false;
    showPreview(currentTitle());
}

void SongWidget::on_songs_view_doubleClicked(QModelIndex index)
{
    // Caled when a song is double-clicked

    ui->listPlaylist->addItem(currentTitle());
    ui->listPlaylist->setCurrentRow(ui->listPlaylist->count()-1);
    ui->listPlaylist->setFocus();
}
