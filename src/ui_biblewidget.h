/********************************************************************************
** Form generated from reading ui file 'biblewidget.ui'
**
** Created: Sun Jun 21 09:06:50 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_BIBLEWIDGET_H
#define UI_BIBLEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BibleWidget
{
public:
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QWidget *widget;
    QGridLayout *gridLayout_4;
    QLabel *lblBook;
    QLabel *lblChapter;
    QSpinBox *spinChapter;
    QListWidget *listBook;
    QListWidget *listChapterNum;
    QLineEdit *lineEditBook;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget1;
    QGridLayout *gridLayout_5;
    QLabel *lblVerse;
    QSpinBox *spinVerse;
    QSpacerItem *horizontalSpacer;
    QListWidget *listChapter;
    QPushButton *btnLive;

    void setupUi(QWidget *BibleWidget)
    {
        if (BibleWidget->objectName().isEmpty())
            BibleWidget->setObjectName(QString::fromUtf8("BibleWidget"));
        BibleWidget->resize(628, 468);
        BibleWidget->setMinimumSize(QSize(500, 0));
        gridLayout = new QGridLayout(BibleWidget);
        gridLayout->setSpacing(3);
        gridLayout->setMargin(3);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(BibleWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        splitter->setChildrenCollapsible(false);
        widget = new QWidget(splitter);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout_4 = new QGridLayout(widget);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        lblBook = new QLabel(widget);
        lblBook->setObjectName(QString::fromUtf8("lblBook"));

        gridLayout_4->addWidget(lblBook, 0, 0, 1, 1);

        lblChapter = new QLabel(widget);
        lblChapter->setObjectName(QString::fromUtf8("lblChapter"));
        lblChapter->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(lblChapter, 0, 3, 1, 1);

        spinChapter = new QSpinBox(widget);
        spinChapter->setObjectName(QString::fromUtf8("spinChapter"));
        spinChapter->setMinimum(1);
        spinChapter->setValue(1);

        gridLayout_4->addWidget(spinChapter, 0, 4, 1, 1);

        listBook = new QListWidget(widget);
        listBook->setObjectName(QString::fromUtf8("listBook"));
        listBook->setAlternatingRowColors(true);

        gridLayout_4->addWidget(listBook, 1, 0, 1, 4);

        listChapterNum = new QListWidget(widget);
        listChapterNum->setObjectName(QString::fromUtf8("listChapterNum"));
        listChapterNum->setMaximumSize(QSize(60, 16777215));
        listChapterNum->setAlternatingRowColors(true);

        gridLayout_4->addWidget(listChapterNum, 1, 4, 1, 1);

        lineEditBook = new QLineEdit(widget);
        lineEditBook->setObjectName(QString::fromUtf8("lineEditBook"));
        lineEditBook->setMinimumSize(QSize(125, 0));

        gridLayout_4->addWidget(lineEditBook, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 24, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 2, 1, 1);

        splitter->addWidget(widget);
        widget1 = new QWidget(splitter);
        widget1->setObjectName(QString::fromUtf8("widget1"));
        gridLayout_5 = new QGridLayout(widget1);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        lblVerse = new QLabel(widget1);
        lblVerse->setObjectName(QString::fromUtf8("lblVerse"));

        gridLayout_5->addWidget(lblVerse, 0, 0, 1, 1);

        spinVerse = new QSpinBox(widget1);
        spinVerse->setObjectName(QString::fromUtf8("spinVerse"));
        spinVerse->setMinimum(1);
        spinVerse->setValue(1);

        gridLayout_5->addWidget(spinVerse, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 24, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer, 0, 2, 1, 1);

        listChapter = new QListWidget(widget1);
        listChapter->setObjectName(QString::fromUtf8("listChapter"));
        listChapter->setAlternatingRowColors(true);
        listChapter->setWordWrap(true);

        gridLayout_5->addWidget(listChapter, 1, 0, 1, 4);

        btnLive = new QPushButton(widget1);
        btnLive->setObjectName(QString::fromUtf8("btnLive"));

        gridLayout_5->addWidget(btnLive, 0, 3, 1, 1);

        splitter->addWidget(widget1);

        gridLayout->addWidget(splitter, 0, 0, 1, 1);


        retranslateUi(BibleWidget);

        QMetaObject::connectSlotsByName(BibleWidget);
    } // setupUi

    void retranslateUi(QWidget *BibleWidget)
    {
        BibleWidget->setWindowTitle(QApplication::translate("BibleWidget", "Form", 0, QApplication::UnicodeUTF8));
        lblBook->setText(QApplication::translate("BibleWidget", "Book:", 0, QApplication::UnicodeUTF8));
        lblChapter->setText(QApplication::translate("BibleWidget", "Chapter: ", 0, QApplication::UnicodeUTF8));
        lblVerse->setText(QApplication::translate("BibleWidget", "Verse: ", 0, QApplication::UnicodeUTF8));
        btnLive->setText(QApplication::translate("BibleWidget", "Go Live", 0, QApplication::UnicodeUTF8));
        btnLive->setShortcut(QApplication::translate("BibleWidget", "Return", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(BibleWidget);
    } // retranslateUi

};

namespace Ui {
    class BibleWidget: public Ui_BibleWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BIBLEWIDGET_H
