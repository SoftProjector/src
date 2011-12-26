/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2011  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

#include "biblewidget.h"
#include "ui_biblewidget.h"
#include "song.h"
#include <time.h>

double diffclock(clock_t clock1,clock_t clock2)
{
        double diffticks=clock1-clock2;
        double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
        return diffms;
}

BibleWidget::BibleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BibleWidget)
{
    ui->setupUi(this);
    on_hide_result_button_clicked();

    chapter_validator = new QIntValidator(1, 1, ui->chapter_ef);
    verse_validator = new QIntValidator(1, 1, ui->verse_ef);

    ui->chapter_ef->setValidator( chapter_validator );
    ui->verse_ef->setValidator( verse_validator );

    search_type_buttongroup.addButton(ui->begin_radioButton);
    search_type_buttongroup.addButton(ui->contain_radioButton);
    ui->contain_radioButton->setChecked(true);

    highlight = new HihghlighterDelegate(ui->search_results_list);
    ui->search_results_list->setItemDelegate(highlight);
}

BibleWidget::~BibleWidget()
{
    delete chapter_validator;
    delete verse_validator;
    delete ui;
}

void BibleWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString BibleWidget::getPrimary()
{
    return bible.primaryId;
}

QString BibleWidget::getSecondary()
{
    return bible.secondaryId;
}

void BibleWidget::loadBibles(QString primaryId, QString secondaryId)
{
    // secondaryId may be "none"
    QString initial_primary_id = bible.primaryId;
    bible.setBibles(primaryId, secondaryId);

    // Check if primary bible is different that what has been loaded already
    // If it is different, then reload the bible list
    if(initial_primary_id!=primaryId)
    {
        ui->listBook->clear();
        ui->listBook->addItems(bible.getBooks());
        ui->listBook->setCurrentRow(0);
    }
}

void BibleWidget::on_listBook_currentTextChanged(QString currentText)
{

    int s = ui->listBook->currentRow();
    if( s != -1 )
    {
        int max_chapter = bible.books.at(bible.getCurrentBookRow(currentText)).chapterCount;
        ui->listChapterNum->clear();
        for(int i=0; i<max_chapter; ++i)
            ui->listChapterNum->addItem(QString::number(i+1));
        chapter_validator->setTop(max_chapter);
        if( ui->listChapterNum->currentRow() != 0 )
            ui->listChapterNum->setCurrentRow(0);
    }
    else
    {
        // No bible book selected
        chapter_validator->setTop(1);
        ui->listChapterNum->clear();
    }

}

void BibleWidget::on_listChapterNum_currentTextChanged(QString currentText)
{
    int s = ui->listChapterNum->currentRow();
    if( s != -1 )
    {
        // This optimization is required in order for the bible filter entry field to work fast:
        if( currentBook != getCurrentBook() || currentChapter != currentText.toInt() )
        {
            currentBook = getCurrentBook();
            currentChapter = currentText.toInt();
            currentChapterList = bible.getChapter(bible.books.at(bible.getCurrentBookRow(currentBook)).bookId, currentChapter);
        }

        ui->chapter_preview_list->clear();
        ui->chapter_preview_list->addItems(currentChapterList);
        ui->chapter_ef->setText(currentText);
        verse_validator->setTop(ui->chapter_preview_list->count());
        ui->chapter_preview_list->setCurrentRow(0);
    }
    else
    {
        ui->chapter_preview_list->clear();
    }
}

QString BibleWidget::getCurrentBook(void)
{
    return ui->listBook->currentItem()->text();
}
int BibleWidget::getCurrentChapter(void)
{
    return ui->listChapterNum->currentItem()->text().toInt();
}

QString BibleWidget::getCaption()
{
    // Get the caption string to show above the show list (right-most list)
    QString id = bible.currentIdList.at(0);
    QStringList temp = bible.getVerseAndCaption(id, bible.primaryId);
    QStringList temp1;
    temp1 = temp[1].split(":");
    return temp1[0];
}
/*
bool BibleWidget::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->spinVerse && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::KeyDown) {
            qDebug() << "DOWN KEY";
            // Special tab handling
            return true;
        } else
            return false;
    }
    return false;
}
*/

