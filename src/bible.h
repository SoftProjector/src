#ifndef BIBLE_H
#define BIBLE_H

#include <QtSql>

class Bible
{
public:
    Bible();
    QStringList getBooks();
    QStringList getChapter(QString book, int chapter);
    QStringList getVerseAndCaption(QString id, QString bibleId);
    int maxChapters(QString book, QString bibleName);
    QString getDbBibleName(QString bibleName);
    void loadSecondaryData();
    QStringList getCurrentVerseAndCaption(int currentRow);
    QStringList verseList;
    QStringList idList;
    QStringList currentVerseList; // the right-most table data
    QStringList currentCaptionList; // the right-most table data
    QString primaryId;
    QString secondaryId;
    bool by_chapter; // to pre load bible by chapter or by verse
    // Chapter that is currently in the show list:
    QString currentBible;
    int currentChapter;

private slots:
    


};



#endif // BIBLE_H
