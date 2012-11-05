/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2011  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

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

//void AnnounceWidget::drawToPainter(QPainter *painter, int width, int height)
//{
//    // Margins:
//    int left = 30;
//    int top = 20;
//    int w = width - left - left;
//    int h = height - top - top;

//    int flags = get_align_flags();

//    QRect rect = QRect(left, top, w, h);
//    QString announce_text = ui->announceTextEdit->toPlainText();
//    display->paintTextToRect(painter, rect, flags, announce_text);
//}

void AnnounceWidget::on_btnLive_clicked()
{
    //qDebug() << "Go LIVE TEXT:";
    //qDebug() << ui->announceTextEdit->toPlainText();
//    QString text = ui->announceTextEdit->toPlainText();
    Announcement a;
    a.text = ui->announceTextEdit->toPlainText();
    a.align_flags = get_align_flags();
    emit sendText(a);
}

void AnnounceWidget::on_add_to_history_pushButton_clicked()
{
    Announcement a;
    a.text = ui->announceTextEdit->toPlainText();
    a.align_flags = get_align_flags();
    history_items.append(a);
    ui->history_listWidget->addItem(a.text);
    emit annouceListChanged(true);
}

void AnnounceWidget::on_remove_from_history_pushButton_clicked()
{
    int current_row = ui->history_listWidget->currentRow();
    // FIXME disable this item if there is no row selected
    if (current_row >= 0)
    {
        ui->history_listWidget->takeItem(current_row);
        history_items.takeAt(current_row);
        emit annouceListChanged(true);
    }
}
void AnnounceWidget::on_history_listWidget_currentRowChanged(int currentRow)
{
    if( currentRow != -1 )
    {
        Announcement a = history_items.at(currentRow);
        ui->announceTextEdit->setText(a.text);
        set_align_boxes(a.align_flags);
    }
}

void AnnounceWidget::on_history_listWidget_doubleClicked(QModelIndex index)
{
    Announcement a = history_items.at(index.row());
    //ui->announceTextEdit->setText(a.text);
    emit sendText(a);
}

int AnnounceWidget::get_align_flags()
{
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

    return flags;
}

void AnnounceWidget::set_align_boxes(int flags)
{
    if(flags == 4129)
    {
        ui->horizontal_comboBox->setCurrentIndex(0);
        ui->vertical_comboBox->setCurrentIndex(0);
    }
    else if(flags == 4225)
    {
        ui->horizontal_comboBox->setCurrentIndex(0);
        ui->vertical_comboBox->setCurrentIndex(1);
    }
    else if(flags == 4161)
    {
        ui->horizontal_comboBox->setCurrentIndex(0);
        ui->vertical_comboBox->setCurrentIndex(2);
    }
    else if(flags == 4132)
    {
        ui->horizontal_comboBox->setCurrentIndex(1);
        ui->vertical_comboBox->setCurrentIndex(0);
    }
    else if(flags == 4228)
    {
        ui->horizontal_comboBox->setCurrentIndex(1);
        ui->vertical_comboBox->setCurrentIndex(1);
    }
    else if(flags == 4164)
    {
        ui->horizontal_comboBox->setCurrentIndex(1);
        ui->vertical_comboBox->setCurrentIndex(2);
    }
    else if(flags == 4130)
    {
        ui->horizontal_comboBox->setCurrentIndex(2);
        ui->vertical_comboBox->setCurrentIndex(0);
    }
    else if(flags == 4226)
    {
        ui->horizontal_comboBox->setCurrentIndex(2);
        ui->vertical_comboBox->setCurrentIndex(1);
    }
    else if(flags == 4162)
    {
        ui->horizontal_comboBox->setCurrentIndex(2);
        ui->vertical_comboBox->setCurrentIndex(2);
    }
}

QList<Announcement> AnnounceWidget::getAnnouncements()
{
    return history_items;
}

void AnnounceWidget::loadFromFile(QList<Announcement> anns)
{
    QStringList a_list;
    history_items = anns;
    int count = anns.count();
    ui->history_listWidget->clear();
    if(count>0)
    {
        for(int i(0); i<count; ++i)
            a_list.append(anns.at(i).text);
        ui->history_listWidget->addItems(a_list);
    }
}

void AnnounceWidget::setAlingment(int v, int h)
{
    ui->vertical_comboBox->setCurrentIndex(v);
    ui->horizontal_comboBox->setCurrentIndex(h);
}
