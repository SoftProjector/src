/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyleft (Ɔ) 2013  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

ManageDataDialog::ManageDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageDataDialog)
{
    ui->setupUi(this);

    // Set tables
    bible_model = new BiblesModel;
    songbook_model = new SongbooksModel;
    themeModel = new ThemeModel;

    // Set Bible Table
    load_bibles();
    ui->bibleTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->bibleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bibleTableView->verticalHeader()->hide();
    ui->bibleTableView->setColumnWidth(0, 395);

    // Set Songbooks Table
    load_songbooks();
    ui->songbookTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->songbookTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->songbookTableView->verticalHeader()->hide();
    ui->songbookTableView->setColumnWidth(0, 195);
    ui->songbookTableView->setColumnWidth(1, 195);

    // Set Theme Table
    loadThemes();
    ui->TableViewTheme->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->TableViewTheme->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->TableViewTheme->verticalHeader()->hide();
    ui->TableViewTheme->setColumnWidth(0, 195);
    ui->TableViewTheme->setColumnWidth(1, 195);

    //Set reload cariers to false
    reload_bible = false;
    reload_songbook = false;
    reloadThemes = false;
}

ManageDataDialog::~ManageDataDialog()
{
    delete bible_model;
    delete songbook_model;
    delete themeModel;
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
    songbook_model->setSongbook(songbook_list);
    ui->songbookTableView->setModel(songbook_model);
    updateSongbookButtons();
}

void ManageDataDialog::load_bibles()
{
    Database db;
    // Set Bible Table
    bible_list = db.getBibles();
    bible_model->setBible(bible_list);
    ui->bibleTableView->setModel(bible_model);
    updateBibleButtons();
}

void ManageDataDialog::loadThemes()
{
    Database db;
    // Set Songbooks Table
    themeList = db.getThemes();
    themeModel->setThemes(themeList);
    ui->TableViewTheme->setModel(themeModel);
    updateThemeButtons();
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

void ManageDataDialog::updateThemeButtons()
{
    bool enable_edit;
    bool enable_export;
    bool enable_delete;
    bool enable_export_all;

    if (ui->TableViewTheme->hasFocus())
    {
        enable_edit = true;
        enable_export = true;
        if (themeModel->rowCount()>=2)
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
    if (themeModel->rowCount()>=1)
        enable_export_all = true;
    else
        enable_export_all = false;

    ui->pushButtonThemeEdit->setEnabled(enable_edit);
    ui->pushButtonThemeExport->setEnabled(enable_export);
    ui->pushButtonThemeDelete->setEnabled(enable_delete);
    ui->pushButtonThemeExportAll->setEnabled(enable_export_all);
}

void ManageDataDialog::on_import_songbook_pushButton_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this,
                                                     tr("Select a songbook to import"),
                                                     ".",
                                                     tr("SoftProjector songbook file ") + "(*.sps)");

    // if file_path exits or "Open" is clicked, then import a Songbook
    if( !file_path.isNull() )
        importSongbook(file_path);
}

