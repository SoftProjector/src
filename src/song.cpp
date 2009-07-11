#include "song.h"
#include <QDebug>

Song::Song()
{
}

bool Song::hasTitle(QString title)
{
    QSqlQuery sq;
    bool ys=false;
    sq.exec("SELECT id FROM songs WHERE title = '" + clean(title) +"'");
    while (sq.next()){
        ys = true;
    }
    return ys;
}

void Song::saveUpdate()
{
    QSqlTableModel sq;
    QSqlRecord sr;
    sq.setTable("songs");
    sq.setFilter("id = " + QString::number(songID));
    sq.select();

    sr = sq.record(0);
    sr.setValue(1,pv3300);
    sr.setValue(2,pv2500);
    sr.setValue(3,pv2001);
    sr.setValue(4,pv2000a);
    sr.setValue(5,pv2000b);
    sr.setValue(6,pv1730);
    sr.setValue(7,pvCt);
    sr.setValue(8,pvUser);
    sr.setValue(9,uaPsalm);
    sr.setValue(10,uaEpisni);
    sr.setValue(11,clean(title));
    sr.setValue(12,category);
    sr.setValue(13,tune);
    sr.setValue(14,wordsBy);
    sr.setValue(15,musicBy);
    sr.setValue(16,songText);
    sq.setRecord(0,sr);
    sq.submitAll();
}

void Song::saveNew()
{
    QSqlTableModel sq;
    sq.setTable("songs");
    sq.insertRows(0,1);
    sq.setData(sq.index(0,1),pv3300);
    sq.setData(sq.index(0,2),pv2500);
    sq.setData(sq.index(0,3),pv2001);
    sq.setData(sq.index(0,4),pv2000a);
    sq.setData(sq.index(0,5),pv2000b);
    sq.setData(sq.index(0,6),pv1730);
    sq.setData(sq.index(0,7),pvCt);
    sq.setData(sq.index(0,8),pvUser);
    sq.setData(sq.index(0,9),uaPsalm);
    sq.setData(sq.index(0,10),uaEpisni);
    sq.setData(sq.index(0,11),clean(title));
    sq.setData(sq.index(0,12),category);
    sq.setData(sq.index(0,13),tune);
    sq.setData(sq.index(0,14),wordsBy);
    sq.setData(sq.index(0,15),musicBy);
    sq.setData(sq.index(0,16),songText);
    sq.submitAll();
}

QString Song::clean(QString str)
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

QString Song::getSong(QString gtitle)
{
    QSqlQuery sq;
    sq.exec("SELECT id, pv3300, pv2500, pv2001, pv2000a, pv2000b, pv1700, pvCt, pvUser, uaPsalm, uaEpisni, category, tune, words, music, songText FROM songs WHERE title = '" + gtitle + "'");
    while (sq.next()){
        songID = sq.value(0).toInt();
        pv3300 = sq.value(1).toInt();
        pv2500 = sq.value(2).toInt();
        pv2001 = sq.value(3).toInt();
        pv2000a = sq.value(4).toInt();
        pv2000b = sq.value(5).toInt();
        pv1730 = sq.value(6).toInt();
        pvCt = sq.value(7).toInt();
        pvUser = sq.value(8).toInt();
        uaPsalm = sq.value(9).toInt();
        uaEpisni = sq.value(10).toInt();
        title = gtitle;
        category = sq.value(11).toInt();
        tune = sq.value(12).toString();
        wordsBy = sq.value(13).toString();
        musicBy = sq.value(14).toString();
        songText = sq.value(15).toString();
    }
    return songText;
}

QString Song::getSong(int number, QString sbornik)
{
    QSqlQuery sq;
    sq.exec("SELECT id, pv3300, pv2500, pv2001, pv2000a, pv2000b, pv1700, pvCt, pvUser, uaPsalm, uaEpisni, title, category, tune, words, music, songText FROM songs WHERE " + sbornik +" = '" + QString::number(number) + "'");
    while (sq.next()){
        songID = sq.value(0).toInt();
        pv3300 = sq.value(1).toInt();
        pv2500 = sq.value(2).toInt();
        pv2001 = sq.value(3).toInt();
        pv2000a = sq.value(4).toInt();
        pv2000b = sq.value(5).toInt();
        pv1730 = sq.value(6).toInt();
        pvCt = sq.value(7).toInt();
        pvUser = sq.value(8).toInt();
        uaPsalm = sq.value(9).toInt();
        uaEpisni = sq.value(10).toInt();
        title = sq.value(11).toString();
        category = sq.value(12).toInt();
        tune = sq.value(13).toString();
        wordsBy = sq.value(14).toString();
        musicBy = sq.value(15).toString();
        songText = sq.value(16).toString();
    }
    return songText;
}

