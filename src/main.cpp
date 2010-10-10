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

#include <QtGui/QApplication>
#include <QMessageBox>
#include <QtSql>
#include "softprojector.h"

bool connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("spData.sqlite");
    if (!db.open()){
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

    bool fileExist = QFile::exists("spData.sqlite");
    if (!fileExist)
    {
        QMessageBox mb;
        mb.setText("Database file 'spData.sqlite' cannot be found.\nThe program will Terminate!");
        mb.setWindowTitle("Database File Error");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
        return 1;
    }
    if( !connect() )
    {
        QMessageBox mb;
        mb.setText("Failed to connect to database 'spData.sqlite'");
        mb.setWindowTitle("Database File Error");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
        return 1;
    }

    // Connected to the database OK:
    SoftProjector w;
    w.show();
    splash.finish(&w);
    return a.exec();
}
