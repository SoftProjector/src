#ifndef VIDEOPLAYERWIDGET_H
#define VIDEOPLAYERWIDGET_H

#include <QtGui>
#include <phonon>

class VideoPlayerWidget : public Phonon::VideoWidget
{
    Q_OBJECT
public:
    VideoPlayerWidget(QWidget *parent = 0);
public slots:

    void setFullScreen(bool);

signals:
    void playPause();
    void handleDrops(QDropEvent *e);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    bool event(QEvent *e);
    void timerEvent(QTimerEvent *e);

private:
    QBasicTimer m_timer;
};

#endif // VIDEOPLAYERWIDGET_H
