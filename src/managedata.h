#ifndef MANAGEDATA_H
#define MANAGEDATA_H

#include <QtGui>
#include <QtSql>

class ManageData
{
public:
    ManageData();
};

///////////////////////
// Bibles Class
///////////////////////
class Bibles
{
public:
    Bibles();
    QString bibleId;
    QString title;
};

///////////////////////
// Bibles Model Class
///////////////////////
class BiblesModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_DISABLE_COPY(BiblesModel)

public:
    BiblesModel();
    QList<Bibles> bible_list;

    void setBible(QList<Bibles> bibles);
    void addBible(Bibles bible);
    Bibles getBible(int row);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

};

///////////////////////
// Songbook Class
///////////////////////
class Songbook
{
public:
    Songbook();
//    Songbook(QString id);
//    void readData();
    QString songbookId;
    QString title;
    QString info;
};

///////////////////////
// Songbooks Model Class
///////////////////////
class SongbooksModel : public QAbstractTableModel
//  Class for storing data for Songbook Table
{
    Q_OBJECT
    Q_DISABLE_COPY(SongbooksModel)

public:
    SongbooksModel();
    QList<Songbook> songbook_list;

    void setSongbook(QList<Songbook> songbooks);
    void addSongbook(Songbook songbook);
    Songbook getSongbook(int row);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

};

///////////////////////
// Database Class
///////////////////////
class Database
{
public:
    Database();
    QList<Songbook> getSongbooks();
    QList<Bibles> getBibles();
};

#endif // MANAGEDATA_H