void ManageDataDialog::importSongbook(QString path)
{
    setWaitCursor();
    int row(0), max(0);
    QFile file(path), file2(path), fileXml(path);
    QString line, code, title, info, num;
    QStringList split;
    QSqlQuery sq, sq1;
    SongDatabase sdb;

    // get max number for progress bar
    if (file2.open(QIODevice::ReadOnly))
    {
        while (!file2.atEnd())
        {
            line = QString::fromUtf8(file2.readLine());
            if(line.startsWith("SQLite"))
                break;
            ++max;
        }
    }
    file2.close();

    // Start Importing
    QProgressDialog progress(tr("Importing..."), tr("Cancel"), 0, max, this);
    progress.setValue(row);

    if (file.open(QIODevice::ReadOnly))
    {

        reload_songbook = true;
        //Set Songbook Title and Information
        line = QString::fromUtf8(file.readLine());
        if (line.startsWith("##")) // Files format before vertion 2.0
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
            toMultiLine(info);

            // Create songbook
            sdb.addSongbook(title, info.trimmed());

            // Set Songbook Code
            code = "0";
            sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Songbooks'");
            while (sq.next())
                code = sq.value(0).toString();
            sq.clear();

            // Import Songs
            QSqlDatabase::database().transaction();
            sq.prepare("INSERT INTO Songs (songbook_id, number, title, category, tune, words, music, "
                       "song_text, font, background_name, notes)"
                       "VALUES (?,?,?,?,?,?,?,?,?,?,?)");
            while (!file.atEnd())
            {
                if (progress.wasCanceled())
                    break;
                line = QString::fromUtf8(file.readLine());
                split = line.split("#$#");

                num = split[0];

                // Add song to Songs table
                sq.addBindValue(code);//songbook id
                sq.addBindValue(num);//number
                sq.addBindValue(split[1]);//title
                sq.addBindValue(split[2]);//cat
                sq.addBindValue(split[3]);//tune
                sq.addBindValue(split[4]);//words
                sq.addBindValue(split[5]);//music
                sq.addBindValue(split[6]);//song text
                if (split.count() > 7)
                {
                    sq.addBindValue(split[7]);//font
                    //sq.addBindValue(split[8]);//alignment
                    sq.addBindValue(split[9]);//background
                    if(split.count()>10)
                    {
                        QString note = split[10];
                        toMultiLine(note);
                        sq.addBindValue(note);//notes
                    }
                    else
                        sq.addBindValue("");//notes
                }
                else
                {
                    sq.addBindValue("");//font
                    //sq.addBindValue("");//alignment
                    sq.addBindValue("");//background
                    sq.addBindValue("");//notes
                }
                sq.exec();

                ++row;
                progress.setValue(row);
            }
            QSqlDatabase::database().commit();
        }
        else if(line.startsWith("<?xml")) // XML file format
        {
            fileXml.open(QIODevice::ReadOnly);
            QXmlStreamReader xml (&fileXml);
            while(!xml.atEnd())
            {
                xml.readNext();
                if(xml.StartElement && xml.name() == "spSongBook")
                {
                    double sb_version = xml.attributes().value("version").toString().toDouble();
                    if(sb_version == 2.0) // check supported songbook version
                    {
                        xml.readNext();
                        // Prepare to import Songs
                        QSqlDatabase::database().transaction();
                        sq.prepare("INSERT INTO Songs (songbook_id, number, title, category, tune, words, music, "
                                   "song_text, notes, use_private, alignment_v, alignment_h, color, font, "
                                   "background_name, count, date)"
                                   "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

                        while(xml.tokenString() != "EndElement" && xml.name() != "spSongBook")
                        {
                            xml.readNext();
                            if(xml.StartElement && xml.name() == "SongBook")
                            {
                                QString xtitle,xinfo;
                                // Read songbook data
                                xml.readNext();
                                while(xml.tokenString() != "EndElement")
                                {
                                    xml.readNext();
                                    if(xml.StartElement && xml.name() == "title")
                                    {
                                        xtitle = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "info")
                                    {
                                        xinfo = xml.readElementText();
                                        xml.readNext();
                                    }
                                    ++row;
                                    progress.setValue(row);
                                }
                                // Save songbook
                                sdb.addSongbook(xtitle,xinfo);

                                // Set Songbook Code
                                code = "0";
                                sq1.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Songbooks'");
                                sq1.first();
                                code = sq1.value(0).toString();

                                xml.readNext();
                            }
                            else if (xml.StartElement && xml.name() == "Song")
                            {
                                QString xnum,xtitle,xcat,xtune,xwords,xmusic,xtext,xnotes,
                                        xuse,xalign,xcolor,xfont,xback,xcount,xdate;
                                // Read song data
                                xnum = xml.attributes().value("number").toString();
                                xml.readNext();
                                while(xml.tokenString() != "EndElement")
                                {
                                    xml.readNext();
                                    if(xml.StartElement && xml.name() == "title")
                                    {
                                        xtitle = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "category")
                                    {
                                        xcat = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "tune")
                                    {
                                        xtune = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "words")
                                    {
                                        xwords = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "music")
                                    {
                                        xmusic = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "song_text")
                                    {
                                        xtext = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "notes")
                                    {
                                        xnotes = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "use_private")
                                    {
                                        xuse = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "alignment")
                                    {
                                        xalign = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "color")
                                    {
                                        xcolor = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "font")
                                    {
                                        xfont = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "background")
                                    {
                                        xback = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "count")
                                    {
                                        xcount = xml.readElementText();
                                        xml.readNext();
                                    }
                                    else if(xml.StartElement && xml.name() == "date")
                                    {
                                        xdate = xml.readElementText();
                                        xml.readNext();
                                    }
                                }

                                // Save song
                                sq.addBindValue(code);
                                sq.addBindValue(xnum);
                                sq.addBindValue(xtitle);
                                sq.addBindValue(xcat);
                                sq.addBindValue(xtune);
                                sq.addBindValue(xwords);
                                sq.addBindValue(xmusic);
                                sq.addBindValue(xtext);
                                sq.addBindValue(xnotes);
                                sq.addBindValue(xuse);
                                if(xalign.contains(","))
                                {
                                    QStringList l = xalign.split(",");
                                    sq.addBindValue(l.at(0));
                                    sq.addBindValue(l.at(1));
                                }
                                else
                                {
                                    sq.addBindValue(1);
                                    sq.addBindValue(1);
                                }
                                sq.addBindValue(xcolor);
                                sq.addBindValue(xfont);
                                sq.addBindValue(xback);
                                sq.addBindValue(xcount);
                                sq.addBindValue(xdate);
                                sq.exec();

                                row += 16;
                                progress.setValue(row);
                                xml.readNext();
                            }
                        }// end while xml.tokenString() != "EndElement" && xml.name() != "spSongBook"
                        QSqlDatabase::database().commit();
                    }// end correct version
                } // end if xml name is spSongBook
            }
        }
        else if(line.startsWith("SQLite")) // SQLITE database file
        {
            file.close();
            sq.clear();
            {
                QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","sps");
                db.setDatabaseName(path);
                if(db.open())
                {
                    QSqlQuery q(db);
                    q.exec("PRAGMA user_version");
                    q.first();
                    int spsVer = q.value(0).toInt();
                    if(spsVer == 2)
                    {
                        QSqlDatabase::database().transaction();
                        progress.setMaximum(0);
                        progress.show();
                        // Prepare and save songbook
                        q.exec("SELECT title, info from SongBook");
                        q.first();
                        sq.exec(QString("INSERT INTO Songbooks (name,info) VALUES('%1','%2')")
                                .arg(q.value(0).toString()).arg(q.value(1).toString()));
                        sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Songbooks'");
                        sq.first();
                        int sbookid = sq.value(0).toInt();

                        // Get and insert songs
                        q.exec("SELECT * FROM Songs");
                        sq.prepare("INSERT INTO Songs (songbook_id,number,title,category,tune,words,music,song_text,notes,"
                                   "use_private,alignment_v,alignment_h,color,font,info_color,info_font,ending_color,"
                                   "ending_font,use_background,background_name,background,count,date) "
                                   "VALUES(:id, :num, :ti, :ca, :tu, :wo, :mu, :st, :no, :up, :av, :ah, :tc, :tf, "
                                   ":ic, :if, :ec, :ef, :ub, :bn, :b, :ct, :d)");
                        while(q.next())
                        {
                            sq.bindValue(":id",sbookid);
                            sq.bindValue(":num",q.record().value("number"));
                            sq.bindValue(":ti",q.record().value("title"));
                            sq.bindValue(":ca",q.record().value("category"));
                            sq.bindValue(":tu",q.record().value("tune"));
                            sq.bindValue(":wo",q.record().value("words"));
                            sq.bindValue(":mu",q.record().value("music"));
                            sq.bindValue(":st",q.record().value("song_text"));
                            sq.bindValue(":no",q.record().value("notes"));
                            sq.bindValue(":up",q.record().value("use_private"));
                            sq.bindValue(":av",q.record().value("alignment_v"));
                            sq.bindValue(":ah",q.record().value("alignment_h"));
                            sq.bindValue(":tc",q.record().value("color"));
                            sq.bindValue(":tf",q.record().value("font"));
                            sq.bindValue(":ic",q.record().value("info_color"));
                            sq.bindValue(":if",q.record().value("info_font"));
                            sq.bindValue(":ec",q.record().value("ending_color"));
                            sq.bindValue(":ef",q.record().value("ending_font"));
                            sq.bindValue(":ub",q.record().value("use_background"));
                            sq.bindValue(":bn",q.record().value("background_name"));
                            sq.bindValue(":b",q.record().value("background"));
                            sq.bindValue(":ct",q.record().value("count"));
                            sq.bindValue(":d",q.record().value("date"));
                            sq.exec();
                        }
                        progress.close();

                        QSqlDatabase::database().commit();
                    }
                    else if(spsVer > 2)
                    {
                        QMessageBox mb;
                        mb.setWindowTitle(tr("Unsupported SongBook version."));
                        mb.setText(tr("The SongBook file you are opening, is of a later release and \n"
                                      "is not supported by current version of SoftProjector.\n"
                                      "You are trying to open SongBook version %1.\n"
                                      "Please upgrade to latest version of SoftProjector and try again.").arg(spsVer));
                        mb.setIcon(QMessageBox::Information);
                        mb.exec();
                    }
                    else
                    {
                        QMessageBox mb;
                        mb.setWindowTitle(tr("Unsupported SongBook version."));
                        mb.setText(tr("The SongBook file you are opening, is not supported \n"
                                      "by current version of SoftProjector.\n"));
                        mb.setIcon(QMessageBox::Information);
                        mb.exec();
                    }
                }
            }
            QSqlDatabase::removeDatabase("sps");
        }
        else// too old file format.
        {
            //User friednly box for incorrect file version
            QMessageBox mb;
            mb.setWindowTitle(tr("Too old SongBook file format"));
            mb.setText(tr("The SongBook file you are opening, is in very old format\n"
                          "and is no longer supported by current version of SoftProjector.\n"
                          "You may try to import it with version 1.07 and then export it, and import it again."));
            mb.setIcon(QMessageBox::Information);
            mb.exec();
        }
    }

    load_songbooks();
    setArrowCursor();
}

