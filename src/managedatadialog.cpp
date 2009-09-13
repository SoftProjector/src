#include "managedatadialog.h"
#include "ui_managedatadialog.h"

ManageDataDialog::ManageDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageDataDialog)
{
    ui->setupUi(this);
    Database db;

    // Set Bible Table
    bible_list = db.getBibles();
    bible_model = new BiblesModel;
    bible_model->setBible(bible_list);
    ui->bibleTableView->setModel(bible_model);

    // Set Sborniks Table
    sbornik_list = db.getSborniks();
    sbornik_model = new SborniksModel;
    sbornik_model->setSbornik(sbornik_list);
    ui->sbornikTableView->setModel(sbornik_model);

}

ManageDataDialog::~ManageDataDialog()
{
    delete ui;
}

void ManageDataDialog::changeEvent(QEvent *e)
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

//***//***//***//***//***//***//***//***//***//***//***//***//***//***//***//
//***//***//***//       Please use the space below ONLY          //***//***//
//***//***//***//        for Sbornik and Bibles classes          //***//***//
//***//***//***//***//***//***//***//***//***//***//***//***//***//***//***//

