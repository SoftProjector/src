#ifndef BIBLE_H
#define BIBLE_H

#include <QtSql>

class Bible
{
public:
    Bible();
    QStringList getBooks(QString bible);
    QStringList getChapter(QString bible, QString book, int chapter);
    QStringList getVerse(QString id, QString bible);
    int maxChapters(QString book, QString bible);
    void setSecondary(QString bible);

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
