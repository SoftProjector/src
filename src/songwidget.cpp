#include <QDebug>
#include "songwidget.h"
#include "ui_songwidget.h"

SongWidget::SongWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongWidget)
{
    Song t;
    ui->setupUi(this);
    allTitles = t.getTitle();
    loadTitles("ALL");
    sbornik = "pv3300";
    titleType=0;
    ui->listPreview->addItems(songPreview.getSongList(1, sbornik));
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
    if (tSbornik == "ALL") ui->listTitles->addItems(t.getTitle());
    else ui->listTitles->addItems(t.getTitle(tSbornik));
}

void SongWidget::showPreview(QString title)
{
    QStringList list = title.split(" - ");
    ui->listPreview->clear();
    if (list.size()==1)ui->listPreview->addItems(songPreview.getSongList(title));
    else ui->listPreview->addItems(songPreview.getSongList(list[1]));
    ui->listPreview->setCurrentRow(0);

}

void SongWidget::on_comboBoxPvNumber_currentIndexChanged(int index)
{
    if (index == 0) sbornik = "pv3300";
    else if (index==1) sbornik = "pv2001";
    else if (index==2) sbornik = "uaEpisni";
    else if (index==3) sbornik = "pvUser";

    titleType=1;
    Song t;
    ui->listTitles->clear();
    ui->listTitles->addItems(t.getTitle2(sbornik));

}

void SongWidget::on_spinBoxPvNumber_valueChanged(int value)
{
    if (!titleType==1){
        titleType=1;
        Song t;
        ui->listTitles->clear();
        ui->listTitles->addItems(t.getTitle2(sbornik));
    }
    bool empty = ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).isEmpty();
    if (!empty) ui->listTitles->setCurrentItem(ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).takeFirst());
    ui->listTitles->scrollTo(ui->listTitles->currentIndex(),QAbstractItemView::PositionAtCenter);
    //    selector=1;
//    setSong(value,1);
//    ui->listPreview->clear();
//    ui->listPreview->addItems(songPreview.getSongList(value, sbornik));
}


void SongWidget::on_listTitles_currentTextChanged(QString currentText)
{
    showPreview(currentText);
}

void SongWidget::on_listPlaylist_currentTextChanged(QString currentText)
{
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

}

void SongWidget::on_listPreview_itemDoubleClicked(QListWidgetItem* item)
{

}

void SongWidget::on_btnLive_clicked()
{
//    emit sendSong(songPreview.songList,songPreview.title,0);
//    if (!(ui->listTitles->selectedItems().count()==0))
        emit sendSong(songPreview.songList,ui->listTitles->currentItem()->text(),ui->listPreview->currentRow());
//    else emit sendSong(songPreview.songList,songPreview.title,ui->listPreview->currentRow());
}

void SongWidget::on_btnSort_clicked()
{
    titleType=0;
    ui->listTitles->clear();
    ui->listTitles->addItems(allTitles);
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
    if (ui->checkBoxAny->isChecked())
    {
        titleType =0;
        ui->listTitles->clear();
        QStringList tlist = text.split(" ");
        QStringList tlist2;
        if(tlist.count()==1){
            QString tx = tlist[0];
//            qDebug() << "tx1 = " + tx;
            tlist2 = allTitles.filter(tx.trimmed(),Qt::CaseInsensitive);
            ui->listTitles->addItems(tlist2);
        }
        if(tlist.count()==2)
        {
            QString tx = tlist[0];
//            qDebug() << "tx1 = " + tx;
            tlist2 = allTitles.filter(tx.trimmed(),Qt::CaseInsensitive);
            tx = tlist[1];
//            qDebug() << "tx2 = " + tx;
            ui->listTitles->addItems(tlist2.filter(tx,Qt::CaseInsensitive));
        }
    }
    else
    {
        if (!titleType==0)
        {
            titleType =0;
            ui->listTitles->clear();
            ui->listTitles->addItems(allTitles);
        }
        bool empty = ui->listTitles->findItems(text,Qt::MatchStartsWith).isEmpty();
        if (!empty) ui->listTitles->setCurrentItem(ui->listTitles->findItems(text,Qt::MatchStartsWith).takeFirst());
        ui->listTitles->scrollTo(ui->listTitles->currentIndex(),QAbstractItemView::PositionAtTop);
    }
}


void SongWidget::on_listPreview_doubleClicked(QModelIndex index)
{
//    if (!(ui->listTitles->selectedItems().count()==0))
        emit sendSong(songPreview.songList,ui->listTitles->currentItem()->text(),index.row());
//    else emit sendSong(songPreview.songList,songPreview.title,index.row());
}
