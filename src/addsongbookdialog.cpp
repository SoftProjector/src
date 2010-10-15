/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2010  Vladislav Kobzar and Matvey Adzhigirey
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
    info = ui->songbook_info_box->toPlainText().trimmed();
    close();
}

void AddSongbookDialog::on_buttonBox_rejected()
{
    close();
}

void AddSongbookDialog::setSongbook(QString tit, QString inf)
{
    ui->songbook_title_box->setText(tit);
    ui->songbook_info_box->setPlainText(inf);
}
