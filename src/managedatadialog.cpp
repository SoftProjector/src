#include "managedatadialog.h"
#include "ui_managedatadialog.h"

ManageDataDialog::ManageDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageDataDialog)
{
    ui->setupUi(this);
    QList<Sbornik> sbor;
    ManageDb db;
    sbor = db.getSborniks();

    SborniksModel *sb;
    sb = new SborniksModel;
    sb->setSbornik(sbor);
    ui->sbornikTableView->setModel(sb);

}

ManageDataDialog::~ManageDataDialog()
{
    delete ui;
}

void ManageDataDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
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
//****        Bibles                ****
//***************************************

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
    return 3;
}

QVariant SborniksModel::data(const QModelIndex &index, int role) const
{
    if( index.isValid() && role == Qt::DisplayRole )
    {
        Sbornik sbornik = sbornik_list.at(index.row());
        if( index.column() == 0 )
            return QVariant(sbornik.sbornikId);
        else if( index.column() == 1 )
            return QVariant(sbornik.title);
        else
            return QVariant(sbornik.info);
    }
    else
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
            return QVariant("Id");
        case 1:
            return QVariant("Title");
        case 2:
            return QVariant("Info");
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
//****        BiblesModel                ****
//***************************************

//***************************************
//****        ManageDb                ****
//***************************************
ManageDb::ManageDb()
{
}

QList<Sbornik> ManageDb::getSborniks()
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

QList<Bibles> ManageDb::getBibles()
{
}
