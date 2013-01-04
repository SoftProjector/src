/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyleft (Ɔ) 2013  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

#ifndef BIBLE_H
#define BIBLE_H

#include <QtSql>
#include "theme.h"

class Verse
{
public:
    // For now both primary and secondary information is stored in
    // primary_text and primary_caption
    QString primary_text;
    QString secondary_text;
    QString primary_caption;
    QString secondary_caption;
    QString trinary_text;
    QString trinary_caption;
};

class BibleSearch
{
    // For holding search results
public:
    QString book;
    QString chapter;
    QString verse;
    QString verse_text;
    QString verse_id;
    QString display_text;
    int first_v;
    int last_v;
};

class BibleBook
{
    // For Holding Bible book infromation
public:
    QString book;
    QString bookId;
    int chapterCount;
};

class Bible
{
public:
    Bible();
    QList<BibleSearch> searchBible(bool begins, QString searchText);
    QList<BibleSearch> searchBible(bool begins, QString book, QString searchText);
    QList<BibleSearch> searchBible(bool begins, QString book, QString chapter, QString searchText);
    QStringList getBooks();
    QStringList getChapter(QString book, int chapter);
    void getVerseAndCaption(QString &verse, QString &caption, QString verId, QString &bibId, bool useAbbr);
    int getCurrentBookRow(QString book);
    Verse getCurrentVerseAndCaption(QList<int> currentRows, BibleSettings& sets);
    QStringList verseList;
    QStringList previewIdList; // Verses that are in the preview (chapter) list
    QStringList currentIdList; // Verses that are in the show list
    void setBiblesId(QString& id);
    QList<BibleBook> books;
    QString getBibleName();
private:
    QString bibleId;
    void retrieveBooks();

private slots:

};



#endif // BIBLE_H
