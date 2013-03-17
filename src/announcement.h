#ifndef ANNOUNCEMENT_H
#define ANNOUNCEMENT_H

//#include <QString>
//#include <QFont>
//#include <QColor>
#include <QtGui>
#include "spfunctions.h"

class AnnounceSlide
{
public:
    QString text;
    bool usePrivateSettings;
    int alignmentV;
    int alignmentH;
    QString backgroundPath;
    QColor color;
    QFont font;
};

class Announcement
{
public:
    Announcement();
    Announcement(int id);
    int idNum;
    QString title;
    QString text;
    bool usePrivateSettings;
    bool useAutoNext;
    bool loop;
    int slideTimer;
    QFont font;
    QColor color;
    bool useBackground;
    QString backgroundPath;
    int alignmentV;
    int alignmentH;

    int align_flags;

    void saveNew();
    void saveUpdate();
    void deleteAnnouce();
    void readData();

    QStringList getAnnounceList();
    AnnounceSlide getAnnounceSlide(int current);

private:
    void setDefaults();
};

class AnnounceModel : public QAbstractTableModel
// Class to store data from Announcement table
{
    Q_OBJECT
    Q_DISABLE_COPY(AnnounceModel)
public:
    AnnounceModel();
    void setAnnoucements(QList<Announcement> announcements);
    void addAnnouncement(Announcement announce);
    Announcement getAnnounce(int row);
    Announcement getAnnounce(QModelIndex index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool removeRows( int row, int count, const QModelIndex & parent = QModelIndex() );

    void emitLayoutChanged();
    void emitLayoutAboutToBeChanged();
    void updateAnnounceFromDatabase(int annId);
//    void updateAnnounceFromDatabase(int newAnnId, int oldAnnId);
    bool isInTable(int annId);

    QList<Announcement> announceList;
};

class AnnounceProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    AnnounceProxyModel(QObject *parent = 0);
    void setFilterString(QString new_string, bool new_match_beginning, bool new_exact_match);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    QString filterString;
    bool matchBeginning, matchExact;
};


#endif // ANNOUNCEMENT_H
