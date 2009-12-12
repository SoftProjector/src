#include "highlight.h"

Highlight::Highlight(QTextDocument *parent)
        : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    QStringList patterns;

    // Verse formating
    verseFormat.setForeground(Qt::red);
    verseFormat.setBackground(Qt::yellow);
    patterns << "^Verse[^\n]*" << QString::fromUtf8("^Куплет[^\n]*")
            << "^&Verse[^\n]*" << QString::fromUtf8("^&Куплет[^\n]*");
    foreach (const QString &pattern, patterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = verseFormat;
        highlightingRules.append(rule);
    }

    patterns.clear();

    // Chorus formating
    chorusFormat.setFontItalic(true);
    chorusFormat.setForeground(Qt::darkBlue);
    chorusFormat.setBackground(QColor(212,240,28,255));
    patterns << "^Chorus[^\n]*" << "^Refrain[^\n]*" << QString::fromUtf8("^Припев[^\n]*")
            << "^&Chorus[^\n]*" << "^&Refrain[^\n]*" << QString::fromUtf8("^&Припев[^\n]*");
    foreach (const QString &pattern, patterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = chorusFormat;
        highlightingRules.append(rule);
    }

    patterns.clear();

    // Vsavka formating
    vstavkaFormat.setForeground(Qt::darkMagenta);
    vstavkaFormat.setBackground(QColor(255,140,0,255));
    patterns << "^Insert[^\n]*" << "^Slide[^\n]*" << "^Ending[^\n]*"
            << "^Intro[^\n]*" << QString::fromUtf8("^Вставка[^\n]*")
            << QString::fromUtf8("^Вступление[^\n]*") << QString::fromUtf8("^Окончание[^\n]*");
    foreach (const QString &pattern, patterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = vstavkaFormat;
        highlightingRules.append(rule);
    }
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
