#ifndef EDITWIDGET_H
#define EDITWIDGET_H

#include <QtGui/QWidget>
#include "song.h"

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
    void setEdit(Sbornik sEdit);
    void setNew();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::EditWidget *ui;
    Sbornik editSong, newSong;
    QString sbornik, format;
    QStringList allTitles;
    int titleType;

private slots:
    void on_txtSearch_textChanged(QString );
    void on_bntDelete_clicked();
    void on_listTitles_currentTextChanged(QString currentText);
    void on_comboBoxSbornik_currentIndexChanged(int index);
    void on_spinBoxSongNum_valueChanged(int );
    void on_btnSortTitles_clicked();
    void on_btnClose_clicked();
    void on_btnCancel_clicked();
    void on_btnSave_clicked();
    void on_btnNew_clicked();
    void on_btnEdit_clicked();
    void resetUiItems();
    void setUiItems();
    void setSave();
    void disablePV();
    QString resetLyric(QString lyric);
    QString setSongText(QString text);
    void loadTitles(QString tSbornik);
};

#endif // EDITWIDGET_H
