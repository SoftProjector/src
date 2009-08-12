#include "song.h"
#include <QDebug>



QString clean(QString str)
{
    str.remove("\"");
    str.remove("\\");
    str.remove(".");
    str.remove(",");
    str.remove("?");
    str.remove("!");
    str.remove("'");
    str.remove("`");
    str.remove("-");
    str.remove("_");
    str.remove("@");
    str.remove("#");
    str.remove("$");
    str.remove("%");
    str.remove("^");
    str.remove("&");
    str.remove("*");
    str.remove("(");
    str.remove(")");
    str.remove("{");
    str.remove("}");
    str.remove("[");
    str.remove("]");
    str.remove("|");
    str.remove(";");
    str.remove(":");
    str.remove("/");
    str.remove(">");
    str.remove("<");
    str.remove("~");
    str.remove("=");
    str.remove("+");
    str = str.trimmed();
    return str;
}


Song::Song(int song_id, int song_num, QString song_title, QString song_sbornik)
{
    songID = song_id;
    num = song_num;
    title = song_title;
    sbornik = song_sbornik;
}
Song::Song()
{
}

SongsModel::SongsModel()
{
}


Song SongsModel::getSong(int row)
{
    return song_list.at(row);
}

void SongsModel::setSongs(QList<Song> songs)
{
    song_list.clear();
    for (int i = 0; i < songs.size(); i++) {
        Song song = songs.at(i);
        song_list.append(song);
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
    beginRemoveRows(QModelIndex(), row, row+count-1);
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
     if (!index.isValid() || role != Qt::DisplayRole)
         return QVariant();

     Song song = song_list.at(index.row());
     if( index.column() == 0 )
         return QVariant(song.num);
     else if( index.column() == 1 )
         return QVariant(song.title);
     else
         return QVariant(song.sbornik);
 }

QVariant SongsModel::headerData(int /* section */,
                                 Qt::Orientation /* orientation */,
                                 int role) const
{
    //if (role == Qt::SizeHintRole)
    //   return QSize(1, 1);
    return QVariant();
}



/*
void SongsModel::sort(int column, Qt::SortOrder order)
{
    QList<Song> sorted_song_list;
    if (order == Qt::AscendingOrder )
    {
        qDebug("ASCENDING ORDER");
        song_list
        for (int i = 0; i < song_list.size(); i++) {
            Song song = song_list.at(i);
            song_list.append(song);
    }
    }
    else
    {
        qDebug("DESCENDING ORDER");
    }
    song_list = sorted_song_list;
}*/

SongDatabase::SongDatabase()
{
}

bool SongDatabase::hasTitle(QString title)
{
    QSqlQuery sq;
    bool ys=false;
    sq.exec("SELECT id FROM Songs WHERE title = '" + clean(title) +"'");
    while (sq.next()){
        ys = true;
    }
    return ys;
}

void Song::saveUpdate()
{
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
}

void Song::saveNew()
{
    QSqlTableModel sq;
    sq.setTable("Songs");
    sq.insertRows(0,1);
    sq.setData(sq.index(0,1),clean(title));
    sq.setData(sq.index(0,2),category);
    sq.setData(sq.index(0,3),tune);
    sq.setData(sq.index(0,4),wordsBy);
    sq.setData(sq.index(0,5),musicBy);
    sq.setData(sq.index(0,6),songText);
    sq.setData(sq.index(0,7),font);
    sq.setData(sq.index(0,8),alingment);
    sq.setData(sq.index(0,9),background);
    sq.submitAll();
}

Song SongDatabase::getSong(int id)
{
    QSqlQuery sq;
    Song song;
    sq.exec("SELECT title, category, tune, words, music, song_text, font, alingment, background FROM Songs WHERE id = " + QString::number(id));
    while (sq.next()){
        song.songID = id;
        song.title = sq.value(0).toString();
        song.category = sq.value(1).toInt();
        song.tune = sq.value(2).toString();
        song.wordsBy = sq.value(3).toString();
        song.musicBy = sq.value(4).toString();
        song.songText = sq.value(5).toString();
        song.font = sq.value(6).toString();
        song.alingment = sq.value(7).toString();
        song.background = sq.value(8).toString();
        break;
    }
    return song;
}

Song SongDatabase::getSong(QString gtitle)
{
    QSqlQuery sq;
    Song song;
    sq.exec("SELECT id, category, tune, words, music, song_text, font, alingment, background FROM Songs WHERE title like '" + gtitle + "'");
    while (sq.next()){
        song.songID = sq.value(0).toInt();
        song.title = gtitle;
        song.category = sq.value(1).toInt();
        song.tune = sq.value(2).toString();
        song.wordsBy = sq.value(3).toString();
        song.musicBy = sq.value(4).toString();
        song.songText = sq.value(5).toString();
        song.font = sq.value(6).toString();
        song.alingment = sq.value(7).toString();
        song.background = sq.value(8).toString();
        break;
    }
    return song;
}

QStringList SongDatabase::getSongList(Song orig_song)
{    
    Song song = getSong(orig_song.title);
    // For some reason we need to re-get the song for songText to be set correctly
    QStringList song_list = formatSongList(song.songText);
    return song_list;
}

QStringList SongDatabase::formatSongList(QString song)
{
    QStringList formatedSong;
    QString text, text2, codec;
    QStringList split, songlist;
    QString chorus;
    bool has_chorus=0;
    int i(0),j(0),k(0),chor(0);

    songlist = song.split("@$");//songText.split("@$");

    while(i < songlist.size() )
    {
        text = songlist[i];
        //text.chop(1);
        split = text.split("@%"); // split the text into rythmic line seperated by @%
        k=split.size();
        text=""; // clear text
        j=0;
        text2=split[0];
        //text2.remove(6,10);
        if (text2.startsWith(codec.fromUtf8("Куплет")))
        {// Fill Verse
            while (j<k)
            {
                if (j==k-1) 
                    text += split[j];
                else 
                    text += split[j] + "\n";
                j++;
            }
            formatedSong += text.trimmed();
            if (has_chorus){
                formatedSong += chorus;
            }
        } 
        else if (text2.startsWith(codec.fromUtf8("Припев")))
        { // Fill Chorus
            while (j<k)
            {
                if (j==k-1)
                    text += split[j];
                else
                    text += split[j] + "\n";
                ++j;
            }
            chorus = text.trimmed();
            if (chorus.size()>3)
            {
                has_chorus=1; 
                chor++;
                if (chor ==1) 
                    formatedSong += chorus;
                else if (chor ==2)
                {
                    formatedSong[formatedSong.size()-1] = chorus;
                    chor-- ;
                }
            }
        }
        ++i;
    }
    text = formatedSong[formatedSong.size()-1];
    text += "\n* * *";
    formatedSong[formatedSong.size()-1] = text;
    return formatedSong; // Fill verse_list widget
}


QList<Song> SongDatabase::getSongs(QString sbornik)
{
    QList<Song> songs;

    if( sbornik == QString("ALL") ) {
        // Loads all titles in database
        QSqlQuery sq, sq2;

        sq.exec("SELECT song_id, song_number, sbornik_id FROM SongLink");
        while(sq.next())
        {
            int song_id = sq.value(0).toInt();
            int song_num = sq.value(1).toInt();
            QString sbornik = sq.value(2).toString();
            QString song_title;
            sq2.exec("SELECT title FROM Songs WHERE id = " + QString::number(song_id));
            sq2.first();
            song_title = sq2.value(0).toString();
            Song song = Song(song_id, song_num, song_title, sbornik);
            songs.append(song);
        }
    }
    else
    {
        QSqlQuery sq, sq1;

        sq.exec("SELECT song_id, song_number FROM SongLink WHERE sbornik_id like '"+sbornik+"'");
        while(sq.next())
        {
            int song_id = sq.value(0).toInt();
            int song_num = sq.value(1).toInt();
            QString song_title;
            sq1.exec("SELECT title FROM Songs WHERE id = " + QString::number(song_id));
            sq1.first();
            song_title = sq1.value(0).toString();
            Song song = Song(song_id, song_num, song_title, sbornik);
            songs.append(song);
        }
    } // end of not all sborniks
    return songs;
}

int SongDatabase::lastUser()
{
    int last;
    QList <int> lastInt;
    QSqlQuery sq;
    sq.exec("SELECT pvUser FROM songs WHERE pvUser > 0");
    while (sq.next())
        lastInt << sq.value(0).toInt();
    qSort(lastInt);
    if (lastInt.isEmpty())
        last=1;
    else
        last = lastInt.takeLast() + 1;
    return last;
}

bool SongDatabase::isUserOnly(int sId)
{
    int i(0);
    QSqlQuery sq;
    sq.exec("SELECT pv3300, pv2500, pv2001, pv2000a, pv2000b, pv1700, pvCt, uaPsalm, uaEpisni FROM songs WHERE id = '" + QString::number(sId) + "'");
    while (sq.next()){
        i += sq.value(0).toInt();
        i += sq.value(1).toInt();
        i += sq.value(2).toInt();
        i += sq.value(3).toInt();
        i += sq.value(4).toInt();
        i += sq.value(5).toInt();
        i += sq.value(6).toInt();
        i += sq.value(7).toInt();
        i += sq.value(8).toInt();
    }
    if(i==0)
        return true;
    else
        return false;
}

void SongDatabase::deleteSong(int song_id)
{
    QSqlQuery sq;
    sq.exec("DELETE FROM Songs WHERE id = " + QString::number(song_id) );
}
