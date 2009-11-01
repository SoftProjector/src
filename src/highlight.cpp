#include "highlight.h"

Highlight::Highlight(QTextDocument *parent)
        : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    verseFormat.setForeground(Qt::red);
    verseFormat.setBackground(Qt::yellow);
    rule.pattern = QRegExp(QString::fromUtf8("Куплет[^\n]*"));
    rule.format = verseFormat;
    highlightingRules.append(rule);

    chorusFormat.setFontItalic(true);
    chorusFormat.setForeground(Qt::darkBlue);
    chorusFormat.setBackground(Qt::yellow);
    rule.pattern = QRegExp(QString::fromUtf8("Припев[^\n]*"));
    rule.format = chorusFormat;
    highlightingRules.append(rule);

    vstavkaFormat.setFontItalic(true);
    vstavkaFormat.setForeground(Qt::darkBlue);
    vstavkaFormat.setBackground(Qt::yellow);
    rule.pattern = QRegExp(QString::fromUtf8("Вставка[^\n]*"));
    rule.format = vstavkaFormat;
    highlightingRules.append(rule);
}

void Highlight::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
}

// *** Highligting for search results ***
HighlightSearch::HighlightSearch(QTextDocument *parent)
        : QSyntaxHighlighter(parent)
{

}

void HighlightSearch::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);
}

void HighlightSearch::setHighlightText(QString text)
{
    HighlightingRule rule;
    highlightingRules.clear();
    resultFormat.setForeground(Qt::red);
//    resultFormat.setBackground(Qt::yellow);
    rule.pattern = QRegExp(text);
    rule.format = resultFormat;
    highlightingRules.append(rule);
}
