#include <QtSql>
#include <QMessageBox>
#include "exportdialog.h"
#include "ui_exportdialog.h"
#include "song.h"

ExportDialog::ExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    preload();
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::changeEvent(QEvent *e)
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

void ExportDialog::on_buttonBox_accepted()
{
    QFileDialog file;
    QString filepath;

    QString sbornik = ui->sbornikComboBox->currentText();
    QStringList list = sbornik.split(" - ");
    sbornik = list.at(0);

    if (ui->exportRadioButton->isChecked())
    {
        filepath = file.getSaveFileName(this,tr("Save exported sbornik as:"),
                                        ".",tr("Sbornik Files (*.sps)"));
        exportSbornik(sbornik.trimmed(), filepath);
    }
    else if (ui->exportDeleteRadioButton->isChecked())
    {
        filepath = file.getSaveFileName(this,tr("Save exported sbornik as:"),
                                        ".",tr("Sbornik Files (*.sps)"));
        exportSbornik(sbornik.trimmed(),filepath);
        deleteSbornik(sbornik.trimmed());
    }
    else if (ui->deleteRadioButton->isChecked())
        deleteSbornik(sbornik.trimmed());
    
}


void ExportDialog::preload()
{
    SongDatabase db;
    ui->sbornikComboBox->addItems(db.getUserSborniks());
}

void ExportDialog::exportSbornik(QString sbornik, QString file_path)
{
    QSqlQuery sq,sq1;
    QString songs,song,num,id,title,info;

    sq.exec("SELECT name, info FROM Sborniks WHERE id = '" + sbornik + "'");
    sq.first();
    title = sq.value(0).toString().trimmed();
    info = sq.value(1).toString().trimmed();
    sq.clear();

    songs = "##" + sbornik + "\n##"
            + title + "\n##"
            + info + "\n";

    int i(0);
    sq.exec("SELECT song_id, song_number FROM SongLink WHERE sbornik_id like '" + sbornik +"'");
    while (sq.next())
    {
        id = sq.value(0).toString();
        num = sq.value(1).toString();
        sq1.exec("SELECT title, category, tune, words, music, song_text, font, alingment, background FROM Songs WHERE id = "+id);
        while (sq1.next())
        {

            song = sq1.value(0).toString() + "#$#" + //title
                     sq1.value(1).toString() + "#$#" + //category
                     sq1.value(2).toString() + "#$#" + //tune
                     sq1.value(3).toString() + "#$#" + //words
                     sq1.value(4).toString() + "#$#" + //music
                     sq1.value(5).toString().trimmed() + "#$#" + //song_text
                     sq1.value(6).toString() + "#$#" + //font
                     sq1.value(7).toString() + "#$#" + //alignment
                     sq1.value(8).toString() + "\n"; //background
        }
        songs += num + "#$#"+ song;
    }

    QFile ofile;
    ofile.setFileName(file_path);
    if (ofile.open(QIODevice::WriteOnly))
    {
        QTextStream out(&ofile);
        out.setCodec("UTF8");
        out << songs;
    }
    ofile.close();
}

void ExportDialog::deleteSbornik(QString sbornik)
{
    QSqlQuery sq,sq1;
    QString id;

    // Delete from Sbornik Table
    sq.exec("DELETE FROM Sborniks WHERE id = '" + sbornik + "'");
    sq.clear();

    // Delete form Songs Table
    int i(0);
    sq.exec("SELECT song_id FROM SongLink WHERE sbornik_id like '" + sbornik +"'");
    while (sq.next())
    {
        id = sq.value(0).toString();
        sq1.exec("DELETE FROM Songs WHERE id = "+id);
    }

    // Delete from SongLink Table
    sq.clear();
    sq.exec("DELETE FROM SongLink WHERE sbornik_id like '" + sbornik +"'");
}

