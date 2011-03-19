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

#include "song.h"
#include <QDebug>



QString clean(QString str)
{
    //str.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]")));
    // NOTE: Modified this string to make it work on Mac OS X:
    str.remove(QRegExp(QString::fromUtf8("[-`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\[]")));
    str = str.simplified();
    return str;
}

Song::Song()
{
}

Song::Song(int id)
{
    songID = id;
}

Song::Song(int song_id, int song_num, QString song_songbook)
{
    songID = song_id;
    num = song_num;
    songbook_id = song_songbook;
}

void Song::readData()
{
    QSqlQuery sq;
    sq.exec("SELECT title, category, tune, words, music, song_text, font, alingment, background FROM Songs WHERE id = " + QString::number(songID));
    sq.first();
    title = sq.value(0).toString();
    category = sq.value(1).toInt();
    tune = sq.value(2).toString();
    wordsBy = sq.value(3).toString();
    musicBy = sq.value(4).toString();
    songText = sq.value(5).toString();
    font = sq.value(6).toString();
    alingment = sq.value(7).toString();
    background = sq.value(8).toString();

    if( !songbook_id.isEmpty() )
    {
        sq.exec("SELECT name FROM Songbooks WHERE id = " + songbook_id );
        sq.first();
        songbook_name = sq.value(0).toString();
    }
}

QStringList Song::getSongTextList()
{
    // This function prepares a song list that will be shown in the song preview and show list.
    // It will it will automatically prepare correct sining order of verses and choruses.
    QStringList formatedSong; // List container for correctely ordered item. This item will be returned.
    QString text, text2, codec;
    QStringList split, songlist;
    QStringList chorus;
    bool has_chorus=false;
    bool has_vstavka=false;
    int pnum = 0;
    int chor = 0;
    int chorus_block_count=0; // Chorus slide counter.

    songlist = songText.split("@$");// Splits song text line that from database into
                                    // stansas which were delimited by "@$"
    
    while(pnum < songlist.size() )
    {
        text = songlist[pnum];
        split = text.split("@%"); // Split each stansa into rythmic line delimited by @%
        int split_size = split.size();
        text=""; // clear text
        int j = 0;
        text2 = split[0];

        // From here on, the program will determine what each stasa it: Verse, Chorus or Insert
        if (text2.startsWith("Verse") || text2.startsWith(QString::fromUtf8("Куплет")))
        {// Fill Verse
            while (j<split_size) // convert form list to string
            {
                if (j==split_size-1)
                    text += split[j];
                else
                    text += split[j] + "\n";
                ++j;
            }
            formatedSong += text.trimmed(); // add Verse stansa to the formated list
            if (has_chorus){ // add Chorus stansa to the formated list if it exists
                formatedSong.append(chorus);
            }
            has_vstavka=false;
        }
        else if (text2.startsWith("&Verse") || text2.startsWith(QString::fromUtf8("&Куплет")))
        {// Fill Additional parts of the verse
            text2.remove("&"); // remove '&' from stansa title
            text += text2 + "\n";
            ++j;
            while (j < split_size) // convert form list to string
            {
                if (j==split_size-1)
                    text += split[j];
                else
                    text += split[j] + "\n";
                ++j;
            }
            if (has_chorus) // it chorus esits, this means that it was added to the formated list
                            // and needs to be removed before adding addintion Veres stansas to formated list
            {
                formatedSong.removeLast();
                int i(1);
                while (i<chorus_block_count)
                {
                    ++i;
                    formatedSong.removeLast();
                }
            }
            formatedSong += text.trimmed(); // add Verse stansa to the formated list
            if (has_chorus){ // add Chorus stansa to the formated list if it exists
                formatedSong.append(chorus);
            }
            has_vstavka=false;
        }
        else if (text2.startsWith("Slide") || text2.startsWith("Insert") || text2.startsWith(QString::fromUtf8("Вставка"))
            || text2.startsWith("Intro") || text2.startsWith(QString::fromUtf8("Вступление"))
            || text2.startsWith("Ending") || text2.startsWith(QString::fromUtf8("Окончание")))
        {// Fill Insert
            while (j < split_size) // convert form list to string
            {
                if (j==split_size-1)
                    text += split[j];
                else
                    text += split[j] + "\n";
                ++j;
            }
            formatedSong += text.trimmed(); // Add Insert stansa to the formated list.
                                            // Chorus is not added to Insert, if one is needed,
                                            // it should be added when song is edited, otherwise
                                            // there is no difirence between Veres and Insert
            has_vstavka=true;
        }
        else if (text2.startsWith("Chorus") || text2.startsWith("Refrain") || text2.startsWith(QString::fromUtf8("Припев")))
        { // Fill Chorus

            while (j<split_size)  // convert form list to string
            {
                if (j==split_size-1)
                    text += split[j];
                else
                    text += split[j] + "\n";
                ++j;
            }
            chorus.clear(); // clear chorus list
            chorus += text.trimmed(); // add chorus text to chorus list
            has_chorus=true; // let getSongTextList function know that chorus exists
            ++chor;
            if (chor ==1) // if first Chorus, add chorus to formated list
                formatedSong.append(chorus);
            else if ((chor ==2) && !has_vstavka ) // if second chorus and Insert was not added
                                                  // remove exising chorus
                                                  // and add new chorus to formated list
            {
                formatedSong.removeLast();
                if (chorus_block_count>1)
                {
                    int i(1);
                    while (i<chorus_block_count)
                    {
                        ++i;
                        formatedSong.removeLast();
                    }
                }

                formatedSong.append(chorus);
                chor-- ;
            }
            else if ((chor ==2) && has_vstavka ) // if second chorus and Insert was added
                                                  // and add new chorus to formated list
            {
                formatedSong += chorus;
                chor-- ;
            }
            chorus_block_count=1; // set chorus_block_count to 1 because its the first chorus item count
            has_vstavka=false;
        }
        else if (text2.startsWith("&Chorus") || text2.startsWith("&Refrain") || text2.startsWith(QString::fromUtf8("&Припев")))
        { // Fill other chorus parts to Chorus block
            ++chorus_block_count; // increase chorus block count
            text2.remove("&");  // remove '&' from stansa title
            text += text2 +"\n";
            ++j;
            while (j<split_size) // convert form list to string
            {
                if (j==split_size-1)
                    text += split[j];
                else
                    text += split[j] + "\n";
                ++j;
            }
            chorus += text.trimmed();

            if (chorus_block_count>1) // remove existing chorus parts
            {
                int i(1);
                while (i<chorus_block_count)
                {
                    ++i;
                    formatedSong.removeLast();
                }
            }
            formatedSong.append(chorus); // replace removed chorus parts with complete chorus list

        }
        ++pnum;
    }
    return formatedSong;
}

