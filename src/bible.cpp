#include "bible.h"

Bible::Bible()
{
}

QStringList Bible::getBooks(QString bibleId)
{
    QStringList books;
    QSqlQuery sq;

    sq.exec("SELECT book_name FROM BibleBooks WHERE bible_id like '"+ bibleId +"'");
    while (sq.next())
        books << sq.value(0).toString();
    return books;
}

QStringList Bible::getChapter(QString bibleId, QString book, int chapter)
{
    QSqlQuery sq;
    QString verse, verseText, id;
    idList.clear();
    verseList.clear();
    verseList1.clear();
    captionList1.clear();
    primaryId = bibleId;

    sq.exec("SELECT verse_id,verse,verse_text FROM BibleVerse WHERE bible_id like '"
            + bibleId + "' AND book = '" + book + "' AND chapter = " + QString::number(chapter) );
    while (sq.next())
    {
        verseText = sq.value(2).toString();
        verseList1 << verseText;
        verse  = sq.value(1).toString();
        verseList << verse + ". " + verseText;
        captionList1 << book + " " + QString::number(chapter) + ":" + verse;
        id = sq.value(0).toString();
        idList << id;

    }
    return verseList;
}

void Bible::loadSecondaryData()
{
    QString id, book, chapter, verse, verseText;
    QString bibleDbName = getDbBibleName(secondaryId);

    int idCount = idList.count();
    QSqlQuery sq;
    verseList2.clear();
    captionList2.clear();
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
        if(hasVerse)
        {
            verseList2 << verseText;
            captionList2 << book + " " + chapter + ":" + verse;
        }
        else
        {
            verseList2 << " ";
            captionList2 << " ";
        }
    }
}

QStringList Bible::getVerseAndCaption(int currentRow)
{
    QString verse = verseList1[currentRow];
    QString caption = captionList1[currentRow];
    if( primaryId==secondaryId || secondaryId=="none" )
    {
    }
    else
    {
        verse.append("\n");
        if (by_chapter)
            verse += verseList2[currentRow];
        else
            verse += getSecondaryVerse(idList.at(currentRow), secondaryId);
        caption.append("    ");
        if (by_chapter)
            caption += captionList2[currentRow];
        else
            caption += getSecondaryCaption(idList.at(currentRow), secondaryId);
    }
    QStringList verse_and_caption;
    verse_and_caption.append(verse);
    verse_and_caption.append(caption);
    return verse_and_caption;

}

QString Bible::getSecondaryVerse(QString id, QString bibleId)
{
    QString ver="";
    QSqlQuery sq;
    sq.exec("SELECT verse_text FROM BibleVerse WHERE verse_id like '"+id+"%' AND bible_id like '"+bibleId+"'");
    while (sq.next())
        ver += sq.value(0).toString();
    return ver;
}

QString Bible::getSecondaryCaption(QString id, QString bibleId)
{
    QString ver="";
    QSqlQuery sq;
    sq.exec("SELECT book,chapter,verse FROM BibleVerse WHERE verse_id like '"+id+"%' AND bible_id like '"+bibleId+"'");
    sq.first();
    ver = sq.value(0).toString() + " " + sq.value(1).toString() + ":" + sq.value(2).toString();
    return ver;
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
