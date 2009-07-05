#include <QtGui/QApplication>
#include <QMessageBox>
#include <QtSql>
#include "softprojector.h"

bool connect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("spData.db");
    if (!db.open()){
        QMessageBox mb;
        mb.setText("spData Error"
                   "Could not connec to the database spData.db!\n\n"
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
    QApplication::setStyle("Plastique");

    bool fileExist = QFile::exists("spData.db");
    if (fileExist)
    {
        if (!connect())return 1;
    }
    else
    {
        QMessageBox mb;
        mb.setText("Database file 'spData.db' cannot be found.\nThe program will Terminate!");
        mb.setWindowTitle("Database File Error");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
        return 1;
    }
    
    SoftProjector w;
    w.setWindowTitle("softProjector 0.1.10");
    w.show();
    return a.exec();
}