void ManageDataDialog::on_export_songbook_pushButton_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(this,tr("Save the songbook as:"),
                                                     ".",tr("SoftProjector songbook file (*.sps)"));
    if(!file_path.isEmpty())
    {
        if(!file_path.endsWith(".sps"))
            file_path = file_path + ".sps";
        exportSongbook(file_path);
    }
}

void ManageDataDialog::exportSongbook(QString path)
{
    setWaitCursor();
    int row = ui->songbookTableView->currentIndex().row();
    QString songbook_id = songbook_model->getSongbook(row).songbookId;
    QSqlQuery sq;
    QString title;

    // First Delete file if one already exists

    if(QFile::exists(path))
    {
        if(!QFile::remove(path))
        {
            QMessageBox mb;
            mb.setText(tr("An error has ocured when overwriting existing file.\n"
                          "Please try again with different file name."));
            mb.setIcon(QMessageBox::Information);
            mb.setStandardButtons(QMessageBox::Ok);
            mb.exec();
            return;
        }
    }

    {
        // Prepare SQLite songbook database file
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","sps");
        db.setDatabaseName(path);
        if(db.open())
        {
            db.transaction();
            QSqlQuery q(db);
            q.exec("PRAGMA user_version = 2");
            q.exec("CREATE TABLE 'SongBook' ('title' TEXT, 'info' TEXT)");
            q.exec("CREATE TABLE 'Songs' ('number' INTEGER, 'title' TEXT, 'category' INTEGER DEFAULT 0, "
                   "'tune' TEXT, 'words' TEXT, 'music' TEXT, 'song_text' TEXT, 'notes' TEXT, "
                   "'use_private' BOOL, 'alignment_v' INTEGER, 'alignment_h' INTEGER, 'color' INTEGER, 'font' TEXT, "
                   "'info_color' INTEGER, 'info_font' TEXT, 'ending_color' INTEGER, 'ending_font' TEXT, "
                   "'use_background' BOOL, 'background_name' TEXT, 'background' BLOB, 'count' INTEGER DEFAULT 0, 'date' TEXT)");

            // Get/Write SongBook information
            sq.exec("SELECT name, info FROM Songbooks WHERE id = " + songbook_id);
            sq.first();

            q.exec(QString("INSERT INTO SongBook (title,info) VALUES('%1','%2')")
                   .arg(sq.value(0).toString()).arg(sq.value(1).toString()));
            title = sq.value(0).toString();
            sq.clear();

            // Write Songs
            sq.exec("SELECT * FROM Songs WHERE songbook_id = " + songbook_id);
            q.prepare("INSERT INTO Songs (number,title,category,tune,words,music,song_text,notes,"
                      "use_private,alignment_v,alignment_h,color,font,info_color,info_font,ending_color,ending_font,"
                      "use_background,background_name,background,count,date) "
                      "VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
            while(sq.next())
            {
                q.addBindValue(sq.record().value("number"));
                q.addBindValue(sq.record().value("title"));
                q.addBindValue(sq.record().value("category"));
                q.addBindValue(sq.record().value("tune"));
                q.addBindValue(sq.record().value("words"));
                q.addBindValue(sq.record().value("music"));
                q.addBindValue(sq.record().value("song_text"));
                q.addBindValue(sq.record().value("notes"));
                q.addBindValue(sq.record().value("use_private"));
                q.addBindValue(sq.record().value("alignment_v"));
                q.addBindValue(sq.record().value("alignment_h"));
                q.addBindValue(sq.record().value("color"));
                q.addBindValue(sq.record().value("font"));
                q.addBindValue(sq.record().value("info_color"));
                q.addBindValue(sq.record().value("info_font"));
                q.addBindValue(sq.record().value("ending_color"));
                q.addBindValue(sq.record().value("ending_font"));
                q.addBindValue(sq.record().value("use_background"));
                q.addBindValue(sq.record().value("background_name"));
                q.addBindValue(sq.record().value("background"));
                q.addBindValue(sq.record().value("count"));
                q.addBindValue(sq.record().value("date"));
                q.exec();
            }
            db.commit();
        }
    }
    QSqlDatabase::removeDatabase("sps");

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
    QSqlQuery sq;
    QString id = songbook.songbookId.trimmed();

    // Delete from Songbook Table
    sq.exec("DELETE FROM Songbooks WHERE id = '" + id + "'");
    sq.clear();

    // Delete from Songs Table
    sq.exec("DELETE FROM Songs WHERE songbook_id = '" + id +"'");

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
                              "Please download lattest Bibles and import them."));
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
                                                     tr("SoftProjector Bible file ") + "(*.spb)");
    if(!file_path.isEmpty())
    {
        if(!file_path.endsWith(".spb"))
            file_path = file_path + ".spb";
        exportBible(file_path,bible);
    }

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

