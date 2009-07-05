/********************************************************************************
** Form generated from reading ui file 'songwidget.ui'
**
** Created: Sun Jun 21 09:17:00 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SONGWIDGET_H
#define UI_SONGWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
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

class Ui_SongWidget
{
public:
    QGridLayout *gridLayout_2;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QGridLayout *_2;
    QFrame *line;
    QListWidget *listTitles;
    QPushButton *btnAddToPlaylist;
    QLineEdit *lineEditSearch;
    QLabel *label_2;
    QComboBox *comboBoxPvNumber;
    QPushButton *btnRemoveFromPlaylist;
    QPushButton *btnSort;
    QCheckBox *checkBoxAny;
    QSpinBox *spinBoxPvNumber;
    QListWidget *listPlaylist;
    QWidget *widget;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnLive;
    QListWidget *listPreview;

    void setupUi(QWidget *SongWidget)
    {
        if (SongWidget->objectName().isEmpty())
            SongWidget->setObjectName(QString::fromUtf8("SongWidget"));
        SongWidget->resize(527, 473);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(SongWidget->sizePolicy().hasHeightForWidth());
        SongWidget->setSizePolicy(sizePolicy);
        SongWidget->setMinimumSize(QSize(500, 0));
        gridLayout_2 = new QGridLayout(SongWidget);
        gridLayout_2->setSpacing(3);
        gridLayout_2->setMargin(3);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        splitter_2 = new QSplitter(SongWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        splitter->setChildrenCollapsible(false);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        _2 = new QGridLayout(layoutWidget);
#ifndef Q_OS_MAC
        _2->setSpacing(6);
#endif
        _2->setMargin(0);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setContentsMargins(0, 0, 0, 0);
        line = new QFrame(layoutWidget);
        line->setObjectName(QString::fromUtf8("line"));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 127, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(0, 0, 191, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(0, 0, 159, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(0, 0, 63, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(0, 0, 84, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        QBrush brush6(QColor(0, 0, 0, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush4);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
        line->setPalette(palette);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        _2->addWidget(line, 2, 0, 1, 5);

        listTitles = new QListWidget(layoutWidget);
        listTitles->setObjectName(QString::fromUtf8("listTitles"));
        listTitles->setAlternatingRowColors(true);

        _2->addWidget(listTitles, 4, 0, 1, 7);

        btnAddToPlaylist = new QPushButton(layoutWidget);
        btnAddToPlaylist->setObjectName(QString::fromUtf8("btnAddToPlaylist"));

        _2->addWidget(btnAddToPlaylist, 6, 0, 1, 2);

        lineEditSearch = new QLineEdit(layoutWidget);
        lineEditSearch->setObjectName(QString::fromUtf8("lineEditSearch"));
        lineEditSearch->setEnabled(true);
        lineEditSearch->setMinimumSize(QSize(125, 0));

        _2->addWidget(lineEditSearch, 3, 1, 1, 4);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        _2->addWidget(label_2, 3, 0, 1, 1);

        comboBoxPvNumber = new QComboBox(layoutWidget);
        comboBoxPvNumber->setObjectName(QString::fromUtf8("comboBoxPvNumber"));
        comboBoxPvNumber->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
        comboBoxPvNumber->setDuplicatesEnabled(true);

        _2->addWidget(comboBoxPvNumber, 0, 0, 1, 6);

        btnRemoveFromPlaylist = new QPushButton(layoutWidget);
        btnRemoveFromPlaylist->setObjectName(QString::fromUtf8("btnRemoveFromPlaylist"));

        _2->addWidget(btnRemoveFromPlaylist, 6, 2, 1, 5);

        btnSort = new QPushButton(layoutWidget);
        btnSort->setObjectName(QString::fromUtf8("btnSort"));
        btnSort->setMaximumSize(QSize(24, 16777215));

        _2->addWidget(btnSort, 3, 6, 1, 1);

        checkBoxAny = new QCheckBox(layoutWidget);
        checkBoxAny->setObjectName(QString::fromUtf8("checkBoxAny"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(checkBoxAny->sizePolicy().hasHeightForWidth());
        checkBoxAny->setSizePolicy(sizePolicy1);
        checkBoxAny->setMaximumSize(QSize(15, 15));

        _2->addWidget(checkBoxAny, 3, 5, 1, 1);

        spinBoxPvNumber = new QSpinBox(layoutWidget);
        spinBoxPvNumber->setObjectName(QString::fromUtf8("spinBoxPvNumber"));
        spinBoxPvNumber->setMinimum(1);
        spinBoxPvNumber->setMaximum(2800);
        spinBoxPvNumber->setValue(1);

        _2->addWidget(spinBoxPvNumber, 0, 6, 1, 1);

        splitter->addWidget(layoutWidget);
        listPlaylist = new QListWidget(splitter);
        listPlaylist->setObjectName(QString::fromUtf8("listPlaylist"));
        listPlaylist->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        listPlaylist->setAlternatingRowColors(true);
        listPlaylist->setMovement(QListView::Static);
        listPlaylist->setProperty("isWrapping", QVariant(false));
        listPlaylist->setUniformItemSizes(true);
        listPlaylist->setWordWrap(true);
        splitter->addWidget(listPlaylist);
        splitter_2->addWidget(splitter);
        widget = new QWidget(splitter_2);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        btnLive = new QPushButton(widget);
        btnLive->setObjectName(QString::fromUtf8("btnLive"));

        gridLayout->addWidget(btnLive, 0, 1, 1, 1);

        listPreview = new QListWidget(widget);
        listPreview->setObjectName(QString::fromUtf8("listPreview"));
        listPreview->setMinimumSize(QSize(250, 0));
        listPreview->setAlternatingRowColors(true);
        listPreview->setSpacing(5);

        gridLayout->addWidget(listPreview, 1, 0, 1, 2);

        splitter_2->addWidget(widget);

        gridLayout_2->addWidget(splitter_2, 0, 0, 1, 1);


        retranslateUi(SongWidget);

        QMetaObject::connectSlotsByName(SongWidget);
    } // setupUi

    void retranslateUi(QWidget *SongWidget)
    {
        SongWidget->setWindowTitle(QApplication::translate("SongWidget", "Form", 0, QApplication::UnicodeUTF8));
        btnAddToPlaylist->setText(QApplication::translate("SongWidget", "Add to playlist", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("SongWidget", "Search Tiles:", 0, QApplication::UnicodeUTF8));
        comboBoxPvNumber->clear();
        comboBoxPvNumber->insertItems(0, QStringList()
         << QApplication::translate("SongWidget", "\320\237\320\265\321\201\320\275\321\214 \320\222\320\276\320\267\320\276\321\200\320\266\320\264\320\265\320\275\320\270\321\217 2800", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SongWidget", "\320\237\320\265\321\201\320\275\321\214 \320\222\320\276\320\267\320\276\321\200\320\266\320\264\320\265\320\275\320\270\321\217 2001", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SongWidget", "\320\204\320\262\320\260\320\275\320\263\320\265\320\273\321\201\321\214\320\272\321\226 \320\237\321\226\321\201\320\275\321\226 ", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("SongWidget", "User", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_STATUSTIP
        comboBoxPvNumber->setStatusTip(QApplication::translate("SongWidget", "Select Sbornik to use", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        btnRemoveFromPlaylist->setText(QApplication::translate("SongWidget", "Remove from playlist", 0, QApplication::UnicodeUTF8));
        btnSort->setText(QApplication::translate("SongWidget", "az", 0, QApplication::UnicodeUTF8));
        checkBoxAny->setText(QString());
        btnLive->setText(QApplication::translate("SongWidget", "Go Live", 0, QApplication::UnicodeUTF8));
        btnLive->setShortcut(QApplication::translate("SongWidget", "Return", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(SongWidget);
    } // retranslateUi

};

namespace Ui {
    class SongWidget: public Ui_SongWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SONGWIDGET_H
