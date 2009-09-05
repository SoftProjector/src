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
    QStringList getCurrentVerseAndCaption(int currentRow);
    QStringList verseList;
    QStringList previewIdList; // Verses that are in the preview (chapter) list
    QStringList currentIdList; // Verses that are in the show list
    QString primaryId;
    QString secondaryId;

private slots:
    


};



#endif // BIBLE_H
