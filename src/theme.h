/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2014  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

#ifndef THEME_H
#define THEME_H

#include <QtSql>
#include "settings.h"

class ThemeInfo
{
public:
    ThemeInfo();
    int themeId;
    QString name;
    QString comments;
};

class Theme
{
public:
    Theme();
    TextSettingsBase common;
    TextSettingsBase common2; // Holds secondary display screen settings
    TextSettings passive;
    TextSettings passive2; // Holds secondary display screen settings
    BibleSettings bible;
    BibleSettings bible2; // Holds secondary display screen settings
    SongSettings song;
    SongSettings song2; // Holds secondary display screen settings
    TextSettings announce;
    TextSettings announce2; // Holds secondary display screen settings

//public slots:
    void saveThemeNew();
    void saveThemeUpdate();
    void loadTheme();
    void setThemeId(int id){m_info.themeId = id;}
    int getThemeId(){return m_info.themeId;}
    void setThemeInfo(ThemeInfo info);
    ThemeInfo getThemeInfo();

private:
    ThemeInfo m_info;
};

#endif // THEME_H
