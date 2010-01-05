#include "bible.h"

Bible::Bible()
{
}

QStringList Bible::getBooks()
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
    return books;
}

QStringList Bible::getChapter(QString book, int chapter)
{
    QString verse, verseText, id;
    QSqlQuery sq;
    previewIdList.clear();
    verseList.clear();
    book = book_ids.at(books.indexOf(book,0));
    sq.exec("SELECT verse_id,verse,verse_text FROM BibleVerse WHERE bible_id = "
            + primaryId + " AND book = '" + book + "' AND chapter = " + QString::number(chapter) );
    while (sq.next())
    {
        verseText = sq.value(2).toString();
        verse  = sq.value(1).toString();
        verseList << verse + ". " + verseText;
        id = sq.value(0).toString();
        previewIdList << id;
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
    QString verse;
    QString caption;
    QString book;

    QSqlQuery sq;
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

    QStringList list;
    list.append(verse);
    list.append(caption);
    return list;
}


QString Bible::getDbBibleName(QString bibleId)
{
    QString bibleDbName;
    if( bibleId == "Russian" )
        bibleDbName = "bibleRu";
    else if( bibleId == "Ukrainian" )
        bibleDbName = "bibleUa";
    else if( bibleId == "English (KJV)" )
        bibleDbName = "bibleKjv";
    return bibleDbName;
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
 /*
    QString s_book,s_chapter,verse,verse_text;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;
    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT verse_id, book, chapter, verse, verse_text "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT verse_id, book, chapter, verse, verse_text "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");
    while (sq.next())
    {
        s_book = sq.value(1).toString();
        s_chapter = sq.value(2).toString();
        verse = sq.value(3).toString();

        // Get Book name instead of number
        sq1.exec("SELECT book_name FROM BibleBooks WHERE id = "
                 + s_book + " AND bible_id = " + bibleId);
        sq1.first();
        s_book = sq1.value(0).toString();

        results.verse_text = s_book + " " + s_chapter + ":" + verse + " " + sq.value(4).toString().trimmed();
        results.book = s_book;
        results.chapter = s_chapter;
        results.verse = verse;
        return_results.append(results);
    }
    return return_results; */

///*
    QString s_id, s_book,s_chapter,verse,verse_text;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;
    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT verse_id, book, chapter, verse "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT verse_id, book, chapter, verse "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");
    while (sq.next())
    {
        s_id = sq.value(0).toString();
        s_book = sq.value(1).toString();
        s_chapter = sq.value(2).toString();
        verse = sq.value(3).toString();

        // Get Book name instead of number
        sq1.exec("SELECT book_name FROM BibleBooks WHERE id = "
                 + s_book + " AND bible_id = " + bibleId);
        sq1.first();
        s_book = sq1.value(0).toString();
        sq1.clear();

        // Get properly formated text instead of simplified
        sq1.exec("SELECT verse_text FROM BibleVerse WHERE verse_id like '" + s_id + "'"
                 "AND bible_id = '" + bibleId + "'");
        sq1.first();
        verse_text = sq1.value(0).toString().trimmed();

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
    /*
    QString s_book,s_chapter,verse,verse_text;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;
    book = book_ids.at(books.indexOf(book,0));

    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT verse_id, book, chapter, verse, verse_text "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND book = '" + book + "' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT verse_id, book, chapter, verse, verse_text "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND book = '" + book + "' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");
    while (sq.next())
    {
        s_book = sq.value(1).toString();
        s_chapter = sq.value(2).toString();
        verse = sq.value(3).toString();

        // Get Book name instead of number
        sq1.exec("SELECT book_name FROM BibleBooks WHERE id = "
                 + s_book + " AND bible_id = " + bibleId);
        sq1.first();
        s_book = sq1.value(0).toString();

        results.verse_text = s_book + " " + s_chapter + ":" + verse + " " + sq.value(4).toString().trimmed();
        results.book = s_book;
        results.chapter = s_chapter;
        results.verse = verse;
        return_results.append(results);
    }
    return return_results;
    */
    QString s_id, s_book,s_chapter,verse,verse_text;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;
    book = book_ids.at(books.indexOf(book,0));

    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT verse_id, book, chapter, verse "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND book = '" + book + "' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT verse_id, book, chapter, verse "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND book = '" + book + "' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");
    while (sq.next())
    {
        s_id = sq.value(0).toString();
        s_book = sq.value(1).toString();
        s_chapter = sq.value(2).toString();
        verse = sq.value(3).toString();

        // Get Book name instead of number
        sq1.exec("SELECT book_name FROM BibleBooks WHERE id = "
                 + s_book + " AND bible_id = " + bibleId);
        sq1.first();
        s_book = sq1.value(0).toString();
        sq1.clear();

        // Get properly formated text instead of simplified
        sq1.exec("SELECT verse_text FROM BibleVerse WHERE verse_id like '" + s_id + "'"
                 "AND bible_id = '" + bibleId + "'");
        sq1.first();
        verse_text = sq1.value(0).toString().trimmed();

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
    /*
    QString s_book,s_chapter,verse,verse_text;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;
    book = book_ids.at(books.indexOf(book,0));

    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT verse_id, book, chapter, verse, verse_text "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND book = '" + book + "' AND chapter = '" + chapter + "' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT verse_id, book, chapter, verse, verse_text "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND book = '" + book + "' AND chapter = '" + chapter + "' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");
    while (sq.next())
    {
        s_book = sq.value(1).toString();
        s_chapter = sq.value(2).toString();
        verse = sq.value(3).toString();

        // Get Book name instead of number
        sq1.exec("SELECT book_name FROM BibleBooks WHERE id = "
                 + s_book + " AND bible_id = " + bibleId);
        sq1.first();
        s_book = sq1.value(0).toString();

        results.verse_text = s_book + " " + s_chapter + ":" + verse + " " + sq.value(4).toString().trimmed();
        results.book = s_book;
        results.chapter = s_chapter;
        results.verse = verse;
        return_results.append(results);
    }
    return return_results;
    */
    QString s_id, s_book,s_chapter,verse,verse_text;
    BibleSearch results;
    QList<BibleSearch> return_results;
    QSqlQuery sq,sq1;
    book = book_ids.at(books.indexOf(book,0));

    if (begins) // Search verses that begin with searchText
        sq.exec("SELECT verse_id, book, chapter, verse "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND book = '" + book + "' AND chapter = '" + chapter + "' "
                "AND verse_text like '" + searchText.trimmed() + "%'");
    else        // Search verses that contain searchText
        sq.exec("SELECT verse_id, book, chapter, verse "
                "FROM BibleVerse WHERE bible_id = '" + bibleId + "_' "
                "AND book = '" + book + "' AND chapter = '" + chapter + "' "
                "AND verse_text like '%" + searchText.trimmed() + "%'");
    while (sq.next())
    {
        s_id = sq.value(0).toString();
        s_book = sq.value(1).toString();
        s_chapter = sq.value(2).toString();
        verse = sq.value(3).toString();

        // Get Book name instead of number
        sq1.exec("SELECT book_name FROM BibleBooks WHERE id = "
                 + s_book + " AND bible_id = " + bibleId);
        sq1.first();
        s_book = sq1.value(0).toString();
        sq1.clear();

        // Get properly formated text instead of simplified
        sq1.exec("SELECT verse_text FROM BibleVerse WHERE verse_id like '" + s_id + "'"
                 "AND bible_id = '" + bibleId + "'");
        sq1.first();
        verse_text = sq1.value(0).toString().trimmed();

        results.verse_text = s_book + " " + s_chapter + ":" + verse + " " + verse_text;
        results.book = s_book;
        results.chapter = s_chapter;
        results.verse = verse;
        return_results.append(results);
    }
    return return_results;
}
