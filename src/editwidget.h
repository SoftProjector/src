#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QtGui/QWidget>
#include <QtGui>
#include "song.h"
#include "addsongbookdialog.h"
#include "highlight.h"

namespace Ui {
    class EditWidget;
}

class EditWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(EditWidget)
public:
    explicit EditWidget(QWidget *parent = 0);
    virtual ~EditWidget();

public slots:
    void setCopy(Song copy);
    void setEdit(Song sEdit);
    void setNew();

signals:
    // For communicating with SongsModel
    void updateSongFromDatabase(int songid);
    void addedNew();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::EditWidget *ui;
    Song editSong, newSong;
    SongDatabase song_database;
    bool is_new;
    Highlight *highlight;
    QIntValidator *song_num_validator;
    void setWaitCursor();
    void setArrowCursor();
    QString add_to_songbook;


private slots:
    void addNewSong(Song song, QString msgNewSongbook, QString msgCaption);
    void addSongbook();
    void on_wall_default_button_clicked();
    void on_wall_button_clicked();
    void on_font_default_button_clicked();
    void on_font_button_clicked();
    void on_btnCancel_clicked();
    void on_btnSave_clicked();
    void resetUiItems();
    void setUiItems();
    void setSave();
    void setSongbook(int id);
    QString resetLyric(QString lyric);
    QString setSongText(QString text);
};

#endif // EDITWIDGET_H
