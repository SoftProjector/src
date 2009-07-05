/********************************************************************************
** Form generated from reading ui file 'display.ui'
**
** Created: Mon Jun 8 22:07:13 2009
**      by: Qt User Interface Compiler version 4.5.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Display
{
public:

    void setupUi(QWidget *Display)
    {
        if (Display->objectName().isEmpty())
            Display->setObjectName(QString::fromUtf8("Display"));
        Display->resize(400, 300);

        retranslateUi(Display);

        QMetaObject::connectSlotsByName(Display);
    } // setupUi

    void retranslateUi(QWidget *Display)
    {
        Display->setWindowTitle(QApplication::translate("Display", "Form", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Display);
    } // retranslateUi

};

namespace Ui {
    class Display: public Ui_Display {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DISPLAY_H