void BibleWidget::on_chapter_preview_list_currentRowChanged(int currentRow)
{
    ui->verse_ef->setText(QString::number(currentRow+1));
}

void BibleWidget::on_chapter_preview_list_doubleClicked(QModelIndex index)
{
    // Called when a chapter or verse is double clicked
    sendToProjector(true);
}

void BibleWidget::sendToProjector(bool add_to_history)
{
    bible.currentIdList = bible.previewIdList;
    QItemSelection selectedItems = ui->chapter_preview_list->selectionModel()->selection();
    emit goLive(bible.verseList, getCaption(),selectedItems);
    if (add_to_history)
        addToHistory();
}

void BibleWidget::on_lineEditBook_textChanged(QString text)
{
    // Called when the bible book filter field is modified.

    QStringList all_books = bible.getBooks();

    // Remove trailing spaces:
    text = text.trimmed();

    int chapter = 0;
    int verse = 0;

    // Check whether the user entered a search string that include book, chapter,
    // and verse. For example: "1King 3 13"
    QStringList search_words = text.split(" ");

    // Allows the user to use more than one space as a seperator:
    search_words.removeAll("");

    if( search_words.count() > 1 )
    {
        bool ok;
        int num1 = search_words.last().toInt(&ok);
        if( ok )
        {
            chapter = num1;
            search_words.removeLast();
            if( search_words.count() > 1 )
            {
                bool ok2;
                int num2 = search_words.last().toInt(&ok2);
                if( ok2 )
                {
                    search_words.removeLast();
                    chapter = num2;
                    verse = num1;
                }
            }
            text = search_words.join(" ");
        }

    }


    // Now search all books to find the matching book:
    if( text.isEmpty() )
    {
        // Show all bible books
        if( ui->listBook->count() != all_books.count() )
        {
            // This is an important optimization
            ui->listBook->clear();
            ui->listBook->addItems(all_books);
        }
    }
    else
    {
        // Show only the bible books that match the filter
        QStringList filtered_books;
        if( text.at(0).isDigit() )
        {
            // First character of filter text is a number. Special search, where the
            // first character must be the first character of the first word of the book;
            // while the rest of the filter must be the beginning of the second book word.
            QString num_str(text.at(0));
            QString name_str = text.remove(0, 1);
            for(int i=0; i<all_books.count(); i++)
            {
                QString book = all_books.at(i);
                QStringList book_words = book.split(" ");

                if( ! book_words.at(0).startsWith(num_str) )
                    continue;
                if( !book_words.at(1).startsWith(name_str, Qt::CaseInsensitive) )
                    continue;

                filtered_books.append(book);
            }
        }
        else
            filtered_books = all_books.filter(text, Qt::CaseInsensitive);

        if( ui->listBook->count() != filtered_books.count() )
        {
            // This is an important optimization
            // FIXME don't just check the count; check values
            ui->listBook->clear();
            ui->listBook->addItems(filtered_books);
        }
    }

    if( ui->listBook->count() > 0 )
        // Select the first row. This will take a longer time only if it will cause
        // a new chapter to be loaded into the preview
        ui->listBook->setCurrentRow(0);


//    clock_t begin=clock();
//    qDebug() << "Time to select first row:" << double(diffclock(clock(), begin)) << " ms";

    if( chapter != 0 && chapter <= ui->listChapterNum->count() )
    {
        if( ui->listChapterNum->currentRow() != (chapter-1) )
            ui->listChapterNum->setCurrentRow(chapter-1);
        if( verse != 0 && verse <= ui->chapter_preview_list->count() )
            ui->chapter_preview_list->setCurrentRow(verse-1);
    }
}

void BibleWidget::on_btnLive_clicked()
{
    sendToProjector(true);
}

void BibleWidget::on_verse_ef_textChanged(QString new_string)
{
    int value = new_string.toInt();
    ui->chapter_preview_list->setCurrentRow(value-1);
}

