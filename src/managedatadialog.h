#ifndef MANAGEDATADIALOG_H
#define MANAGEDATADIALOG_H

#include <QtGui/QDialog>
#include <QtGui>
#include <QtSql>

namespace Ui {
    class ManageDataDialog;
}

class ManageDataDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ManageDataDialog)
public:
    explicit ManageDataDialog(QWidget *parent = 0);
    virtual ~ManageDataDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ManageDataDialog *ui;
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
// Bibles Class
///////////////////////
class Bibles
{
    QString bibleId;
    QString title;
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
// Bibles Model Class
///////////////////////
class BiblesModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_DISABLE_COPY(BiblesModel)

public:
    BiblesModel();
};

///////////////////////
// Database Class
///////////////////////
class ManageDb
{
public:
    ManageDb();
    QList<Sbornik> getSborniks();
    QList<Bibles> getBibles();
};

#endif // MANAGEDATADIALOG_H
