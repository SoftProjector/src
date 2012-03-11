##**************************************************************************
##
##    softProjector - an open source media projection software
##    Copyright (C) 2011  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
##
##    This program is free software: you can redistribute it and/or modify
##    it under the terms of the GNU General Public License as published by
##    the Free Software Foundation version 3 of the License.
##
##    This program is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with this program.  If not, see <http:##www.gnu.org/licenses/>.
##
##**************************************************************************


QT += sql
TARGET = softProjector
TEMPLATE = app
//CONFIG += x86 ppc x86_64 ppc64 # Compile a universal build

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
    helpdialog.cpp \
    songcounter.cpp \
    bibleinformationdialog.cpp \
    settings.cpp \
    generalsettingwidget.cpp \
    biblesettingwidget.cpp \
    songsettingwidget.cpp \
    announcementsettingwidget.cpp
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
    helpdialog.h \
    songcounter.h \
    bibleinformationdialog.h \
    settings.h \
    generalsettingwidget.h \
    biblesettingwidget.h \
    songsettingwidget.h \
    announcementsettingwidget.h
FORMS += softprojector.ui \
    songwidget.ui \
    biblewidget.ui \
    editwidget.ui \
    settingsdialog.ui \
    aboutdialog.ui \
    addsongbookdialog.ui \
    managedatadialog.ui \
    announcewidget.ui \
    helpdialog.ui \
    songcounter.ui \
    bibleinformationdialog.ui \
    generalsettingwidget.ui \
    biblesettingwidget.ui \
    songsettingwidget.ui \
    announcementsettingwidget.ui
TRANSLATIONS += softpro_de.ts\
    softpro_ru.ts\
    softpro_cs.ts
CODECFORTR = UTF-8
RESOURCES += softprojector.qrc

win32 {
    RC_FILE = softprojector.rc
}






