QString Song::getSongbookName()
{
    if( songbook_name.isEmpty() )
        readData();
    return songbook_name;
}

SongsModel::SongsModel()
{
}

Song SongsModel::getSong(int row)
{
    return song_list.at(row);
}

Song SongsModel::getSong(QModelIndex index)
{
    return song_list.at(index.row());
}


void SongsModel::setSongs(QList<Song> songs)
{
    song_list.clear();
    for (int i = 0; i < songs.size(); i++) {
        Song song = songs.at(i);
        song_list.append(song);
    }
    emit layoutChanged();
}

void SongsModel::updateSongFromDatabase(int songid)
{
    for( int i=0; i < song_list.size(); i++) {
        Song *song = (Song*)&(song_list.at(i));
        if( song->songID == songid )
        {
            //qDebug() << "found song. updating data...";
            song->readData();
            emitLayoutChanged(); // To redraw the table
            return;
        }
    }
}


void SongsModel::addSong(Song song)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    song_list.append(song);
    endInsertRows();
}

bool SongsModel::removeRows( int row, int count, const QModelIndex & parent)
{
    beginRemoveRows(parent, row, row+count-1);
    // Need to remove starting from the end:
    for(int i=row+count-1; i>=row; i--)
        song_list.removeAt(i);
    endRemoveRows();
    return true;
}

int SongsModel::rowCount(const QModelIndex &parent) const
{
    return song_list.count();
}

int SongsModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant SongsModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() )
        return QVariant();

    if( role == Qt::DisplayRole )
    {
        Song song = song_list.at(index.row());
        if( index.column() == 0 )
            return QVariant(song.num);
        else if( index.column() == 1 )
            return QVariant(song.title);
        else
            return QVariant(song.getSongbookName());
    }
    return QVariant();
}

QVariant SongsModel::headerData(int section,
                                Qt::Orientation orientation,
                                int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal )
    {
        switch(section) {
        case 0:
            return QVariant(tr("Num"));
        case 1:
            return QVariant(tr("Title"));
        case 2:
            return QVariant(tr("Songbook"));
        }
    }
    return QVariant();
}

void SongsModel::emitLayoutChanged()
{
    emit layoutChanged();
}

void SongsModel::emitLayoutAboutToBeChanged()
{
    emit layoutAboutToBeChanged();
}

SongProxyModel::SongProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

void SongProxyModel::setFilterString(QString new_string, bool new_match_beginning, bool new_exact_match)
{
    filter_string = new_string;
    match_beginning = new_match_beginning;
    exact_match = new_exact_match;
}

void SongProxyModel::setSongbookFilter(QString new_songbook)
{
    songbook_filter = new_songbook;
}


bool SongProxyModel::filterAcceptsRow(int sourceRow,
                                      const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);

    QString str0 = sourceModel()->data(index0).toString();
    QString str1 = sourceModel()->data(index1).toString();
    QString str2 = sourceModel()->data(index2).toString();


    // Exclude rows that are not part of the selected songbook:
    if( songbook_filter != "ALL" )
        if( str2 != songbook_filter )
            return false;

    if( filter_string.isEmpty() )
        // No filter specified
        return true;

    if(exact_match)
        return ( str0.compare(filter_string, Qt::CaseInsensitive) == 0
                 || str1.compare(filter_string, Qt::CaseInsensitive) == 0 );
    else if(match_beginning)
        return (str0.startsWith(filter_string)
                || str1.startsWith(filter_string, Qt::CaseInsensitive) );
    else
        return (str0.contains(filter_string)
                || str1.contains(filter_string, Qt::CaseInsensitive) );

}


