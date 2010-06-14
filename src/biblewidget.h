#ifndef BIBLEWIDGET_H
#define BIBLEWIDGET_H

#include <QtGui/QWidget>
#include <QtGui>
#include "bible.h"
#include "highlight.h"

namespace Ui {
    class BibleWidget;
}

class HihghlighterDelegate: public QItemDelegate
{
    Q_OBJECT

public:
    HihghlighterDelegate(QObject *parent = 0);
    HighlightSearch *highlighter;

protected:
    void drawDisplay(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QString &text) const;

private:
    QTextDocument *textDocument;

};

class BibleWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(BibleWidget)
public:
    explicit BibleWidget(QWidget *parent = 0);
    virtual ~BibleWidget();
    Bible bible;
    QString getCurrentBook(void);
    QString currentBook;
    int currentChapter;
    QStringList currentChapterList;
    int getCurrentChapter(void);
    QString getCaption();

public slots:
    QString getPrimary();
    QString getSecondary();
    QByteArray getHiddenSplitterState();
    QByteArray getShownSplitterState();
    void setHiddenSplitterState(QString state);
    void setShownSplitterState(QString state);
    void loadBibles(QString primaryId, QString secondaryId);
    void sendToProjector(int verse, bool add_to_history);
    //bool eventFilter(QObject *object, QEvent *event);

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void setWaitCursor();
    void setArrowCursor();
    void goLive(QStringList chapter_list, int verse, QString caption);

private slots:
    void on_clear_history_pushButton_clicked();
    void on_history_listWidget_doubleClicked(QModelIndex index);
    void on_history_listWidget_currentRowChanged(int currentRow);
    void on_remove_from_history_pushButton_clicked();
    void on_add_to_history_pushButton_clicked();
    void on_search_results_list_doubleClicked(QModelIndex index);
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

    void addToHistory();


private:
    Ui::BibleWidget *ui;
    HihghlighterDelegate *highlight;
    QList<BibleSearch> search_results;
    QList<BibleSearch> history_items;
    QIntValidator *chapter_validator, *verse_validator;
    QByteArray hidden_splitter_state, shown_splitter_state;
    QButtonGroup search_type_buttongroup;
};
//class HighlightSearch;
//class QTextDocument;


#endif // BIBLEWIDGET_H
