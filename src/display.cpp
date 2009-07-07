#include "display.h"
#include "ui_display.h"

ScreenDisplay::ScreenDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenDisplay)
{
    ui->setupUi(this);
}

ScreenDisplay::~ScreenDisplay()
{
    delete ui;
}

void ScreenDisplay::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
