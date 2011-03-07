/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2010  Vladislav Kobzar and Matvey Adzhigirey
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

#include "managedatadialog.h"
#include "ui_managedatadialog.h"
#include "song.h"

ManageDataDialog::ManageDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageDataDialog)
{
    ui->setupUi(this);
    //    Database db;

    // Set Bible Table
    load_bibles();
    ui->bibleTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->bibleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bibleTableView->verticalHeader()->hide();
    ui->bibleTableView->setColumnWidth(0, 395);
//    updateBibleButtons();

    // Set Songbooks Table
    load_songbooks();
    ui->songbookTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->songbookTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songbookTableView->verticalHeader()->hide();
    ui->songbookTableView->setColumnWidth(0, 195);
    ui->songbookTableView->setColumnWidth(1, 195);

    //Set reload cariers to false
    reload_bible = false;
    reload_songbook = false;

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

void ManageDataDialog::load_songbooks()
{
    Database db;
    // Set Songbooks Table
    songbook_list = db.getSongbooks();
    songbook_model = new SongbooksModel;
    songbook_model->setSongbook(songbook_list);
    ui->songbookTableView->setModel(songbook_model);
    updateSongbookButtons();
}

void ManageDataDialog::load_bibles()
{
    Database db;
    // Set Bible Table
    bible_list = db.getBibles();
    bible_model = new BiblesModel;
    bible_model->setBible(bible_list);
    ui->bibleTableView->setModel(bible_model);
    updateBibleButtons();
}

void ManageDataDialog::updateBibleButtons()
{
    bool enable_edit;
    bool enable_export;
    bool enable_delete;

    if (ui->bibleTableView->hasFocus())
    {
        enable_edit = true;
        enable_export = true;
        if (bible_model->rowCount()>=2)
            enable_delete = true;
        else
            enable_delete = false;
    }
    else
    {
        enable_edit = false;
        enable_export = false;
        enable_delete = false;
    }

    ui->edit_bible_pushButton->setEnabled(enable_edit);
    ui->export_bible_pushButton->setEnabled(enable_export);
    ui->delete_bible_pushButton->setEnabled(enable_delete);
}

void ManageDataDialog::updateSongbookButtons()
{
    bool enable_edit;
    bool enable_export;
    bool enable_delete;

    if (ui->songbookTableView->hasFocus())
    {
        enable_edit = true;
        enable_export = true;
        if (songbook_model->rowCount()>=2)
            enable_delete = true;
        else
            enable_delete = false;
    }
    else
    {
        enable_edit = false;
        enable_export = false;
        enable_delete = false;
    }

    ui->edit_songbook_pushButton->setEnabled(enable_edit);
    ui->export_songbook_pushButton->setEnabled(enable_export);
    ui->delete_songbook_pushButton->setEnabled(enable_delete);
}

void ManageDataDialog::on_import_songbook_pushButton_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this,
                                                     tr("Select a songbook to import"),
                                                     ".",
                                                     tr("softProjector songbook file ") + "(*.sps)");

    // if file_path exits or "Open" is clicked, then import a Songbook
    if( !file_path.isNull() )
        importSongbook(file_path);
}

