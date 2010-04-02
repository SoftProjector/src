# -------------------------------------------------
# Project created by QtCreator 2009-05-13T06:51:58
# -------------------------------------------------
QT += sql
TARGET = softProjector
TEMPLATE = app
SOURCES += main.cpp \
    softprojector.cpp \
    songwidget.cpp \
    biblewidget.cpp \
    editwidget.cpp \
    song.cpp \
    display1.cpp \
    bible.cpp \
    settingsdialog.cpp \
    aboutdialog.cpp \
    addsongbookdialog.cpp \
    highlight.cpp \
    managedatadialog.cpp \
    managedata.cpp \
    announcewidget.cpp \
    helpdialog.cpp
HEADERS += softprojector.h \
    songwidget.h \
    biblewidget.h \
    editwidget.h \
    song.h \
    display1.h \
    bible.h \
    settingsdialog.h \
    aboutdialog.h \
    addsongbookdialog.h \
    highlight.h \
    managedatadialog.h \
    managedata.h \
    announcewidget.h \
    helpdialog.h
FORMS += softprojector.ui \
    songwidget.ui \
    biblewidget.ui \
    editwidget.ui \
    settingsdialog.ui \
    aboutdialog.ui \
    addsongbookdialog.ui \
    managedatadialog.ui \
    announcewidget.ui \
    helpdialog.ui
TRANSLATIONS += sotftpro_de.ts\
    sotftpro_ru.ts\
    sotftpro_ua.ts
CODECFORTR = UTF-8
RESOURCES += softprojector.qrc

win32 {
    RC_FILE = softprojector.rc
}