void ManageDataDialog::toMultiLine(QString &mline)
{
    QStringList line_list = mline.split("@%");
    mline.clear();
    for(int i(0); i<line_list.size();++i)
    {
        mline += line_list[i] + "\n";
    }
    mline = mline.trimmed();
}

void ManageDataDialog::toSingleLine(QString &sline)
{
    QStringList line_list = sline.split("\n");
    sline = line_list[0];
    for(int i(1); i<line_list.size();++i)
    {
        sline += "@%" + line_list[i];
    }
    sline = sline.trimmed();
}

void ManageDataDialog::on_pushButtonThemeNew_clicked()
{
    QSqlTableModel sqt;
    QSqlQuery sq;
    Theme tm;
    ThemeInfo tmi;
    QString nId;

    AddSongbookDialog theme_dia;
    theme_dia.setWindowTitle(tr("Edit Theme"));
    theme_dia.setWindowText(tr("Theme Name:"),tr("Comments:"));
    theme_dia.setSongbook(tr("Default"),tr("This theme will contain program default settings."));
    int ret = theme_dia.exec();
    switch(ret)
    {
    case AddSongbookDialog::Accepted:
        tmi.name = theme_dia.title;
        tmi.comments = theme_dia.info;
        tm.setThemeInfo(tmi);
        tm.saveThemeNew();
//        sqt.setTable("Themes");
//        sqt.insertRow(0);
//        sqt.setData(sqt.index(0,1),theme_dia.title);
//        sqt.setData(sqt.index(0,2),theme_dia.info);
//        sqt.submitAll();


//        sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Themes'");
//        sq.first();
//        nId = sq.value(0).toString();

//        tm.saveNewTheme(nId);

        loadThemes();
        break;
    case AddSongbookDialog::Rejected:
        break;
    }

}