void ManageDataDialog::importSongbook(QString path)
{
    setWaitCursor();
    int row(1),max(0);
    QFile file(path), file2(path);
    QString line, code, title, info, num;
    QStringList split;
    QSqlQuery sq, sq1;
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

    QProgressDialog progress(tr("Importing..."), tr("Cancel"), 0, max, this);
    progress.setValue(row);

    if (file.open(QIODevice::ReadOnly))
    {

        reload_songbook = true;
        //Set Songbook Title and Information
        line = QString::fromUtf8(file.readLine());
        if (line.startsWith("##"))
        {
            // Set Songbook Title
            line = QString::fromUtf8(file.readLine());
            line.remove("#");
            title = line.trimmed();

            // Set Songbook Information
            line = QString::fromUtf8(file.readLine());
            line.remove("#");
            info = line.trimmed();
            // Convert songbook information from single line to multiple line
            QStringList info_list = info.split("@%");
            info.clear();
            for(int i(0); i<info_list.size();++i)
            {
                info += info_list[i] + "\n";
            }

//            line = QString::fromUtf8(file.readLine());
        }
        else
        {
            title = tr("User songbook");
            info = tr("Songbook imported by the user");
        }

        // Create songbook
        SongDatabase sdb;
        sdb.addSongbook(title, info.trimmed());

        // Set Songbook Code
        code = "0";
        sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Songbooks'");
        while (sq.next())
            code = sq.value(0).toString();
        sq.clear();

        // Import Songs
        QSqlDatabase::database().transaction();
        sq1.prepare("INSERT INTO Songs (title, category, tune, words, music, song_text, font, alingment, background)"
                   "VALUES (?,?,?,?,?,?,?,?,?)");
        while (!file.atEnd())
        {
            if (progress.wasCanceled())
                break;
            line = QString::fromUtf8(file.readLine());
            split = line.split("#$#");

            num = split[0];

            // Add song to Songs table
            sq1.addBindValue(split[1]);//title
            sq1.addBindValue(split[2]);//cat
            sq1.addBindValue(split[3]);//tune
            sq1.addBindValue(split[4]);//words
            sq1.addBindValue(split[5]);//music
            sq1.addBindValue(split[6]);//song text
            if (split.count() > 7)
            {
                sq1.addBindValue(split[7]);//font
                sq1.addBindValue(split[8]);//alignment
                sq1.addBindValue(split[9]);//background
            }
            else
            {
                sq1.addBindValue("");//font
                sq1.addBindValue("");//alignment
                sq1.addBindValue("");//background
            }
            sq1.exec();

            // Get song id for the last song added
            QString sid = "";
            sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Songs'");
            while (sq.next()) sid = sq.value(0).toString();
            sq.clear();

            // Connect newly added song with its songbook
            sq.exec("INSERT into SongLink (songbook_id, song_id, song_number) VALUES ("
                    + code + ","
                    + sid + ","
                    + num + ")");

            row++;
            progress.setValue(row);
        }
        QSqlDatabase::database().commit();
    }
    load_songbooks();
    setArrowCursor();
}

void ManageDataDialog::on_export_songbook_pushButton_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(this,tr("Save the songbook as:"),
                                             ".",tr("softProjector songbook file (*.sps)"));
    if( !file_path.isNull() )
        exportSongbook(file_path);
}

void ManageDataDialog::exportSongbook(QString path)
{
    setWaitCursor();
    int row = ui->songbookTableView->currentIndex().row();
    Songbook songbook = songbook_model->getSongbook(row);
    QSqlQuery sq,sq1;
    QString songbook_id = songbook.songbookId;
    QString songs,song,num,id,title,info;
//    QFileDialog file;

    sq.exec("SELECT name, info FROM Songbooks WHERE id = '" + songbook_id + "'");
    sq.first();
    title = sq.value(0).toString().trimmed();
    info = sq.value(1).toString().trimmed();
    sq.clear();

    // Convert songbook information from multiline to single line
    QStringList info_list = info.split("\n");
    info = info_list[0];
    for(int i(1); i<info_list.size();++i)
    {
        info += "@%" + info_list[i];
    }

    songs = "##" + songbook_id + "\n##"
            + title + "\n##"
            + info;

    sq.exec("SELECT song_id, song_number FROM SongLink WHERE songbook_id like '" + songbook_id +"'");
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
    setArrowCursor();

    QMessageBox mb;
    mb.setWindowTitle(tr("Export complete"));
    mb.setText(tr("The songbook \"") + title + tr("\"\nHas been saved to:\n     ") + path);
    mb.setIcon(QMessageBox::Information);
    mb.exec();
}

void ManageDataDialog::on_delete_songbook_pushButton_clicked()
{
    int row = ui->songbookTableView->currentIndex().row();
    Songbook songbook = songbook_model->getSongbook(row);
    QString name = songbook.title;

    QMessageBox ms;
    ms.setWindowTitle(tr("Delete songbook?"));
    ms.setText(tr("Are you sure that you want to delete: ")+ name);
    ms.setInformativeText(tr("This action will permanentrly delete this songbook"));
    ms.setIcon(QMessageBox::Question);
    ms.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    ms.setDefaultButton(QMessageBox::Yes);
    int ret = ms.exec();

    switch (ret) {
    case QMessageBox::Yes:
        // Delete a songbook
        deleteSongbook(songbook);
        break;
    case QMessageBox::No:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }
}

void ManageDataDialog::deleteSongbook(Songbook songbook)
{
    setWaitCursor();
    reload_songbook = true;
    QSqlQuery sq,sq1;
    QString id = songbook.songbookId.trimmed();

    // Delete from Songbook Table
    sq.exec("DELETE FROM Songbooks WHERE id = '" + id + "'");
    sq.clear();

    // Delete form Songs Table
    sq.exec("SELECT song_id FROM SongLink WHERE songbook_id like '" + id +"'");
    QString s;
    QSqlDatabase::database().transaction();
    sq1.prepare("DELETE FROM Songs WHERE id = :id");
    while (sq.next())
    {
        s = sq.value(0).toString().trimmed();
        sq1.bindValue(":id",s);
        sq1.exec();
    }
    QSqlDatabase::database().commit();

    // Delete from SongLink Table
    sq.clear();
    sq.exec("DELETE FROM SongLink WHERE songbook_id like '" + id +"'");

    load_songbooks();
    updateSongbookButtons();
    setArrowCursor();
}

