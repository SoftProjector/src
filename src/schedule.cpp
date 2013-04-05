#include "schedule.h"

Schedule::Schedule()
{
}

Schedule::Schedule(BibleHistory &b)
{
    stype = "bible";
    bible = b;
}

Schedule::Schedule(Song &s)
{
    stype = "song";
    song = s;
}

Schedule::Schedule(SlideShow &s)
{
    stype = "slideshow";
    slideshow = s;
}

Schedule::Schedule(VideoInfo &m)
{
    stype = "media";
    media = m;
}

Schedule::Schedule(Announcement &a)
{
    stype = "announce";
    announce = a;
}
