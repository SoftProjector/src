#include "managedatadialog.h"
#include "ui_managedatadialog.h"
#include "song.h"

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
                                                     tr("Select a Songbook to import"),
                                                     ".",
                                                     tr("softProjector Songbook file (*.txt *.sps)"));

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

            // Set Sbortnik Information
            line = QString::fromUtf8(file.readLine());
            line.remove("#");
            info = line.trimmed();

//            line = QString::fromUtf8(file.readLine());
        }
        else
        {
            title = tr("User Songbook");
            info = tr("Songbook imported by the user");
        }

        // Create songbook
        SongDatabase sdb;
        sdb.addSongbook(title, info);

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
    QString file_path = QFileDialog::getSaveFileName(this,tr("Save the Songbook as:"),
                                             ".",tr("softProjector Songbook file (*.sps)"));
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
    mb.setWindowTitle(tr("Export Complete"));
    mb.setText(tr("The Songbook \"") + title + tr("\"\nHas been saved to:\n     ") + path);
    mb.setIcon(QMessageBox::Information);
    mb.exec();
}

void ManageDataDialog::on_delete_songbook_pushButton_clicked()
{
    int row = ui->songbookTableView->currentIndex().row();
    Songbook songbook = songbook_model->getSongbook(row);
    QString name = songbook.title;

    QMessageBox ms;
    ms.setWindowTitle(tr("Delete Songbook?"));
    ms.setText(tr("Are you sure that you want to delete: ")+ name);
    ms.setInformativeText(tr("This action will permanentrly delete this Songbook"));
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
                                                     ".",
                                                     tr("All supported Bible files (*.spb *.txt);;"
                                                        "softProjector Bible file (*.spb);;Unbound Bible file (*.txt)"));

    // if file_path exits or "Open" is clicked, then import Bible
    if( !file_path.isNull() )
    {
        if(file_path.endsWith("spb",Qt::CaseSensitive))
            importBible(file_path);
        else if (file_path.endsWith("txt",Qt::CaseSensitive))
            importBibleUnbound(file_path);
    }
}

void ManageDataDialog::importBible(QString path)
{
    setWaitCursor();
    QFile file;
    file.setFileName(path);
    QString line, title, id;
    QStringList split;
    QSqlQuery sq;
    int row(0);

    if (file.open(QIODevice::ReadOnly))
    {
        QProgressDialog progress(tr("Importing..."), tr("Cancel"), 0, 31200, this);
        progress.setValue(row);

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
        QSqlDatabase::database().transaction();
        sq.prepare("INSERT INTO BibleBooks (id, book_name, bible_id) VALUES (?,?,?)");
        while (!line.startsWith("---"))
        {
            QString bk_id, bk_name;
            split = line.split("\t");
            bk_id = split[0];
            bk_name = split[1];

            sq.addBindValue(bk_id.trimmed());
            sq.addBindValue(bk_name.trimmed());
            sq.addBindValue(id);
            sq.exec();

            line = QString::fromUtf8(file.readLine());
            row++;
            progress.setValue(row);
        }
        QSqlDatabase::database().commit();
        sq.clear();

        // add bible verses
        QSqlDatabase::database().transaction();
        sq.prepare("INSERT INTO BibleVerse (verse_id, bible_id, book, chapter, verse, verse_text)"
                   "VALUES (?,?,?,?,?,?)");
        QString v="";
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

            // Create duplticate verse for bible search
            sq.addBindValue(split.at(0));
            sq.addBindValue(id+"_");
            sq.addBindValue(split.at(1));
            sq.addBindValue(split.at(2));
            sq.addBindValue(split.at(3));
            v=split.at(4);
            v = v.toLower();    // Convert to all to lower case
            v = clean(v);       // Remove all non alphanumeric charecters
            v = v.simplified(); // Leave only single white space between each word
            sq.addBindValue(v);
            sq.exec();

            row++;
            progress.setValue(row);
        }
        QSqlDatabase::database().commit();
        file.close();
    }

    load_bibles();
    setArrowCursor();
}

