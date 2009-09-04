#include "addsbornikdialog.h"
#include "ui_addsbornikdialog.h"

AddSbornikDialog::AddSbornikDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSbornikDialog)
{
    ui->setupUi(this);
}

AddSbornikDialog::~AddSbornikDialog()
{
    delete ui;
}

void AddSbornikDialog::changeEvent(QEvent *e)
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

void AddSbornikDialog::on_buttonBox_accepted()
{
    code = ui->sbornik_code_box->text().trimmed();
    title = ui->sbornik_title_box->text().trimmed();
    info = ui->sbornik_info_box->toPlainText();
    close();
}

void AddSbornikDialog::on_buttonBox_rejected()
{
    close();
}