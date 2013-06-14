/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyleft (Ɔ) 2013  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

#ifndef PRINTPREVIEWDIALOG_H
#define PRINTPREVIEWDIALOG_H

#include <QtGui>
#include <QtCore>
#include "song.h"
#include "bible.h"
#include "announcewidget.h"
#include "schedule.h"

namespace Ui {
class PrintPreviewDialog;
}

class PrintPreviewDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PrintPreviewDialog(QWidget *parent = 0);
    ~PrintPreviewDialog();

public slots:
    void setText(Song song);
    void setText(QString bible,QString book,int chapter);
    void setText(Announcement announce);
//    void setText(QString project, QList<BibleSearch> histories, QList<Song> songs, QList<Announcement> announcements);
    void setSchedule(QString scheduleName, const QList<Schedule> &schedule, bool printDetail);
private slots:
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_spinBoxFontSize_valueChanged(int arg1);
    void on_pushButtonPDF_clicked();
    void on_pushButtonPrint_clicked();
    void on_comboBox_currentIndexChanged(const QString &arg1);

    void updateMargins();
    void on_doubleSpinBoxLeft_editingFinished();
    void on_doubleSpinBoxTop_editingFinished();
    void on_doubleSpinBoxRight_editingFinished();
    void on_doubleSpinBoxBottom_editingFinished();

private:
    QPrinter* printer;
    Ui::PrintPreviewDialog *ui;
};

#endif // PRINTPREVIEWDIALOG_H
