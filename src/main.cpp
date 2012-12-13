/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2011  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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
    else return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    #ifndef Q_WS_MAC
//    QApplication::setStyle("Plastique");
    #endif

    QPixmap pixmap(":icons/icons/splash.png");
    QSplashScreen splash(pixmap);
    splash.setMask(pixmap.mask());
    splash.show();
    a.processEvents();

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));


    // Look for the database in all the same places that the QSql module will look,
    // and display a friendly error if it was not found:
    QString database_file = a.applicationDirPath() + QDir::separator() + "spData.sqlite";

    bool database_exists = ( QFile::exists(database_file) );

    if (!database_exists)
    {
        QMessageBox mb;
        mb.setText("Database file 'spData.sqlite' cannot be found.\nThe program will Terminate!");
        mb.setWindowTitle("Database File Error");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
        return 1;
    }

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
    QString currentVersion = "2db4";
    QSqlQuery sq;
    sq.exec("SELECT Version FROM spData");
    sq.first();
    QString dbVersion = sq.value(0).toString().trimmed();
    if(currentVersion != dbVersion)
    {
        QString errortxt = QString("SoftProjector requires database vesion # %1\n"
                                   "The database you are trying to open has vesion # %2\n"
                                   "Please use supplied database with current version of softProjector.\n"
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
