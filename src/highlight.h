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

    QTextCharFormat verseFormat;
    QTextCharFormat chorusFormat;

};

#endif // HIGHLIGHT_H
