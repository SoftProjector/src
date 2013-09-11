#include "moduledownloaddialog.h"
#include "ui_moduledownloaddialog.h"

ModuleDownloadDialog::ModuleDownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleDownloadDialog)
{
    ui->setupUi(this);
}

ModuleDownloadDialog::~ModuleDownloadDialog()
{
    delete ui;
}

void ModuleDownloadDialog::setList(QStringList &modList)
{
    ui->listWidget->clear();
    foreach(const QString &mod, modList)
    {
        ui->listWidget->addItem(mod);
    }
}

QList<int> ModuleDownloadDialog::getSelected()
{
    QList<int> mods;
    int count = ui->listWidget->count();
    for(int i(0);count>i;++i)
    {
        if(ui->listWidget->item(i)->isSelected())
            mods.append(i);
    }
    return mods;
}

void ModuleDownloadDialog::on_buttonBox_accepted()
{

}

void ModuleDownloadDialog::on_buttonBox_rejected()
{

}

void ModuleDownloadDialog::on_pushButtonSelectAll_clicked()
{
    ui->listWidget->selectAll();
}

void ModuleDownloadDialog::on_pushButtonDeselectAll_clicked()
{
    ui->listWidget->clearSelection();
}
