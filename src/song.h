#ifndef SONG_H
#define SONG_H
#include <QtGui>
#include <QtSql>


class SongsModel : public QAbstractTableModel
// Class for storing the data for the song table
{
    Q_OBJECT
    Q_DISABLE_COPY(SongsModel)
public:
    SongsModel();
    QString currentTitle();
    void setTitles(QStringList titles);
    int getNum(int row);
    QString getTitle(int row);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    QList<int> num_list;
    QStringList song_list;
    QStringList sbornik_list;
};


class Song
// Class for stroring song information: number, name, sbornik
{
    Song(int song_num, QString song_title, QString song_sbornik);
    int num;
    QString title;
    QString sbornik;
};

class Sbornik
{
public:
    Sbornik();
//    void setSong(QString gtitle);
//    void setSong(QStringList setSbornik, QString setTitle, int setCat, QStringList setBy, QString setText);
    bool hasTitle(QString title);
    bool isUserOnly(int songId);
    void saveUpdate();
    void saveNew();
    void deleteSong(int songId);
    QString clean(QString str);
    QString getSong(QString title);
    QString getSong(int number, QString sbornik);
    QStringList getSongList(QString title);
    QStringList getSongList(int number, QString sbornik);
    QStringList formatSongList(QString song);
    QStringList getTitle();
    QStringList getTitle(QString sbornik, bool sorted);
    int lastUser();
//private:
    int songID;
    int pv3300;
    int pv2500;
    int pv2001;
    int pv2000a;
    int pv2000b;
    int pv1730;
    int pvCt;
    int pvUser;
    int uaPsalm;
    int uaEpisni;
    QString title;
    int category;
    int language;
    int favorite;
    QString tune;
    QString wordsBy;
    QString musicBy;
    QString songText;
    QStringList songList;

};

#endif // SONG_H
