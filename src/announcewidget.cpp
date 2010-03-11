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
    if( ui->horizontal_comboBox->currentText() == tr("Left") )
        flags = flags | Qt::AlignLeft;
    else if( ui->horizontal_comboBox->currentText() == tr("Right") )
        flags = flags | Qt::AlignRight;
    else if( ui->horizontal_comboBox->currentText() == tr("Center") )
        flags = flags | Qt::AlignHCenter;
    else
        qDebug() << "ERROR no such horizontal alignment";

    if( ui->vertical_comboBox->currentText() == tr("Top") )
        flags = flags | Qt::AlignTop;
    else if( ui->vertical_comboBox->currentText() == tr("Bottom") )
        flags = flags | Qt::AlignBottom;
    else if( ui->vertical_comboBox->currentText() == tr("Middle") )
        flags = flags | Qt::AlignVCenter;
    else
        qDebug() << "ERROR no such vertical alignment";

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




void AnnounceWidget::on_add_to_history_pushButton_clicked()
{
    Announcement a;
    a.text = ui->announceTextEdit->toPlainText();
    history_items.append(a);
    ui->history_listWidget->addItem(a.text);
}

void AnnounceWidget::on_remove_from_history_pushButton_clicked()
{
    int current_row = ui->history_listWidget->currentRow();
    // FIXME disable this item if there is no row selected
    if (current_row >= 0)
    {
        ui->history_listWidget->takeItem(current_row);
        history_items.takeAt(current_row);
    }
}
void AnnounceWidget::on_history_listWidget_currentRowChanged(int currentRow)
{
    if( currentRow != -1 )
    {
        Announcement a = history_items.at(currentRow);
        ui->announceTextEdit->setText(a.text);
    }
}

void AnnounceWidget::on_history_listWidget_doubleClicked(QModelIndex index)
{
    Announcement a = history_items.at(index.row());
    //ui->announceTextEdit->setText(a.text);
    emit sendText(a.text);
}
