/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyleft (Ɔ) 2013  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

class PassiveSettings
{
public:
    PassiveSettings();
    bool useBackground;
    QString backgroundPath;
    bool useDisp2settings;
};

class BibleSettings
{    // To store Bible projection related settings
public:
    BibleSettings();
    QString primaryBible;
    QString secondaryBible;
    QString trinaryBible;
    QString operatorBible;
    bool useShadow;
    bool useFading;
    bool useBlurShadow;
    bool useBackground;
    QString backgroundPath;
    QFont textFont;
    QColor textColor;
    int textAlingmentV;
    int textAlingmentH;
    bool useAbbriviations;
    int maxScreen;
    QString maxScreenFrom;

    bool useDisp2settings;
};

class SongSettings
{   // To store Song related settings
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
    bool useBackground;
    QString backgroundPath; // file path for background image
    QPixmap background;
    QColor textColor;
    QFont textFont;
    int textAlingmentV;
    int textAlingmentH;
    int screenUse;
    int screenUseAlign; // 0 = Top, 1 = Bottom
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
    QString backgroundPath; // file path for background image
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
    QString themeId;
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
    void setThemeId(QString id){themeId = id;}
    QString getThemeId(){return themeId;}
    void saveTheme();
    void saveNewTheme(QString newId);
    void loadTheme();

private:
    QString themeId;

private slots:
    void passiveSettingFromString(QString &sets, PassiveSettings &settings);
    void bibleSettingFromString(QString &sets, BibleSettings &settings);
    void songSettingFromString(QString &sets, SongSettings &settings);
    void announceSettingFromString(QString &sets, AnnounceSettings &settings);
    QString passiveSettingToString(PassiveSettings &settings);
    QString bibleSettingToString(BibleSettings &settings);
    QString songSettingToString(SongSettings &settings);
    QString announceSettingToString(AnnounceSettings &settings);
    void saveNewTheme();
};

#endif // THEME_H
