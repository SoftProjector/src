#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include <QString>

class VideoInfo
{
public:
    VideoInfo();
public:
    QString filePath;
    QString fileName;
    int aspectRatio;
};

#endif // VIDEOINFO_H
