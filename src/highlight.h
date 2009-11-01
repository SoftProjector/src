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
