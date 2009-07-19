#ifndef BIBLE_H
#define BIBLE_H

#include <QtSql>

class Bible
{
public:
    Bible();
    QStringList getBooks(QString bibleName);
    QStringList getChapter(QString bibleName, QString book, int chapter);
    QStringList getVerse(QString id, QString bibleName);
    int maxChapters(QString book, QString bibleName);
    QString getDbBibleName(QString bibleName);
    void setSecondary(QString bibleName);

    QStringList verseList;
    QStringList idList;
    QStringList verseList1;
    QStringList captionList1;
    QStringList verseList2;
    QStringList captionList2;
    QString primary;

private:
    bool hasSecondary;

private slots:


};

#endif // BIBLE_H
