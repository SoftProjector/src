/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2010  Vladislav Kobzar and Matvey Adzhigirey
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

#include "bible.h"

Bible::Bible()
{
}

void Bible::setBibles(QString pId, QString sId)
{
    primaryId = pId;
    secondaryId = sId;
    retrieveBooks();
}

void Bible::retrieveBooks()
{
    //    QStringList books;
    QSqlQuery sq;
    books.clear();
    book_ids.clear();

    sq.exec("SELECT book_name, id FROM BibleBooks WHERE bible_id = "+ primaryId );
    while (sq.next())
    {
        books << sq.value(0).toString();
        book_ids << sq.value(1).toString();
    }
}

QStringList Bible::getBooks()
{
    if( books.count() == 0 )
        retrieveBooks();
    return books;
}

QStringList Bible::getChapter(QString book, int chapter)
{
    QString verse, verseText, id, verse_old;
    QSqlQuery sq;
    previewIdList.clear();
    verseList.clear();
    book = book_ids.at(books.indexOf(book,0));
    sq.exec("SELECT verse_id,verse,verse_text FROM BibleVerse WHERE bible_id = "
            + primaryId + " AND book = '" + book + "' AND chapter = " + QString::number(chapter) );
    while (sq.next())
    {
        verse  = sq.value(1).toString();
        if(verse==verse_old)
        {
            verseText = verseText.simplified() + " " + sq.value(2).toString();
            id += "," + sq.value(0).toString();
            verseList.removeLast();
            previewIdList.removeLast();
        }
        else
        {
            verseText = sq.value(2).toString();
            id = sq.value(0).toString();
        }
        verseList << verse + ". " + verseText;
        previewIdList << id;
        verse_old = verse;
    }
    return verseList;
}


Verse Bible::getCurrentVerseAndCaption(int currentRow)
{
    QString id = currentIdList.at(currentRow);
    QStringList list = getVerseAndCaption(id, primaryId);
    Verse v;
    v.primary_text = list[0];
    v.primary_caption = list[1];

    if( primaryId==secondaryId || secondaryId=="none" )
    {
    }
    else
    {
        list = getVerseAndCaption(id, secondaryId);
        v.secondary_text = list[0];
        v.secondary_caption = list[1];
    }
    return v;

}


QStringList Bible::getVerseAndCaption(QString id, QString bibleId)
{
    QString verse, verse_old, verse_n, verse_nold;
    QString caption;
    QString book;
    QStringList ids;
    QSqlQuery sq;

    if (id.contains(","))// Run if more than one database verse items exist or show muliple verses
    {
        ids = id.split(",");
        for (int i(0); i<ids.count(); ++i)
        {
            sq.exec("SELECT book,chapter,verse,verse_text FROM BibleVerse WHERE verse_id like '"
                    +ids.at(i)+"%' AND bible_id = " + bibleId);
            sq.first();
            verse = sq.value(3).toString().trimmed();

            book = sq.value(0).toString();
            verse_n = sq.value(2).toString();

            if (verse_n==verse_nold  ||  verse_nold.isEmpty())
                // If second(nold) verse number is the same or it is empty, then create a regular sigle verse
                // Else create a single display of muliple verses
            {
                caption =" " + sq.value(1).toString() + ":" + verse_n;
                verse = verse_old + " " + verse;
            }
            else
            {
                caption =" " + sq.value(1).toString() + ":" + verse_nold + "," + verse_n;
                verse = "(" + verse_nold + ") " + verse_old + " (" + verse_n + ") " + verse;
            }

            verse_old = verse;
            verse_nold = verse_n;
            verse = verse.simplified();
            sq.clear();

            sq.exec("SELECT book_name FROM BibleBooks WHERE id = "
                    + book + " AND bible_id = " + bibleId);
            sq.first();
            caption = sq.value(0).toString() + caption;
            sq.clear();

        }
    }
    else // Run as standard single verse item from database
    {
        sq.exec("SELECT book,chapter,verse,verse_text FROM BibleVerse WHERE verse_id like '"
                +id+"%' AND bible_id = " + bibleId);
        sq.first();
        verse = sq.value(3).toString().trimmed();// Remove the empty line at the end using .trimmed()
        //qDebug() << "VERSE: '" << verse << "'";

        book = sq.value(0).toString();
        caption =" " + sq.value(1).toString() + ":" + sq.value(2).toString();
        sq.clear();

        sq.exec("SELECT book_name FROM BibleBooks WHERE id = "
                + book + " AND bible_id = " + bibleId);
        sq.first();
        caption = sq.value(0).toString() + caption;
    }

    QStringList list;
    list.append(verse.simplified());
    list.append(caption.simplified());
    return list;
}


int Bible::maxChapters(QString book, QString bibleId)
{
    int chapters(0);
    QString book_id;
    QSqlQuery sq;
    int k = books.indexOf(book,0);
    book_id = book_ids[k];

    sq.exec("SELECT count FROM ChapterCount WHERE id = "+book_id);
    sq.first();
    chapters = sq.value(0).toInt();
    sq.clear();

    return chapters;
}

