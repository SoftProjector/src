#ifndef DISPLAY_H
#define DISPLAY_H

#include <QtGui/QWidget>

namespace Ui {
    class Display;
}

class Display : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(Display)
public:
    explicit Display(QWidget *parent = 0);
    virtual ~Display();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::Display *ui;
};

#endif // DISPLAY_H