void ManageDataDialog::on_ok_pushButton_clicked()
{
    close();
}

void ManageDataDialog::on_import_bible_pushButton_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this,
                                                     tr("Select Bible file to import"),
                                                     ".",tr("SoftProjector Bible file ") + "(*.spb)");


    // if file_path exits or "Open" is clicked, then import Bible
    if( !file_path.isNull() )
    {
        QFile file;
        QString line, version;
        QStringList split;
        file.setFileName(file_path);

        if (file.open(QIODevice::ReadOnly))
        {
            line = QString::fromUtf8(file.readLine());
            // check file format and version
            if (!line.startsWith("##spData")) // Old bible format verison
            {
                QMessageBox mb;
                mb.setWindowTitle(tr("Old Bible file format"));
                mb.setText(tr("The Bible format you are importing is of an old version.\n"
                              "Your current SoftProjector does not support this format.\n"
                              "Please download lattest Bibles and import them"));
                mb.setIcon(QMessageBox::Critical);
                mb.exec();
            }
            else if (line.startsWith("##spData")) //New bible format version
            {
                split = line.split("\t");
                version = split.at(1);
                if (version.trimmed() == "1") // Version 1
                {
                    importBible(file_path, version.trimmed());
                }
                else // Other than version 1
                {
                    QMessageBox mb;
                    mb.setWindowTitle(tr("New Bible file format"));
                    mb.setText(tr("The Bible format you are importing is of an new version.\n"
                                  "Your current SoftProjector does not support this format.\n"
                                  "Please upgrade SoftProjector to latest version."));
                    mb.setIcon(QMessageBox::Critical);
                    mb.exec();
                }
            }
            file.close();
        }
    }
}

void ManageDataDialog::importBible(QString path, QString version)
{
    setWaitCursor();
    QFile file;
    file.setFileName(path);
    QString line , title, abbr, info, rtol, id;
    QStringList split;
    QSqlQuery sq;
    int row(0);

    if (file.open(QIODevice::ReadOnly))
    {
        line = QString::fromUtf8(file.readLine()); // read version

        line = QString::fromUtf8(file.readLine()); // read title
        split = line.split("\t");
        title = split.at(1);
        line = QString::fromUtf8(file.readLine()); // read abbreviation
        split = line.split("\t");
        abbr = split.at(1);
        line = QString::fromUtf8(file.readLine()); // read info
        split = line.split("\t");
        info = split.at(1);
        // Convert bible information from single line to multiple line
        QStringList info_list = info.split("@%");
        info.clear();
        for(int i(0); i<info_list.size();++i)
        {
            info += info_list[i] + "\n";
        }
        line = QString::fromUtf8(file.readLine()); // read right to left
        split = line.split("\t");
        rtol = split.at(1);


        QProgressDialog progress(tr("Importing..."), tr("Cancel"), 0, 31200, this);
        progress.setValue(row);

        // add Bible Name and information
        QSqlDatabase::database().transaction();
        sq.prepare("INSERT INTO BibleVersions (bible_name, abbreviation, information, right_to_left) VALUES (?,?,?,?)");
        sq.addBindValue(title.trimmed());
        sq.addBindValue(abbr.trimmed());
        sq.addBindValue(info.trimmed());
        sq.addBindValue(rtol.trimmed());
        sq.exec();
        QSqlDatabase::database().commit();
        sq.clear();

        // get Bible id of newly added Bible
        sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'BibleVersions'");
        sq.first() ;
        id = sq.value(0).toString();
        sq.clear();

        // If this bible is the first bible, reload bibles
        if (id.trimmed() == "1")
            reload_bible = true;

        // add Bible book names
        line = QString::fromUtf8(file.readLine());
        QSqlDatabase::database().transaction();
        sq.prepare("INSERT INTO BibleBooks (bible_id, id, book_name, chapter_count) VALUES (?,?,?,?)");
        while (!line.startsWith("---"))
        {
            QString bk_id, bk_name, ch_count;
            split = line.split("\t");
            bk_id = split.at(0);
            bk_name = split.at(1);
            ch_count = split.at(2);

            sq.addBindValue(id);
            sq.addBindValue(bk_id.trimmed());
            sq.addBindValue(bk_name.trimmed());
            sq.addBindValue(ch_count.trimmed());
            sq.exec();

            line = QString::fromUtf8(file.readLine());
            ++row;
            progress.setValue(row);
        }
        QSqlDatabase::database().commit();
        sq.clear();

        // add bible verses
        QSqlDatabase::database().transaction();
        sq.prepare("INSERT INTO BibleVerse (verse_id, bible_id, book, chapter, verse, verse_text)"
                   "VALUES (?,?,?,?,?,?)");
        while (!file.atEnd())
        {
            if (progress.wasCanceled())
                break;

            line = QString::fromUtf8(file.readLine());
            split = line.split("\t");
            sq.addBindValue(split.at(0));
            sq.addBindValue(id);
            sq.addBindValue(split.at(1));
            sq.addBindValue(split.at(2));
            sq.addBindValue(split.at(3));
            sq.addBindValue(split.at(4));
            sq.exec();

            ++row;
            progress.setValue(row);
        }
        QSqlDatabase::database().commit();

        file.close();
    }

    load_bibles();
    setArrowCursor();
}

