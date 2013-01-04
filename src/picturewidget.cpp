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

#include "picturewidget.h"
#include "ui_picturewidget.h"

PictureWidget::PictureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureWidget)
{
    ui->setupUi(this);
    ui->pushButtonMoveUp->setEnabled(false);
    ui->pushButtonMoveDown->setEnabled(false);
    ui->pushButtonRemoveImage->setEnabled(false);
}

PictureWidget::~PictureWidget()
{
    delete ui;
}

void PictureWidget::on_listWidget_currentRowChanged(int currentRow)
{
    if(currentRow>=0)
    {
        ui->labelPreview->setPixmap(slides.at(currentRow).imagePreview);
        ui->labelPixInfo->setText("Preview slide: "+slides.at(currentRow).name);
    }
}

void PictureWidget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    sendToProjector();
//    emit sendImageList(imagesToShow, index.row());
}

void PictureWidget::on_pushButtonAddImages_clicked()
{
//    imagesPreview.clear();
//    imagesToShow.clear();
    QStringList imageFilePaths = QFileDialog::getOpenFileNames(this,tr("Select Images to Open"),".",tr("Images(*.png *.jpg *.jpeg)"));


    if(imageFilePaths.count()>0)
    {
        this->setCursor(Qt::WaitCursor);
//        ui->listWidget->clear();
        int i(0);
        QProgressDialog progress(tr("Adding files..."), tr("Cancel"), 0, imageFilePaths.count(), this);
        ui->listWidget->setIconSize(QSize(100,100));
        foreach(const QString &file, imageFilePaths)
        {
            ++i;
            progress.setValue(i);

            QPixmap img;
            SlideShowItem sd;
            img.load(file);
            // set display image
            if(img.width()>1280 || img.height()>1280)
                sd.image = img.scaled(1280,1280, Qt::KeepAspectRatio);
            else
                sd.image = img;

            // set preview image
            if(img.width()>400 || img.height()>400)
                sd.imagePreview = img.scaled(400,400, Qt::KeepAspectRatio);
            else
                sd.imagePreview = img;

            // set list image
            if(img.width()>100 || img.height()>100)
                sd.imageSmall = img.scaled(100,100, Qt::KeepAspectRatio);
            else
                sd.imageSmall = img;

            // set file name
            QFileInfo f(file);
            sd.name = f.fileName();

            // add to slideshow
            slides.append(sd);

            // add to slide show list
            QListWidgetItem *itm = new QListWidgetItem;
            QIcon ico(sd.imageSmall);

            itm->setIcon(ico);
            ui->listWidget->addItem(itm);
        }
        this->setCursor(Qt::ArrowCursor);
    }
}

void PictureWidget::on_pushButtonRemoveImage_clicked()
{

}

void PictureWidget::on_pushButtonMoveUp_clicked()
{

}

void PictureWidget::on_pushButtonMoveDown_clicked()
{

}

void PictureWidget::on_pushButtonGoLive_clicked()
{
    sendToProjector();
}

void PictureWidget::sendToProjector()
{
    emit sendSlideShow(slides, ui->listWidget->currentRow());
}
