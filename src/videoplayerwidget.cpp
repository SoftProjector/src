#include "videoplayerwidget.h"

VideoPlayerWidget::VideoPlayerWidget(QWidget *parent):
    Phonon::VideoWidget(parent)

{

}

void VideoPlayerWidget::setFullScreen(bool enabled)
{
    Phonon::VideoWidget::setFullScreen(enabled);
}

void VideoPlayerWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    Phonon::VideoWidget::mouseDoubleClickEvent(e);
    setFullScreen(!isFullScreen());
}

void VideoPlayerWidget::keyPressEvent(QKeyEvent *e)
{
//    if(!e->modifiers())
//    {
        int key = e->key();
        if (key == Qt::Key_Space)
        {
          emit playPause();
            e->accept();
            return;
        }
        else if (key == Qt::Key_Escape)
        {
            setFullScreen(false);
            e->accept();
            return;
        }
//    }
    Phonon::VideoWidget::keyPressEvent(e);
}

bool VideoPlayerWidget::event(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::Close:
        //we just ignore the cose events on the video widget
        //this prevents ALT+F4 from having an effect in fullscreen mode
        e->ignore();
        return true;
    case QEvent::MouseMove:
#ifndef QT_NO_CURSOR
        unsetCursor();
#endif
        //fall through
    case QEvent::WindowStateChange:
        {
//            const Qt::WindowFlags flags = m_player->windowFlags();
            if (windowState() & Qt::WindowFullScreen) {
                m_timer.start(1000, this);
            } else {
                m_timer.stop();
#ifndef QT_NO_CURSOR
                unsetCursor();
#endif
            }
        }
        break;
    default:
        break;
    }

    return Phonon::VideoWidget::event(e);
}

void VideoPlayerWidget::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == m_timer.timerId()) {
        //let's store the cursor shape
#ifndef QT_NO_CURSOR
        setCursor(Qt::BlankCursor);
#endif
    }
    Phonon::VideoWidget::timerEvent(e);
}
