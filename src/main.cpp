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

#include <QtGui/QApplication>
#include <QMessageBox>
#include <QtSql>
#include "softprojector.h"

bool connect(QString database_file)
{
    bool database_exists = ( QFile::exists(database_file) );

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(database_file);
    if (!db.open())
    {
        QMessageBox mb;
        mb.setText("spData Error"
                   "Could not connect to the database spData.sqlite!\n\n"
                   "Following Errors:\n"
                   + db.lastError().databaseText() + "\n"
                   + db.lastError().driverText() +"\n"
                   + db.databaseName()
                   + "\n\nThis is a Fatal Error. Please make sure that all QtSql libraries are inlcuded."
                   "\nThe program will terminate");
        mb.setWindowTitle("Database Connection Error");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
       return false;
    }
    else
    {
        // If no files exited, then database has been created now we need to fill it
        if(!database_exists)
        {
            QSqlQuery sq;
            sq.exec("PRAGMA user_version = 990206");
            sq.exec("CREATE TABLE 'Announcements' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , "
                    "'title' TEXT, 'text' TEXT, 'usePrivate' BOOL, 'useAuto' BOOL, 'loop' BOOL, 'slideTime' INTEGER, "
                    "'useBackground' BOOL, 'backgoundPath' TEXT, 'font' TEXT, 'color' TEXT, 'alignment' TEXT)");
            sq.exec("CREATE TABLE 'BibleBooks' ('bible_id' INTEGER, 'id' INTEGER, 'book_name' "
                    "TEXT, 'chapter_count' INTEGER DEFAULT 0)");
            sq.exec("CREATE TABLE 'BibleVerse' ('verse_id' TEXT, 'bible_id' TEXT, 'book' TEXT, "
                    "'chapter' INTEGER, 'verse' INTEGER, 'verse_text' TEXT)");
            sq.exec("CREATE TABLE 'BibleVersions' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL, "
                    "'bible_name' TEXT, 'abbreviation' TEXT, 'information' TEXT, 'right_to_left' INTEGER DEFAULT 0)");
            sq.exec("CREATE TABLE 'Media' ('long_path' TEXT, 'short_path' TEXT)");
            sq.exec("CREATE TABLE 'Settings' ('type' TEXT, 'sets' TEXT)");
            sq.exec("CREATE TABLE 'SlideShows' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 'name' TEXT, 'info' TEXT)");
            sq.exec("CREATE TABLE 'Slides' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "
                    "'ss_id' INTEGER, 'p_order' INTEGER, 'name' TEXT, 'path' TEXT, "
                    "'pix' BLOB, 'pix_small' BLOB, 'pix_prev' BLOB)");
            sq.exec("CREATE TABLE 'Songbooks' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 'name' TEXT, 'info' TEXT)");
            sq.exec("CREATE TABLE 'Songs' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "
                    "'songbook_id' INTEGER, 'number' INTEGER, 'title' TEXT, 'category' INTEGER DEFAULT 0, "
                    "'tune' TEXT, 'words' TEXT, 'music' TEXT, 'song_text' TEXT, 'notes' TEXT, "
                    "'use_private' BOOL, 'alignment_v' INTEGER, 'alignment_h' INTEGER, 'color' INTEGER, 'font' TEXT, "
                    "'info_color' INTEGER, 'info_font' TEXT, 'ending_color' INTEGER, 'ending_font' TEXT, "
                    "'use_background' BOOL, 'background_name' TEXT, 'background' BLOB, 'count' INTEGER DEFAULT 0, 'date' TEXT)");
            sq.exec("CREATE TABLE 'ThemeData' ('theme_id' INTEGER, 'type' TEXT, 'sets' TEXT)");
            sq.exec("CREATE TABLE 'Themes' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 'name' TEXT, 'comment' TEXT)");
        }
        return true;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
//    #ifndef Q_WS_MAC
//    QApplication::setStyle("Plastique");
//    #endif

    QPixmap pixmap(":icons/icons/splash.png");
    QSplashScreen splash(pixmap);
    splash.setMask(pixmap.mask());
    splash.show();
    a.processEvents();

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));


    // Look for the database in all the same places that the QSql module will look,
    // and display a friendly error if it was not found:
    QString database_file = a.applicationDirPath() + QDir::separator() + "spData.sqlite";


    // Try to connect to database
    if( !connect(database_file) )
    {
        QMessageBox mb;
        mb.setText("Failed to connect to database 'spData.sqlite'");
        mb.setWindowTitle("Database File Error");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
        return 1;
    }
    // Connected to the database OK:

    // Make sure that database is of correct version
    int currentVersion = 990206;
    QSqlQuery sq;
    sq.exec("PRAGMA user_version");
    sq.first();
    int dbVersion = sq.value(0).toInt();
    if(currentVersion != dbVersion)
    {
        QString errortxt = QString("SoftProjector requires database vesion # %1\n"
                                   "The database you are trying to open has vesion # %2\n"
                                   "Please use database with current version\n"
                                   "OR rename/remove all the database files\n"
                                   "and let softProjector to create needed database file.\n"
                                   "The program will terminate!"
                                   ).arg(currentVersion).arg(dbVersion);
        QMessageBox mb;
        mb.setText(errortxt);
        mb.setWindowTitle("Incorrect Database Version");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
        return 1;
    }
    // Database is of correct version


    SoftProjector w;
    w.show();
    splash.finish(&w);
    return a.exec();
}
