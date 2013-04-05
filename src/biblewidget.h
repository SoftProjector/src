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

#ifndef BIBLEWIDGET_H
#define BIBLEWIDGET_H

#include <QtGui/QWidget>
#include <QtGui>
#include "bible.h"
#include "highlight.h"
#include "theme.h"


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
    QString getCurrentBook();

    // For optimization:
    QString currentBook;
    int currentChapter;
    QStringList currentChapterList;

    int getCurrentChapter();

public slots:
    QByteArray getHiddenSplitterState();
    QByteArray getShownSplitterState();
    void setHiddenSplitterState(QByteArray& state);
    void setShownSplitterState(QByteArray& state);
    void loadBibles(QString initialId);
    void sendToProjector(bool add_to_history);
    void setSettings(BibleSettings& sets);
    BibleHistory getCurrentVerse();
    void addToHistory(BibleHistory &b);
    void clearHistory();
    void setSelectedHistory(BibleHistory &b);

    //bool eventFilter(QObject *object, QEvent *event);

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void setWaitCursor();
    void setArrowCursor();
    void goLive(QStringList chapter_list, QString caption, QItemSelection selectItems);
    void historyListChanged(bool changed);

private slots:
    void on_history_listWidget_doubleClicked(QModelIndex index);
    void on_history_listWidget_currentRowChanged(int currentRow);

    void on_search_results_list_doubleClicked(QModelIndex index);
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
//    BibleSearch prepareAddToHistory();


private:
    BibleSettings mySettings;
    Ui::BibleWidget *ui;
    HighlighterDelegate *highlight;
    QList<BibleSearch> search_results;
    QList<BibleHistory> history_items;
    QIntValidator *chapter_validator, *verse_validator;
    QByteArray hidden_splitter_state, shown_splitter_state;
    QButtonGroup search_type_buttongroup;
};
//class HighlightSearch;
//class QTextDocument;


#endif // BIBLEWIDGET_H
