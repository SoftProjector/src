/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2010  Vladislav Kobzar and Matvey Adzhigirey
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

#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include <QtGui>
#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

class Highlight : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlight(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat verseFormat, chorusFormat, vstavkaFormat;
};

// *** Highligting for search results ***
class HighlightSearch : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    HighlightSearch(QTextDocument *parent = 0);
    void setHighlightText(QString text);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
    QTextCharFormat resultFormat;
};

#endif // HIGHLIGHT_H
