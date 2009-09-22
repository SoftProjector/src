#include "managedatadialog.h"
#include "ui_managedatadialog.h"

ManageDataDialog::ManageDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageDataDialog)
{
    ui->setupUi(this);
//    Database db;

    // Set Bible Table
//    bible_list = db.getBibles();
//    bible_model = new BiblesModel;
//    bible_model->setBible(bible_list);
//    ui->bibleTableView->setModel(bible_model);
    load_bibles();
    ui->bibleTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->bibleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bibleTableView->verticalHeader()->hide();

    // Set Sborniks Table
//    sbornik_list = db.getSborniks();
//    sbornik_model = new SborniksModel;
//    sbornik_model->setSbornik(sbornik_list);
//    ui->sbornikTableView->setModel(sbornik_model);
    load_sborniks();
    ui->sbornikTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->sbornikTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sbornikTableView->verticalHeader()->hide();
    // FIXME set widths like this:
    // ui->songs_view->setColumnWidth(0, 55);

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

void ManageDataDialog::load_sborniks()
{
    Database db;
    // Set Sborniks Table
    sbornik_list = db.getSborniks();
    sbornik_model = new SborniksModel;
    sbornik_model->setSbornik(sbornik_list);
    ui->sbornikTableView->setModel(sbornik_model);
}

void ManageDataDialog::load_bibles()
{
    Database db;
    // Set Bible Table
    bible_list = db.getBibles();
    bible_model = new BiblesModel;
    bible_model->setBible(bible_list);
    ui->bibleTableView->setModel(bible_model);
}


void ManageDataDialog::on_import_sbornik_pushButton_clicked()
{

}

void ManageDataDialog::on_export_sbornik_pushButton_clicked()
{
    int row = ui->sbornikTableView->currentIndex().row();
    Sbornik sbornik = sbornik_model->getSbornik(row);
    QSqlQuery sq,sq1;
    QString sbornik_id = sbornik.sbornikId.trimmed();
    QString songs,song,num,id,title,info;
    QFileDialog file;
    QString filepath = file.getSaveFileName(this,tr("Save exported sbornik as:"),
                                        ".",tr("Sbornik Files (*.sps)"));

    sq.exec("SELECT name, info FROM Sborniks WHERE id = '" + sbornik_id + "'");
    sq.first();
    title = sq.value(0).toString().trimmed();
    info = sq.value(1).toString().trimmed();
    sq.clear();

    songs = "##" + sbornik_id + "\n##"
            + title + "\n##"
            + info + "\n";

    int i(0);
    sq.exec("SELECT song_id, song_number FROM SongLink WHERE sbornik_id like '" + sbornik_id +"'");
    while (sq.next())
    {
        id = sq.value(0).toString();
        num = sq.value(1).toString();
        sq1.exec("SELECT title, category, tune, words, music, song_text, font, alingment, background FROM Songs WHERE id = "+id);
        while (sq1.next())
        {

            song = sq1.value(0).toString().trimmed() + "#$#" + //title
                     sq1.value(1).toString().trimmed() + "#$#" + //category
                     sq1.value(2).toString().trimmed() + "#$#" + //tune
                     sq1.value(3).toString().trimmed() + "#$#" + //words
                     sq1.value(4).toString().trimmed() + "#$#" + //music
                     sq1.value(5).toString().trimmed() + "#$#" + //song_text
                     sq1.value(6).toString().trimmed() + "#$#" + //font
                     sq1.value(7).toString().trimmed() + "#$#" + //alignment
                     sq1.value(8).toString().trimmed() + "\n"; //background
        }
        songs += num + "#$#"+ song;
    }

    QFile ofile;
    ofile.setFileName(filepath);
    if (ofile.open(QIODevice::WriteOnly))
    {
        QTextStream out(&ofile);
        out.setCodec("UTF8");
        out << songs;
    }
    ofile.close();
}

void ManageDataDialog::on_delete_sbornik_pushButton_clicked()
{
    int row = ui->sbornikTableView->currentIndex().row();
    Sbornik sbornik = sbornik_model->getSbornik(row);
    QSqlQuery sq,sq1;
    QString id = sbornik.sbornikId.trimmed();

    // Delete from Sbornik Table
    sq.exec("DELETE FROM Sborniks WHERE id = '" + id + "'");
    sq.clear();

    // Delete form Songs Table
    sq.exec("SELECT song_id FROM SongLink WHERE sbornik_id like '" + id +"'");
    while (sq.next())
    {
        QString s;
        s = sq.value(0).toString();
        sq1.exec("DELETE FROM Songs WHERE id = "+s);
    }

    // Delete from SongLink Table
    sq.clear();
    sq.exec("DELETE FROM SongLink WHERE sbornik_id like '" + id +"'");

    load_sborniks();
}
