#ifndef BIBLE_H
#define BIBLE_H

#include <QtSql>

class Verse
{
public:
    // For now both primary and secondary information is stored in
    // primary_text and primary_caption
    QString primary_text;
    QString secondary_text;
    QString primary_caption;
    QString secondary_caption;
};

class Bible
{
public:
    Bible();
    QStringList getBooks();
    QStringList getChapter(QString book, int chapter);
    QStringList getVerseAndCaption(QString id, QString bibleId);
    int maxChapters(QString book, QString bibleName);
    QString getDbBibleName(QString bibleName);
    Verse getCurrentVerseAndCaption(int currentRow);
    QStringList verseList;
    QStringList previewIdList; // Verses that are in the preview (chapter) list
    QStringList currentIdList; // Verses that are in the show list
    QString primaryId;
    QString secondaryId;

private:
    QStringList books;
    QStringList book_ids;

private slots:
    


};



#endif // BIBLE_H