void ManageDataDialog::on_pushButtonThemeImport_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this,
                                                     tr("Select a Theme to import"),
                                                     ".",
                                                     tr("SoftProjector Theme file ") + "(*.spt)");

    // if file_path exits or "Open" is clicked, then import a Songbook
    if( !file_path.isNull() )
    {
        setWaitCursor();

        QFile file(file_path),filen(file_path);

        // get theme count
        int max(0),cur(0);
        filen.open(QIODevice::ReadOnly);
        while(!filen.atEnd())
        {
            QString l = filen.readLine();
            if(l.contains("<Theme>"))
                ++max;
        }
        QProgressDialog progress(tr("Importing..."), tr("Cancel"), 0, max, this);

        file.open(QIODevice::ReadOnly);
        QXmlStreamReader xmlr(&file);
        while(!xmlr.atEnd())
        {
            xmlr.readNext();
            if(xmlr.StartElement && xmlr.name() == "spThemes")
            {
                double st_version = xmlr.attributes().value("version").toString().toDouble();
                if(st_version == 2.0)
                {
                    xmlr.readNext();
                    while(xmlr.tokenString() != "EndElement" && xmlr.name() != "spThemes")
                    {
                        xmlr.readNext();
                        if(xmlr.StartElement && xmlr.name() == "Theme")
                        {
                            ++cur;
                            progress.setValue(cur);
                            importTheme(xmlr);
                            xmlr.readNext();
                        }
                    }
                }// end correct version
            }// end of xmlr name is spThemes
        }
        loadThemes();
        progress.close();
        setArrowCursor();
    }
}

