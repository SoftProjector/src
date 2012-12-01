#include "controlbuttons.h"
#include "ui_controlbuttons.h"

ControlButtons::ControlButtons(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlButtons)
{
    ui->setupUi(this);
}

ControlButtons::~ControlButtons()
{
    delete ui;
}
