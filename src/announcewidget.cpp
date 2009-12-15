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


void AnnounceWidget::drawToPainter(QPainter *painter, int width, int height)
{
    // Margins:
    int left = 30;
    int top = 20;
    int w = width - left - left;
    int h = height - top - top;

    int flags = Qt::TextWordWrap;
    if( ui->horizontal_comboBox->currentText() == "Left" )
        flags = flags | Qt::AlignLeft;
    else if( ui->horizontal_comboBox->currentText() == "Right" )
        flags = flags | Qt::AlignRight;
    else
        flags = flags | Qt::AlignCenter;

    if( ui->vertical_comboBox->currentText() == "Top" )
        flags = flags | Qt::AlignTop;
    else if( ui->vertical_comboBox->currentText() == "Bottom" )
        flags = flags | Qt::AlignBottom;
    else
        flags = flags | Qt::AlignHCenter;

    QRect rect = QRect(left, top, w, h);
    QString announce_text = ui->announceTextEdit->toPlainText();
    display->paintTextToRect(painter, rect, flags, announce_text);
}




void AnnounceWidget::on_btnLive_clicked()
{
    //qDebug() << "Go LIVE TEXT:";
    //qDebug() << ui->announceTextEdit->toPlainText();
    QString text = ui->announceTextEdit->toPlainText();
    emit sendText(text);
}
