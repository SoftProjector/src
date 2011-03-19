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

#include "managedata.h"

ManageData::ManageData()
{
}


//***************************************
//****        Bibles                ****
//***************************************
Bibles::Bibles()
{
}

//***************************************
//****        BiblesModel                ****
//***************************************
BiblesModel::BiblesModel()
{
}

void BiblesModel::setBible(QList<Bibles> bibles)
{
    bible_list.clear();
    for (int i(0); i < bibles.size(); i++)
    {
        Bibles bible = bibles.at(i);
        bible_list.append(bible);
    }
    emit layoutChanged();
}

void BiblesModel::addBible(Bibles bible)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    bible_list.append(bible);
    endInsertRows();
}

Bibles BiblesModel::getBible(int row)
{
    return bible_list.at(row);
}

int BiblesModel::rowCount(const QModelIndex &parent) const
{
    return bible_list.count();
}

int BiblesModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant BiblesModel::data(const QModelIndex &index, int role) const
{
    if( index.isValid() && role == Qt::DisplayRole )
    {
        Bibles bible = bible_list.at(index.row());
        if( index.column() == 0 )
            return QVariant(bible.title);
    }
    return QVariant();
}

QVariant BiblesModel::headerData(int section,
                                 Qt::Orientation orientation,
                                 int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal )
    {
        switch(section) {
        case 0:
            return QVariant(tr("Title"));
        }
    }
    return QVariant();
}

bool BiblesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count-1);
    // Need to remove starting from the end:
    for(int i=row+count-1; i>=row; i--)
        bible_list.removeAt(i);
    endRemoveRows();
    return true;
}


//***************************************
//****        Songbook                ****
//***************************************
Songbook::Songbook()
{
}

//Songbook::Songbook(QString id)
//{
//    songbookId = id;
//}
//
//void Songbook::readData()
//{
//    QSqlQuery sq;
//    sq.exec("SELECT id, name, info FROM Songbooks WHERE id = '" + songbookId + "'");
//    sq.first();
//    title = sq.value(1).toString();
//    info = sq.value(2).toString();
//}

//***************************************
//****        Songbooks Model         ****
//***************************************
SongbooksModel::SongbooksModel()
{
}

void SongbooksModel::setSongbook(QList<Songbook> songbooks)
{
    songbook_list.clear();
    for (int i(0); i < songbooks.size(); i++)
    {
        Songbook songbook = songbooks.at(i);
        songbook_list.append(songbook);
    }
    emit layoutChanged();
}

void SongbooksModel::addSongbook(Songbook songbook)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    songbook_list.append(songbook);
    endInsertRows();
}

Songbook SongbooksModel::getSongbook(int row)
{
    return songbook_list.at(row);
}

int SongbooksModel::rowCount(const QModelIndex &parent) const
{
    return songbook_list.count();
}

int SongbooksModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant SongbooksModel::data(const QModelIndex &index, int role) const
{
    if( index.isValid() && role == Qt::DisplayRole )
    {
        Songbook songbook = songbook_list.at(index.row());
        if( index.column() == 0 )
            return QVariant(songbook.title);
        else if( index.column() == 1 )
            return QVariant(songbook.info);
    }
    return QVariant();
}

QVariant SongbooksModel::headerData(int section,
                                 Qt::Orientation orientation,
                                 int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal )
    {
        switch(section) {
        case 0:
            return QVariant(tr("Title"));
        case 1:
            return QVariant(tr("Information"));
        }
    }
    return QVariant();
}

bool SongbooksModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count-1);
    // Need to remove starting from the end:
    for(int i=row+count-1; i>=row; i--)
        songbook_list.removeAt(i);
    endRemoveRows();
    return true;
}

//***************************************
//****        Database                ****
//***************************************
Database::Database()
{
}

QList<Songbook> Database::getSongbooks()
{
    QList<Songbook> songbooki;
    Songbook songbook;
    QSqlQuery sq;
    sq.exec("SELECT id, name, info FROM Songbooks");
    while (sq.next())
    {
        songbook.songbookId = sq.value(0).toString();
        songbook.title = sq.value(1).toString();
        songbook.info = sq.value(2).toString();
        songbooki.append(songbook);
    }
    return songbooki;
}

QList<Bibles> Database::getBibles()
{
    QList<Bibles> bibles;
    Bibles bible;
        QSqlQuery sq;
    sq.exec("SELECT bible_name, id, abbreviation, information, right_to_left FROM BibleVersions");
    while (sq.next())
    {
        bible.title = sq.value(0).toString();
        bible.bibleId = sq.value(1).toString();
        bible.abbr = sq.value(2).toString();
        bible.info = sq.value(3).toString();
        int r = sq.value(4).toInt();
        if (r==0)
            bible.isRtoL = false;
        else if (r==1)
            bible.isRtoL = true;
        bibles.append(bible);
    }
    return bibles;
}
