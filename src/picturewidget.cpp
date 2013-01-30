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
//    ui->pushButtonMoveUp->setEnabled(false);
//    ui->pushButtonMoveDown->setEnabled(false);
    ui->pushButtonRemoveImage->setEnabled(false);

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
        ui->labelPixInfo->setText("Preview slide: "+slides.at(currentRow).name);
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

void PictureWidget::on_pushButton_clicked()
{
//    QSqlTableModel sqt;
//    QSqlQuery sq;

//    AddSongbookDialog newSlide;
//    newSlide.setWindowTitle(tr("New Slide Show"));
//    newSlide.setWindowText(tr("Slide Show Name"),tr("Info"));
////    newSlide.setSongbook(tr("Slide Show");
//    int rt = newSlide.exec();
//    switch(rt)
//    {
//    case AddSongbookDialog::Accepted:
//        sqt.setTable("SlideShows");
//        sqt.insertRow(0);
//        sqt.setData(sqt.index(0,1),newSlide.title);
//        sqt.setData(sqt.index(0,2),newSlide.info);
//        sqt.submitAll();


//        loadSlideShows();
//        break;
//    case AddSongbookDialog::Rejected:
//        break;
//    }
    SlideShowEditor * sse = new SlideShowEditor;
//    sse->setSlideShow(currentSlideShow);
//    sse->show();
    sse->exec();
    loadSlideShows();
}

void PictureWidget::on_pushButton_2_clicked()
{
    currentSlideShow.slides = slides;
//    currentSlideShow.saveSideShow();
    SlideShowEditor * sse = new SlideShowEditor;
    sse->setSlideShow(currentSlideShow);
    sse->exec();
    loadSlideShows();
//    sse->show();
}

void PictureWidget::on_listWidgetSlideShow_currentRowChanged(int currentRow)
{
    if(currentRow>=0)
    {
        loadSlideShow(slideShows.at(currentRow).slideSwId);
    }

}

void PictureWidget::loadSlideShow(int ss_id)
{
//    currentSlideShow.slideShowId = ;
    qDebug()<<"ss_id:"<<ss_id;
    currentSlideShow.loadSlideShow(ss_id);
//    QStringList ln;
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

//    slides = currentSlideShow.slides;

//    ui->listWidgetSlides->addItems(ln);

}
