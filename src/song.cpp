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

void SongsModel::addSong(Song song)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    song_list.append(song);
    endInsertRows();
}

bool SongsModel::removeRows( int row, int count, const QModelIndex & parent)
{
    qDebug("begin");
    qDebug() << row;
    qDebug() << count;
    qDebug() << row+count-1;
    qDebug("row count:");
    qDebug() << rowCount();

    beginRemoveRows(parent, row, row+count-1);
    // Need to remove starting from the end:
    for(int i=row+count-1; i>=row; i--)
    {
        qDebug("Removing row:");
        qDebug() << i;
        song_list.removeAt(i);
    }
    endRemoveRows();
    qDebug("end");
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
     if (!index.isValid() )
         return QVariant();

     if ( role != Qt::DisplayRole )
         return QVariant();

     Song song = song_list.at(index.row());
     if( index.column() == 0 )
         return QVariant(song.num);
     else if( index.column() == 1 )
         return QVariant(song.title);
     else
         return QVariant(song.sbornik);
 }

QVariant SongsModel::headerData(int section,
                                 Qt::Orientation orientation,
                                 int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch(section) {
    case 0:
        return QVariant("Num");
    case 1:
        return QVariant("Title");
    case 2:
        return QVariant("Sbornik");
    }
}

void SongsModel::emitLayoutChanged()
{
    emit layoutChanged();
}


SongProxyModel::SongProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

void SongProxyModel::setFilterString(QString new_string, bool new_match_beginning)
{
    filter_string = new_string;
    match_beginning = new_match_beginning;
}

bool SongProxyModel::filterAcceptsRow(int sourceRow,
              const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);

    QString str0 = sourceModel()->data(index0).toString();
    QString str1 = sourceModel()->data(index1).toString();

    QRegExp reg = QRegExp(filter_string);

    if(match_beginning)
        return (str0.startsWith(filter_string) || str1.startsWith(filter_string, Qt::CaseInsensitive));
    else
        return (str0.contains(filter_string) || str1.contains(filter_string, Qt::CaseInsensitive));

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
             + QString::number(num) + " WHERE sbornik_id = '"
             + sbornik.trimmed() + "' AND song_id = " + QString::number(songID));
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

    // Connect song with sbornik
    QSqlQuery sq;
    QString sid = "";
    sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Songs'");
    while (sq.next())
        sid = sq.value(0).toString();
    sq.clear();

    sq.exec("INSERT into SongLink (sbornik_id, song_id, song_number) VALUES ('"
            + sbornik.trimmed() + "',"
            + sid + ","
            + QString::number(num) + ")");
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

int SongDatabase::lastUser(QString sbornik)
{
    int last;
    QList <int> lastInt;
    QSqlQuery sq;
    sq.exec("SELECT song_number FROM SongLink WHERE sbornik_id = '" +sbornik+"'");
    while (sq.next())
        lastInt << sq.value(0).toInt();
    qSort(lastInt);
    if (lastInt.isEmpty())
        last=1;
    else
        last = lastInt.takeLast() + 1;
    return last;
}

bool SongDatabase::hasUserSbornik()
{
    int i(0);
    QSqlQuery sq;
    sq.exec("SELECT id FROM Sborniks WHERE is_user = 1");
    while (sq.next())
        ++i;
    if(i>0)
        return true;
    else
        return false;
}

QStringList SongDatabase::getUserSborniks()
{
    QStringList user_sbornik;
    QSqlQuery sq;
    sq.exec("SELECT id, name FROM Sborniks WHERE is_user = 1");
    while (sq.next())
        user_sbornik << sq.value(0).toString() + " - " + sq.value(1).toString();

    return user_sbornik;
}

void SongDatabase::deleteSong(int song_id)
{
    QSqlQuery sq;
    sq.exec("DELETE FROM Songs WHERE id = " + QString::number(song_id) );
    sq.clear();
    sq.exec("DELETE FROM SongLink WHERE song_id = " + QString::number(song_id) );
}

bool SongDatabase::addSbornik(QString code, QString name, QString info)
{
    QSqlQuery sq;
    QSqlTableModel sqt;
    int i(0);
    qDebug() << code;
    qDebug() << sq.exec("SELECT id FROM Sborniks WHERE id = '" + code + "'");
    while(sq.next())
        ++i;
    if (i>0)
        return false;
    else
    {
        sqt.setTable("Sborniks");
        sqt.insertRows(0,1);
        sqt.setData(sqt.index(0,0),code.trimmed());
        sqt.setData(sqt.index(0,1),name.trimmed());
        sqt.setData(sqt.index(0,2),info);
        sqt.setData(sqt.index(0,3),1);
        sqt.submitAll();
        return true;
    }
}
