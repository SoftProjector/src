#include "bible.h"

Bible::Bible()
{
}

QStringList Bible::getBooks()
{
    QStringList books;
    QSqlQuery sq;

    sq.exec("SELECT book_name FROM BibleBooks WHERE bible_id like '"+ primaryId +"'");
    while (sq.next())
        books << sq.value(0).toString();
    return books;
}

QStringList Bible::getChapter(QString book, int chapter)
{
    QString verse, verseText, id;
    QSqlQuery sq;
    idList.clear();
    verseList.clear();

    sq.exec("SELECT verse_id,verse,verse_text FROM BibleVerse WHERE bible_id like '"
            + primaryId + "' AND book = '" + book + "' AND chapter = " + QString::number(chapter) );
    while (sq.next())
    {
        verseText = sq.value(2).toString();
        verse  = sq.value(1).toString();
        verseList << verse + ". " + verseText;
        id = sq.value(0).toString();
        idList << id;
    }
    return verseList;
}

void Bible::loadSecondaryData()
{
    QString id, book, chapter, verse, verseText;
    QString bibleDbName = getDbBibleName(secondaryId);
    QSqlQuery sq;

    int idCount = idList.count();
    for (int i(0); i<idCount; ++i)
    {
        id = idList[i];
        bool hasVerse = sq.exec("SELECT book, chapter, verse, verse_text FROM BibleVerse WHERE verse_id like '"+id+"%' AND bible_id like '"+secondaryId+"'");
        verseText.clear();
        while(sq.next())
        {
            book = sq.value(0).toString();
            chapter = sq.value(1).toString();
            verse = sq.value(2).toString();
            verseText += sq.value(3).toString();
        }

    }
}

QStringList Bible::getCurrentVerseAndCaption(int currentRow)
{
    QString id = idList.at(currentRow);
    QStringList list = getVerseAndCaption(id, primaryId);
    QString verse = list[0];
    QString caption = list[1];

    if( primaryId==secondaryId || secondaryId=="none" )
    {
    }
    else
    {
        list = getVerseAndCaption(id, secondaryId);
        verse.append("\n");
        verse += list[0];
        caption.append("    ");
        caption += list[1];
    }
    QStringList verse_and_caption;
    verse_and_caption.append(verse);
    verse_and_caption.append(caption);
    return verse_and_caption;

}


QStringList Bible::getVerseAndCaption(QString id, QString bibleId)
{
    QString verse="";
    QString caption="";

    QSqlQuery sq;
    sq.exec("SELECT book,chapter,verse,verse_text FROM BibleVerse WHERE verse_id like '"+id+"%' AND bible_id like '"+bibleId+"'");
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
    sq.exec("SELECT id FROM BibleBooks WHERE book_name like '"+book+"' AND bible_id like '"+bibleId+"'");
    sq.first();
    book_id = sq.value(0).toString();
    sq.clear();

    sq.exec("SELECT count FROM ChapterCount WHERE id = "+book_id);
    sq.first();
    chapters = sq.value(0).toInt();
    sq.clear();

    return chapters;
}
