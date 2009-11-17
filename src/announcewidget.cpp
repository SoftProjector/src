#include <QDebug>
#include "announcewidget.h"
#include "ui_announcewidget.h"

AnnounceWidget::AnnounceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnnounceWidget)
{
    ui->setupUi(this);

}

AnnounceWidget::~AnnounceWidget()
{
    delete ui;
}

void AnnounceWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString AnnounceWidget::getText()
{
    return ui->announceTextEdit->toPlainText();
}

void AnnounceWidget::sendToProjector()
{
    // Display the specified song text in the right-most column of softProjector:
    //emit sendSong(song, row);
}



void AnnounceWidget::on_btnLive_clicked()
{
    //qDebug() << "Go LIVE TEXT:";
    //qDebug() << ui->announceTextEdit->toPlainText();
    QString text = ui->announceTextEdit->toPlainText();
    emit sendText(text);
}
