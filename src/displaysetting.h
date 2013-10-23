#ifndef DISPLAYSETTING_H
#define DISPLAYSETTING_H

#include <QRect>
#include <QFont>

class DisplaySetting
{
public:
    DisplaySetting();
};

class BibleDisplaySettings
{
public:
    QRect ptRect;
    QRect pcRect;
    QRect stRect;
    QRect scRect;
    QRect ttRect;
    QRect tcRect;
    QFont tFont;
    QFont cFont;
    void clear();
};

class SongDisplaySettings
{
public:
    QRect cRect;
    QRect nRect;
    QRect tRect;
    QRect eRect;
    int tFlags;
    void clear();
};

class AnnounceDisplaySettings
{
public:
    QRect tRect;
    void clear();
};

#endif // DISPLAYSETTING_H
