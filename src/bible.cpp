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

#include "bible.h"

Bible::Bible()
{
}

void Bible::setBiblesId(QString& id)
{
    bibleId = id;
    retrieveBooks();
}

QString Bible::getBibleName()
{
    if(bibleId.isEmpty())
        return "";
    QSqlQuery sq;
    sq.exec("SELECT bible_name FROM BibleVersions WHERE id = "+ bibleId );
    sq.first();
    QString b = sq.value(0).toString().trimmed();
    return b;
}

void Bible::retrieveBooks()
{
    BibleBook book;
    QSqlQuery sq;
    books.clear();
    sq.exec("SELECT book_name, id, chapter_count FROM BibleBooks WHERE bible_id = "+ bibleId );
    while (sq.next())
    {
        book.book = sq.value(0).toString().trimmed();
        book.bookId = sq.value(1).toString();
        book.chapterCount = sq.value(2).toInt();
        books.append(book);
    }
}

QStringList Bible::getBooks()
{
    QStringList book_list;
    if( books.count() == 0 )
        retrieveBooks();
    for(int i(0); books.count()>i;++i)
    {
        book_list.append(books.at(i).book);
//        qDebug()<< books.at(i).book;
    }
    return book_list;
}

int Bible::getCurrentBookRow(QString book)
{
    int chapters(0);
    for(int i(0); books.count()>i;++i)
    {
        if(books.at(i).book==book)
        {
            chapters = i;
            break;
        }
    }
    return chapters;
}

