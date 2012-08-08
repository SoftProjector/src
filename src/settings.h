#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtGui>

class GeneralSettings
        // To store General Program Settings
{
public:
    bool displayIsOnTop;
    bool useShadow;
    bool useFading;
    bool useBlurShadow;

    bool useBackground;
    QString backgroundPath;

};

class BibleSettings /*: public QObject*/
        // To store Bible projection related settings
{
public:
    QString primaryBible;
    QString secondaryBible;
    QString trinaryBible;
    QString controlBible;

    bool useBackground;
    QString backgroundPath; // file path for background image
    QFont textFont;
    QColor textColor;
    int textFlags; // text alingment

    bool useAbbriviations;

};

class SongSettings
        // To store Song related settings
{
public:
    bool showStanzaTitle;
    bool showSongKey;
    bool showSongNumber;

    bool showSongEnding;
    int songEndingType; // 0 for (***), 1 for song copyright info

    bool useBackground;
    QString backgroundPath; // file path for background image
    QFont textFont;
    QColor textColor;
    int textFlags; // text alingment
};

class AnnounceSetteings
        // To store Announcement related settings
{
public:
    bool useBackground;
    QString backgroundPath; // file path for background image
    QFont textFont;
    QColor textColor;
    int textFlags; // text alingment
};

class SettingsForDisplay
        // to store display settings for concurrent projection
{
public:
    bool useBackground;
    QString backgroundPath;
    QFont textFont;
    QColor textColor;
    int textFlags;
};

class Settings
{
public:
    GeneralSettings generalSettings;
    BibleSettings bibleSettings;
    SongSettings songSettings;
    AnnounceSetteings announceSettings;

//private slots:
//    void readGeneralSettings(QString name, QString value);
//    void loadSettings();
//    void saveSettings();
//public:
//    Settings();

////signals:

//public slots:
//    GeneralSettings getGeneralSettings();
//    void setGeneralSettings(GeneralSettings g);

//    void setSave();


};

#endif // SETTINGS_H
