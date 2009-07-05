#include "display.h"
#include "ui_display.h"

Display::Display(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);
}

Display::~Display()
{
    delete ui;
}

void Display::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
