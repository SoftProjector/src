#ifndef CONTROLBUTTON_H
#define CONTROLBUTTON_H

#include <QtCore>
#include <QtGui>

class ControlButton : public QPushButton
{
    Q_OBJECT

public:
    ControlButton(QWidget * parent = 0);
    ControlButton(const QIcon & icon, const QIcon & iconHovered, const QIcon & iconPressed, QWidget * parent = 0);

    ~ControlButton();

    //Range: 0.0 [invisible] - 1.0 [opaque]
    void setOpacity(qreal opacity)        { m_opacity = opacity; }

protected:
    void paintEvent(QPaintEvent * pe);
    void enterEvent(QEvent * e);
    void leaveEvent(QEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);

private:
    QRect calculateIconPosition(QRect button_rect, QSize icon_size);

private:
    bool m_hovered;
    bool m_pressed;

    qreal m_opacity;

    QIcon m_icon;
    QIcon m_iconHovered;
    QIcon m_iconPressed;
};

#endif // CONTROLBUTTON_H
