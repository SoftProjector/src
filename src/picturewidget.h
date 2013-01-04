/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyleft (Ɔ) 2013  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QtGui>
#include "slideshow.h"

namespace Ui {
class PictureWidget;
}

class PictureWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PictureWidget(QWidget *parent = 0);
    ~PictureWidget();
    
signals:
    void sendSlideShow(QList<SlideShowItem> &slideShow, int row);

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_pushButtonAddImages_clicked();
    void on_pushButtonRemoveImage_clicked();
    void on_pushButtonMoveUp_clicked();
    void on_pushButtonMoveDown_clicked();
    void on_pushButtonGoLive_clicked();
    void sendToProjector();
private:
    Ui::PictureWidget *ui;
    QList<SlideShowItem> slides;
    QList<QPixmap> imagesPreview;
    QList<QPixmap> imagesToShow;
};

#endif // PICTUREWIDGET_H
