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
    loadSlideShows();
}

PictureWidget::~PictureWidget()
{
    delete ui;
}

void PictureWidget::loadSlideShows()
{
    QSqlQuery sq;
    sq.exec("SELECT * FROM SlideShows");
    QStringList sl;

    slideShows.clear();

    while(sq.next())
    {
        SlideShowInfo ssi;
        ssi.slideSwId = sq.value(0).toInt();
        ssi.name = sq.value(1).toString();
        ssi.info = sq.value(2).toString();
        slideShows.append(ssi);
        sl.append( sq.value(1).toString());
    }
    ui->listWidgetSlideShow->clear();
    ui->listWidgetSlideShow->addItems(sl);
}

void PictureWidget::on_listWidgetSlides_currentRowChanged(int currentRow)
{
    if(currentRow>=0)
    {
        ui->labelPreview->setPixmap(slides.at(currentRow).imagePreview);
        ui->labelPixInfo->setText(tr("Preview slide: ")+slides.at(currentRow).name);
    }
}

void PictureWidget::on_listWidgetSlides_doubleClicked(const QModelIndex &index)
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
//        ui->listWidgetSlides->clear();
        int i(0);
        QProgressDialog progress(tr("Adding files..."), tr("Cancel"), 0, imageFilePaths.count(), this);
        ui->listWidgetSlides->setIconSize(QSize(100,100));
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
                sd.imagePreview = img.scaled(400,300, Qt::KeepAspectRatio);
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
            sd.path = f.filePath();

            // add to slideshow
            slides.append(sd);

            // add to slide show list
            QListWidgetItem *itm = new QListWidgetItem;
            QIcon ico(sd.imageSmall);

            itm->setIcon(ico);
            ui->listWidgetSlides->addItem(itm);
        }
        this->setCursor(Qt::ArrowCursor);
    }
}

void PictureWidget::on_pushButtonRemoveImage_clicked()
{
    int c = ui->listWidgetSlides->currentRow();
    if(c>=0)
    {
        slides.removeAt(c);
        ui->listWidgetSlides->clear();
        foreach(const SlideShowItem &sst, slides)
        {
            QListWidgetItem *itm = new QListWidgetItem;
            QIcon ico(sst.imageSmall);
            itm->setIcon(ico);
            ui->listWidgetSlides->addItem(itm);
        }
        ui->listWidgetSlides->setCurrentRow(c);
    }
}

void PictureWidget::on_pushButtonClearImages_clicked()
{
    ui->listWidgetSlideShow->setCurrentRow(-1);
    ui->listWidgetSlides->clear();
    slides.clear();
    ui->labelPreview->clear();
    ui->labelPreview->setText(tr("Picture Preview"));
    ui->labelPixInfo->setText(tr("Preview slide: "));
}

void PictureWidget::on_pushButtonMoveUp_clicked()
{
    int c = ui->listWidgetSlides->currentRow();
    int u = c-1;
    if(u>=0)
    {
        slides.move(c,u);
        ui->listWidgetSlides->clear();
        foreach(const SlideShowItem &sst, slides)
        {
            QListWidgetItem *itm = new QListWidgetItem;
            QIcon ico(sst.imageSmall);
            itm->setIcon(ico);
            ui->listWidgetSlides->addItem(itm);
        }
        ui->listWidgetSlides->setCurrentRow(u);
    }
}

void PictureWidget::on_pushButtonMoveDown_clicked()
{
    int c = ui->listWidgetSlides->currentRow();
    int d = c+1;
    if(d<slides.count())
    {
        slides.move(c,d);
        ui->listWidgetSlides->clear();
        foreach(const SlideShowItem &sst, slides)
        {
            QListWidgetItem *itm = new QListWidgetItem;
            QIcon ico(sst.imageSmall);
            itm->setIcon(ico);
            ui->listWidgetSlides->addItem(itm);
        }
        ui->listWidgetSlides->setCurrentRow(d);
    }
}

void PictureWidget::on_pushButtonGoLive_clicked()
{
    sendToProjector();
}

void PictureWidget::sendToProjector()
{
    emit sendSlideShow(slides, ui->listWidgetSlides->currentRow());
}

void PictureWidget::loadSlideShow(int ss_id)
{
    currentSlideShow.loadSlideShow(ss_id);
    slides.clear();
    ui->listWidgetSlides->clear();
    foreach(const SlideShowItem &sst, currentSlideShow.slides)
    {
        slides.append(sst);
        QListWidgetItem *itm = new QListWidgetItem;
        QIcon ico(sst.imageSmall);
        itm->setIcon(ico);
        ui->listWidgetSlides->addItem(itm);
    }
    ui->listWidgetSlides->setCurrentRow(0);
}

SlideShow PictureWidget::getCurrentSlideshow()
{
    currentSlideShow.slides = slides;
    return currentSlideShow;
}

bool PictureWidget::isSlideShowSelected()
{
    int css = ui->listWidgetSlideShow->selectionModel()->selection().count();
    if(css>0)
        return true;
    else
        return false;
}

void PictureWidget::deleteSlideShow()
{
    QSqlQuery sq;
    int ssId = slideShows.at(ui->listWidgetSlideShow->currentRow()).slideSwId;
    sq.exec(QString("DELETE FROM SlideShows WHERE id = %1").arg(ssId));
    sq.exec(QString("DELETE FROM Slides WHERE ss_id = %1").arg(ssId));
    loadSlideShows();
}

void PictureWidget::on_listWidgetSlideShow_itemSelectionChanged()
{
    int cRow = ui->listWidgetSlideShow->currentRow();
    if(cRow>=0)
        loadSlideShow(slideShows.at(cRow).slideSwId);
}
