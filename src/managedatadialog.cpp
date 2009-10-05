#include "managedatadialog.h"
#include "ui_managedatadialog.h"

ManageDataDialog::ManageDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageDataDialog)
{
    ui->setupUi(this);
    ui->pbar->setVisible(false);
    //    Database db;

    // Set Bible Table
    load_bibles();
    ui->bibleTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->bibleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bibleTableView->verticalHeader()->hide();
    ui->bibleTableView->setColumnWidth(0, 395);

    // Set Sborniks Table
    load_sborniks();
    ui->sbornikTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->sbornikTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sbornikTableView->verticalHeader()->hide();
    ui->sbornikTableView->setColumnWidth(0, 195);
    ui->sbornikTableView->setColumnWidth(1, 195);

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
    QString file_path = QFileDialog::getOpenFileName(this,
                                                     "Select sbornik file to import",
                                                     ".",
                                                     "softProjector Sbornik File (*.txt *.sps)");

    // if file_path exits or "Open" is clicked, then import Sbornik
    if( !file_path.isNull() )
        importSbornik(file_path);
}

void ManageDataDialog::importSbornik(QString path)
{
    int row(1),max(0);
    QFile file(path), file2(path);
    QString line, code, title, info, num;
    QStringList split;
    QSqlQuery sq;
    QSqlTableModel sqt;
    QMessageBox mb;

    // get max number for progress bar
    if (file2.open(QIODevice::ReadOnly))
    {
        while (!file2.atEnd())
        {
            line = QString::fromUtf8(file2.readLine());
            max++;
        }
    }

    ui->pbar->setVisible(true);
    ui->pbar->setMaximum(max);
    ui->pbar->setValue(row);


    if (file.open(QIODevice::ReadOnly))
    {
        //Set Sbornik Title and Information
        line = QString::fromUtf8(file.readLine());
        if (line.startsWith("##"))
        {
            // Set Sbornik Title
            line = QString::fromUtf8(file.readLine());
            line.remove("#");
            title = line.trimmed();

            // Set Sbortnik Information
            line = QString::fromUtf8(file.readLine());
            line.remove("#");
            info = line.trimmed();

//            line = QString::fromUtf8(file.readLine());
        }
        else
        {
            title = "User Sbornik";
            info = "Sbornik Imported by User";
        }

        // Create sbornik
        SongDatabase sdb;
        sdb.addSbornik(title, info);

        // Set Sbornik Code
        code = "0";
        sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Sborniks'");
        while (sq.next())
            code = sq.value(0).toString();
        sq.clear();

        // Import Songs
        while (!file.atEnd())
        {
            line = QString::fromUtf8(file.readLine());
            split = line.split("#$#");

            num = split[0];

            // Add song to Songs table
            sqt.setTable("Songs");
            sqt.insertRows(0,1);
            sqt.setData(sqt.index(0,1),split[1]);//title
            sqt.setData(sqt.index(0,2),split[2]);//cat
            sqt.setData(sqt.index(0,3),split[3]);//tune
            sqt.setData(sqt.index(0,4),split[4]);//words
            sqt.setData(sqt.index(0,5),split[5]);//music
            sqt.setData(sqt.index(0,6),split[6]);//song text
            if (split.count() > 7)
            {
                sqt.setData(sqt.index(0,7),split[7]);//font
                sqt.setData(sqt.index(0,8),split[8]);//alignment
                sqt.setData(sqt.index(0,9),split[9]);//background
            }
            sqt.submitAll();

            // Get song id for the last song added
            QString sid = "";
            sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Songs'");
            while (sq.next()) sid = sq.value(0).toString();
            sq.clear();

            // Connect newly added song with its sbornik
            sq.exec("INSERT into SongLink (sbornik_id, song_id, song_number) VALUES ("
                    + code + ","
                    + sid + ","
                    + num + ")");

            row++;
            ui->pbar->setValue(row);
        }
    }
    ui->pbar->setVisible(false);
    load_sborniks();
}

void ManageDataDialog::on_export_sbornik_pushButton_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(this,tr("Save exported sbornik as:"),
                                             ".",tr("softProjector Sbornik File (*.sps)"));
    if( !file_path.isNull() )
        exportSbornik(file_path);
}

