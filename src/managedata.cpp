#include "managedata.h"

ManageData::ManageData()
{
}


//***************************************
//****        Bibles                ****
//***************************************
Bibles::Bibles()
{
}

//***************************************
//****        BiblesModel                ****
//***************************************
BiblesModel::BiblesModel()
{
}

void BiblesModel::setBible(QList<Bibles> bibles)
{
    bible_list.clear();
    for (int i(0); i < bibles.size(); i++)
    {
        Bibles bible = bibles.at(i);
        bible_list.append(bible);
    }
    emit layoutChanged();
}

void BiblesModel::addBible(Bibles bible)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    bible_list.append(bible);
    endInsertRows();
}

Bibles BiblesModel::getBible(int row)
{
    return bible_list.at(row);
}

int BiblesModel::rowCount(const QModelIndex &parent) const
{
    return bible_list.count();
}

int BiblesModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant BiblesModel::data(const QModelIndex &index, int role) const
{
    if( index.isValid() && role == Qt::DisplayRole )
    {
        Bibles bible = bible_list.at(index.row());
        if( index.column() == 0 )
            return QVariant(bible.title);
    }
    return QVariant();
}

QVariant BiblesModel::headerData(int section,
                                 Qt::Orientation orientation,
                                 int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal )
    {
        switch(section) {
        case 0:
            return QVariant(tr("Title"));
        }
    }
    return QVariant();
}

bool BiblesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count-1);
    // Need to remove starting from the end:
    for(int i=row+count-1; i>=row; i--)
        bible_list.removeAt(i);
    endRemoveRows();
    return true;
}


//***************************************
//****        Sbornik                ****
//***************************************
Sbornik::Sbornik()
{
}

//Sbornik::Sbornik(QString id)
//{
//    sbornikId = id;
//}
//
//void Sbornik::readData()
//{
//    QSqlQuery sq;
//    sq.exec("SELECT id, name, info FROM Sborniks WHERE id = '" + sbornikId + "'");
//    sq.first();
//    title = sq.value(1).toString();
//    info = sq.value(2).toString();
//}

//***************************************
//****        Sborniks Model         ****
//***************************************
SborniksModel::SborniksModel()
{
}

void SborniksModel::setSbornik(QList<Sbornik> sborniks)
{
    sbornik_list.clear();
    for (int i(0); i < sborniks.size(); i++)
    {
        Sbornik sbornik = sborniks.at(i);
        sbornik_list.append(sbornik);
    }
    emit layoutChanged();
}

void SborniksModel::addSbornik(Sbornik sbornik)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    sbornik_list.append(sbornik);
    endInsertRows();
}

Sbornik SborniksModel::getSbornik(int row)
{
    return sbornik_list.at(row);
}

int SborniksModel::rowCount(const QModelIndex &parent) const
{
    return sbornik_list.count();
}

int SborniksModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

QVariant SborniksModel::data(const QModelIndex &index, int role) const
{
    if( index.isValid() && role == Qt::DisplayRole )
    {
        Sbornik sbornik = sbornik_list.at(index.row());
        if( index.column() == 0 )
            return QVariant(sbornik.title);
        else if( index.column() == 1 )
            return QVariant(sbornik.info);
    }
    return QVariant();
}

QVariant SborniksModel::headerData(int section,
                                 Qt::Orientation orientation,
                                 int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal )
    {
        switch(section) {
        case 0:
            return QVariant(tr("Title"));
        case 1:
            return QVariant(tr("Infomation"));
        }
    }
    return QVariant();
}

bool SborniksModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count-1);
    // Need to remove starting from the end:
    for(int i=row+count-1; i>=row; i--)
        sbornik_list.removeAt(i);
    endRemoveRows();
    return true;
}

//***************************************
//****        Database                ****
//***************************************
Database::Database()
{
}

QList<Sbornik> Database::getSborniks()
{
    QList<Sbornik> sborniki;
    Sbornik sbornik;
    QSqlQuery sq;
    sq.exec("SELECT id, name, info FROM Sborniks");
    while (sq.next())
    {
        sbornik.sbornikId = sq.value(0).toString();
        sbornik.title = sq.value(1).toString();
        sbornik.info = sq.value(2).toString();
        sborniki.append(sbornik);
    }
    return sborniki;
}

QList<Bibles> Database::getBibles()
{
    QList<Bibles> bibles;
    Bibles bible;
        QSqlQuery sq;
    sq.exec("SELECT bible_name, id FROM BibleVersions");
    while (sq.next())
    {
        bible.title = sq.value(0).toString();
        bible.bibleId = sq.value(1).toString();
        bibles.append(bible);
    }
    return bibles;
}
