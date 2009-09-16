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
    //QApplication::setStyle("Plastique");

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
    w.setWindowTitle("softProjector 0.9.Alpha04");
    w.show();
    return a.exec();
}
