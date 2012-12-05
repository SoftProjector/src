#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui>
#include <QtSql>

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
    bool useBackground;
    bool useBackground2;
    QString backgroundPath;
    QString backgroundPath2;
    int displayScreen; // stores primary display screen location
    int displayScreen2; // stores secondary display screen location
    DisplayControlsSettings displayControls;
    bool useDisplaySettings2;
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
    bool showStanzaTitle;
    bool showSongKey;
    bool showSongNumber;
    bool showSongEnding;
    int songEndingType; // 0 for (***), 1 for song copyright info
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

class Settings
{
public:
    Settings();
    GeneralSettings general;
    BibleSettings bible;
    BibleSettings bible2; // Holds secondary display screen settings
    SongSettings song;
    SongSettings song2; // Holds secondary display screen settings
    AnnounceSettings announce;
    AnnounceSettings announce2; // Holds secondary display screen settings
    SpSettings spmain;
public slots:
    void loadSettings(QString user);
    void saveSettings(QString user);
private slots:
    QByteArray textToByt(QString text);
    void bibleSettingFromString(QString &sets, BibleSettings &settings);
    void songSettingFromString(QString &sets, SongSettings &settings);
    void announceSettingFromString(QString &sets, AnnounceSettings &settings);
    QString bibleSettingToString(BibleSettings &settings);
    QString songSettingToString(SongSettings &settings);
    QString announceSettingToString(AnnounceSettings &settings);
};

#endif // SETTINGS_H
