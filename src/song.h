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

#ifndef SONG_H
#define SONG_H
#include <QtGui>
#include <QtSql>

QString clean(QString str);
bool isStanzaTitle(QString string);
bool isStanzaVerseTitle(QString string);
bool isStanzaAndVerseTitle(QString string);
bool isStanzaRefrainTitle(QString string);
bool isStanzaAndRefrainTitle(QString string);
bool isStanzaSlideTitle(QString string);

class Song
// Class for storing song information: number, name, songbook
// The instance of this class is specific to a song & songbook.
{
 public:
    Song();
    Song(int id);
    Song(int id, int num, QString songbook_id);
    void readData();
    int num; // Number of the song in the specified songbook
    QString title;
    QString songbook_id;
    QString songbook_name;

    void saveUpdate();
    void saveNew();
    QStringList getSongTextList();
    QString getSongbookName();
    bool isValid();

//private:
    int songID; // Database ID of this song
    int category;
    int language;
    int favorite;
    QString tune;
    QString wordsBy;
    QString musicBy;
    QString songText;
    QString font;
    QString alingment;
    QString background;
};


class SongsModel : public QAbstractTableModel
// Class for storing the data for the song table
{
    Q_OBJECT
    Q_DISABLE_COPY(SongsModel)
public:
    SongsModel();
    void setSongs(QList<Song> songs);
    void addSong(Song song);
    Song getSong(int row);
    Song getSong(QModelIndex index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    //void sort(int column, Qt::SortOrder order);

    bool removeRows( int row, int count, const QModelIndex & parent = QModelIndex() );
    QList<Song> song_list;
    void emitLayoutChanged();
    void emitLayoutAboutToBeChanged();
    void updateSongFromDatabase(int songid);
};


class SongProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    SongProxyModel(QObject *parent = 0);
    void setFilterString(QString new_string, bool new_match_beginning, bool new_exact_match);
    void setSongbookFilter(QString new_songbook);
    void setCategoryFilter(int category);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    QString filter_string, songbook_filter, category_filter;
    bool match_beginning, exact_match;
};


class SongDatabase
{
public:
    SongDatabase();
    void addSongbook(QString name, QString info);
    void saveUpdate();
    void saveNew();
    void deleteSong(int songId);
    QString getSongbookIdStringFromName(QString songbook_name);
    Song getSong(int id);
    QList<Song> getSongs(QString songbook_id);
    int lastUser(QString songbook_id);
//private:
};

#endif // SONG_H
