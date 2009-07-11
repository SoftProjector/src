#ifndef SOFTPROJECTOR_H
#define SOFTPROJECTOR_H

#include <QtGui/QMainWindow>
#include "songwidget.h"
#include "biblewidget.h"
#include "display1.h"
#include "editwidget.h"
#include "bible.h"
#include "importdialog.h"
#include "exportdialog.h"


class QActionGroup;

namespace Ui
{
    class SoftProjectorClass;
}

class SoftProjector : public QMainWindow
{
    Q_OBJECT

public:
    SoftProjector(QWidget *parent = 0);
    SongWidget *songWidget;
    BibleWidget *bibleWidget;
    Display1 *display;
    EditWidget *editWidget;
    ImportDialog *importSongs;
    ExportDialog *exportSongs;
    ~SoftProjector();

signals:
    void sendDisplay(QString verse, QString caption);

private:
    Ui::SoftProjectorClass *ui;
    QActionGroup *primaryGroup;
    QActionGroup *secondaryGroup;
    QString type, bible2;
    Bible bible;
    int cRow;

private slots:
    void on_actionImportSongs_triggered();
    void on_actionExportSongs_triggered();
    void on_actionEditDialog_triggered();
    void on_actionNewSong_triggered();
    void on_actionEditSong_triggered();
    void on_btnBlack_clicked();
    void on_actionEnglish_Kjv_2_triggered();
    void on_actionUkranian_2_triggered();
    void on_actionRussian_2_triggered();
    void on_actionEnglish_Kjv_triggered();
    void on_actionUkranian_triggered();
    void on_actionRussian_triggered();
    void on_listShow_currentRowChanged(int currentRow);
//    void on_listShow_currentTextChanged(QString currentText);
    void on_actionClose_triggered();
    void setSongList(QStringList showList, QString caption, int row);
    void setBibleList(Bible bible, QString bibleSecondary, int row);

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // SOFTPROJECTOR_H
