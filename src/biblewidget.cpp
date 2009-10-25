#include "biblewidget.h"
#include "ui_biblewidget.h"

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
    bible.primaryId = primaryId;
    bible.secondaryId = secondaryId;
    ui->listBook->clear();
    ui->listBook->addItems(bible.getBooks());
    ui->listBook->setCurrentRow(0);
}


void BibleWidget::on_listBook_currentTextChanged(QString currentText)
{
    int s = ui->listBook->currentRow();
    if( s != -1 )
    {
        int maxVerse = bible.maxChapters(currentText, bible.primaryId);
        ui->listChapterNum->clear();
        for(int i=0; i<maxVerse; ++i)
            ui->listChapterNum->addItem(QString::number(i+1));
        chapter_validator->setTop(maxVerse);
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
        QStringList chapters = bible.getChapter(getCurrentBook(), currentText.toInt());
        ui->chapter_preview_list->clear();
        ui->chapter_preview_list->addItems(chapters);
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
    sendToProjector(index.row());
}

void BibleWidget::sendToProjector(int verse)
{
    bible.currentIdList = bible.previewIdList;
    emit goLive(bible.verseList, verse, getCaption());
}


void BibleWidget::on_lineEditBook_textChanged(QString text)
{
    // Called when the bible book filter field is modified.

    // FIXME We need to cache this info:
    // If we do this, make sure the cache is updated whe primary bible is changed
    QStringList all_books = bible.getBooks();

    ui->listBook->clear();
    if( text.isEmpty() )
        ui->listBook->addItems(all_books);
    else
    {
        if( text.at(0).isDigit() )
        {
            // First character of filter text is a number. Special search, where the
            // first character must be the first character of the first word of the book;
            // while the rest of the filter must be the beginning of the second book word.
            QString num_str(text.at(0));
            QString name_str = text.remove(0, 1);
            QStringList filtered_books;
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
            ui->listBook->addItems(filtered_books);
        }
        else
        {
            ui->listBook->addItems(all_books.filter(text, Qt::CaseInsensitive));
        }
    }
    if( ui->listBook->count() > 0 )
        ui->listBook->setCurrentRow(0);
}

void BibleWidget::on_btnLive_clicked()
{
    int value = ui->verse_ef->text().toInt();
    sendToProjector(value - 1);
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
    this->setCursor(Qt::WaitCursor);
    ui->result_lable->show();;
    ui->search_results_list->show();
    ui->hide_result_button->show();
    ui->search_layout->addLayout(ui->result_layout);
    

    QString search_text = ui->search_ef->text();

    if (ui->begin_radioButton->isChecked())
        search_results = bible.searchStartsWith(getPrimary(),search_text);
    else if (ui->contain_radioButton->isChecked())
        search_results = bible.searchContains(getPrimary(),search_text);

    ui->search_results_list->clear();

    if (!search_results.verse_text.isEmpty())
        ui->search_results_list->addItems(search_results.verse_text);
    else
    {
        QMessageBox mb;
        mb.setText(tr("No search results have retrieved"));
        mb.setWindowTitle(tr("No search results"));
        mb.setIcon(QMessageBox::Information);
        mb.exec();
    }
    this->setCursor(Qt::ArrowCursor);
}

void BibleWidget::on_hide_result_button_clicked()
{
    ui->result_lable->hide();
    ui->search_results_list->hide();
    ui->hide_result_button->hide();
    ui->search_layout->removeItem(ui->result_layout);

}

void BibleWidget::on_search_results_list_currentRowChanged(int currentRow)
{
    if (!(currentRow == -1))
    {
        QStringList all_books = bible.getBooks();

        int row = all_books.indexOf(search_results.book.at(currentRow));
        ui->listBook->setCurrentRow(row);


        ui->chapter_ef->setText(search_results.chapter.at(currentRow));
        ui->verse_ef->setText(search_results.verse.at(currentRow));
    }
}

void BibleWidget::on_search_ef_textChanged(QString text)
{
    if(text.size()>3)
        ui->search_button->setEnabled(true);
    else
        ui->search_button->setEnabled(false);
}
