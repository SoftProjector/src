#ifndef SONGWIDGET_H
#define SONGWIDGET_H

#include <QtGui/QWidget>
#include "song.h"
//#include "softprojector.h"

namespace Ui {
    class SongWidget;
}

class SongWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(SongWidget)
public:
    explicit SongWidget(QWidget *parent = 0);
    virtual ~SongWidget();

public slots:
    Song sendToEdit();

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void sendSong(QStringList songList, QString caption, int currentItem);

private slots:
    void on_listPreview_doubleClicked(QModelIndex index);
    void on_btnSort_clicked();
    void on_lineEditSearch_textEdited(QString Text);
    void on_btnRemoveFromPlaylist_clicked();
    void on_btnAddToPlaylist_clicked();
    void on_btnLive_clicked();
    void on_listPreview_itemDoubleClicked(QListWidgetItem* item);
    void on_listPlaylist_itemDoubleClicked(QListWidgetItem* item);
    void on_listTitles_itemDoubleClicked(QListWidgetItem* item);
    void on_listPlaylist_currentTextChanged(QString currentText);
    void on_listTitles_currentTextChanged(QString currentText);
    void on_spinBoxPvNumber_valueChanged(int value);
    void on_comboBoxPvNumber_currentIndexChanged(int index);
    void showPreview(QString title);
//    void loadSong();
//    void loadPreview();
    void loadTitles(QString tSbornik);

private:
    Ui::SongWidget *ui;
    QString sbornik;
    QStringList allTitles;
    int titleType;
    Song songPreview;
//    SoftProjector *toShow;
};

#endif // SONGWIDGET_H
