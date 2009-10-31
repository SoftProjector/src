#ifndef MANAGEDATADIALOG_H
#define MANAGEDATADIALOG_H

#include <QtGui/QDialog>
#include <QtGui>
#include <QtSql>

#include "managedata.h"
#include "song.h"
#include "addsbornikdialog.h"

namespace Ui {
    class ManageDataDialog;
}

class ManageDataDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ManageDataDialog)
public:
    explicit ManageDataDialog(QWidget *parent = 0);
    virtual ~ManageDataDialog();
    bool reload_bible;
    bool reload_sbornik;

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void setMainWaitCursor();
    void setMainArrowCursor();

private:
    QList<Bibles> bible_list;
    QList<Sbornik> sbornik_list;
    BiblesModel *bible_model;
    SborniksModel *sbornik_model;
    Ui::ManageDataDialog *ui;


private slots:
    void setWaitCursor();
    void setArrowCursor();
    void on_sbornikTableView_clicked(QModelIndex index);
    void on_bibleTableView_clicked(QModelIndex index);
    void updateBibleButtons();
    void updateSbornikButtons();
    void on_edit_bible_pushButton_clicked();
    void on_edit_sbornik_pushButton_clicked();
    void on_delete_bible_pushButton_clicked();
    void on_export_bible_pushButton_clicked();
    void on_import_bible_pushButton_clicked();
    void on_ok_pushButton_clicked();
    void on_delete_sbornik_pushButton_clicked();
    void on_export_sbornik_pushButton_clicked();
    void on_import_sbornik_pushButton_clicked();
    void deleteBible(Bibles bilbe);
    void importBible(QString path);
    void exportBible(QString path);
    void deleteSbornik(Sbornik sbornik);
    void importSbornik(QString path);
    void exportSbornik(QString path);
    void load_sborniks();
    void load_bibles();
};


#endif // MANAGEDATADIALOG_H
