#include "addsongbookdialog.h"
#include "ui_addsongbookdialog.h"

AddSongbookDialog::AddSongbookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSongbookDialog)
{
    ui->setupUi(this);
}

AddSongbookDialog::~AddSongbookDialog()
{
    delete ui;
}

void AddSongbookDialog::changeEvent(QEvent *e)
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

void AddSongbookDialog::on_buttonBox_accepted()
{
    title = ui->songbook_title_box->text().trimmed();
    info = ui->songbook_info_box->text().trimmed();
    close();
}

void AddSongbookDialog::on_buttonBox_rejected()
{
    close();
}

void AddSongbookDialog::setSongbook(QString tit, QString inf)
{
    ui->songbook_title_box->setText(tit);
    ui->songbook_info_box->setText(inf);
}