void ManageDataDialog::exportSbornik(QString path)
{
    int row = ui->sbornikTableView->currentIndex().row();
    Sbornik sbornik = sbornik_model->getSbornik(row);
    QSqlQuery sq,sq1;
    QString sbornik_id = sbornik.sbornikId;
    QString songs,song,num,id,title,info;
//    QFileDialog file;

    sq.exec("SELECT name, info FROM Sborniks WHERE id = '" + sbornik_id + "'");
    sq.first();
    title = sq.value(0).toString().trimmed();
    info = sq.value(1).toString().trimmed();
    sq.clear();

    songs = "##" + sbornik_id + "\n##"
            + title + "\n##"
            + info;

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
                     sq1.value(8).toString().trimmed(); //background
        }
        songs += "\n" + num + "#$#"+ song;
    }

    QFile ofile;
    ofile.setFileName(path);
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
    QString name = sbornik.title;

    QMessageBox ms;
    ms.setWindowTitle("Delete Sbornik?");
    ms.setText("Are you sure that you want to delete: "+ name);
    ms.setInformativeText("This action will permanentrly delete this sbornik");
    ms.setIcon(QMessageBox::Question);
    ms.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    ms.setDefaultButton(QMessageBox::Yes);
    int ret = ms.exec();

    switch (ret) {
    case QMessageBox::Yes:
        // Delete a sbornik
        deleteSbornik(sbornik);
        break;
    case QMessageBox::No:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }
}

void ManageDataDialog::deleteSbornik(Sbornik sbornik)
{
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

void ManageDataDialog::on_ok_pushButton_clicked()
{
    close();
}

void ManageDataDialog::on_import_bible_pushButton_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this,
                                                     "Select Bible file to import",
                                                     ".",
                                                     "softProjector Bible File (*.spb)");

    // if file_path exits or "Open" is clicked, then import Bible
    if( !file_path.isNull() )
        importBible(file_path);
}

void ManageDataDialog::importBible(QString path)
{
    QFile file;
    file.setFileName(path);
    QString line, title, id;
    QStringList split;
    QSqlQuery sq;
    QSqlTableModel sqt;
    int row(0);

    if (file.open(QIODevice::ReadOnly))
    {
        ui->pbar->setVisible(true);
        ui->pbar->setMaximum(31200);
        ui->pbar->setValue(row);

        // add Bible Name
        title = QString::fromUtf8(file.readLine());
        sq.exec("INSERT INTO BibleVersions (bible_name)"
                            "VALUES ('" + title.trimmed()+"')");
        sq.clear();

        // get Bible id of newly added Bible
        sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'BibleVersions'");
        sq.first() ;
        id = sq.value(0).toString();
        sq.clear();

        // add Bible book names
        line = QString::fromUtf8(file.readLine());
        while (!line.startsWith("---"))
        {
            QString bk_id, bk_name;
            split = line.split("\t");
            bk_id = split[0];
            bk_name = split[1];

            sqt.setTable("BibleBooks");
            sqt.insertRows(0,1);
            sqt.setData(sqt.index(0,0), bk_id.trimmed());
            sqt.setData(sqt.index(0,1), bk_name.trimmed());
            sqt.setData(sqt.index(0,2), id);
            sqt.submitAll();
            sqt.clear();

            line = QString::fromUtf8(file.readLine());
            row++;
            ui->pbar->setValue(row);
        }

        // add bible verses
        bool ok;
        while (!file.atEnd())
        {
            line = QString::fromUtf8(file.readLine());
            split = line.split("\t");
            ok = sq.exec("INSERT INTO BibleVerse VALUES ('"
                         + split[0] +"',"
                         + id + ","
                         + split[1] +"','"
                         + split[2] +"','"
                         + split[3] +"','"
                         + split[4] +"')");
            if (!ok)
            {
                sqt.setTable("BibleVerse");
                sqt.insertRows(0,1);
                sqt.setData(sqt.index(0,0), split[0]);
                sqt.setData(sqt.index(0,1), id);
                sqt.setData(sqt.index(0,2), split[1]);
                sqt.setData(sqt.index(0,3), split[2]);
                sqt.setData(sqt.index(0,4), split[3]);
                sqt.setData(sqt.index(0,5), split[4]);
                sqt.submitAll();
            }
            row++;
            ui->pbar->setValue(row);
        }
        file.close();
    }
    ui->pbar->setVisible(false);
    load_bibles();
}

void ManageDataDialog::on_export_bible_pushButton_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(this,tr("Save exported Bible as:"),
                                             ".",tr("softProjector Bible File (*.spb)"));
    if( !file_path.isNull() )
        exportBible(file_path);
}