void ManageDataDialog::on_export_bible_pushButton_clicked()
{
    int row = ui->bibleTableView->currentIndex().row();
    Bibles bible = bible_model->getBible(row);

    QString file_path = QFileDialog::getSaveFileName(this,tr("Save exported Bible as:"),
                                             clean(bible.title),
                                             tr("softProjector Bible file ") + "(*.spb)");
    if( !file_path.isNull() )
        exportBible(file_path,bible);
}

void ManageDataDialog::exportBible(QString path, Bibles bible)
{
    setWaitCursor();
    QProgressDialog progress(tr("Exporting..."), tr("Cancel"), 0, 31000, this);
    int p(1);
    progress.setValue(p);

    QSqlQuery sq;
    QString id = bible.bibleId;
    QString to_file = "##spDataVersion:\t1\n"; // SoftProjector bible file version number is 1 as of 2/26/2011

    QString title, abbr, info, rtol;

    // get Bible version information
    sq.exec("SELECT bible_name, abbreviation, information, right_to_left FROM BibleVersions WHERE id = " + id );
    sq.first();
    title = sq.value(0).toString().trimmed();
    abbr = sq.value(1).toString().trimmed();
    info = sq.value(2).toString().trimmed();
    rtol = sq.value(3).toString().trimmed();
    sq.clear();

    // Convert bible information from multiline to single line
    QStringList info_list = info.split("\n");
    info = info_list[0];
    qDebug()<< QString::number(info_list.size());
    for(int i(1); i<info_list.size();++i)
    {
        info += "@%" + info_list[i];
    }

    to_file += "##Title:\t" + title + "\n" +
               "##Abbreviation:\t" + abbr + "\n" +
               "##Information:\t" + info.trimmed() + "\n" +
               "##RightToLeft:\t" + rtol + "\n";


    // get Bible books information
    sq.exec("SELECT id, book_name, chapter_count FROM BibleBooks WHERE bible_id = " + id );
    while (sq.next())
    {
        ++p;
        progress.setValue(p);

        to_file += sq.value(0).toString().trimmed() + "\t" +    //book id
                   sq.value(1).toString().trimmed() + "\t" +    //book name
                   sq.value(2).toString().trimmed() + "\n";     //chapter count
    }

    // get Bible verses
    to_file += "-----";
    sq.exec("SELECT verse_id, book, chapter, verse, verse_text FROM BibleVerse WHERE bible_id = " + id );
    while (sq.next())
    {
        ++p;
        progress.setValue(p);

        to_file += "\n" + sq.value(0).toString().trimmed() + "\t" + //verse id
                   sq.value(1).toString().trimmed() + "\t" +        //book
                   sq.value(2).toString().trimmed() + "\t" +        //chapter
                   sq.value(3).toString().trimmed() + "\t" +        //verse
                   sq.value(4).toString().trimmed();                //verse text
    }

    QFile ofile;
    ofile.setFileName(path);
    if (ofile.open(QIODevice::WriteOnly))
    {
        QTextStream out(&ofile);
        out.setCodec("UTF8");
        out << to_file;
    }
    ofile.close();
    setArrowCursor();

    QMessageBox mb;
    mb.setWindowTitle(tr("Bible has been exported"));
    mb.setText(tr("Bible:\n     ") + bible.title + tr("\nHas been saved to:\n     " )+ path);
    mb.setIcon(QMessageBox::Information);
    mb.exec();
}