QStringList Bible::getChapter(QString book, int chapter)
{
    QString verse, verseText, id, verse_old;
    QSqlQuery sq;
    previewIdList.clear();
    verseList.clear();
    sq.exec(QString("SELECT verse_id,verse,verse_text FROM BibleVerse WHERE bible_id = %1 AND book = '%2' AND chapter = %3").arg(bibleId).arg(book).arg(QString::number(chapter)));
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

Verse Bible::getCurrentVerseAndCaption(QList<int>  currentRows, BibleSettings& sets)
{
    QString verse_id;
    for(int i(0);i<currentRows.count();++i)
    {
        verse_id += currentIdList.at(currentRows.at(i)) + ",";
    }
    verse_id.chop(1);

    Verse v;
    // get primary verse
    getVerseAndCaption(v.primary_text,v.primary_caption,verse_id,sets.primaryBible,sets.useAbbriviations);

    // get secondary verse
    if(sets.primaryBible!=sets.secondaryBible && sets.secondaryBible!="none")
        getVerseAndCaption(v.secondary_text,v.secondary_caption,verse_id,sets.secondaryBible,sets.useAbbriviations);

    // get trinary versse
    if(sets.trinaryBible!=sets.primaryBible && sets.trinaryBible!=sets.secondaryBible && sets.trinaryBible!="none")
        getVerseAndCaption(v.trinary_text,v.trinary_caption,verse_id,sets.trinaryBible,sets.useAbbriviations);

    return v;

}

void Bible::getVerseAndCaption(QString& verse, QString& caption, QString verId, QString& bibId, bool useAbbr)
{
    QString verse_old, verse_show, verse_n, verse_nold, verse_nfirst, chapter;
    QString book;
    QStringList ids;
    QSqlQuery sq;

    // clean old verses
    verse.clear();
    caption.clear();

    if (verId.contains(","))// Run if more than one database verse items exist or show muliple verses
    {
        ids = verId.split(",");
        verId.replace(",", "' OR verse_id = '");
        sq.exec("SELECT book,chapter,verse,verse_text FROM BibleVerse WHERE ( verse_id = '"
                + verId +"' ) AND bible_id = " + bibId);
        while (sq.next())
        {
            book = sq.value(0).toString();
            chapter = sq.value(1).toString();
            verse_n = sq.value(2).toString();
            verse = sq.value(3).toString().trimmed();

            // Set first verse number
            if (verse_nfirst.isEmpty())
                verse_nfirst = verse_n;

            // If second(nold) verse number is the same or it is empty, then create a regular sigle verse
            // Else create a single display of muliple verses
            if (verse_n==verse_nold)
            {
                // If current verse number is same as first verse number,
                // then remove verse number from verse text and caption
                // shows only current verse number
                if(verse_n == verse_nfirst)
                {
                    int j(0);
                    for(int i(0);i<verse_show.count(); ++i)
                    {
                        j = 1 + i;
                        QString p =verse_show.at(i);
                        if(p==(")"))
                            break;
                    }
                    verse_show = verse_show.remove(0,j);

                    caption = " " + chapter + ":" + verse_n;
                }
                else
                    // Else if current verse number does match first verse number,
                    // then show bigening and eding verse numbers in caption
                    caption = " " + chapter + ":" + verse_nfirst + "-" + verse_n;

                verse_show += " " + verse;
            }
            else
            {
                caption =" " + chapter + ":" + verse_nfirst + "-" + verse_n;
                verse = " (" + verse_n + ") " + verse;
                verse_show += verse;
                if(!verse_show.startsWith(" ("))
                    verse_show = " ("+ verse_nfirst + ") " + verse_show;
            }
            verse_old = verse;
            verse_nold = verse_n;
        }
        verse = verse_show.simplified();
    }
    else // Run as standard single verse item from database
    {
        sq.exec("SELECT book,chapter,verse,verse_text FROM BibleVerse WHERE verse_id = '"
                +verId+"' AND bible_id = " + bibId);

        sq.first();
        verse = sq.value(3).toString().trimmed();// Remove the empty line at the end using .trimmed()

        book = sq.value(0).toString();
        caption =" " + sq.value(1).toString() + ":" + sq.value(2).toString();
        sq.clear();
    }

    // Add book name to caption
    sq.exec("SELECT book_name FROM BibleBooks WHERE id = "
            + book + " AND bible_id = " + bibId);
    sq.first();
    caption = sq.value(0).toString() + caption;
    sq.clear();

    // Add bible abbreveation if to to use it
    if(useAbbr)
    {
        sq.exec("SELECT abbreviation FROM BibleVersions WHERE id = " + bibId);
        sq.first();
        QString abr = sq.value(0).toString().trimmed();
        if (!abr.isEmpty())
            caption = QString("%1 (%2)").arg(caption).arg(abr);
    }

    verse = verse.simplified();
    caption = caption.simplified();
}

QList<BibleSearch> Bible::searchBible(bool begins, QString searchText)
{   ///////// Search entire Bible //////////

    QString s_book,s_chapter,verse,verse_text;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;

    // Search for text and return verse ids
    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT book, chapter, verse, verse_text FROM BibleVerse "
                "WHERE bible_id = '" + bibleId + "' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT book, chapter, verse, verse_text FROM BibleVerse "
                "WHERE bible_id = '" + bibleId + "' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");

    while (sq.next())
    {
        s_book = sq.value(0).toString().trimmed();
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

QList<BibleSearch> Bible::searchBible(bool begins, QString book, QString searchText)
{   ///////// Search in selected book //////////

    QString s_book,s_chapter,verse,verse_text;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;

    // Search for text and return verse ids
    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT book, chapter, verse, verse_text FROM BibleVerse "
                "WHERE bible_id = '" + bibleId + "' "
                "AND book = '" + book + "' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT book, chapter, verse, verse_text FROM BibleVerse "
                "WHERE bible_id = '" + bibleId + "' "
                "AND book = '" + book + "' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");

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

QList<BibleSearch> Bible::searchBible(bool begins, QString book, QString chapter, QString searchText)
{   ///////// Search in selected chapter //////////

    QString s_book,s_chapter,verse,verse_text;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;

    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT book, chapter, verse, verse_text FROM BibleVerse "
                "WHERE bible_id = '" + bibleId + "' "
                "AND book = '" + book + "' AND chapter = '" + chapter + "' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT book, chapter, verse, verse_text FROM BibleVerse "
                "WHERE bible_id = '" + bibleId + "' "
                "AND book = '" + book + "' AND chapter = '" + chapter + "' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");

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
