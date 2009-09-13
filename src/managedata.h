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
// Sbornik Class
///////////////////////
class Sbornik
{
public:
    Sbornik();
//    Sbornik(QString id);
//    void readData();
    QString sbornikId;
    QString title;
    QString info;
};

///////////////////////
// Sborniks Model Class
///////////////////////
class SborniksModel : public QAbstractTableModel
//  Class for storing data for Sbornik Table
{
    Q_OBJECT
    Q_DISABLE_COPY(SborniksModel)

public:
    SborniksModel();
    QList<Sbornik> sbornik_list;

    void setSbornik(QList<Sbornik> sborniks);
    void addSbornik(Sbornik sbornik);
    Sbornik getSbornik(int row);

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
    QList<Sbornik> getSborniks();
    QList<Bibles> getBibles();
};

#endif // MANAGEDATA_H
