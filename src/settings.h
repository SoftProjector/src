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

#ifndef SETTINGS_H
#define SETTINGS_H

//#include <QtCore>
#include <QtSql>
#include <QPixmap>

class TextSettings
{
public:
    TextSettings();
    //Text
    bool commonFont;
    QFont textFont;
    bool commonColor;
    QColor textColor;
    QColor textShadowColor;
    int textAlingmentV;
    // 0 - Top, 1 - Middle, 3 - Botton
    int textAlingmentH;
    // 0 - Left, 1 - Center, 3 - Right

    //Effects
    int transitionType;
    // -1 - Common, 0 - None, 1 - Fade, 2 - Fade out->in,
    // 3 - Move Right, 4 - Move Left, 5 - Move Up, 6 - Move Down
    bool useFading;// TODO: REMOVE
    int effectsType;
    // -1 - Common, 0 - None, 1 - Shadow, 2 - Blurred Shadow
    bool useShadow;// TODO: REMOVE
    bool useBluredShadow;// TODO: REMOVE

    //Background
    bool useBackground;// TODO: REMOVE
    int backgroundType;
    // -1 - Common, 0 - Solid Color, 1 - Picture, 2 - Video
    QColor backgroundColor;
    QString backgroundName;
    QPixmap background; //TODO: Rename to backgroundPix
    QString backgroundVideoPath;

    //Layout
    bool commonLayout;
    int screenUse;
    int screenPosition;
    // 0 - Top of Screen, 1 - Botton of Screen

    bool useSameForDisp2;
    bool useDisp2Settings; // TODO: REMOVE
};

//class MainScreenSettings : TextSettings
//{
//public:
//    MainScreenSettings();
//};

class BibleSettings : public TextSettings
{
public:
    BibleSettings();
    QFont captionFont;
    QColor captionColor;
    int captionAlingment;
    int captionPosition;
    bool useAbbriviation;
};

class SongSettings : public TextSettings
{
public:
    SongSettings();
    // Stanza Details
    bool showStanzaTitle;
    bool showSongKey;
    bool showSongNumber;
    bool showSongEnding;
    // Info
    QColor infoColor;
    QFont infoFont;
    int infoAling; // 0 = Top, 1 = Bottom
    //Ending
    QColor endingColor;
    QFont endingFont;
    int endingType; // 0 = ***, 1 = ---, 2 = °°°, 3 = •••, 4 = ●●●, 5 = ▪▪▪, 6 = ■■■, 7 = for song copyright info
    int endingPosition;
};

//class AnnounceSettings : TextSettings
//{
//public:
//    AnnounceSettings();
//};

class DisplayControlsSettings
{
public:
    DisplayControlsSettings();
    int buttonSize;
    int alignmentV;
    int alignmentH;
    qreal opacity;
};

class GeneralSettings
{   // To store General Program Settings
public:
    GeneralSettings();
    bool displayIsOnTop;
    int displayScreen; // stores primary display screen location
    int displayScreen2; // stores secondary display screen location
    DisplayControlsSettings displayControls;
    int currentThemeId;
};

class DisplaySettings
{   // to store display settings for concurrent projection
public:
    bool useBackground;
    QString backgroundPath;
    QFont textFont;
    QColor textColor;
    int textAlingmentV;
    int textAlingmentH;
};

class SpSettings
{   // stores main window settings, none user modifiable
public:
    SpSettings();
    QByteArray spSplitter;
    QByteArray bibleHiddenSplitter;
    QByteArray bibleShowSplitter;
    QByteArray songSplitter;
    bool isWindowMaximized;
    QString uiTranslation;
};

class BibleVersionSettings
{
public:
    BibleVersionSettings();
    QString primaryBible;
    QString secondaryBible;
    QString trinaryBible;
    QString operatorBible;

};

class SlideShowSettings
{
public:
    SlideShowSettings();
    bool expandSmall;
    int fitType;
    bool resize;
    int boundType;
    int boundWidth;
};

class Settings
{
public:
    Settings();
    GeneralSettings general;
    SpSettings spMain;
    BibleVersionSettings bibleSets;
    BibleVersionSettings bibleSets2;
    SlideShowSettings slideSets;

public slots:
    void loadSettings();
    void saveSettings();
    void saveNewSettings();

private slots:
    QByteArray textToByt(QString text);
};

#endif // SETTINGS_H
