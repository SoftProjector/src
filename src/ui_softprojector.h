/********************************************************************************
** Form generated from reading ui file 'softprojector.ui'
**
** Created: Sun Jun 21 16:08:09 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SOFTPROJECTOR_H
#define UI_SOFTPROJECTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SoftProjectorClass
{
public:
    QAction *actionHelp;
    QAction *actionAbout;
    QAction *actionSettings;
    QAction *actionClose;
    QAction *actionDataBase;
    QAction *actionRussian;
    QAction *actionUkranian;
    QAction *actionEnglish_Kjv;
    QAction *actionRussian_2;
    QAction *actionUkranian_2;
    QAction *actionEnglish_Kjv_2;
    QAction *actionShowBible;
    QAction *actionShowSong;
    QAction *actionEditSong;
    QAction *actionNewSong;
    QAction *actionEditDialog;
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QSplitter *splitter;
    QTabWidget *tabWidget;
    QWidget *Tab;
    QGridLayout *gridLayout_2;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *btnBlack;
    QPushButton *btnShow;
    QCheckBox *checkBoxLive;
    QSpacerItem *horizontalSpacer;
    QLabel *labelShow;
    QListWidget *listShow;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuTools;
    QMenu *menuHelp;
    QMenu *menuBible;
    QMenu *menuPrimary_Bible;
    QMenu *menuSecondary_Bible;
    QMenu *menuSongs;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *SoftProjectorClass)
    {
        if (SoftProjectorClass->objectName().isEmpty())
            SoftProjectorClass->setObjectName(QString::fromUtf8("SoftProjectorClass"));
        SoftProjectorClass->resize(900, 550);
        SoftProjectorClass->setMinimumSize(QSize(900, 550));
        actionHelp = new QAction(SoftProjectorClass);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionAbout = new QAction(SoftProjectorClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionSettings = new QAction(SoftProjectorClass);
        actionSettings->setObjectName(QString::fromUtf8("actionSettings"));
        actionClose = new QAction(SoftProjectorClass);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionDataBase = new QAction(SoftProjectorClass);
        actionDataBase->setObjectName(QString::fromUtf8("actionDataBase"));
        actionDataBase->setVisible(false);
        actionRussian = new QAction(SoftProjectorClass);
        actionRussian->setObjectName(QString::fromUtf8("actionRussian"));
        actionRussian->setCheckable(true);
        actionUkranian = new QAction(SoftProjectorClass);
        actionUkranian->setObjectName(QString::fromUtf8("actionUkranian"));
        actionUkranian->setCheckable(true);
        actionUkranian->setEnabled(false);
        actionEnglish_Kjv = new QAction(SoftProjectorClass);
        actionEnglish_Kjv->setObjectName(QString::fromUtf8("actionEnglish_Kjv"));
        actionEnglish_Kjv->setCheckable(true);
        actionRussian_2 = new QAction(SoftProjectorClass);
        actionRussian_2->setObjectName(QString::fromUtf8("actionRussian_2"));
        actionRussian_2->setCheckable(true);
        actionUkranian_2 = new QAction(SoftProjectorClass);
        actionUkranian_2->setObjectName(QString::fromUtf8("actionUkranian_2"));
        actionUkranian_2->setCheckable(true);
        actionUkranian_2->setEnabled(false);
        actionEnglish_Kjv_2 = new QAction(SoftProjectorClass);
        actionEnglish_Kjv_2->setObjectName(QString::fromUtf8("actionEnglish_Kjv_2"));
        actionEnglish_Kjv_2->setCheckable(true);
        actionShowBible = new QAction(SoftProjectorClass);
        actionShowBible->setObjectName(QString::fromUtf8("actionShowBible"));
        actionShowSong = new QAction(SoftProjectorClass);
        actionShowSong->setObjectName(QString::fromUtf8("actionShowSong"));
        actionEditSong = new QAction(SoftProjectorClass);
        actionEditSong->setObjectName(QString::fromUtf8("actionEditSong"));
        actionNewSong = new QAction(SoftProjectorClass);
        actionNewSong->setObjectName(QString::fromUtf8("actionNewSong"));
        actionEditDialog = new QAction(SoftProjectorClass);
        actionEditDialog->setObjectName(QString::fromUtf8("actionEditDialog"));
        centralWidget = new QWidget(SoftProjectorClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(2);
        gridLayout_3->setMargin(11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(3, 3, 3, 2);
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        splitter->setChildrenCollapsible(false);
        tabWidget = new QTabWidget(splitter);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setMinimumSize(QSize(600, 400));
        Tab = new QWidget();
        Tab->setObjectName(QString::fromUtf8("Tab"));
        gridLayout_2 = new QGridLayout(Tab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setMargin(11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tabWidget->addTab(Tab, QString());
        splitter->addWidget(tabWidget);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setMargin(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        btnBlack = new QPushButton(layoutWidget);
        btnBlack->setObjectName(QString::fromUtf8("btnBlack"));

        gridLayout->addWidget(btnBlack, 0, 0, 1, 1);

        btnShow = new QPushButton(layoutWidget);
        btnShow->setObjectName(QString::fromUtf8("btnShow"));

        gridLayout->addWidget(btnShow, 0, 1, 1, 1);

        checkBoxLive = new QCheckBox(layoutWidget);
        checkBoxLive->setObjectName(QString::fromUtf8("checkBoxLive"));

        gridLayout->addWidget(checkBoxLive, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 3, 1, 1);

        labelShow = new QLabel(layoutWidget);
        labelShow->setObjectName(QString::fromUtf8("labelShow"));

        gridLayout->addWidget(labelShow, 1, 0, 1, 4);

        listShow = new QListWidget(layoutWidget);
        listShow->setObjectName(QString::fromUtf8("listShow"));
        listShow->setAlternatingRowColors(true);

        gridLayout->addWidget(listShow, 2, 0, 1, 4);

        splitter->addWidget(layoutWidget);

        gridLayout_3->addWidget(splitter, 0, 0, 1, 1);

        SoftProjectorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(SoftProjectorClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 900, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuTools = new QMenu(menuBar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuBible = new QMenu(menuBar);
        menuBible->setObjectName(QString::fromUtf8("menuBible"));
        menuPrimary_Bible = new QMenu(menuBible);
        menuPrimary_Bible->setObjectName(QString::fromUtf8("menuPrimary_Bible"));
        menuPrimary_Bible->setContextMenuPolicy(Qt::CustomContextMenu);
        menuSecondary_Bible = new QMenu(menuBible);
        menuSecondary_Bible->setObjectName(QString::fromUtf8("menuSecondary_Bible"));
        menuSongs = new QMenu(menuBar);
        menuSongs->setObjectName(QString::fromUtf8("menuSongs"));
        SoftProjectorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(SoftProjectorClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        SoftProjectorClass->setStatusBar(statusBar);
        toolBar = new QToolBar(SoftProjectorClass);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMovable(false);
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        toolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
        toolBar->setFloatable(false);
        SoftProjectorClass->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuTools->menuAction());
        menuBar->addAction(menuBible->menuAction());
        menuBar->addAction(menuSongs->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionClose);
        menuTools->addAction(actionDataBase);
        menuTools->addSeparator();
        menuTools->addAction(actionSettings);
        menuHelp->addAction(actionHelp);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        menuBible->addAction(actionShowBible);
        menuBible->addSeparator();
        menuBible->addAction(menuPrimary_Bible->menuAction());
        menuBible->addAction(menuSecondary_Bible->menuAction());
        menuPrimary_Bible->addAction(actionRussian);
        menuPrimary_Bible->addAction(actionUkranian);
        menuPrimary_Bible->addAction(actionEnglish_Kjv);
        menuSecondary_Bible->addAction(actionRussian_2);
        menuSecondary_Bible->addAction(actionUkranian_2);
        menuSecondary_Bible->addAction(actionEnglish_Kjv_2);
        menuSongs->addAction(actionShowSong);
        menuSongs->addSeparator();
        menuSongs->addAction(actionEditSong);
        menuSongs->addAction(actionNewSong);
        menuSongs->addSeparator();
        menuSongs->addAction(actionEditDialog);

        retranslateUi(SoftProjectorClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SoftProjectorClass);
    } // setupUi

    void retranslateUi(QMainWindow *SoftProjectorClass)
    {
        SoftProjectorClass->setWindowTitle(QApplication::translate("SoftProjectorClass", "SoftProjector", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("SoftProjectorClass", "Help", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("SoftProjectorClass", "About", 0, QApplication::UnicodeUTF8));
        actionSettings->setText(QApplication::translate("SoftProjectorClass", "Settings", 0, QApplication::UnicodeUTF8));
        actionClose->setText(QApplication::translate("SoftProjectorClass", "Close", 0, QApplication::UnicodeUTF8));
        actionClose->setShortcut(QApplication::translate("SoftProjectorClass", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        actionDataBase->setText(QApplication::translate("SoftProjectorClass", "DataBase", 0, QApplication::UnicodeUTF8));
        actionRussian->setText(QApplication::translate("SoftProjectorClass", "Russian", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionRussian->setStatusTip(QApplication::translate("SoftProjectorClass", "Set Russian Bible (\320\241\320\270\320\275\320\276\320\264\320\260\320\273\321\214\320\275\321\213\320\271 \320\237\320\265\321\200\320\265\320\262\320\276\320\264) as primary Bible language", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionUkranian->setText(QApplication::translate("SoftProjectorClass", "Ukranian", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionUkranian->setStatusTip(QApplication::translate("SoftProjectorClass", "Set Ukrainian Bible (\320\237\320\265\321\200\320\265\320\262\320\276\320\264 \320\236\320\263\320\270\320\265\320\275\320\272\320\276 1962\320\263.) as primary Bible language", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionEnglish_Kjv->setText(QApplication::translate("SoftProjectorClass", "English (Kjv)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionEnglish_Kjv->setStatusTip(QApplication::translate("SoftProjectorClass", "Set English Bible (King James Version) as primary Bible language", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionRussian_2->setText(QApplication::translate("SoftProjectorClass", "Russian", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionRussian_2->setStatusTip(QApplication::translate("SoftProjectorClass", "Set Russian Bible (\320\241\320\270\320\275\320\276\320\264\320\260\320\273\321\214\320\275\321\213\320\271 \320\237\320\265\321\200\320\265\320\262\320\276\320\264) as secondary Bible language", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionUkranian_2->setText(QApplication::translate("SoftProjectorClass", "Ukranian", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionUkranian_2->setStatusTip(QApplication::translate("SoftProjectorClass", "Set Ukrainian Bible (\320\237\320\265\321\200\320\265\320\262\320\276\320\264 \320\236\320\263\320\270\320\265\320\275\320\272\320\276 1962\320\263.) as secondary Bible language", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionEnglish_Kjv_2->setText(QApplication::translate("SoftProjectorClass", "English (Kjv)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionEnglish_Kjv_2->setStatusTip(QApplication::translate("SoftProjectorClass", "Set English Bible (King James Version) as secondary Bible language", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionShowBible->setText(QApplication::translate("SoftProjectorClass", "Show", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionShowBible->setStatusTip(QApplication::translate("SoftProjectorClass", "Show Bible Tab Below", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionShowBible->setShortcut(QApplication::translate("SoftProjectorClass", "Ctrl+Shift+B", 0, QApplication::UnicodeUTF8));
        actionShowSong->setText(QApplication::translate("SoftProjectorClass", "Show", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        actionShowSong->setStatusTip(QApplication::translate("SoftProjectorClass", "Show Song Tab Below", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        actionShowSong->setShortcut(QApplication::translate("SoftProjectorClass", "Ctrl+Shift+S", 0, QApplication::UnicodeUTF8));
        actionEditSong->setText(QApplication::translate("SoftProjectorClass", "Edit Current Song", 0, QApplication::UnicodeUTF8));
        actionEditSong->setShortcut(QApplication::translate("SoftProjectorClass", "Ctrl+E", 0, QApplication::UnicodeUTF8));
        actionNewSong->setText(QApplication::translate("SoftProjectorClass", "Add New Song", 0, QApplication::UnicodeUTF8));
        actionNewSong->setShortcut(QApplication::translate("SoftProjectorClass", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionEditDialog->setText(QApplication::translate("SoftProjectorClass", "Open Editing Window", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(Tab), QApplication::translate("SoftProjectorClass", "Tab", 0, QApplication::UnicodeUTF8));
        btnBlack->setText(QApplication::translate("SoftProjectorClass", "Black", 0, QApplication::UnicodeUTF8));
        btnBlack->setShortcut(QApplication::translate("SoftProjectorClass", "Esc", 0, QApplication::UnicodeUTF8));
        btnShow->setText(QApplication::translate("SoftProjectorClass", "Show", 0, QApplication::UnicodeUTF8));
        checkBoxLive->setText(QApplication::translate("SoftProjectorClass", "Live", 0, QApplication::UnicodeUTF8));
        labelShow->setText(QApplication::translate("SoftProjectorClass", "TextLabel", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("SoftProjectorClass", "File", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("SoftProjectorClass", "Tools", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("SoftProjectorClass", "Help", 0, QApplication::UnicodeUTF8));
        menuBible->setTitle(QApplication::translate("SoftProjectorClass", "Bible", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_STATUSTIP
        menuPrimary_Bible->setStatusTip(QApplication::translate("SoftProjectorClass", "Set Bible Primary Version", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
        menuPrimary_Bible->setTitle(QApplication::translate("SoftProjectorClass", "Primary Bible", 0, QApplication::UnicodeUTF8));
        menuSecondary_Bible->setTitle(QApplication::translate("SoftProjectorClass", "Secondary Bible", 0, QApplication::UnicodeUTF8));
        menuSongs->setTitle(QApplication::translate("SoftProjectorClass", "Songs", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("SoftProjectorClass", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SoftProjectorClass: public Ui_SoftProjectorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOFTPROJECTOR_H
