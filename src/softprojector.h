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
    ~SoftProjector();
    SongWidget *songWidget;
    BibleWidget *bibleWidget;
    Display1 *display;
    EditWidget *editWidget;
    ImportDialog *importSongs;
    ExportDialog *exportSongs;

    void readConfigurationFile();
    void writeConfigurationFile();



private:
    Ui::SoftProjectorClass *ui;
    QString type;
    int cRow;
    bool showing; // whether we are currently showing to the projector

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_actionAbout_triggered();
    void on_listShow_doubleClicked(QModelIndex index);
    void on_show_button_clicked();
    void on_actionSettings_triggered();
    void on_actionImportSongs_triggered();
    void on_actionExportSongs_triggered();
    void on_actionEditDialog_triggered();
    void on_actionNewSong_triggered();
    void on_actionEditSong_triggered();
    void on_clear_button_clicked();
    void on_listShow_currentRowChanged(int currentRow);
//    void on_listShow_currentTextChanged(QString currentText);
    void on_actionClose_triggered();
    void setSongList(QStringList showList, QString caption, int row);
    void showBibleVerse(int row);

public:
    void setByChapter(bool bychap);
    bool getByChapter();

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // SOFTPROJECTOR_H