void BibleWidget::on_chapter_ef_textChanged(QString new_string)
{
    int value = new_string.toInt();
    ui->listChapterNum->setCurrentRow(value-1);
}

void BibleWidget::on_search_button_clicked()
{
    emit setWaitCursor();
    
    QString search_text = ui->search_ef->text();
    search_text = clean(search_text); // remove all none alphanumeric charecters
    search_text = search_text.simplified(); // remove all extra spaces
    highlight->highlighter->setHighlightText(search_text); // set highlighting rule

    if (ui->entire_bible_radioButton->isChecked()) // Search entire Bible
    {
        search_results = bible.searchBible(ui->begin_radioButton->isChecked(),getPrimary(),search_text);
    }
    else if (ui->current_book_radioButton->isChecked()) // Search current book only
    {
        search_results = bible.searchBible(ui->begin_radioButton->isChecked(),getPrimary(),
                                           bible.books.at(bible.getCurrentBookRow(ui->listBook->currentItem()->text())).bookId,
                                           search_text);
    }
    else if (ui->current_chapter_radioButton->isChecked()) // Search current chapter only
    {
        search_results = bible.searchBible(ui->begin_radioButton->isChecked(),getPrimary(),
                                           bible.books.at(bible.getCurrentBookRow(ui->listBook->currentItem()->text())).bookId,
                                           ui->listChapterNum->currentItem()->text(),search_text);
    }
    ui->search_results_list->clear();

    if (!search_results.isEmpty()) // If have results, then show them
    {
        if( not ui->result_label->isVisible() )
        {
            ui->lineEditBook->clear();
            hidden_splitter_state = ui->results_splitter->saveState();
            ui->result_label->show();
            ui->result_count_label->show();
            ui->search_results_list->show();
            ui->hide_result_button->show();
            ui->search_layout->addItem(ui->results_layout);
            ui->results_splitter->restoreState(shown_splitter_state);
        }
        QStringList verse_list;
        int count = search_results.count();

        ui->result_count_label->setText(tr("Total\n") + QString::number(count) + tr("\nresults"));
//          ui->result_count_label->setText(tr("Total of ") + QString::number(count) + tr(" search results returned."));

        for(int i(0);i<count;i++)
            verse_list.append(search_results.at(i).verse_text);
        ui->search_results_list->addItems(verse_list);
    }
    else // If no relust, notify the user and hide result list
    {
        QMessageBox mb;
        mb.setText(tr("No search results have retrieved"));
        mb.setWindowTitle(tr("No search results"));
        mb.setIcon(QMessageBox::Information);
        mb.exec();
        on_hide_result_button_clicked();
    }
    emit setArrowCursor();
}

void BibleWidget::on_hide_result_button_clicked()
{
    shown_splitter_state = ui->results_splitter->saveState();
    ui->result_label->hide();
    ui->result_count_label->hide();
    ui->search_results_list->hide();
    ui->hide_result_button->hide();
    ui->search_layout->removeItem(ui->results_layout);
    ui->results_splitter->restoreState(hidden_splitter_state);
}

void BibleWidget::on_search_results_list_currentRowChanged(int currentRow)
{
    if (!(currentRow == -1))
    {
        QStringList all_books = bible.getBooks();

        int row = all_books.indexOf(search_results.at(currentRow).book);
        ui->listBook->setCurrentRow(row);


        ui->chapter_ef->setText(search_results.at(currentRow).chapter);
        ui->verse_ef->setText(search_results.at(currentRow).verse);
    }
}

void BibleWidget::on_search_ef_textChanged(QString text)
{
    if(text.size()>3)
        ui->search_button->setEnabled(true);
    else
        ui->search_button->setEnabled(false);
}

void BibleWidget::on_search_results_list_doubleClicked(QModelIndex index)
{
    on_btnLive_clicked();
}

