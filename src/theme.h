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

#include <QtCore>
#include <QtSql>
#include "spfunctions.h"

class PassiveSettings
{
public:
    PassiveSettings();
    bool useBackground;
    QString backgroundName;
    QPixmap background;
    bool useDisp2settings;
};

class BibleSettings
{
    // To store Bible projection related settings
public:
    BibleSettings();
    bool useShadow;
    bool useFading;
    bool useBlurShadow;
    bool useBackground;
    QString backgroundName;
    QPixmap background;
    QFont textFont;
    QColor textColor;
    int textAlingmentV;
    int textAlingmentH;
    QFont captionFont;
    QColor captionColor;
    int captionAlingment;
    int captionPosition;
    bool useAbbriviations;
    int screenUse;
    int screenPosition;

    bool useDisp2settings;
};

class SongSettings
{
    // To store Song related settings
public:
    SongSettings();
    bool useFading;
    bool useShadow;
    bool useBlurShadow;
    bool showStanzaTitle;
    bool showSongKey;
    bool showSongNumber;
    QColor infoColor;
    QFont infoFont;
    int infoAling; // 0 = Top, 1 = Bottom
    bool showSongEnding;
    QColor endingColor;
    QFont endingFont;
    int endingType; // 0 = ***, 1 = ---, 2 = °°°, 3 = •••, 4 = ●●●, 5 = ▪▪▪, 6 = ■■■, 7 = for song copyright info
    int endingPosition;
    bool useBackground;
    QString backgroundName; // file path for background image
    QPixmap background;
    QColor textColor;
    QFont textFont;
    int textAlingmentV;
    int textAlingmentH;
    int screenUse;
    int screenPositon; // 0 = Top, 1 = Bottom
    bool useDisp2settings;
};

class AnnounceSettings
{   // To store Announcement related settings
public:
    AnnounceSettings();
    bool useShadow;
    bool useFading;
    bool useBlurShadow;
    bool useBackground;
    QString backgroundName; // file path for background image
    QPixmap background;
    QFont textFont;
    QColor textColor;
    int textAlingmentV;
    int textAlingmentH;

    bool useDisp2settings;
};

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
    PassiveSettings passive;
    PassiveSettings passive2; // Holds secondary display screen settings
    BibleSettings bible;
    BibleSettings bible2; // Holds secondary display screen settings
    SongSettings song;
    SongSettings song2; // Holds secondary display screen settings
    AnnounceSettings announce;
    AnnounceSettings announce2; // Holds secondary display screen settings

public slots:
    void saveThemeNew();
    void saveThemeUpdate();
    void loadTheme();
    void setThemeId(int id){themeId = id;}
    int getThemeId(){return themeId;}
    void setThemeInfo(ThemeInfo info);
    ThemeInfo getThemeInfo();

private:
    int themeId;
    QString name;
    QString comments;

private slots:
    void savePassiveNew(int screen, PassiveSettings &settings);
    void saveBibleNew(int screen, BibleSettings &settings);
    void saveSongNew(int screen, SongSettings &settings);
    void saveAnnounceNew(int screen, AnnounceSettings &settings);
    void savePassiveUpdate(int screen, PassiveSettings &settings);
    void saveBibleUpdate(int screen, BibleSettings &settings);
    void saveSongUpdate(int screen, SongSettings &settings);
    void saveAnnounceUpdate(int screen, AnnounceSettings &settings);
    void loadPassive(int screen, PassiveSettings &settings);
    void loadBible(int screen, BibleSettings &settings);
    void loadSong(int screen, SongSettings &settings);
    void loadAnnounce(int screen, AnnounceSettings &settings);

};

#endif // THEME_H
