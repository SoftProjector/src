/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2011  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

#ifndef MANAGEDATADIALOG_H
#define MANAGEDATADIALOG_H

#include <QtGui/QDialog>
#include <QtGui>
#include <QtSql>

#include "managedata.h"
#include "song.h"
#include "addsongbookdialog.h"
#include "bibleinformationdialog.h"

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
    bool reload_songbook;

public slots:
    void load_songbooks();

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void setMainWaitCursor();
    void setMainArrowCursor();

private:
    QList<Bibles> bible_list;
    QList<Songbook> songbook_list;
    BiblesModel *bible_model;
    SongbooksModel *songbook_model;
    Ui::ManageDataDialog *ui;


private slots:
    QString get3(int i);
    QString getBibleIdUnbound(QString id);
    QString getVerseId(QString book, QString chapter, QString verse);
    void setWaitCursor();
    void setArrowCursor();
    void on_songbookTableView_clicked(QModelIndex index);
    void on_bibleTableView_clicked(QModelIndex index);
    void updateBibleButtons();
    void updateSongbookButtons();
    void on_edit_bible_pushButton_clicked();
    void on_edit_songbook_pushButton_clicked();
    void on_delete_bible_pushButton_clicked();
    void on_export_bible_pushButton_clicked();
    void on_import_bible_pushButton_clicked();
    void on_ok_pushButton_clicked();
    void on_delete_songbook_pushButton_clicked();
    void on_export_songbook_pushButton_clicked();
    void on_import_songbook_pushButton_clicked();
    void deleteBible(Bibles bilbe);
    void importBible(QString path, QString version);
    void exportBible(QString path, Bibles bible);
    void deleteSongbook(Songbook songbook);
    void importSongbook(QString path);
    void exportSongbook(QString path);
    void load_bibles();
};


#endif // MANAGEDATADIALOG_H
