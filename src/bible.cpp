#include "bible.h"

Bible::Bible()
{
}

QStringList Bible::getBooks(QString bible)
{
    QStringList books;
    QSqlQuery sq;
    sq.exec("SELECT " + bible + " FROM books");
    while (sq.next()) books << sq.value(0).toString();
    return books;
}

QStringList Bible::getChapter(QString bible, QString book, int chapter)
{
    QSqlQuery sq;
    QString verse, verseText, id;
    idList.clear();
    verseList.clear();
    verseList1.clear();
    captionList1.clear();
    primary = bible;
    sq.exec("SELECT id,verse,verseText FROM " + bible
            + " WHERE bookName = '" + book + "' AND chapter = '" + QString::number(chapter) + "'");
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

void Bible::setSecondary(QString bible)
{
    QString id, book, chapter, verse, verseText;
    int idCount = idList.count();
    QSqlQuery sq;
    verseList2.clear();
    captionList2.clear();
    for (int i(0);i<idCount;++i)
    {
        id = idList[i];
        bool hasVerse = sq.exec("SELECT bookName, chapter, verse, verseText FROM " +
                                bible + " WHERE id = '" + id + "'" );
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

QStringList Bible::getVerse(QString id, QString bible)
{
}

int Bible::maxChapters(QString book, QString bible)
{
    int chapters(0);
    QSqlQuery sq;
    if(bible=="bibleRu")sq.exec("SELECT chapters FROM books WHERE bibleRue = '"+book+"'");
    else sq.exec("SELECT chapters FROM books WHERE " +bible+ " = '"+book+"'");
    while(sq.next()) chapters = sq.value(0).toInt();
    return chapters;
}