SongDatabase::SongDatabase()
{
}

void Song::saveUpdate()
{
    // Update song information
    QSqlTableModel sq;
    QSqlRecord sr;
    sq.setTable("Songs");
    sq.setFilter("id = " + QString::number(songID));
    sq.select();

    sr = sq.record(0);
    sr.setValue(1,clean(title));
    sr.setValue(2,category);
    sr.setValue(3,tune);
    sr.setValue(4,wordsBy);
    sr.setValue(5,musicBy);
    sr.setValue(6,songText);
    sr.setValue(7,font);
    sr.setValue(8,alingment);
    sr.setValue(9,background);
    sq.setRecord(0,sr);
    sq.submitAll();

    // Update SongLink information
    QSqlQuery sqq;
    sqq.exec("UPDATE SongLink SET song_number = "
             + QString::number(num) + " WHERE songbook_id = "
             + songbook_id + " AND song_id = " + QString::number(songID));
}

void Song::saveNew()
{
    // Add a new song
    QSqlTableModel sqt;
    sqt.setTable("Songs");
    sqt.insertRows(0,1);
    sqt.setData(sqt.index(0,1),clean(title));
    sqt.setData(sqt.index(0,2),category);
    sqt.setData(sqt.index(0,3),tune);
    sqt.setData(sqt.index(0,4),wordsBy);
    sqt.setData(sqt.index(0,5),musicBy);
    sqt.setData(sqt.index(0,6),songText);
    sqt.setData(sqt.index(0,7),font);
    sqt.setData(sqt.index(0,8),alingment);
    sqt.setData(sqt.index(0,9),background);
    sqt.submitAll();

    // Connect song with songbook
    QSqlQuery sq;
    QString sid = "";
    sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Songs'");
    while (sq.next())
        sid = sq.value(0).toString();
    sq.clear();

    sq.exec("INSERT into SongLink (songbook_id, song_id, song_number) VALUES ('"
            + songbook_id.trimmed() + "',"
            + sid + ","
            + QString::number(num) + ")");
}

Song SongDatabase::getSong(int id)
{
    Song song(id);
    song.readData();
    return song;
}




QList<Song> SongDatabase::getSongs(QString songbook_id)
{
    QList<Song> songs;

    if( songbook_id == QString("ALL") ) {
        // Loads all titles in database
        QSqlQuery sq, sq2;

        sq.exec("SELECT song_id, song_number, songbook_id FROM SongLink");
        while(sq.next())
        {
            int song_id = sq.value(0).toInt();
            int song_num = sq.value(1).toInt();
            QString songbook_id = sq.value(2).toString();
            QString song_title;
            sq2.exec("SELECT title FROM Songs WHERE id = " + QString::number(song_id));
            sq2.first();
            song_title = sq2.value(0).toString();
            Song song = Song(song_id, song_num, songbook_id);
            song.readData();
            songs.append(song);
        }
    }
    else
    {
        QSqlQuery sq, sq1;

        sq.exec("SELECT song_id, song_number FROM SongLink WHERE songbook_id = "+songbook_id);
        while(sq.next())
        {
            int song_id = sq.value(0).toInt();
            int song_num = sq.value(1).toInt();
            Song song = Song(song_id, song_num, songbook_id);
            song.readData();
            songs.append(song);
        }
    } // end of not all songbooks
    return songs;
}

int SongDatabase::lastUser(QString songbook_id)
{
    int last;
    QList <int> lastInt;
    QSqlQuery sq;
    sq.exec("SELECT song_number FROM SongLink WHERE songbook_id = " +songbook_id);
    while (sq.next())
        lastInt << sq.value(0).toInt();
    qSort(lastInt);
    if (lastInt.isEmpty())
        last=1;
    else
        last = lastInt.takeLast() + 1;
    return last;
}

void SongDatabase::deleteSong(int song_id)
{
    QSqlQuery sq;
    sq.exec("DELETE FROM Songs WHERE id = " + QString::number(song_id) );
    sq.clear();
    sq.exec("DELETE FROM SongLink WHERE song_id = " + QString::number(song_id) );
}

QString SongDatabase::getSongbookIdStringFromName(QString songbook_name)
{
    QSqlQuery sq;
    sq.exec("SELECT id FROM Songbooks WHERE name = '" + songbook_name +"'"); //ilya
    sq.first();
    return sq.value(0).toString();
}

void SongDatabase::addSongbook(QString name, QString info)
{
    QSqlTableModel sqt;
    sqt.setTable("Songbooks");
    sqt.insertRows(0,1);
    sqt.setData(sqt.index(0,1),name.trimmed());
    sqt.setData(sqt.index(0,2),info);
    sqt.setData(sqt.index(0,3),1);
    sqt.submitAll();
}