void BibleWidget::addToHistory()
{
    BibleSearch b;
    QString selected_ids;

    b.book = ui->listBook->currentItem()->text();
    b.chapter = ui->chapter_ef->text();
    b.verse = ui->verse_ef->text();

    int first_selected(-1),last_selected(-1);
    for(int i(0);i<ui->chapter_preview_list->count();++i)
    {
        if(ui->chapter_preview_list->item(i)->isSelected())
        {
            if(first_selected == -1)
                first_selected = i;
            last_selected = i;
            selected_ids += bible.previewIdList.at(i) + ",";
        }
    }
    selected_ids.chop(1);

    b.verse_text = ui->chapter_preview_list->item(first_selected)->text().trimmed();
    b.verse_id = selected_ids;
    b.first_v = first_selected;
    b.last_v = last_selected;

    if(first_selected==last_selected)
        b.display_text = b.book + " " + b.chapter + ":" + b.verse_text;
    else
    {   // Create multi verse caption for display
        int f(first_selected+1), l(last_selected+1),j(0);
        QString v=b.verse_text,p=".";
        while(v.at(j)!=p.at(0))
            ++j;
        v = v.remove(0,j);

        b.display_text = b.book + " " + b.chapter + ":" + QString::number(f) + "-" + QString::number(l) + v + "...";
    }
    history_items.append(b);
    ui->history_listWidget->addItem(b.display_text);
    emit historyListChanged(true);

}

void BibleWidget::on_add_to_history_pushButton_clicked()
{
    addToHistory();
}

void BibleWidget::on_remove_from_history_pushButton_clicked()
{
    int current_row = ui->history_listWidget->currentRow();
    if (current_row>=0)
    {
        ui->history_listWidget->takeItem(current_row);
        history_items.takeAt(current_row);
        emit historyListChanged(true);
    }
}

void BibleWidget::on_clear_history_pushButton_clicked()
{
    ui->history_listWidget->clear();
    history_items.clear();
    emit historyListChanged(true);
}

void BibleWidget::on_history_listWidget_currentRowChanged(int currentRow)
{
    if (!(currentRow == -1))
    {
        QStringList all_books = bible.getBooks();
        ui->listBook->clear();
        ui->listBook->addItems(all_books);

        int row = all_books.indexOf(history_items.at(currentRow).book);
        ui->listBook->setCurrentRow(row);

        ui->chapter_ef->setText(history_items.at(currentRow).chapter);
        QItemSelection sel;
        sel.select(ui->chapter_preview_list->model()->index(history_items.at(currentRow).first_v,0,QModelIndex()),
                   ui->chapter_preview_list->model()->index(history_items.at(currentRow).last_v,0,QModelIndex()));
        if(history_items.at(currentRow).first_v>=0)
            ui->verse_ef->setText(QString::number(history_items.at(currentRow).first_v+1));
        else
            ui->verse_ef->setText(history_items.at(currentRow).verse);
        ui->chapter_preview_list->selectionModel()->select(sel,QItemSelectionModel::Select);
    }
}

void BibleWidget::on_history_listWidget_doubleClicked(QModelIndex index)
{
    sendToProjector( false);
}

QByteArray BibleWidget::getHiddenSplitterState()
{
    if(ui->hide_result_button->isHidden())
        hidden_splitter_state = ui->results_splitter->saveState();
    return hidden_splitter_state;
}

QByteArray BibleWidget::getShownSplitterState()
{
    if(!ui->hide_result_button->isHidden())
        shown_splitter_state = ui->results_splitter->saveState();
    return shown_splitter_state;
}

void BibleWidget::setHiddenSplitterState(QString state)
{
    hidden_splitter_state.clear();
    hidden_splitter_state.insert(0,state);
    hidden_splitter_state = hidden_splitter_state.fromHex(hidden_splitter_state);
    ui->results_splitter->restoreState(hidden_splitter_state);
}

void BibleWidget::setShownSplitterState(QString state)
{
    shown_splitter_state.clear();
    shown_splitter_state.insert(0,state);
    shown_splitter_state = shown_splitter_state.fromHex(shown_splitter_state);
}

QList<BibleSearch> BibleWidget::getHistoryList()
{
    return history_items;
}

