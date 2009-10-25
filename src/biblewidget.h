#ifndef BIBLEWIDGET_H
#define BIBLEWIDGET_H

#include <QtGui/QWidget>
//#include <QtCore/QKeyEvent>
#include "bible.h"

namespace Ui {
    class BibleWidget;
}

class BibleWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(BibleWidget)
public:
    explicit BibleWidget(QWidget *parent = 0);
    virtual ~BibleWidget();
    Bible bible;
    QString getCurrentBook(void);
    int getCurrentChapter(void);
    QString getCaption();

public slots:
    QString getPrimary();
    QString getSecondary();
    void loadBibles(QString primaryId, QString secondaryId);
    void sendToProjector(int verse);
    //bool eventFilter(QObject *object, QEvent *event);

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void goLive(QStringList chapter_list, int verse, QString caption);

private slots:
    void on_search_ef_textChanged(QString text);
    void on_search_results_list_currentRowChanged(int currentRow);
    void on_hide_result_button_clicked();
    void on_search_button_clicked();
    void on_chapter_ef_textChanged(QString new_string);
    void on_verse_ef_textChanged(QString new_string);
    void on_btnLive_clicked();
    void on_lineEditBook_textChanged(QString );
    void on_chapter_preview_list_doubleClicked(QModelIndex index);
    void on_chapter_preview_list_currentRowChanged(int currentRow);
    void on_listChapterNum_currentTextChanged(QString currentText);
    void on_listBook_currentTextChanged(QString currentText);


private:
    Ui::BibleWidget *ui;
    BibleSearch search_results;
    QIntValidator *chapter_validator, *verse_validator;

};

#endif // BIBLEWIDGET_H
