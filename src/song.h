#ifndef SONG_H
#define SONG_H
#include <QtGui>
#include <QtSql>

class Song
{
public:
    Song();
//    void setSong(QString gtitle);
//    void setSong(QStringList setSbornik, QString setTitle, int setCat, QStringList setBy, QString setText);
    bool hasTitle (QString title);
    void saveUpdate();
    void saveNew();
    QString clean(QString str);
    QString getSong(QString title);
    QString getSong(int number, QString sbornik);
    QStringList getSongList(QString title);
    QStringList getSongList(int number, QString sbornik);
    QStringList formatSongList(QString song);
    QStringList getTitle();
    QStringList getTitle(QString sbornik);
    QStringList getTitle2(QString sbornik);
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
