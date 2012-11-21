#ifndef PRINTPREVIEWDIALOG_H
#define PRINTPREVIEWDIALOG_H

#include <QtGui>
#include <QtCore>
#include "song.h"
#include "bible.h"
#include "announcewidget.h"

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
    void setText(QString project, QList<BibleSearch> histories, QList<Song> songs, QList<Announcement> announcements);
    void setText(QList<Announcement> announcements);
    
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
