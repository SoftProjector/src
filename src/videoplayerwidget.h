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
    // Over-riding non-virtual Phonon::VideoWidget slot
    void setFullScreen(bool);

signals:
    void fullScreenChanged(bool);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    bool event(QEvent *e);
    void timerEvent(QTimerEvent *e);
    void dropEvent(QDropEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);

private:
    QBasicTimer m_timer;
    QAction m_action;
};

#endif // VIDEOPLAYERWIDGET_H
