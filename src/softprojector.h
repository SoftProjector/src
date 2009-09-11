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
    QDesktopWidget *desktop;
    Display1 *display;
    EditWidget *editWidget;
    bool showing; // whether we are currently showing to the projector
    bool display_on_top; // whether display screen is alway on top on not

    void applySetting(QString name, QString value);
    void readXMLConfigurationFile();
    void writeXMLConfigurationFile();



private:
    Ui::SoftProjectorClass *ui;
    QString type;
    QString main_text;
    QString caption_text;

private slots:
    void on_tabWidget_currentChanged(int index);
    void on_actionAbout_triggered();
    void on_listShow_doubleClicked(QModelIndex index);
    void on_actionSettings_triggered();
    void on_actionImportSongs_triggered();
    void on_actionExportSongs_triggered();
    void on_actionEditDialog_triggered();
    void on_actionNewSong_triggered();
    void on_actionEditSong_triggered();
    void on_show_button_clicked();
    void on_clear_button_clicked();
    void on_listShow_currentRowChanged(int currentRow);
    void on_actionClose_triggered();
    void setSongList(QStringList showList, QString caption, int row);
    void setChapterList(QStringList chapter_list, int verse, QString caption);
    void drawText(QPainter *painter, int width, int height);

public:
    void updateScreen();
    void setAllText(QString main, QString caption);


protected:
    void closeEvent(QCloseEvent *event);
};

#endif // SOFTPROJECTOR_H
