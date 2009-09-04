#ifndef BIBLE_H
#define BIBLE_H

#include <QtSql>

class Bible
{
public:
    Bible();
    QStringList getBooks();
    QStringList getChapter(QString bibleName, QString book, int chapter);
    QString getSecondaryVerse(QString id, QString bibleName);
    QString getSecondaryCaption(QString id, QString bibleName);
    int maxChapters(QString book, QString bibleName);
    QString getDbBibleName(QString bibleName);
    void loadSecondaryData();
    QStringList getVerseAndCaption(int currentRow);
    QStringList verseList;
    QStringList idList;
    QStringList verseList1;
    QStringList captionList1;
    QStringList verseList2;
    QStringList captionList2;
    QString primaryId;
    QString secondaryId;
    bool by_chapter; // to pre load bible by chapter or by verse

private slots:


};



#endif // BIBLE_H
