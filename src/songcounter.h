#ifndef SONGCOUNTER_H
#define SONGCOUNTER_H

#include <QDialog>
#include <QtSql>
#include "song.h"

namespace Ui {
    class SongCounter;
}

/************************************************************************/
class Counter
{
public:
    Counter();
    QString title;
    int count;
};

class SongCounterModel : public QAbstractTableModel
//  Class for storing data for Song Use Counter Table
{
    Q_OBJECT
    Q_DISABLE_COPY(SongCounterModel)

public:
    SongCounterModel();
    QList<Counter> song_count_list;

    void setCounter(QList<Counter> song_counts);
    void addCounter(Counter song_count);
    Counter getSongCount(int row);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

};
/************************************************************************/


class SongCounter : public QDialog
{
    Q_OBJECT

public:
    explicit SongCounter(QWidget *parent = 0);
    ~SongCounter();

private:
    QList<Counter> song_counts;
    SongCounterModel *songCounterModel;
    Ui::SongCounter *ui;

public slots:
    void addSongCount(Song song);

private slots:
    void load_counts();
    void on_resetButton_clicked();
    void on_closeButton_clicked();
    QList<Counter> getSongCounts();
};

#endif // SONGCOUNTER_H
