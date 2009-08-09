#ifndef SONG_H
#define SONG_H
#include <QtGui>
#include <QtSql>

QString clean(QString str);

class Song
// Class for stroring song information: number, name, sbornik
{
 public:
    Song(int song_num, QString song_title, QString song_sbornik);
    Song();
    int num;
    QString title;
    QString sbornik;
    QString sbornik_info;

    void saveUpdate();
    void saveNew();

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
    Song currentSong();
    void setSongs(QList<Song> songs);
    void addSong(Song song);
    Song getSong(int row);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QList<Song> song_list;
};



class SongDatabase
{
public:
    SongDatabase();
    bool hasTitle(QString title);
    bool isUserOnly(int songId);
    void saveUpdate();
    void saveNew();
    void deleteSong(int songId);
    Song getSong(int id);
    Song getSong(QString title);
    QStringList getSongList(Song song);
    QStringList formatSongList(QString song);
    QList<Song> getSongs(QString sbornik);
    int lastUser();
//private:
};

#endif // SONG_H