void ManageDataDialog::exportBible(QString path)
{
    int row = ui->bibleTableView->currentIndex().row();
    Bibles bible = bible_model->getBible(row);
    QSqlQuery sq;
    QString id = bible.bibleId;
    QString to_file;

    // get Bible version information
    sq.exec("SELECT bible_name FROM BibleVersions WHERE id = " + id );
    sq.first();
    to_file = sq.value(0).toString().trimmed() + "\n";
    sq.clear();

    // get Bible books information
    sq.exec("SELECT id, book_name FROM BibleBooks WHERE bible_id = " + id );
    while (sq.next())
        to_file += sq.value(0).toString().trimmed() + "\t" + //book id
                   sq.value(1).toString().trimmed() + "\n"; //book name

    // get Bible verses
    to_file += "-----";
    sq.exec("SELECT verse_id, book, chapter, verse, verse_text FROM BibleVerse WHERE bible_id = " + id );
    while (sq.next())
        to_file += "\n" + sq.value(0).toString().trimmed() + "\t" + //verse id
                   sq.value(1).toString().trimmed() + "\t" + //book
                   sq.value(2).toString().trimmed() + "\t" + //chapter
                   sq.value(3).toString().trimmed() + "\t" + //verse
                   sq.value(4).toString().trimmed(); //verse text

    QFile ofile;
    ofile.setFileName(path);
    if (ofile.open(QIODevice::WriteOnly))
    {
        QTextStream out(&ofile);
        out.setCodec("UTF8");
        out << to_file;
    }
    ofile.close();
}

void ManageDataDialog::on_delete_bible_pushButton_clicked()
{
    int row = ui->bibleTableView->currentIndex().row();
    Bibles bible = bible_model->getBible(row);
    QString name = bible.title;

    QMessageBox ms;
    ms.setWindowTitle("Delete Bible?");
    ms.setText("Are you sure that you want to delete: "+ name);
    ms.setInformativeText("This action will permanentrly delete this Bible");
    ms.setIcon(QMessageBox::Question);
    ms.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    ms.setDefaultButton(QMessageBox::Yes);
    int ret = ms.exec();

    switch (ret) {
    case QMessageBox::Yes:
        // Delete a Bible
        deleteBible(bible);
        break;
    case QMessageBox::No:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }
}

void ManageDataDialog::deleteBible(Bibles bible)
{
    QSqlQuery sq;
    QString id = bible.bibleId.trimmed();

    // Delete from BibleBooks Table
    sq.exec("DELETE FROM BibleBooks WHERE bible_id = '" + id + "'");
    sq.clear();

    // Delete form BibleVerse Table
    sq.exec("Delete FROM BibleVerse WHERE bible_id = '" + id +"'");
    sq.clear();

    // Delete from BibleVersions Table
    sq.clear();
    sq.exec("DELETE FROM BibleVersions WHERE id = '" + id +"'");

    load_bibles();
}

void ManageDataDialog::on_edit_sbornik_pushButton_clicked()
{
    int row = ui->sbornikTableView->currentIndex().row();
    Sbornik sbornik = sbornik_model->getSbornik(row);
    QSqlTableModel sq;
    QSqlRecord sr;

    AddSbornikDialog sbornik_dialog;
    sbornik_dialog.setSbornik(sbornik.title,sbornik.info);
    int ret = sbornik_dialog.exec();
    switch(ret)
    {
    case AddSbornikDialog::Accepted:
        sq.setTable("Sborniks");
        sq.setFilter("id = " + sbornik.sbornikId);
        sq.select();
                sr = sq.record(0);
        sr.setValue(1,sbornik_dialog.title.trimmed());
        sr.setValue(2,sbornik_dialog.info.trimmed());
        sq.setRecord(0,sr);
        sq.submitAll();
        break;
    case AddSbornikDialog::Rejected:
//        close();
        break;
    }

    load_sborniks();
}

void ManageDataDialog::on_edit_bible_pushButton_clicked()
{
    int row = ui->bibleTableView->currentIndex().row();
    Bibles bible = bible_model->getBible(row);
    bool ok;
    QString name = QInputDialog::getText(this,"Edit Bible Name",
                                         "Bible title:",QLineEdit::Normal,
                                         bible.title, &ok);
    if(ok)
    {
        QSqlTableModel sq;
        QSqlRecord sr;
        sq.setTable("BibleVersions");
        sq.setFilter("id = " + bible.bibleId);
        sq.select();
        sr = sq.record(0);
        sr.setValue(1,name.trimmed());
        sq.setRecord(0,sr);
        sq.submitAll();
    }
    load_bibles();
}
