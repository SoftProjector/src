/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2010  Vladislav Kobzar and Matvey Adzhigirey
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

#include "songcounter.h"
#include "ui_songcounter.h"

SongCounter::SongCounter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SongCounter)
{
    ui->setupUi(this);
    load_counts();

    // Modify the column widths:
    ui->countTable->setColumnWidth(0, 250);
    ui->countTable->setColumnWidth(1, 100);
    // Decrease the row height:
    ui->countTable->resizeRowsToContents();

}

SongCounter::~SongCounter()
{
    delete ui;
}

void SongCounter::on_closeButton_clicked()
{
    close();
}

void SongCounter::on_resetButton_clicked()
{
    // Code to reset counter to 0
    QSqlQuery sq;
    sq.exec("UPDATE Songs SET count = 0 WHERE count > 0");

    load_counts();
}

void SongCounter::addSongCount(Song song)
{
    int id = song.songID;
    int current_count(0);

    // get current song count
    QSqlQuery sq;
    sq.exec("SELECT count FROM Songs WHERE id = '" + QString::number(id) + "' ");
    sq.first();
    current_count = sq.value(0).toInt();
    sq.clear();

    // add one count to song
    ++current_count;
    sq.exec("UPDATE Songs SET count = " + QString::number(current_count) + " WHERE id = " + QString::number(id));
}

void SongCounter::load_counts()
{
    song_counts = getSongCounts();
    songCounterModel = new SongCounterModel;
    songCounterModel->setCounter(song_counts);
    ui->countTable->setModel(songCounterModel);
}



//***********************************
//***********************************
QList<Counter> SongCounter::getSongCounts()
{
    QList<Counter> song_counts;
    Counter song_count;
    QSqlQuery sq;
    sq.exec("SELECT title, count FROM Songs WHERE count > 0");
    while (sq.next())
    {
        song_count.title = sq.value(0).toString();
        song_count.count = sq.value(1).toInt();
        song_counts.append(song_count);
    }
    return song_counts;
}

//***************************************
//****    Song Use Counter Model     ****
//***************************************
Counter::Counter()
{
}

SongCounterModel::SongCounterModel()
{
}

void SongCounterModel::setCounter(QList<Counter> song_counts)
{
    song_count_list.clear();
    for (int i(0); i < song_counts.size(); i++)
    {
        Counter song_count = song_counts.at(i);
        song_count_list.append(song_count);
    }
    emit layoutChanged();
}

void SongCounterModel::addCounter(Counter song_count)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    song_count_list.append(song_count);
    endInsertRows();
}

Counter SongCounterModel::getSongCount(int row)
{
    return song_count_list.at(row);
}

int SongCounterModel::rowCount(const QModelIndex &parent) const
{
    return song_count_list.count();
}

int SongCounterModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant SongCounterModel::data(const QModelIndex &index, int role) const
{
    if( index.isValid() && role == Qt::DisplayRole )
    {
        Counter song_count = song_count_list.at(index.row());
        if( index.column() == 0 )
            return QVariant(song_count.title);
        else if( index.column() == 1 )
            return QVariant(song_count.count);
    }
    return QVariant();
}

QVariant SongCounterModel::headerData(int section,
                                 Qt::Orientation orientation,
                                 int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal )
    {
        switch(section) {
        case 0:
            return QVariant(tr("Song Title"));
        case 1:
            return QVariant(tr("Times showed"));
        }
    }
    return QVariant();
}

bool SongCounterModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count-1);
    // Need to remove starting from the end:
    for(int i=row+count-1; i>=row; i--)
        song_count_list.removeAt(i);
    endRemoveRows();
    return true;
}
