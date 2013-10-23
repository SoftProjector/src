#include "displaysetting.h"

DisplaySetting::DisplaySetting()
{
}

void BibleDisplaySettings::clear()
{
    ptRect = QRect();
    pcRect = QRect();
    stRect = QRect();
    scRect = QRect();
    ttRect = QRect();
    scRect = QRect();
    tFont = QFont();
    cFont = QFont();
}

void SongDisplaySettings::clear()
{
    cRect = QRect();
    nRect = QRect();
    tRect = QRect();
    eRect = QRect();
    tFlags = 0;
}

void AnnounceDisplaySettings::clear()
{
    tRect = QRect();
}