QStringList Song::getSongList(QString gtitle)
{
    QSqlQuery sq;
    sq.exec("SELECT id, pv3300, pv2500, pv2001, pv2000a, pv2000b, pv1700, pvCt, pvUser, uaPsalm, uaEpisni, category, tune, words, music, songText FROM songs WHERE title = '" + gtitle + "'");
    while (sq.next()){
        songID = sq.value(0).toInt();
        pv3300 = sq.value(1).toInt();
        pv2500 = sq.value(2).toInt();
        pv2001 = sq.value(3).toInt();
        pv2000a = sq.value(4).toInt();
        pv2000b = sq.value(5).toInt();
        pv1730 = sq.value(6).toInt();
        pvCt = sq.value(7).toInt();
        pvUser = sq.value(8).toInt();
        uaPsalm = sq.value(9).toInt();
        uaEpisni = sq.value(10).toInt();
        title = gtitle;
        category = sq.value(11).toInt();
        tune = sq.value(12).toString();
        wordsBy = sq.value(13).toString();
        musicBy = sq.value(14).toString();
        songText = sq.value(15).toString();
    }
    songList = formatSongList(songText);
    return songList;
}

QStringList Song::getSongList(int number, QString sbornik)
{
    QSqlQuery sq;
    sq.exec("SELECT id, pv3300, pv2500, pv2001, pv2000a, pv2000b, pv1700, pvCt, pvUser, uaPsalm, uaEpisni, title, category, tune, words, music, songText FROM songs WHERE " + sbornik +" = '" + QString::number(number) + "'");
    while (sq.next()){
        songID = sq.value(0).toInt();
        pv3300 = sq.value(1).toInt();
        pv2500 = sq.value(2).toInt();
        pv2001 = sq.value(3).toInt();
        pv2000a = sq.value(4).toInt();
        pv2000b = sq.value(5).toInt();
        pv1730 = sq.value(6).toInt();
        pvCt = sq.value(7).toInt();
        pvUser = sq.value(8).toInt();
        uaPsalm = sq.value(9).toInt();
        uaEpisni = sq.value(10).toInt();
        title = sq.value(11).toString();
        category = sq.value(12).toInt();
        tune = sq.value(13).toString();
        wordsBy = sq.value(14).toString();
        musicBy = sq.value(15).toString();
        songText = sq.value(16).toString();
    }
    songList = formatSongList(songText);
    return songList;
}

QStringList Song::formatSongList(QString song)
{
    QStringList formatedSong;
    QString text, text2, codec;
    QStringList split, songlist;
    QString chorus;
    bool has_chorus=0;
    int i(0),j(0),k(0),chor(0);

    songlist = song.split("@$");//songText.split("@$");

    while (i <songlist.size())
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
                if (j==k-1) text += split[j];
                else text += split[j] + "\n";
                ++j;
            }
            formatedSong += text.trimmed();
            if (has_chorus){
                formatedSong += chorus;
            }
        } else if (text2.startsWith(codec.fromUtf8("Припев")))
        { // Fill Chorus
            while (j<k)
            {
                if (j==k-1) text += split[j];
                else text += split[j] + "\n";
                ++j;
            }
            chorus = text.trimmed();
            if (chorus.size()>3)
            {
                has_chorus=1; ++chor;
                if (chor ==1) formatedSong += chorus;
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

QStringList Song::getTitle()
{
    QStringList titles;
    QSqlQuery sq;
    sq.exec("SELECT title FROM songs");
    while (sq.next()) titles << sq.value(0).toString();
    titles.sort();
    return titles;
}

QStringList Song::getTitle(QString sbornik)
{
    QStringList titles;
    QSqlQuery sq;
    sq.exec("SELECT title FROM songs WHERE "+sbornik+">0");
    while (sq.next()) titles << sq.value(0).toString();
    titles.sort();
    return titles;
}

QStringList Song::getTitle2(QString sbornik)
{
    QStringList titles;
//    titles << "a";
    QMap<int, QString> tMap;
    QSqlQuery sq;
    sq.exec("SELECT title, "+sbornik+" FROM songs WHERE "+sbornik+">0");
//    while (sq.next()) titles << sq.value(1).toString()
//        + " - " + sq.value(0).toString();
    while(sq.next()) tMap[sq.value(1).toInt()] = sq.value(0).toString();
//    qDebug() << "Map Set";
//    qSort(tMap.values().begin(),tMap.values().end());
    QList<int> list1;
    list1 = tMap.keys();
    QStringList list2;
    list2 = tMap.values();
    int mapLenght(tMap.size());
    int i(0);
    QString aa;
    while (i<mapLenght)
    {
        titles << QString::number(list1[i]) + " - " + list2[i];
        aa = title[i];
//        qDebug() << aa;
        ++i;
    }
//    foreach(QString str, tMap) qDebug()<< str;
//    titles.sort();
    return titles;
}

int Song::lastUser()
{
    int last;
    QList <int> lastInt;
    QSqlQuery sq;
    sq.exec("SELECT pvUser FROM songs WHERE pvUser > 0");
    while (sq.next()) lastInt << sq.value(0).toInt();
    qSort(lastInt);
    if (lastInt.isEmpty()) last=1;
    else last = lastInt.takeLast() + 1;
    return last;
}

bool Song::isUserOnly(int sId)
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
    if(i==0) return true;
    else return false;
}

void Song::deleteSong(int sId)
{
    QSqlQuery sq;
    sq.exec("DELETE FROM songs WHERE id = '" + QString::number(sId) + "'");
}
