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

class BibleSearch
{
    // For holding search results
public:
    QString book;
    QString chapter;
    QString verse;
    QString verse_text;
};

class Bible
{
public:
    Bible();
    QList<BibleSearch> searchBible(bool begins, QString bibleId, QString searchText);
    QList<BibleSearch> searchBible(bool begins, QString bibleId, QString book, QString searchText);
    QList<BibleSearch> searchBible(bool begins, QString bibleId, QString book, QString chapter, QString searchText);
    QStringList getBooks();
    QStringList getChapter(QString book, int chapter);
    QStringList getVerseAndCaption(QString id, QString bibleId);
    int maxChapters(QString book, QString bibleName);
    Verse getCurrentVerseAndCaption(int currentRow);
    QStringList verseList;
    QStringList previewIdList; // Verses that are in the preview (chapter) list
    QStringList currentIdList; // Verses that are in the show list
    QString primaryId;
    QString secondaryId;
    void setBibles(QString primaryId, QString secondaryId);

private:
    QStringList books;
    QStringList book_ids;
    void retrieveBooks();

private slots:
    


};



#endif // BIBLE_H
