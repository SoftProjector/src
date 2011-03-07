#include "bibleinformationdialog.h"
#include "ui_bibleinformationdialog.h"

BibleInformationDialog::BibleInformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BibleInformationDialog)
{
    ui->setupUi(this);
}

BibleInformationDialog::~BibleInformationDialog()
{
    delete ui;
}

void BibleInformationDialog::on_buttonBox_accepted()
{
    title = ui->bible_name_lineEdit->text().trimmed();
    abbr = ui->abbr_lineEdit->text().trimmed();
    info = ui->info_TextEdit->toPlainText().trimmed();
    if (ui->rtol_checkBox->isChecked())
        isRtoL = true;
    else
        isRtoL = false;

    close();
}

void BibleInformationDialog::on_buttonBox_rejected()
{
    close();
}

void BibleInformationDialog::setBibleIformation(QString _title, QString _abbr, QString _info, bool is_rtol)
{
   ui->bible_name_lineEdit->setText(_title);
   ui->abbr_lineEdit->setText(_abbr);
   ui->info_TextEdit->setPlainText(_info);
   ui->rtol_checkBox->setChecked(is_rtol);
}
