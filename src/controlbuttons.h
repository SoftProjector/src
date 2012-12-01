#ifndef CONTROLBUTTONS_H
#define CONTROLBUTTONS_H

#include <QWidget>

namespace Ui {
class ControlButtons;
}

class ControlButtons : public QWidget
{
    Q_OBJECT
    
public:
    explicit ControlButtons(QWidget *parent = 0);
    ~ControlButtons();
    
private:
    Ui::ControlButtons *ui;
};

#endif // CONTROLBUTTONS_H