void ManageDataDialog::importTheme(QXmlStreamReader &xml)
{
//    ThemeInfo tminfo;
//                QSqlTableModel sqt;
//                QSqlQuery sq;
//    xml.readNext();
//    while(xml.tokenString() != "EndElement" && xml.name() != "Theme")
//    {
//        xml.readNext();
//        if(xml.StartElement && xml.name() == "ThemeInfo")
//        {

//            xml.readNext();
//            while(xml.tokenString() != "EndElement")
//            {
//                xml.readNext();
//                if(xml.StartElement && xml.name() == "name")
//                {
//                    tminfo.name = xml.readElementText();
//                    xml.readNext();
//                }
//                else if(xml.StartElement && xml.name() == "comments")
//                {
//                    tminfo.comments = xml.readElementText();
//                    xml.readNext();
//                }
//            }
//            sqt.setTable("Themes");
//            sqt.insertRow(0);
//            sqt.setData(sqt.index(0,1),tminfo.name);
//            sqt.setData(sqt.index(0,2),tminfo.comments);
//            sqt.submitAll();


//            sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Themes'");
//            sq.first();
//            tminfo.themeId = sq.value(0).toString();
//            sq.clear();
////            tmx.saveNewTheme(nId);
//            xml.readNext();
//        }
//        else if(xml.StartElement && xml.name() == "ThemeData")
//        {
//            QSqlDatabase::database().transaction();
//            sq.prepare("INSERT INTO ThemeData (theme_id, type, sets) VALUES (?,?,?)");
//            xml.readNext();
//            while(xml.tokenString() != "EndElement")
//            {
//                xml.readNext();
//                if(xml.StartElement && xml.name() == "passive")
//                {
//                    sq.addBindValue(tminfo.themeId);
//                    sq.addBindValue("passive");
//                    sq.addBindValue(xml.readElementText());
//                    sq.exec();
//                    xml.readNext();
//                }
//                else if(xml.StartElement && xml.name() == "passive2")
//                {
//                    sq.addBindValue(tminfo.themeId);
//                    sq.addBindValue("passive2");
//                    sq.addBindValue(xml.readElementText());
//                    sq.exec();
//                    xml.readNext();
//                }
//                else if(xml.StartElement && xml.name() == "bible")
//                {
//                    sq.addBindValue(tminfo.themeId);
//                    sq.addBindValue("bible");
//                    sq.addBindValue(xml.readElementText());
//                    sq.exec();
//                    xml.readNext();
//                }
//                else if(xml.StartElement && xml.name() == "bible2")
//                {
//                    sq.addBindValue(tminfo.themeId);
//                    sq.addBindValue("bible2");
//                    sq.addBindValue(xml.readElementText());
//                    sq.exec();
//                    xml.readNext();
//                }
//                else if(xml.StartElement && xml.name() == "song")
//                {
//                    sq.addBindValue(tminfo.themeId);
//                    sq.addBindValue("song");
//                    sq.addBindValue(xml.readElementText());
//                    sq.exec();
//                    xml.readNext();
//                }
//                else if(xml.StartElement && xml.name() == "song2")
//                {
//                    sq.addBindValue(tminfo.themeId);
//                    sq.addBindValue("song2");
//                    sq.addBindValue(xml.readElementText());
//                    sq.exec();
//                    xml.readNext();
//                }
//                else if(xml.StartElement && xml.name() == "announce")
//                {
//                    sq.addBindValue(tminfo.themeId);
//                    sq.addBindValue("announce");
//                    sq.addBindValue(xml.readElementText());
//                    sq.exec();
//                    xml.readNext();
//                }
//                else if(xml.StartElement && xml.name() == "announce2")
//                {
//                    sq.addBindValue(tminfo.themeId);
//                    sq.addBindValue("announce2");
//                    sq.addBindValue(xml.readElementText());
//                    sq.exec();
//                    xml.readNext();
//                }
//            }
//            xml.readNext();
//            QSqlDatabase::database().commit();
//        }
//    }
}

