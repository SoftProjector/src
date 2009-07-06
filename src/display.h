#ifndef SCREENDISPLAY_H
#define SCREENDISPLAY_H

#include <QtGui/QWidget>

namespace Ui {
    class ScreenDisplay;
}

class ScreenDisplay : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(ScreenDisplay)
public:
    explicit ScreenDisplay(QWidget *parent = 0);
    virtual ~ScreenDisplay();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ScreenDisplay *ui;
};

#endif // DISPLAY_H