void ManageDataDialog::on_delete_bible_pushButton_clicked()
{
    int row = ui->bibleTableView->currentIndex().row();
    Bibles bible = bible_model->getBible(row);
    QString name = bible.title;

    QMessageBox ms;
    ms.setWindowTitle(tr("Delete Bible?"));
    ms.setText(tr("Are you sure that you want to delete: ")+ name);
    ms.setInformativeText(tr("This action will permanentrly delete this Bible"));
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
    setWaitCursor();
    reload_bible = true;
    QSqlQuery sq;
    QString id = bible.bibleId.trimmed();

    // Delete from BibleBooks Table
    sq.exec("DELETE FROM BibleBooks WHERE bible_id = '" + id + "'");
    sq.clear();

    // Delete form BibleVerse Table
    sq.exec("DELETE FROM BibleVerse WHERE bible_id = '" + id +"'");
    sq.clear();

    // Delete from BibleVersions Table
    sq.clear();
    sq.exec("DELETE FROM BibleVersions WHERE id = '" + id +"'");

    load_bibles();
    setArrowCursor();
}

void ManageDataDialog::on_edit_songbook_pushButton_clicked()
{
    int row = ui->songbookTableView->currentIndex().row();
    Songbook songbook = songbook_model->getSongbook(row);
    QSqlTableModel sq;
    QSqlRecord sr;

    AddSongbookDialog songbook_dialog;
    songbook_dialog.setWindowTitle(tr("Edit Songbook"));
    songbook_dialog.setSongbook(songbook.title,songbook.info);
    int ret = songbook_dialog.exec();
    switch(ret)
    {
    case AddSongbookDialog::Accepted:
        reload_songbook = true;
        sq.setTable("Songbooks");
        sq.setFilter("id = " + songbook.songbookId);
        sq.select();
        sr = sq.record(0);
        sr.setValue(1,songbook_dialog.title.trimmed());
        sr.setValue(2,songbook_dialog.info.trimmed());
        sq.setRecord(0,sr);
        sq.submitAll();
        break;
    case AddSongbookDialog::Rejected:
        break;
    }

    load_songbooks();
}

void ManageDataDialog::on_edit_bible_pushButton_clicked()
{
    int row = ui->bibleTableView->currentIndex().row();
    Bibles bible = bible_model->getBible(row);
    QSqlTableModel sq;
    QSqlRecord sr;
    int r(0);

    BibleInformationDialog bible_info;
    bible_info.setBibleIformation(bible.title,bible.abbr,bible.info,bible.isRtoL);

    int ret = bible_info.exec();
    switch(ret)
    {
    case BibleInformationDialog::Accepted:
        sq.setTable("BibleVersions");
        sq.setFilter("id = " + bible.bibleId.trimmed());
        sq.select();
        sr = sq.record(0);
        sr.setValue(1,bible_info.title.trimmed());
        sr.setValue(2,bible_info.abbr.trimmed());
        sr.setValue(3,bible_info.info.trimmed());
        if(!bible_info.isRtoL)
            r = 0;
        else if (bible_info.isRtoL)
            r = 1;
        sr.setValue(4,r);
        sq.setRecord(0,sr);
        sq.submitAll();
        break;
    case BibleInformationDialog::Rejected:
        break;
    }

    load_bibles();
}

void ManageDataDialog::on_bibleTableView_clicked(QModelIndex index)
{
    updateBibleButtons();
}

void ManageDataDialog::on_songbookTableView_clicked(QModelIndex index)
{
    updateSongbookButtons();
}

void ManageDataDialog::setArrowCursor()
{
    this->setCursor(Qt::ArrowCursor);
    emit setMainArrowCursor();
}

void ManageDataDialog::setWaitCursor()
{
    this->setCursor(Qt::WaitCursor);
    emit setMainWaitCursor();
}

QString ManageDataDialog::getBibleIdUnbound(QString id)
{
    id.remove("O");
    id.remove("N");
    id.remove("A");
    int n_id(0);
    n_id = id.toInt();
    id = QString::number(n_id);
    return id;
}

QString ManageDataDialog::getVerseId(QString book, QString chapter, QString verse)
{
    QString id;
    id = "B" + get3(book.toInt()) + "C" + get3(chapter.toInt()) + "V" + get3(verse.toInt());
    return id;
}

QString ManageDataDialog::get3(int i)
{
    QString st;
    if (i>=100)
    {
        st = st.number(i);
    }
    else if (i>=10)
    {
        st = "0" + st.number(i);
    }
    else
    {
        st = "00" + st.number(i);
    }
    return st;
}