void ManageDataDialog::on_pushButtonThemeEdit_clicked()
{
    int row = ui->TableViewTheme->currentIndex().row();
    ThemeInfo theme = themeModel->getTheme(row);
    QSqlQuery sq;

    AddSongbookDialog theme_dia;
    theme_dia.setWindowTitle(tr("Edit Theme"));
    theme_dia.setWindowText(tr("Theme Name:"),tr("Comments:"));
    theme_dia.setSongbook(theme.name,theme.comments);
    int ret = theme_dia.exec();
    switch(ret)
    {
    case AddSongbookDialog::Accepted:
        reloadThemes = true;
        sq.prepare("UPDATE Themes SET name = ?, comment = ? WHERE id = ?");
        sq.addBindValue(theme_dia.title);
        sq.addBindValue(theme_dia.info);
        sq.addBindValue(theme.themeId);
        sq.exec();
        loadThemes();
        break;
    case AddSongbookDialog::Rejected:
        break;
    }
}

void ManageDataDialog::on_pushButtonThemeExport_clicked()
{
    int row = ui->TableViewTheme->currentIndex().row();
    ThemeInfo tmInfo = themeModel->getTheme(row);

    QString file_path = QFileDialog::getSaveFileName(this,tr("Export Theme as:"),
                                                     clean(tmInfo.name),
                                                     tr("SoftProjector Theme file ") + "(*.spt)");
    if(!file_path.isEmpty())
    {
        if(!file_path.endsWith(".spt"))
            file_path = file_path + ".spt";

        QFile file(file_path);
        file.open(QIODevice::WriteOnly);

        QXmlStreamWriter xmlw(&file);
        xmlw.setAutoFormatting(true);
        xmlw.setCodec("UTF8");

        xmlw.writeStartDocument();
        xmlw.writeStartElement("spThemes");
        xmlw.writeAttribute("version","2.0");

        // write theme data
        exportTheme(xmlw,tmInfo);

        xmlw.writeEndElement();// End spThemes
        xmlw.writeEndDocument();
        file.close();
    }
}

