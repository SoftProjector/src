#ifndef SONG_H
#define SONG_H
#include <QtGui>
#include <QtSql>

QString clean(QString str);

class Song
// Class for storing song information: number, name, sbornik
// The instance of this class is specific to a song & sbornik.
{
 public:
    Song();
    Song(int id);
    Song(int id, int num, QString sbornik_id);
    void readData();
    int num; // Number of the song in the specified sbornik
    QString title;
    QString sbornik_id;
    QString sbornik_name;

    void saveUpdate();
    void saveNew();
    QStringList getSongTextList();
    QString getSbornikName();

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
};


class SongProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    SongProxyModel(QObject *parent = 0);
    void setFilterString(QString new_string, bool new_match_beginning);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    QString filter_string;
    bool match_beginning;
};


class SongDatabase
{
public:
    SongDatabase();
    bool addSbornik(QString code, QString name, QString info);
    bool hasUserSbornik();
    void saveUpdate();
    void saveNew();
    void deleteSong(int songId);
    QString getSbornikIdStringFromName(QString sbornik_name);
    Song getSong(int id);
    QStringList getUserSborniks();
    QList<Song> getSongs(QString sbornik_id);
    int lastUser(QString sbornik_id);
//private:
};

#endif // SONG_H
