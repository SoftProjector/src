#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui>
#include <QtSql>

class GeneralSettings
{   // To store General Program Settings
public:
    GeneralSettings();
    bool displayIsOnTop;
    bool useShadow;
    bool useFading;
    bool useBlurShadow;
    bool useBackground;
    QString backgroundPath;
    int displayScreen; // stores which sreen to display on
};

class BibleSettings
{    // To store Bible projection related settings
public:
    BibleSettings();
    QString primaryBible;
    QString secondaryBible;
    QString trinaryBible;
    QString operatorBible;
    bool useBackground;
    QString backgroundPath;
    QFont textFont;
    QColor textColor;
    int textAlingmentV;
    int textAlingmentH;
    bool useAbbriviations;
    int maxScreen;
    QString maxScreenFrom;
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
    bool useBackground;
    QString backgroundPath; // file path for background image
    QFont textFont;
    QColor textColor;
    int textAlingmentV;
    int textAlingmentH;
};

class AnnounceSettings
{   // To store Announcement related settings
public:
    AnnounceSettings();
    bool useBackground;
    QString backgroundPath; // file path for background image
    QFont textFont;
    QColor textColor;
    int textAlingmentV;
    int textAlingmentH;
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
    SongSettings song;
    AnnounceSettings announce;
    SpSettings spmain;
public slots:
    void loadSettings(QString user);
    void saveSettings(QString user);
private slots:
    QByteArray textToByt(QString text);
};

#endif // SETTINGS_H