void ManageDataDialog::on_pushButtonThemeExportAll_clicked()
{
    QString file_path = QFileDialog::getSaveFileName(this,tr("Export all theme as:"),
                                                     ".", tr("softProjector Theme file ") + "(*.spt)");
    if(!file_path.isEmpty())
    {
        if(!file_path.endsWith(".spt"))
            file_path = file_path + ".spt";

        QFile file(file_path);
        file.open(QIODevice::WriteOnly);

        QXmlStreamWriter xmlw(&file);
        xmlw.setAutoFormatting(true);
        xmlw.setCodec("UTF8");

        xmlw.writeStartDocument();
        xmlw.writeStartElement("spThemes");
        xmlw.writeAttribute("version","2.0");

        // write theme data
        for(int i(0);i<themeList.count();++i)
        {
            ThemeInfo tminf = themeList.at(i);
            exportTheme(xmlw,tminf);
        }


        xmlw.writeEndElement();// End spThemes
        xmlw.writeEndDocument();
        file.close();
    }
}

void ManageDataDialog::exportTheme(QXmlStreamWriter &xml, ThemeInfo &tmInfo)
{
    // Start Theme
//    xml.writeStartElement("Theme"); // start Theme

//    xml.writeStartElement("ThemeInfo"); // start Theme Info
//    xml.writeTextElement("name",tmInfo.name);
//    xml.writeTextElement("comments",tmInfo.comments);
//    xml.writeEndElement(); // End Theme Info

//    // Write theme data
//    xml.writeStartElement("ThemeData"); // Start Theme Data
//    QSqlQuery sq;
//    sq.exec("SELECT type, sets FROM ThemeData WHERE theme_id = '"+tmInfo.themeId+"'");

//    while(sq.next())
//        xml.writeTextElement(sq.value(0).toString().trimmed(),sq.value(1).toString().trimmed());

//    xml.writeEndElement(); // End Theme Data

//    xml.writeEndElement();  // End Theme
}

void ManageDataDialog::on_pushButtonThemeDelete_clicked()
{

    int row = ui->TableViewTheme->currentIndex().row();
    ThemeInfo tm = themeModel->getTheme(row);
    QString name = tm.name;

    QMessageBox ms;
    ms.setWindowTitle(tr("Delete Theme?"));
    ms.setText(tr("Are you sure that you want to delete theme: ")+ name);
    ms.setInformativeText(tr("This action will permanentrly delete this theme"));
    ms.setIcon(QMessageBox::Question);
    ms.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    ms.setDefaultButton(QMessageBox::Yes);
    int ret = ms.exec();

    switch (ret) {
    case QMessageBox::Yes:
        // Delete a songbook
        deleteTheme(tm);
        break;
    case QMessageBox::No:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }
}

void ManageDataDialog::deleteTheme(ThemeInfo tme)
{
    setWaitCursor();
    QSqlQuery sq;
    int id = tme.themeId;
    reloadThemes = true;

    // Delete from Themes Table
    sq.exec("DELETE FROM Themes WHERE id = " + QString::number(id));
    sq.clear();

    // Delete from ThemeData Table
    sq.exec("DELETE FROM ThemePassive WHERE theme_id = " + QString::number(id));
    sq.exec("DELETE FROM ThemeBible WHERE theme_id = " + QString::number(id));
    sq.exec("DELETE FROM ThemeSong WHERE theme_id = " + QString::number(id));
    sq.exec("DELETE FROM ThemeAnnounce WHERE theme_id = " + QString::number(id));

    loadThemes();
    updateThemeButtons();
    setArrowCursor();
}

void ManageDataDialog::on_TableViewTheme_clicked(const QModelIndex &index)
{
    updateThemeButtons();
}
