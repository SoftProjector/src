#include "bible.h"

Bible::Bible()
{
}

QStringList Bible::getBooks()
{
    QStringList books;
    QSqlQuery sq;

    sq.exec("SELECT book_name FROM BibleBooks WHERE bible_id = "+ primaryId );
    while (sq.next())
        books << sq.value(0).toString();
    return books;
}

QStringList Bible::getChapter(QString book, int chapter)
{
    QString verse, verseText, id;
    QSqlQuery sq;
    previewIdList.clear();
    verseList.clear();

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
    QString verse="";
    QString caption="";

    QSqlQuery sq;
    sq.exec("SELECT book,chapter,verse,verse_text FROM BibleVerse WHERE verse_id like '"+id+"%' AND bible_id = " + bibleId);
    sq.first();
    verse = sq.value(3).toString();
    caption = sq.value(0).toString() + " " + sq.value(1).toString() + ":" + sq.value(2).toString();

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
    sq.exec("SELECT id FROM BibleBooks WHERE book_name like '"+book+"' AND bible_id = "+bibleId);
    sq.first();
    book_id = sq.value(0).toString();
    sq.clear();

    sq.exec("SELECT count FROM ChapterCount WHERE id = "+book_id);
    sq.first();
    chapters = sq.value(0).toInt();
    sq.clear();

    return chapters;
}
