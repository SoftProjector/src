#include <QUrl>
#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    QUrl url;
    url.setPath("./help/index.html");
    ui->textBrowser->setSource(url);
//    ui->textBrowser
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::changeEvent(QEvent *e)
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

void HelpDialog::on_close_pushButton_clicked()
{
    close();
}