void ManageDataDialog::importBibleUnbound(QString path)
{
    setWaitCursor();
    QFile file;
    file.setFileName(path);
    QString line, title, id, book_id;
    QStringList split, book_names;
    book_names << "Genesis" << "Exodus" << "Leviticus" << "Numbers" << "Deuteronomy" << "Joshua"
            << "Judges" << "Ruth" << "1 Samuel" << "2 Samuel" << "1 Kings" << "2 Kings" << "1 Chronicles"
            << "2 Chronicles" << "Ezra" << "Nehemiah" << "Esther" << "Job" << "Psalms" << "Proverbs"
            << "Ecclesiastes" << "Song of Solomon" << "Isaiah" << "Jeremiah" << "Lamentations" << "Ezekiel"
            << "Daniel" << "Hosea" << "Joel" << "Amos" << "Obadiah" << "Jonah" << "Micah" << "Nahum"
            << "Habakkuk" << "Zephaniah" << "Haggai" << "Zechariah" << "Malachi" << "Matthew" << "Mark"
            << "Luke" << "John" << "Acts of the Apostles" << "Romans" << "1 Corinthians" << "2 Corinthians"
            << "Galatians" << "Ephesians" << "Philippians" << "Colossians" << "1 Thessalonians"
            << "2 Thessalonians" << "1 Timothy" << "2 Timothy" << "Titus" << "Philemon" << "Hebrews"
            << "James" << "1 Peter" << "2 Peter" << "1 John" << "2 John" << "3 John" << "Jude" << "Revelation";
    QSqlQuery sq;
    int row(0), c_book(0), c_chap(0), c_verse(0), c_text(0);

    if (file.open(QIODevice::ReadOnly))
    {
        line = QString::fromUtf8(file.readLine());
        if(line.startsWith("#THE UNBOUND BIBLE"))
        {
            QProgressDialog progress(tr("Importing..."), tr("Cancel"), 0, 31200, this);
            progress.setValue(row);

            // Prepare Bible
            line = QString::fromUtf8(file.readLine());
            while(line.startsWith("#"))
            {
                if(line.startsWith("#name"))// add Bible Name
                {
                    split = line.split("\t");
                    title = split.at(1);
                    sq.exec("INSERT INTO BibleVersions (bible_name)"
                            "VALUES ('" + title.trimmed()+"')");
                    sq.clear();
                }
                else if(line.startsWith("#columns"))// Prepare column indicies
                {
                    split = line.split("\t");
                    QString col;
                    for(int i(0); i<split.count(); ++i)
                    {
                        col = split.at(i);
                        col = col.trimmed();
                        if(col=="orig_book_index")
                            c_book = i-1;
                        else if(col=="orig_chapter")
                            c_chap = i-1;
                        else if(col=="orig_verse")
                            c_verse = i-1;
                        else if(col=="text")
                            c_text = i-1;
                    }
                    break;
                }
                line = QString::fromUtf8(file.readLine());
            }

            // get Bible id of newly added Bible
            sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'BibleVersions'");
            sq.first() ;
            id = sq.value(0).toString();
            sq.clear();

            // add Bible book names
            QSqlDatabase::database().transaction();
            sq.prepare("INSERT INTO BibleBooks (id, book_name, bible_id) VALUES (?,?,?)");
            QString bk_id, bk_name;
            for(int i(0); i<book_names.count(); ++i)
            {

                bk_id = QString::number(i+1);
                bk_name = book_names.at(i);

                sq.addBindValue(bk_id.trimmed());
                sq.addBindValue(bk_name.trimmed());
                sq.addBindValue(id);
                sq.exec();

                row++;
                progress.setValue(row);
            }
            QSqlDatabase::database().commit();
            sq.clear();

            // add bible verses
            QSqlDatabase::database().transaction();
            sq.prepare("INSERT INTO BibleVerse (verse_id, bible_id, book, chapter, verse, verse_text)"
                       "VALUES (?,?,?,?,?,?)");
            QString v="";
            while (!file.atEnd())
            {
                if (progress.wasCanceled())
                    break;

                line = QString::fromUtf8(file.readLine());

                split = line.split("\t");
                v=split.at(c_text);
                v=v.trimmed() + "\n";
                sq.addBindValue(getVerseId(getBibleIdUnbound(split.at(c_book)),split.at(c_chap),split.at(c_verse)));       // Verse Id
                sq.addBindValue(id);                // Bible Id
                sq.addBindValue(getBibleIdUnbound(split.at(c_book)));  // Book
                sq.addBindValue(split.at(c_chap));  // Chapter
                sq.addBindValue(split.at(c_verse)); // Verse
                sq.addBindValue(v);  // Verse Text
                sq.exec();

                // Create duplticate verse for bible search
                sq.addBindValue(getVerseId(getBibleIdUnbound(split.at(c_book)),split.at(c_chap),split.at(c_verse)));       // Verse Id
                sq.addBindValue(id+"_");
                sq.addBindValue(getBibleIdUnbound(split.at(c_book)));  // Book
                sq.addBindValue(split.at(c_chap));  // Chapter
                sq.addBindValue(split.at(c_verse)); // Verse
                v = v.toLower();    // Convert to all to lower case
                v = clean(v);       // Remove all non alphanumeric charecters
                v = v.simplified(); // Leave only single white space between each word
                sq.addBindValue(v);
                sq.exec();

                row++;
                progress.setValue(row);
            }
            QSqlDatabase::database().commit();
            progress.close();
        }
        else
        {
            //TODO: Message box with wrong file type
        }
        file.close();
    }

    load_bibles();
    setArrowCursor();
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
    setWaitCursor();
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

    // Delete form BibleVerse Table
    sq.exec("DELETE FROM BibleVerse WHERE bible_id = '" + id +"_'");
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
//        close();
        break;
    }

    load_songbooks();
}

void ManageDataDialog::on_edit_bible_pushButton_clicked()
{
    int row = ui->bibleTableView->currentIndex().row();
    Bibles bible = bible_model->getBible(row);
    bool ok;
    QString name = QInputDialog::getText(this,tr("Edit Bible Name"),
                                         tr("Bible title:"),QLineEdit::Normal,
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
