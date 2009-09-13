#ifndef MANAGEDATADIALOG_H
#define MANAGEDATADIALOG_H

#include <QtGui/QDialog>
#include <QtGui>
#include <QtSql>

#include "managedata.h"

namespace Ui {
    class ManageDataDialog;
}

class ManageDataDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ManageDataDialog)
public:
    explicit ManageDataDialog(QWidget *parent = 0);
    virtual ~ManageDataDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    QList<Bibles> bible_list;
    QList<Sbornik> sbornik_list;
    BiblesModel *bible_model;
    SborniksModel *sbornik_model;
    Ui::ManageDataDialog *ui;

};


#endif // MANAGEDATADIALOG_H
