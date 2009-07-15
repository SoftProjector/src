#include <QDebug>
#include "songwidget.h"
#include "ui_songwidget.h"

SongWidget::SongWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongWidget)
{
//    Song t;
    ui->setupUi(this);
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
    Song t;
    ui->listTitles->clear();
    if (tSbornik == "ALL")
        ui->listTitles->addItems(t.getTitle());
    else ui->listTitles->addItems(t.getTitle(tSbornik));
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
    titleType=1;
    Song t;
    ui->listTitles->clear();
    // LOAD All songs alphabetically
    if (index == 0){
        allTitles=t.getTitle();
        ui->listTitles->addItems(allTitles);
        ui->spinBoxPvNumber->setEnabled(false);
        ui->listTitles->setCurrentRow(0);
    }
    // LOAD Песнь Возорждения 2800
    else if (index==1){
        ui->spinBoxPvNumber->setEnabled(true);
        if(ui->sort_box->isChecked())
            ui->listTitles->addItems(t.getTitle("pv3300"));
        else
            ui->listTitles->addItems(t.getTitle2("pv3300"));
        ui->listTitles->setCurrentRow(0);
    }
    // LOAD  Песнь Возорждения 2100
    else if (index==2){
        ui->spinBoxPvNumber->setEnabled(true);
        if(ui->sort_box->isChecked())
            ui->listTitles->addItems(t.getTitle("pv2001"));
        else
            ui->listTitles->addItems(t.getTitle2("pv2001"));
        ui->listTitles->setCurrentRow(0);
    }
    // LOAD Євангелські Пісні
    else if (index==3){
        ui->spinBoxPvNumber->setEnabled(true);
        if(ui->sort_box->isChecked())
            ui->listTitles->addItems(t.getTitle("uaEpisni"));
        else
            ui->listTitles->addItems(t.getTitle2("uaEpisni"));
        ui->listTitles->setCurrentRow(0);
    }
    // LOAD user sbornik
    else if (index==4){
        ui->spinBoxPvNumber->setEnabled(true);
        if(ui->sort_box->isChecked())
            ui->listTitles->addItems(t.getTitle("pvUser"));
        else
            ui->listTitles->addItems(t.getTitle2("pvUser"));
        ui->listTitles->setCurrentRow(0);
    }

}

void SongWidget::on_spinBoxPvNumber_valueChanged(int value)
{
    if (ui->sort_box->isChecked()){
    ui->sort_box->setChecked(false);
    }
    bool empty = ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).isEmpty();
    if (!empty)
        ui->listTitles->setCurrentItem(ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).takeFirst());
    ui->listTitles->scrollTo(ui->listTitles->currentIndex(),QAbstractItemView::PositionAtCenter);
    //    selector=1;
//    setSong(value,1);
//    ui->listPreview->clear();
//    ui->listPreview->addItems(songPreview.getSongList(value, sbornik));
}


void SongWidget::on_listTitles_currentTextChanged(QString currentText)
{
    isPlaylistTitle = false;
    showPreview(currentText);
}

void SongWidget::on_listPlaylist_currentTextChanged(QString currentText)
{
    isPlaylistTitle = true;
    showPreview(currentText);
}

void SongWidget::on_listTitles_itemDoubleClicked(QListWidgetItem* item)
{
    ui->listPlaylist->addItem(item->text());
    ui->listPlaylist->setCurrentRow(ui->listPlaylist->count()-1);
    ui->listPlaylist->setFocus();
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
        emit sendSong(songPreview.songList,ui->listTitles->currentItem()->text(),ui->listPreview->currentRow());
    }
}



void SongWidget::on_btnAddToPlaylist_clicked()
{
    ui->listPlaylist->addItem(ui->listTitles->currentItem()->text());
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
        ui->listTitles->clear();
        QStringList tlist = text.split(" ");
        QStringList tlist2;
        if(tlist.count()==1){
            QString tx = tlist[0];
            tlist2 = allTitles.filter(tx.trimmed(),Qt::CaseInsensitive);
            ui->listTitles->addItems(tlist2);
        }
        if(tlist.count()==2)
        {
            QString tx = tlist[0];
            tlist2 = allTitles.filter(tx.trimmed(),Qt::CaseInsensitive);
            tx = tlist[1];
            ui->listTitles->addItems(tlist2.filter(tx,Qt::CaseInsensitive));
        }

    }
    else
    {
        if (!allSongs)
        {
            allSongs=true;
            ui->listTitles->clear();
            ui->listTitles->addItems(allTitles);
        }
        bool empty = ui->listTitles->findItems(text,Qt::MatchStartsWith).isEmpty();
        if (!empty)
            ui->listTitles->setCurrentItem(ui->listTitles->findItems(text,Qt::MatchStartsWith).takeFirst());
        ui->listTitles->scrollTo(ui->listTitles->currentIndex(),QAbstractItemView::PositionAtTop);
    }
}


void SongWidget::on_listPreview_doubleClicked(QModelIndex index)
{
        emit sendSong(songPreview.songList,ui->listTitles->currentItem()->text(),index.row());
}

Song SongWidget::sendToEdit()
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
