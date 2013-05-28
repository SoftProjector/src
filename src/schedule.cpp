#include "schedule.h"

Schedule::Schedule()
{
    scid = -1;
}

Schedule::Schedule(BibleHistory &b)
{
    scid = -1;
    stype = "bible";
    name = b.caption;
    icon = QIcon(":/icons/icons/book.png");
    bible = b;
}

Schedule::Schedule(Song &s)
{
    scid = -1;
    stype = "song";
    name = QString("%1 %2").arg(s.number).arg(s.title);
    icon = QIcon(":/icons/icons/song_tab.png");
    song = s;
}

Schedule::Schedule(SlideShow &s)
{
    scid = -1;
    stype = "slideshow";
    name = s.name;
    icon = QIcon(":/icons/icons/photo.png");
    slideshow = s;
}

Schedule::Schedule(VideoInfo &m)
{
    scid = -1;
    stype = "media";
    name = m.fileName;
    icon = QIcon(":/icons/icons/video.png");
    media = m;
}

Schedule::Schedule(Announcement &a)
{
    scid = -1;
    stype = "announce";
    name = a.title;
    icon = QIcon(":/icons/icons/announce.png");
    announce = a;
}
