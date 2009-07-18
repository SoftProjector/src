#include <QtSql>
#include <QMessageBox>
#include "exportdialog.h"
#include "ui_exportdialog.h"

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

void ExportDialog::on_buttonBox_rejected()
{
    close();
}

void ExportDialog::on_buttonBox_accepted()
{
    if (!(ui->lblUserNum->text()=="0"))
        exportUser();
    close();
}

void ExportDialog::preload()
{
    QSqlQuery sq;
    int count(0);
    QStringList titles;
    sq.exec("SELECT title FROM songs WHERE pvUser > 0");
    while (sq.next())
    {
        count++;
        titles << sq.value(0).toString();
    }
    ui->progressBar->setMaximum(count);
    ui->lblUserNum->setText(QString::number(count));
    ui->listWidget->addItems(titles);
}

void ExportDialog::exportUser()
{
    QSqlQuery sq;
    QString songs = "";
    int i(0);
    sq.exec("SELECT pvUser, title, category, tune, words, music, songText FROM songs WHERE pvUser > 0");
    while (sq.next())
    {
        songs += sq.value(0).toString() + "#$#" +
                 sq.value(1).toString() + "#$#" +
                 sq.value(2).toString() + "#$#" +
                 sq.value(3).toString() + "#$#" +
                 sq.value(4).toString() + "#$#" +
                 sq.value(5).toString() + "#$#" +
                 sq.value(6).toString() + "\n";
        i++;
        ui->progressBar->setValue(i);
    }

    QFile ofile;
    ofile.setFileName("spDataExport.txt");
    if (ofile.open(QIODevice::WriteOnly))
    {
        QTextStream out(&ofile);
        out.setCodec("UTF8");
        out << songs;
    }
    ofile.close();
    QMessageBox mb;
    mb.setText("User Data has been exported");
    mb.exec();
}
