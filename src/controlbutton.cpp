#include "controlbutton.h"

ControlButton::ControlButton(QWidget * parent)
    : QPushButton(parent), m_hovered(false), m_pressed(false), m_opacity(1.0)
{
}

ControlButton::ControlButton(const QIcon & icon, const QIcon &iconHovered, const QIcon &iconPressed, QWidget *parent)
    : QPushButton(parent), m_icon(icon), m_iconHovered(iconHovered), m_iconPressed(iconPressed),
      m_hovered(false), m_pressed(false), m_opacity(1.0)
{
}

ControlButton::~ControlButton(){}

void ControlButton::paintEvent(QPaintEvent * pe)
{
    Q_UNUSED(pe);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //test for state changes
    QColor button_color;
    QIcon button_icon;
    if(this->isEnabled())
    {
        if(m_hovered)
            button_icon = m_iconHovered;
        else
            button_icon  = m_icon;

        if(m_pressed)
            button_icon = m_iconPressed;
    }
    else
    {
        button_color = QColor(50, 50, 50);
    }

    QRect button_rect = this->geometry();

    if(!button_icon.isNull())
    {
        QSize icon_size = this->iconSize();
        QRect icon_position = this->calculateIconPosition(button_rect, icon_size);
        painter.setOpacity(m_opacity);
        painter.drawPixmap(icon_position, QPixmap(button_icon.pixmap(icon_size)));
    }
}

void ControlButton::enterEvent(QEvent * e)
{
    m_hovered = true;
    this->repaint();

    QPushButton::enterEvent(e);
}

void ControlButton::leaveEvent(QEvent * e)
{
    m_hovered = false;
    this->repaint();

    QPushButton::leaveEvent(e);
}

void ControlButton::mousePressEvent(QMouseEvent * e)
{
    m_pressed = true;
    this->repaint();

    QPushButton::mousePressEvent(e);
}

void ControlButton::mouseReleaseEvent(QMouseEvent * e)
{
    m_pressed = false;
    this->repaint();

    QPushButton::mouseReleaseEvent(e);
}

QRect ControlButton::calculateIconPosition(QRect button_rect, QSize icon_size)
{
    int x = (button_rect.width() / 2) - (icon_size.width() / 2);
    int y = (button_rect.height() / 2) - (icon_size.height() / 2);
    int width = icon_size.width();
    int height = icon_size.height();

    QRect icon_position;
    icon_position.setX(x);
    icon_position.setY(y);
    icon_position.setWidth(width);
    icon_position.setHeight(height);

    return icon_position;
}
