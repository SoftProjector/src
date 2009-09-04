#ifndef BIBLE_H
#define BIBLE_H

#include <QtSql>

class Bible
{
public:
    Bible();
    QStringList getBooks(QString bibleName);
    QStringList getChapter(QString bibleName, QString book, int chapter);
    QString getSecondaryVerse(QString id, QString bibleName);
    QString getSecondaryCaption(QString id, QString bibleName);
    int maxChapters(QString book, QString bibleName);
    QString getDbBibleName(QString bibleName);
    void loadSecondaryData();

    QStringList verseList;
    QStringList idList;
    QStringList verseList1;
    QStringList captionList1;
    QStringList verseList2;
    QStringList captionList2;
    QString primary;
    QString secondaryId;

private:
    bool hasSecondary;

private slots:


};



#endif // BIBLE_H
