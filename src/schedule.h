#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "bible.h"
#include "song.h"
#include "slideshow.h"
#include "videoinfo.h"
#include "announcement.h"

class Schedule
{
public:
    Schedule();
    Schedule(BibleHistory &b);
    Schedule(Song &s);
    Schedule(SlideShow &s);
    Schedule(VideoInfo &m);
    Schedule(Announcement &a);

    QString stype;
    QString name;
    QIcon icon;
    int scid;

    BibleHistory bible;
    Song song;
    SlideShow slideshow;
    VideoInfo media;
    Announcement announce;
};

#endif // SCHEDULE_H
