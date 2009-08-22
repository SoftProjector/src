#include "importdialog.h"
#include "ui_importdialog.h"
#include <QMessageBox>
#include <QtSql>


ImportDialog::ImportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
}

ImportDialog::~ImportDialog()
{
    delete ui;
}

void ImportDialog::changeEvent(QEvent *e)
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

void ImportDialog::load()
{
    file_path = QFileDialog::getOpenFileName(this,
                         "Select sbornik file to import",
                         ".",
                         "Text File (*.txt *.sps)");
    load_file(file_path);
}

void ImportDialog::load_file(QString path)
{
    ui->file_box->setText(file_path);
    QFile file(file_path);
    if (file.open(QIODevice::ReadOnly))
    {
        QString line;
        line = QString::fromUtf8(file.readLine());
        if (line.startsWith("##"))
        {
            has_info = true;
            // Set Sbornik Code
            line.remove("#");
            ui->sbornik_code_box->setText(line);

            // Set Sbornik Title
            line = QString::fromUtf8(file.readLine());
            line.remove("#");
            ui->sbornik_title_box->setText(line);

            // Set Sbortnik Information
            line = QString::fromUtf8(file.readLine());
            line.remove("#");
            ui->sbornik_info_box->setPlainText(line);

            line = QString::fromUtf8(file.readLine());
        }
        else
            has_info = false;

        // Count number of songs in sbornik
        int count(1);
        while (!file.atEnd())
        {
            line = QString::fromUtf8(file.readLine());
            count ++;
        }
        ui->label->setText("This spornik has " + QString::number(count) + " songs.");
        ui->progressBar->setMaximum(count);
        ui->progressBar->setValue(0);
    }
    else
    {
        QMessageBox mb;
        mb.setText("Cannot open specified file");
        mb.show();
    }
    file.close();

}

void ImportDialog::on_buttonBox_accepted()
{
    int row(1);
    QFile file(file_path);
    QString code, title, info, num;
    QStringList split;
    QSqlQuery sq;
    QSqlTableModel sqt;

    code = ui->sbornik_code_box->text();
    title = ui->sbornik_title_box->text();
    info = ui->sbornik_info_box->toPlainText();

    QString line;
    QMessageBox mb;
    if (file.open(QIODevice::ReadOnly))
    {
        sqt.setTable("Sborniks");
        sqt.insertRows(0,1);
        sqt.setData(sqt.index(0,0),code.trimmed());
        sqt.setData(sqt.index(0,1),title.trimmed());
        sqt.setData(sqt.index(0,2),info.trimmed());
        sqt.setData(sqt.index(0,3),1);
        sqt.submitAll();
        sqt.clear();

        if (has_info)
        {
            QString a;
            a = file.readLine();
            a = file.readLine();
            a = file.readLine();
        }
        while (!file.atEnd())
        {
            line = QString::fromUtf8(file.readLine());
            split = line.split("#$#");

            num = split[0];

            sqt.setTable("Songs");
            sqt.insertRows(0,1);
            sqt.setData(sqt.index(0,1),split[1]);//title
            sqt.setData(sqt.index(0,2),split[2]);//cat
            sqt.setData(sqt.index(0,3),split[3]);//tune
            sqt.setData(sqt.index(0,4),split[4]);//words
            sqt.setData(sqt.index(0,5),split[5]);//music
            sqt.setData(sqt.index(0,6),split[6]);//text
            sqt.submitAll();

            QString sid = "";
            sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Songs'");
            while (sq.next()) sid = sq.value(0).toString();
            sq.clear();

            sq.exec("INSERT into SongLink (sbornik_id, song_id, song_number) VALUES ('"
                          + code.trimmed() + "',"
                          + sid + ","
                          + num + ")");

            ui->progressBar->setValue(row);
            row++;
        }

//        mb.setText("Sbornik has been imported sussesfully");
//        mb.exec();

    }

}

void ImportDialog::on_laod_file_button_clicked()
{
    load();
}