QList<BibleSearch> Bible::searchBible(bool begins, QString bibleId, QString searchText)
{
    QString s_id, s_book,s_chapter,verse,verse_text,n;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;
    int max(280), count(0);

    // Search for text and return verse ids
    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT verse_id FROM BibleVerse WHERE bible_id = '" + bibleId + "' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT verse_id FROM BibleVerse WHERE bible_id = '" + bibleId + "' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");

    // Prepare verse ids for search for matching real text verses
    while (sq.next() && (count<=max))
    {
        s_id.append(sq.value(0).toString().trimmed() + "' OR verse_id = '");
        ++count;
    }
    s_id.chop(16);
    sq.clear();

    // Get search results based on ids
    sq.exec("SELECT book, chapter, verse, verse_text FROM BibleVerse "
            "WHERE bible_id = '" + bibleId + "' AND (verse_id = '" + s_id + " )");
    while (sq.next())
    {   s_book = sq.value(0).toString().trimmed();
        s_chapter = sq.value(1).toString().trimmed();
        verse = sq.value(2).toString().trimmed();
        verse_text = sq.value(3).toString().trimmed();

        // Get Book name instead of number
        sq1.exec("SELECT book_name FROM BibleBooks WHERE id = "
                 + s_book + " AND bible_id = " + bibleId);
        sq1.first();
        s_book = sq1.value(0).toString();

        // Prepare results
        results.verse_text = s_book + " " + s_chapter + ":" + verse + " " + verse_text;
        results.book = s_book;
        results.chapter = s_chapter;
        results.verse = verse;
        return_results.append(results);
    }

    return return_results;//*/

}

QList<BibleSearch> Bible::searchBible(bool begins, QString bibleId, QString book, QString searchText)
{
    QString s_id, s_book,s_chapter,verse,verse_text;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;
    book = book_ids.at(books.indexOf(book,0));
    int max(280), count(0);

    // Search for text and return verse ids
    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT verse_id FROM BibleVerse "
                "WHERE bible_id = '" + bibleId + "' "
                "AND book = '" + book + "' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT verse_id FROM BibleVerse "
                "WHERE bible_id = '" + bibleId + "' "
                "AND book = '" + book + "' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");

    // Prepare verse ids for search for matching real text verses
    while (sq.next() && (count<=max))
    {
        s_id.append(sq.value(0).toString().trimmed() + "' OR verse_id = '");
        ++count;
    }
    s_id.chop(16);
    sq.clear();

    // Get search results based on ids
    sq.exec("SELECT book, chapter, verse, verse_text FROM BibleVerse "
            "WHERE bible_id = '" + bibleId + "' AND (verse_id = '" + s_id + " )");
    while (sq.next())
    {   s_book = sq.value(0).toString().trimmed();
        s_chapter = sq.value(1).toString().trimmed();
        verse = sq.value(2).toString().trimmed();
        verse_text = sq.value(3).toString().trimmed();

        // Get Book name instead of number
        sq1.exec("SELECT book_name FROM BibleBooks WHERE id = "
                 + s_book + " AND bible_id = " + bibleId);
        sq1.first();
        s_book = sq1.value(0).toString();

        // Prepare results
        results.verse_text = s_book + " " + s_chapter + ":" + verse + " " + verse_text;
        results.book = s_book;
        results.chapter = s_chapter;
        results.verse = verse;
        return_results.append(results);
    }
    return return_results;
}

QList<BibleSearch> Bible::searchBible(bool begins, QString bibleId, QString book, QString chapter, QString searchText)
{
    QString s_id, s_book,s_chapter,verse,verse_text;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;
    book = book_ids.at(books.indexOf(book,0));
    int max(280), count(0);

    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT verse_id, book, chapter, verse "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "' "
                "AND book = '" + book + "' AND chapter = '" + chapter + "' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT verse_id, book, chapter, verse "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "' "
                "AND book = '" + book + "' AND chapter = '" + chapter + "' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");

    // Prepare verse ids for search for matching real text verses
    while (sq.next() && (count<=max))
    {
        s_id.append(sq.value(0).toString().trimmed() + "' OR verse_id = '");
        ++count;
    }
    s_id.chop(16);
    sq.clear();

    // Get search results based on ids
    sq.exec("SELECT book, chapter, verse, verse_text FROM BibleVerse "
            "WHERE bible_id = '" + bibleId + "' AND (verse_id = '" + s_id + " )");
    while (sq.next())
    {   s_book = sq.value(0).toString().trimmed();
        s_chapter = sq.value(1).toString().trimmed();
        verse = sq.value(2).toString().trimmed();
        verse_text = sq.value(3).toString().trimmed();

        // Get Book name instead of number
        sq1.exec("SELECT book_name FROM BibleBooks WHERE id = "
                 + s_book + " AND bible_id = " + bibleId);
        sq1.first();
        s_book = sq1.value(0).toString();

        // Prepare results
        results.verse_text = s_book + " " + s_chapter + ":" + verse + " " + verse_text;
        results.book = s_book;
        results.chapter = s_chapter;
        results.verse = verse;
        return_results.append(results);
    }
    return return_results;
}
