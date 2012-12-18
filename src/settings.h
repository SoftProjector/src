#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>
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
    int displayScreen; // stores primary display screen location
    int displayScreen2; // stores secondary display screen location
    DisplayControlsSettings displayControls;
    QString currentThemeId;
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
    SpSettings spMain;

public slots:
    void loadSettings();
    void saveSettings();
    void saveNewSettings();

private slots:
    QByteArray textToByt(QString text);
};

#endif // SETTINGS_H
