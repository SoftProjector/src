#include <QDebug>
#include "songwidget.h"
#include "ui_songwidget.h"

SongWidget::SongWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongWidget)
{
//    Sbornik t;
    ui->setupUi(this);
    songs_model = new SongsModel;
    ui->songs_view->setModel(songs_model);
    // Decrease the row height:
    ui->songs_view->resizeRowsToContents();

//    allTitles = t.getTitle();
    //loadTitles("ALL");
//    sbornik = "pv3300";
    on_comboBoxPvNumber_currentIndexChanged(0);
//    titleType=0;

//    ui->listPreview->addItems(songPreview.getSongList(1, sbornik));
//    toShow = new SoftProjector();
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
    Sbornik t;
    if (tSbornik == "ALL")
        songs_model->setTitles(t.getTitle());
    else
        songs_model->setTitles(t.getTitle(tSbornik, false));
}

QString SongWidget::currentTitle()
{
    // Returns the title of the selected song
    QModelIndex current_index = ui->songs_view->currentIndex();
    int current_row = current_index.row();
    QString title = songs_model->getTitle(current_row);
    return title;
}

void SongWidget::selectMatchingSong(QString text)
{
    bool startonly = ui->match_beginning_box->isChecked();
    // Look for a song matching <text>. Select it and scroll to show it.
    for (int i = 0; i < songs_model->song_list.size(); i++)
    {
        QString s = songs_model->song_list.at(i);
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
        ui->listPreview->addItems(songPreview.getSongList(title));
    else
        ui->listPreview->addItems(songPreview.getSongList(list[1]));
    ui->listPreview->setCurrentRow(0);

}

void SongWidget::on_comboBoxPvNumber_currentIndexChanged(int index)
{
    // Called when a different sbornik is selected from the pull-down menu

    titleType=1;
    Sbornik t;
    bool sort = ui->sort_box->isChecked();

    if (index == 0){
        // LOAD All songs alphabetically
        allTitles=t.getTitle();
        songs_model->setTitles(allTitles);
        ui->spinBoxPvNumber->setEnabled(false);
        ui->songs_view->selectRow(0);
    }
    else if (index==1){
        // LOAD Песнь Возорждения 2800
        ui->spinBoxPvNumber->setEnabled(true);
        songs_model->setTitles(t.getTitle("pv3300", sort));
        ui->songs_view->selectRow(0);
    }
    else if (index==2){
        // LOAD  Песнь Возорждения 2100
        ui->spinBoxPvNumber->setEnabled(true);
        songs_model->setTitles(t.getTitle("pv2001", sort));
        ui->songs_view->selectRow(0);
    }
    else if (index==3){
    	// LOAD Євангелські Пісні
        ui->spinBoxPvNumber->setEnabled(true);
        songs_model->setTitles(t.getTitle("uaEpisni", sort));
        ui->songs_view->selectRow(0);
    }
    else if (index==4){
	// LOAD user sbornik
        ui->spinBoxPvNumber->setEnabled(true);
        songs_model->setTitles(t.getTitle("pvUser", sort));
        ui->songs_view->selectRow(0);
    }
    ui->songs_view->update();
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
//    ui->listPreview->addItems(songPreview.getSongList(value, sbornik));
}




void SongWidget::on_listPlaylist_currentTextChanged(QString currentText)
{
    isPlaylistTitle = true;
    showPreview(currentText);
}



void SongWidget::on_listPlaylist_itemDoubleClicked(QListWidgetItem* item)
{
    emit sendSong(songPreview.songList,item->text(),0);
}

void SongWidget::on_btnLive_clicked()
{
    if(isPlaylistTitle){
        emit sendSong(songPreview.songList,ui->listPlaylist->currentItem()->text(),ui->listPreview->currentRow());;
    }
    else{
        emit sendSong(songPreview.songList, currentTitle(), ui->listPreview->currentRow());
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
    if (ui->comboBoxPvNumber->currentIndex()>0)
        ui->comboBoxPvNumber->setCurrentIndex(0);
    if (!ui->match_beginning_box->isChecked())
    {
        allSongs=false;
        QStringList tlist = text.split(" ");
        QStringList tlist2;
        if(tlist.count()==1){
            QString tx = tlist[0];
            tlist2 = allTitles.filter(tx.trimmed(),Qt::CaseInsensitive);
            songs_model->setTitles(tlist2);
        }
        if(tlist.count()==2)
        {
            QString tx = tlist[0];
            tlist2 = allTitles.filter(tx.trimmed(),Qt::CaseInsensitive);
            tx = tlist[1];
            songs_model->setTitles(tlist2.filter(tx,Qt::CaseInsensitive));
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
}


void SongWidget::on_listPreview_doubleClicked(QModelIndex index)
{
    QString title = currentTitle();
    emit sendSong(songPreview.songList, title, index.row());
}

Sbornik SongWidget::sendToEdit()
{
    return songPreview;
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