void BibleWidget::loadHistoriesFromFile(QList<BibleSearch> histories)
{
    // ******* SoftProjector Project File version 1.5 *****************

    history_items.clear();
    history_items = histories;
    QStringList disp_list;

    for(int i(0);i<histories.count();++i)
    {
        disp_list.append(histories.at(i).display_text);
    }

    ui->history_listWidget->clear();
    ui->history_listWidget->addItems(disp_list);
}

void BibleWidget::loadHistoriesFromFile1_0(QStringList histories)
{
    // *************** 1.05 and before code ***************************
    // ******* SoftProjector Project File version 1.0 *****************
    // Reloads history list with saves verses
    // It will reload in the same order as saves
    // Will reload with current primary bible,
    // in case primary Bible has been changed
    BibleSearch h;
    QSqlQuery sq,sq1;
    QString sql_str, cur_item;
    QStringList history_list;
    QList<BibleSearch> h_list;
    QMap<QString, int> map;
    QList<int> order_list;

    history_items.clear();
    ui->history_listWidget->clear();

    // Prepare look verse id up with double verse support
    for(int i(0); i<histories.count(); ++i)
    {
        cur_item = histories.at(i);
        if(cur_item.contains(","))
        {
            QStringList l = cur_item.split(",");
            cur_item = l.at(0);
            cur_item = cur_item.trimmed();
        }
        map.insert(cur_item,i);
        sql_str.append("verse_id = '" + cur_item + "' OR ");
    }
    sql_str.chop(4);

    sq.exec("SELECT verse_id, book, chapter, verse, verse_text FROM BibleVerse WHERE bible_id = " + bible.primaryId +
            " AND(" + sql_str + ")");

    // Retrieve results
    while(sq.next())
    {
        h.verse_id = sq.value(0).toString().trimmed();
        h.book = sq.value(1).toString().trimmed();
        h.chapter = sq.value(2).toString().trimmed();
        h.verse = sq.value(3).toString().trimmed();
        h.verse_text = h.verse + ". " + sq.value(4).toString().trimmed();

        // Get Book name instead of number
        sq1.exec("SELECT book_name FROM BibleBooks WHERE id = "
                 + h.book + " AND bible_id = " + bible.primaryId);
        sq1.first();
        h.book = sq1.value(0).toString().trimmed();
        h_list.append(h);
    }

    // Order items the way they were saved
    order_list = map.values();
    QMap<int,int> map2; // A nessasery step to proper ordering
    for(int i(0); i<order_list.count(); ++i)
        map2.insert(order_list.at(i),i);

    order_list = map2.values();

    if(order_list.count()==h_list.count())
    {   // Check if opened items and retrieved items from primary bible match in count
        for(int i(0); i<order_list.count(); ++i)
        {
            h = h_list.at(order_list.at(i));
            // Prepare history line to show
            h.display_text = h.book + " " + h.chapter + ":" + h.verse_text;
            history_list.append(h.display_text);
            history_items.append(h);
        }
    }
    else
    {   // let user know that opened history items do not match retrieved amound trom primary bible
        QMessageBox mb;
        mb.setWindowTitle(tr("Error opening Bible histories"));
        mb.setInformativeText(tr("Cound not find any or all Bible verses from file withing current primary Bible."
                                 "\nTry changing primary Bible and reopen project file."));
        mb.setIcon(QMessageBox::Information);
        mb.exec();
        return;
    }

    // show items from file
    ui->history_listWidget->addItems(history_list);
}

/**********************************************/
/**** Class for higlighting search results ****/
/**********************************************/
HihghlighterDelegate::HihghlighterDelegate(QObject *parent)
    : QItemDelegate(parent)
{
    textDocument = new QTextDocument(this);
    highlighter = new HighlightSearch(textDocument);
}

void HihghlighterDelegate::drawDisplay(QPainter *painter, const QStyleOptionViewItem &option, const QRect &rect, const QString &text) const
{
    Q_UNUSED(option);
    textDocument->setDocumentMargin(0);
    textDocument->setPlainText(text);

    QPixmap pixmap(rect.size());
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);

    textDocument->drawContents(&p);

    painter->drawPixmap(rect, pixmap);
}
