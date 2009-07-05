/********************************************************************************
** Form generated from reading ui file 'editwidget.ui'
**
** Created: Tue Jun 30 07:49:59 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EDITWIDGET_H
#define UI_EDITWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditWidget
{
public:
    QGridLayout *gridLayout_11;
    QSplitter *splitter_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_10;
    QGridLayout *gridLayout;
    QComboBox *comboBoxSbornik;
    QSpinBox *spinBoxSongNum;
    QPushButton *btnEdit;
    QFrame *line;
    QGridLayout *gridLayout_9;
    QLabel *label_2;
    QLineEdit *txtSearch;
    QCheckBox *checkBoxAny;
    QPushButton *btnSortTitles;
    QListWidget *listTitles;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_7;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_8;
    QGridLayout *gridLayout_3;
    QLabel *label_15;
    QLineEdit *lineEditTitle;
    QLabel *label_16;
    QLineEdit *lineEditWordsBy;
    QLabel *label_18;
    QLineEdit *lineEditMusicBy;
    QLabel *label_20;
    QLabel *lblID;
    QSplitter *splitter;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_4;
    QLabel *label_17;
    QLineEdit *lineEditTune;
    QLabel *label_14;
    QComboBox *comboBoxCategory;
    QLabel *label_19;
    QComboBox *comboBoxLanguage;
    QCheckBox *checkBoxFavorite;
    QGridLayout *gridLayout_2;
    QFrame *line_3;
    QLabel *label_11;
    QLabel *label_3;
    QSpinBox *spinBoxpv3300;
    QLabel *label_4;
    QSpinBox *spinBoxpv2500;
    QLabel *label_5;
    QSpinBox *spinBoxpv2001;
    QLabel *label_6;
    QSpinBox *spinBoxpv2000a;
    QLabel *label_7;
    QSpinBox *spinBoxpv2000b;
    QLabel *label_8;
    QSpinBox *spinBoxpv1730;
    QLabel *label_9;
    QSpinBox *spinBoxpvCt;
    QLabel *label_10;
    QSpinBox *spinBoxuaPsalm;
    QLabel *label_13;
    QSpinBox *spinBoxuaEpisni;
    QLabel *label_12;
    QSpinBox *spinBoxlpvUser;
    QSpacerItem *verticalSpacer_2;
    QLabel *lblEditProg;
    QTextEdit *textEditSong;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_6;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnNew;
    QPushButton *btnSave;
    QPushButton *btnClose;
    QPushButton *btnCancel;

    void setupUi(QWidget *EditWidget)
    {
        if (EditWidget->objectName().isEmpty())
            EditWidget->setObjectName(QString::fromUtf8("EditWidget"));
        EditWidget->resize(800, 569);
        EditWidget->setMinimumSize(QSize(800, 0));
        gridLayout_11 = new QGridLayout(EditWidget);
        gridLayout_11->setSpacing(3);
        gridLayout_11->setMargin(3);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        splitter_2 = new QSplitter(EditWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        groupBox = new QGroupBox(splitter_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_10 = new QGridLayout(groupBox);
        gridLayout_10->setSpacing(3);
        gridLayout_10->setMargin(3);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        comboBoxSbornik = new QComboBox(groupBox);
        comboBoxSbornik->setObjectName(QString::fromUtf8("comboBoxSbornik"));

        gridLayout->addWidget(comboBoxSbornik, 0, 0, 1, 1);

        spinBoxSongNum = new QSpinBox(groupBox);
        spinBoxSongNum->setObjectName(QString::fromUtf8("spinBoxSongNum"));
        spinBoxSongNum->setMinimum(1);
        spinBoxSongNum->setMaximum(9999);
        spinBoxSongNum->setValue(1);

        gridLayout->addWidget(spinBoxSongNum, 0, 1, 1, 1);

        btnEdit = new QPushButton(groupBox);
        btnEdit->setObjectName(QString::fromUtf8("btnEdit"));
        btnEdit->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(btnEdit, 0, 2, 1, 1);


        gridLayout_10->addLayout(gridLayout, 0, 0, 1, 1);

        line = new QFrame(groupBox);
        line->setObjectName(QString::fromUtf8("line"));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 0, 0, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 127, 127, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(255, 63, 63, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(127, 0, 0, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(170, 0, 0, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush6(QColor(255, 255, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        line->setPalette(palette);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_10->addWidget(line, 1, 0, 1, 1);

        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_9->addWidget(label_2, 0, 0, 1, 1);

        txtSearch = new QLineEdit(groupBox);
        txtSearch->setObjectName(QString::fromUtf8("txtSearch"));
        txtSearch->setEnabled(true);
        txtSearch->setMinimumSize(QSize(150, 0));

        gridLayout_9->addWidget(txtSearch, 0, 1, 1, 1);

        checkBoxAny = new QCheckBox(groupBox);
        checkBoxAny->setObjectName(QString::fromUtf8("checkBoxAny"));
        checkBoxAny->setMaximumSize(QSize(15, 15));

        gridLayout_9->addWidget(checkBoxAny, 0, 2, 1, 1);

        btnSortTitles = new QPushButton(groupBox);
        btnSortTitles->setObjectName(QString::fromUtf8("btnSortTitles"));
        btnSortTitles->setMaximumSize(QSize(50, 16777215));

        gridLayout_9->addWidget(btnSortTitles, 0, 3, 1, 1);


        gridLayout_10->addLayout(gridLayout_9, 2, 0, 1, 1);

        listTitles = new QListWidget(groupBox);
        listTitles->setObjectName(QString::fromUtf8("listTitles"));
        listTitles->setAlternatingRowColors(true);
        listTitles->setSortingEnabled(false);

        gridLayout_10->addWidget(listTitles, 3, 0, 1, 1);

        splitter_2->addWidget(groupBox);
        layoutWidget = new QWidget(splitter_2);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        gridLayout_7 = new QGridLayout(layoutWidget);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_7->setContentsMargins(0, 0, 0, 0);
        groupBox_2 = new QGroupBox(layoutWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_8 = new QGridLayout(groupBox_2);
        gridLayout_8->setSpacing(2);
        gridLayout_8->setMargin(2);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_15, 0, 0, 1, 1);

        lineEditTitle = new QLineEdit(groupBox_2);
        lineEditTitle->setObjectName(QString::fromUtf8("lineEditTitle"));

        gridLayout_3->addWidget(lineEditTitle, 0, 1, 1, 3);

        label_16 = new QLabel(groupBox_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_16, 1, 0, 1, 1);

        lineEditWordsBy = new QLineEdit(groupBox_2);
        lineEditWordsBy->setObjectName(QString::fromUtf8("lineEditWordsBy"));

        gridLayout_3->addWidget(lineEditWordsBy, 1, 1, 1, 1);

        label_18 = new QLabel(groupBox_2);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_18, 1, 2, 1, 1);

        lineEditMusicBy = new QLineEdit(groupBox_2);
        lineEditMusicBy->setObjectName(QString::fromUtf8("lineEditMusicBy"));

        gridLayout_3->addWidget(lineEditMusicBy, 1, 3, 1, 1);

        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_3->addWidget(label_20, 0, 4, 1, 1);

        lblID = new QLabel(groupBox_2);
        lblID->setObjectName(QString::fromUtf8("lblID"));

        gridLayout_3->addWidget(lblID, 0, 5, 1, 1);


        gridLayout_8->addLayout(gridLayout_3, 0, 0, 1, 1);

        splitter = new QSplitter(groupBox_2);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        gridLayout_5 = new QGridLayout(layoutWidget1);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_17 = new QLabel(layoutWidget1);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_17, 0, 0, 1, 1);

        lineEditTune = new QLineEdit(layoutWidget1);
        lineEditTune->setObjectName(QString::fromUtf8("lineEditTune"));

        gridLayout_4->addWidget(lineEditTune, 0, 1, 1, 1);

        label_14 = new QLabel(layoutWidget1);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_14, 1, 0, 1, 1);

        comboBoxCategory = new QComboBox(layoutWidget1);
        comboBoxCategory->setObjectName(QString::fromUtf8("comboBoxCategory"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBoxCategory->sizePolicy().hasHeightForWidth());
        comboBoxCategory->setSizePolicy(sizePolicy);
        comboBoxCategory->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);

        gridLayout_4->addWidget(comboBoxCategory, 1, 1, 1, 1);

        label_19 = new QLabel(layoutWidget1);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(label_19, 2, 0, 1, 1);

        comboBoxLanguage = new QComboBox(layoutWidget1);
        comboBoxLanguage->setObjectName(QString::fromUtf8("comboBoxLanguage"));

        gridLayout_4->addWidget(comboBoxLanguage, 2, 1, 1, 1);

        checkBoxFavorite = new QCheckBox(layoutWidget1);
        checkBoxFavorite->setObjectName(QString::fromUtf8("checkBoxFavorite"));

        gridLayout_4->addWidget(checkBoxFavorite, 3, 1, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(1);
        line_3 = new QFrame(layoutWidget1);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout_2->addWidget(line_3, 0, 0, 1, 2);

        label_11 = new QLabel(layoutWidget1);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_11, 1, 0, 1, 2);

        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);

        spinBoxpv3300 = new QSpinBox(layoutWidget1);
        spinBoxpv3300->setObjectName(QString::fromUtf8("spinBoxpv3300"));
        spinBoxpv3300->setMaximumSize(QSize(100, 16777215));
        spinBoxpv3300->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBoxpv3300->setMaximum(3300);

        gridLayout_2->addWidget(spinBoxpv3300, 2, 1, 1, 1);

        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_4, 3, 0, 1, 1);

        spinBoxpv2500 = new QSpinBox(layoutWidget1);
        spinBoxpv2500->setObjectName(QString::fromUtf8("spinBoxpv2500"));
        spinBoxpv2500->setMaximumSize(QSize(100, 16777215));
        spinBoxpv2500->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBoxpv2500->setMaximum(2500);

        gridLayout_2->addWidget(spinBoxpv2500, 3, 1, 1, 1);

        label_5 = new QLabel(layoutWidget1);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_5, 4, 0, 1, 1);

        spinBoxpv2001 = new QSpinBox(layoutWidget1);
        spinBoxpv2001->setObjectName(QString::fromUtf8("spinBoxpv2001"));
        spinBoxpv2001->setMaximumSize(QSize(100, 16777215));
        spinBoxpv2001->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBoxpv2001->setMaximum(2001);

        gridLayout_2->addWidget(spinBoxpv2001, 4, 1, 1, 1);

        label_6 = new QLabel(layoutWidget1);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_6, 5, 0, 1, 1);

        spinBoxpv2000a = new QSpinBox(layoutWidget1);
        spinBoxpv2000a->setObjectName(QString::fromUtf8("spinBoxpv2000a"));
        spinBoxpv2000a->setMaximumSize(QSize(100, 16777215));
        spinBoxpv2000a->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBoxpv2000a->setMaximum(2000);

        gridLayout_2->addWidget(spinBoxpv2000a, 5, 1, 1, 1);

        label_7 = new QLabel(layoutWidget1);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_7, 6, 0, 1, 1);

        spinBoxpv2000b = new QSpinBox(layoutWidget1);
        spinBoxpv2000b->setObjectName(QString::fromUtf8("spinBoxpv2000b"));
        spinBoxpv2000b->setMaximumSize(QSize(100, 16777215));
        spinBoxpv2000b->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBoxpv2000b->setMaximum(2000);

        gridLayout_2->addWidget(spinBoxpv2000b, 6, 1, 1, 1);

        label_8 = new QLabel(layoutWidget1);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 7, 0, 1, 1);

        spinBoxpv1730 = new QSpinBox(layoutWidget1);
        spinBoxpv1730->setObjectName(QString::fromUtf8("spinBoxpv1730"));
        spinBoxpv1730->setMaximumSize(QSize(100, 16777215));
        spinBoxpv1730->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBoxpv1730->setMaximum(1730);

        gridLayout_2->addWidget(spinBoxpv1730, 7, 1, 1, 1);

        label_9 = new QLabel(layoutWidget1);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_9, 8, 0, 1, 1);

        spinBoxpvCt = new QSpinBox(layoutWidget1);
        spinBoxpvCt->setObjectName(QString::fromUtf8("spinBoxpvCt"));
        spinBoxpvCt->setMaximumSize(QSize(100, 16777215));
        spinBoxpvCt->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBoxpvCt->setMaximum(800);

        gridLayout_2->addWidget(spinBoxpvCt, 8, 1, 1, 1);

        label_10 = new QLabel(layoutWidget1);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_10, 9, 0, 1, 1);

        spinBoxuaPsalm = new QSpinBox(layoutWidget1);
        spinBoxuaPsalm->setObjectName(QString::fromUtf8("spinBoxuaPsalm"));
        spinBoxuaPsalm->setMaximumSize(QSize(100, 16777215));
        spinBoxuaPsalm->setButtonSymbols(QAbstractSpinBox::NoButtons);

        gridLayout_2->addWidget(spinBoxuaPsalm, 9, 1, 1, 1);

        label_13 = new QLabel(layoutWidget1);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_13, 10, 0, 1, 1);

        spinBoxuaEpisni = new QSpinBox(layoutWidget1);
        spinBoxuaEpisni->setObjectName(QString::fromUtf8("spinBoxuaEpisni"));
        spinBoxuaEpisni->setMaximumSize(QSize(100, 16777215));
        spinBoxuaEpisni->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBoxuaEpisni->setMaximum(521);

        gridLayout_2->addWidget(spinBoxuaEpisni, 10, 1, 1, 1);

        label_12 = new QLabel(layoutWidget1);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_12, 11, 0, 1, 1);

        spinBoxlpvUser = new QSpinBox(layoutWidget1);
        spinBoxlpvUser->setObjectName(QString::fromUtf8("spinBoxlpvUser"));
        spinBoxlpvUser->setMaximumSize(QSize(100, 16777215));
        spinBoxlpvUser->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spinBoxlpvUser->setMaximum(9999);

        gridLayout_2->addWidget(spinBoxlpvUser, 11, 1, 1, 1);


        gridLayout_5->addLayout(gridLayout_2, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_2, 4, 0, 1, 1);

        lblEditProg = new QLabel(layoutWidget1);
        lblEditProg->setObjectName(QString::fromUtf8("lblEditProg"));
        lblEditProg->setEnabled(true);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush1);
        QBrush brush8(QColor(118, 116, 108, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush8);
        lblEditProg->setPalette(palette1);
        QFont font;
        font.setPointSize(9);
        font.setBold(true);
        font.setWeight(75);
        lblEditProg->setFont(font);
        lblEditProg->setAlignment(Qt::AlignCenter);
        lblEditProg->setTextInteractionFlags(Qt::NoTextInteraction);

        gridLayout_5->addWidget(lblEditProg, 2, 0, 1, 1);

        splitter->addWidget(layoutWidget1);
        textEditSong = new QTextEdit(splitter);
        textEditSong->setObjectName(QString::fromUtf8("textEditSong"));
        splitter->addWidget(textEditSong);

        gridLayout_8->addWidget(splitter, 1, 0, 1, 1);


        gridLayout_7->addWidget(groupBox_2, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(layoutWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_3->sizePolicy().hasHeightForWidth());
        groupBox_3->setSizePolicy(sizePolicy2);
        gridLayout_6 = new QGridLayout(groupBox_3);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        horizontalSpacer = new QSpacerItem(168, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer, 0, 0, 1, 1);

        btnNew = new QPushButton(groupBox_3);
        btnNew->setObjectName(QString::fromUtf8("btnNew"));

        gridLayout_6->addWidget(btnNew, 0, 1, 1, 1);

        btnSave = new QPushButton(groupBox_3);
        btnSave->setObjectName(QString::fromUtf8("btnSave"));

        gridLayout_6->addWidget(btnSave, 0, 2, 1, 1);

        btnClose = new QPushButton(groupBox_3);
        btnClose->setObjectName(QString::fromUtf8("btnClose"));

        gridLayout_6->addWidget(btnClose, 0, 4, 1, 1);

        btnCancel = new QPushButton(groupBox_3);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));

        gridLayout_6->addWidget(btnCancel, 0, 3, 1, 1);


        gridLayout_7->addWidget(groupBox_3, 1, 0, 1, 1);

        splitter_2->addWidget(layoutWidget);

        gridLayout_11->addWidget(splitter_2, 0, 0, 1, 1);


        retranslateUi(EditWidget);

        comboBoxSbornik->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(EditWidget);
    } // setupUi

    void retranslateUi(QWidget *EditWidget)
    {
        EditWidget->setWindowTitle(QApplication::translate("EditWidget", "Form", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("EditWidget", "Song selector", 0, QApplication::UnicodeUTF8));
        comboBoxSbornik->clear();
        comboBoxSbornik->insertItems(0, QStringList()
         << QApplication::translate("EditWidget", "\320\237\320\265\321\201\320\275\321\214 \320\222\320\276\320\267\320\276\321\200\320\266\320\264\320\265\320\275\320\270\321\217 3300", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\320\265\321\201\320\275\321\214 \320\222\320\276\320\267\320\276\321\200\320\266\320\264\320\265\320\275\320\270\321\217 2500", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\320\265\321\201\320\275\321\214 \320\222\320\276\320\267\320\276\321\200\320\266\320\264\320\265\320\275\320\270\321\217 2001", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\320\265\321\201\320\275\321\214 \320\222\320\276\320\267\320\276\321\200\320\266\320\264\320\265\320\275\320\270\321\217 2000a", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\320\265\321\201\320\275\321\214 \320\222\320\276\320\267\320\276\321\200\320\266\320\264\320\265\320\275\320\270\321\217 2000b", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\320\265\321\201\320\275\321\214 \320\222\320\276\320\267\320\276\321\200\320\266\320\264\320\265\320\275\320\270\321\217 1730", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\320\265\321\201\320\275\321\214 \320\222\320\276\320\267. C\320\276\320\262\320\265\321\202 \320\246\320\265\321\200\320\272\320\262\320\265\320\271", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\204\320\262\320\260\320\275\320\263\320\265\320\273\321\201\321\214\320\272\321\226 \320\237\321\226\321\201\320\275\321\226 ", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "User", 0, QApplication::UnicodeUTF8)
        );
        btnEdit->setText(QApplication::translate("EditWidget", "Edit", 0, QApplication::UnicodeUTF8));
        btnEdit->setShortcut(QApplication::translate("EditWidget", "Ctrl+E", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("EditWidget", "Search Tiles:", 0, QApplication::UnicodeUTF8));
        btnSortTitles->setText(QApplication::translate("EditWidget", "A>Z", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("EditWidget", "Edit", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("EditWidget", "Title: ", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("EditWidget", "Words By:  ", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("EditWidget", "Music By: ", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("EditWidget", "ID:", 0, QApplication::UnicodeUTF8));
        lblID->setText(QString());
        label_17->setText(QApplication::translate("EditWidget", "Tune: ", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("EditWidget", "Category: ", 0, QApplication::UnicodeUTF8));
        comboBoxCategory->clear();
        comboBoxCategory->insertItems(0, QStringList()
         << QApplication::translate("EditWidget", "Other", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\221\320\270\320\261\320\273\320\265\320\271\321\201\320\272\320\270\320\265 \320\270\321\201\321\202\320\276\321\200\320\270\320\270", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\221\320\273\320\260\320\263\320\276\320\262\320\265\321\201\321\202\320\262\320\276\320\262\320\260\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\221\320\276\320\263", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\221\320\276\320\266\321\214\321\217 \320\273\321\216\320\261\320\276\320\262\321\214 \320\270 \320\262\320\265\320\273\320\270\321\207\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\222\321\200\320\265\320\274\321\217", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\222\320\276\321\201\320\272\321\200\320\265\321\201\320\265\320\275\320\270\320\265 \320\245\321\200\320\270\321\201\321\202\320\276\320\262\320\276", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\222\321\202\320\276\321\200\320\276\320\265 \320\277\321\200\320\270\321\210\320\265\321\201\321\202\320\262\320\270\320\265 \320\245\321\200\320\270\321\201\321\202\320\260 \320\270 \321\201\321\203\320\264", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\224\320\273\321\217 \320\264\320\265\321\202\320\265\320\271 \320\270 \321\201\320\265\320\274\320\265\320\271\320\275\321\213\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\224\321\203\321\205\320\276\320\262\320\275\320\260\321\217 \320\261\320\276\321\200\321\214\320\261\320\260 \320\270 \320\277\320\276\320\261\320\265\320\264\320\260", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\224\320\273\321\217 \320\275\320\276\320\262\320\276\320\276\320\261\321\200\320\260\321\211\321\221\320\275\320\275\321\213\321\205", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\226\320\260\321\202\320\262\320\265\320\275\320\275\321\213\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\230\320\270\321\201\321\203\321\201 \320\245\321\200\320\270\321\201\321\202\320\276\321\201", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\233\321\216\320\261\320\276\320\262\321\214", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\234\320\260\320\274\320\260", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\234\320\276\320\273\320\270\321\202\320\262\320\260", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\234\320\276\320\273\320\270\321\202\320\262\320\265\320\275\320\275\321\213\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\234\320\276\320\273\320\276\320\264\321\221\320\266\320\275\321\213\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\235\320\260 \320\261\321\200\320\260\320\272\320\276\321\201\320\276\321\207\320\265\321\202\320\260\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\235\320\260 \320\267\320\260\320\272\320\260\321\202\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\235\320\260\321\210 \320\224\321\200\321\203\320\263 \320\270 \320\241\320\277\320\260\321\201\320\270\321\202\320\265\320\273\321\214", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\235\320\260 \320\272\321\200\320\265\321\211\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\235\320\260 \320\275\320\276\320\262\321\213\320\271 \320\263\320\276\320\264", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\235\320\260 n\320\276\320\263\321\200\320\265\320\261\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\235\320\260 \321\200\320\260\321\201\321\201\320\262\320\265\321\202\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\235\320\260 \321\200\321\203\320\272\320\276\320\277\320\276\320\273\320\276\320\266\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\235\320\260 \321\205\320\273\320\265\320\261\320\276\320\277\321\200\320\265\320\273\320\276\320\274\320\273\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\235\320\265\320\261\320\265\321\201\320\275\321\213\320\265 \320\276\320\261\320\270\321\202\320\265\320\273\320\270", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\235\320\260\321\201\321\202\320\260\320\262\320\273\320\265\320\275\320\270\320\265 \320\270 \321\201\320\260\320\274\320\276\320\270\321\201\320\277\321\213\321\202\320\260\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\236 \320\224\321\203\321\205\320\265 \320\241\320\262\321\217\321\202\320\276\320\274, \320\276\321\201\320\262\321\217\321\211\320\265\320\275\320\270\320\265, \321\207\320\265\321\200\320\265\320\267 \320\224\321\203\321\205\320\260 \320\241\320\262\321\217\321\202\320\276\320\263\320\276", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\236 \320\246\320\265\321\200\320\272\320\262\320\270", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\320\265\321\200\320\265\320\264 \320\275\320\260\321\207\320\260\320\273\320\276\320\274 \321\201\320\276\320\261\321\200\320\260\320\275\320\270\321\217", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\320\276\321\201\320\273\320\265\320\264\320\275\320\265\320\265 \320\262\321\200\320\265\320\274\321\217", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\321\200\320\260\320\272\321\202\320\270\321\207\320\265\321\201\320\272\320\260\321\217 \320\266\320\270\320\267\320\275\321\214 \321\201 \320\221\320\276\320\263\320\276\320\274", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\321\200\320\270 \320\267\320\260\320\272\320\273\321\216\321\207\320\265\320\275\320\270\320\270 \321\201\320\276\320\261\321\200\320\260\320\275\320\270\321\217", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\321\200\320\270\320\262\320\265\321\202\321\201\321\202\320\262\320\265\320\275\320\275\321\213\320\265 \320\270 n\321\200\320\276\321\211\320\260\320\273\321\214\320\275\321\213\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\321\200\320\270\320\267\321\213\320\262 \320\272 \321\202\321\200\321\203\320\264\321\203", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\321\200\320\270\320\267\321\213\320\262 \320\272 \320\277\320\276\320\272\320\260\321\217\320\275\320\270\321\216, \320\276 \320\277\320\276\320\272\320\260\321\217\320\275\320\270\320\270", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\237\321\203\321\202\321\214 \320\262\320\265\321\200\321\213", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\240\320\260\320\264\320\276\321\201\321\202\321\214 \321\201\320\277\320\260\321\201\320\265\320\275\320\270\321\217 \320\262\320\276 \320\245\321\200\320\270\321\201\321\202\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\240\320\260\320\267\320\275\321\213\320\265 \321\205\321\200\320\270\321\201\321\202\320\270\320\260\320\275\321\201\320\272\320\270\320\265 \320\277\321\200\320\260\320\267\320\264\320\275\320\270\320\272\320\270", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\240\320\276\320\266\320\264\320\265\321\201\321\202\320\262\320\276 \321\205\321\200\320\270\321\201\321\202\320\276\320\262\320\276", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\240\320\265\321\210\320\270\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\321\214 \320\270 \320\262\320\265\321\200\320\275\320\276\321\201\321\202\321\214", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\240\321\203\320\272\320\270 \320\245\321\200\320\270\321\201\321\202\320\260", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\241\320\273\320\265\320\264\320\276\320\262\320\260\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\241\320\273\320\276\320\262\320\276 \320\221\320\276\320\266\321\214\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\241\320\277\320\260\321\201\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\241\321\202\321\200\320\260\320\264\320\260\320\275\320\270\320\265 \320\270 \321\201\320\274\320\265\321\200\321\202\321\214 \320\245\321\200\320\270\321\201\321\202\320\260", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\243\320\262\320\265\321\200\320\265\320\275\320\275\320\276\321\201\321\202\321\214 \320\262 \320\223\320\276\321\201\320\277\320\276\320\264\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\243\321\202\320\265\321\210\320\265\320\275\320\270\320\265 \320\261\320\276\320\273\321\214\320\275\321\213\320\274, \321\201\321\202\321\200\320\260\320\266\320\264\321\203\321\211\320\270\320\274 \321\203\320\267\320\275\320\270\320\272\320\260\320\274", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\243\321\202\320\265\321\210\320\265\320\275\320\270\320\265 \320\270 \320\276\320\261\320\276\320\264\321\200\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\245\320\262\320\260\320\273\320\260 \320\270 \320\261\320\273\320\260\320\263\320\276\320\264\320\260\321\200\320\265\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "\320\245\321\200\320\270\321\201\321\202\320\270\320\260\320\275\321\201\320\272\320\260\321\217 \321\200\320\260\320\264\320\276\321\201\321\202\321\214", 0, QApplication::UnicodeUTF8)
        );
        label_19->setText(QApplication::translate("EditWidget", "Language: ", 0, QApplication::UnicodeUTF8));
        comboBoxLanguage->clear();
        comboBoxLanguage->insertItems(0, QStringList()
         << QApplication::translate("EditWidget", "Russian", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "Ukranian", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("EditWidget", "English", 0, QApplication::UnicodeUTF8)
        );
        checkBoxFavorite->setText(QApplication::translate("EditWidget", "Favorite", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("EditWidget", "SBORNIK", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("EditWidget", "pv3300: ", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("EditWidget", "pv2500: ", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("EditWidget", "pv2001: ", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("EditWidget", "pv2000a: ", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("EditWidget", "pv2000b: ", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("EditWidget", "pv1730: ", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("EditWidget", "pv \320\241\320\246: ", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("EditWidget", "\320\237\321\201\320\260\320\273\320\274\320\276\320\277\321\226\320\262\321\226: ", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("EditWidget", "\320\204\320\262\320\260\320\275\320\263\320\265\320\273\321\201\321\214\320\272\321\226 \320\237\321\226\321\201\320\275\321\226: ", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("EditWidget", "User: ", 0, QApplication::UnicodeUTF8));
        lblEditProg->setText(QApplication::translate("EditWidget", "EDITING IN PROGRESS", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("EditWidget", "Control", 0, QApplication::UnicodeUTF8));
        btnNew->setText(QApplication::translate("EditWidget", "New", 0, QApplication::UnicodeUTF8));
        btnNew->setShortcut(QApplication::translate("EditWidget", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        btnSave->setText(QApplication::translate("EditWidget", "Save", 0, QApplication::UnicodeUTF8));
        btnSave->setShortcut(QApplication::translate("EditWidget", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        btnClose->setText(QApplication::translate("EditWidget", "Close", 0, QApplication::UnicodeUTF8));
        btnClose->setShortcut(QApplication::translate("EditWidget", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        btnCancel->setText(QApplication::translate("EditWidget", "Cancel", 0, QApplication::UnicodeUTF8));
        btnCancel->setShortcut(QApplication::translate("EditWidget", "Ctrl+Shift+X", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(EditWidget);
    } // retranslateUi

};

namespace Ui {
    class EditWidget: public Ui_EditWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITWIDGET_H
