#include "mediawidget.h"
#include "ui_mediawidget.h"

MediaWidget::MediaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaWidget),
    m_AudioOutput(Phonon::VideoCategory)
{
    ui->setupUi(this);
    videoWidget = new VideoPlayerWidget(this);

    ui->verticalLayoutMedia->addWidget(videoWidget);
    videoWidget->setVisible(false);
    ui->pushButtonGoLive->setEnabled(false);

    playIcon = QIcon(":icons/icons/play.png");
    pauseIcon = QIcon(":icons/icons/pause.png");

    ui->pushButtonPlayPause->setIcon(playIcon);
    ui->comboBoxAspectRatio->setEnabled(false);

    timeSlider = new Phonon::SeekSlider(this);
    timeSlider->setMediaObject(&mediaPlayer);
    volumeSlider = new Phonon::VolumeSlider(&m_AudioOutput);

    QPalette palette;
    palette.setBrush(QPalette::WindowText, Qt::white);

    ui->labelInfo->setStyleSheet("border-image:url(:icons/icons/playerScreen.png)");
    ui->labelInfo->setPalette(palette);
    ui->labelInfo->setText(tr("<center>No media</center>"));

    volumeSlider->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::Preferred);

    ui->horizontalLayoutControls->addWidget(volumeSlider);

    ui->horizontalLayoutTime->insertWidget(0,timeSlider);

    connect(&mediaPlayer, SIGNAL(metaDataChanged()), this, SLOT(updateInfo()));
    connect(&mediaPlayer, SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTime()));
    connect(&mediaPlayer, SIGNAL(tick(qint64)), this, SLOT(updateTime()));
    connect(&mediaPlayer, SIGNAL(finished()), this, SLOT(finished()));
    connect(&mediaPlayer, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    connect(&mediaPlayer, SIGNAL(hasVideoChanged(bool)), this, SLOT(hasVideoChanged(bool)));

    connect(videoWidget, SIGNAL(playPause()), this, SLOT(playPause()));
    connect(videoWidget, SIGNAL(handleDrops(QDropEvent*)), this, SLOT(handleDrop(QDropEvent*)));

    ui->pushButtonPlayPause->setEnabled(false);
    setAcceptDrops(true);

    Phonon::createPath(&mediaPlayer, &m_AudioOutput);
    Phonon::createPath(&mediaPlayer, videoWidget);

    audioExt = "*.mp3 *.acc *.ogg *.oga *.wma *.wav *.asf *.mka";
    videoExt = "*.wmv *.avi *.mkv *.flv *.mp4 *.mpg *.mpeg *.mov *.ogv *.ts";
    loadMediaLibrary();
}

MediaWidget::~MediaWidget()
{
    delete ui;
}

void MediaWidget::loadMediaLibrary()
{
    QSqlQuery sq;
    sq.exec("SELECT * FROM Media");
    while(sq.next())
    {
        mediaFilePaths.append(sq.value(0).toString());
        mediaFileNames.append(sq.value(1).toString());
        ui->listWidgetMediaFiles->clear();
        ui->listWidgetMediaFiles->addItems(mediaFileNames);
    }
}

void MediaWidget::stateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    Q_UNUSED(oldstate);

    switch (newstate)
    {
    case Phonon::ErrorState:
        if (mediaPlayer.errorType() == Phonon::FatalError)
        {
            ui->pushButtonPlayPause->setEnabled(false);
            hasVideoChanged(false);
            ui->labelInfo->setText(tr("<center>No media</center>"));
        }
        else
            mediaPlayer.pause();

        QMessageBox::warning(this, "softProjector MediaPlayer", mediaPlayer.errorString(), QMessageBox::Close);
        break;

    case Phonon::StoppedState:
        ui->pushButtonPlayPause->setIcon(playIcon);
        ui->pushButtonPlayPause->setEnabled(false);
        break;
    case Phonon::PausedState:
        ui->pushButtonPlayPause->setIcon(playIcon);
        if (mediaPlayer.currentSource().type() != Phonon::MediaSource::Invalid)
            ui->pushButtonPlayPause->setEnabled(true);
        else
            ui->pushButtonPlayPause->setEnabled(false);
        break;
    case Phonon::PlayingState:
        ui->pushButtonPlayPause->setEnabled(true);
        ui->pushButtonPlayPause->setIcon(pauseIcon);

        break;
    case Phonon::BufferingState:

        break;
    case Phonon::LoadingState:

        break;
    }
}

void MediaWidget::handleDrop(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    QStringList fileList;
    QStringList fileNameList;

    // Make sure that only supported files get to media player
    QString fext = audioExt + " " + videoExt;
    fext.remove("*");
    fext.replace(" ","$|");
    fext.append("$");
    QRegExp rx(fext);
    foreach(const QUrl &url, urls)
    {
        QString fp = url.toLocalFile();
        if(fp.contains(rx))
        {
            QFileInfo fn(fp);
            fileList.append(fp);
            fileNameList.append(fn.fileName());
        }
    }

    // Add files to library
    if(fileList.count()>0)
    {
        int mcount = ui->listWidgetMediaFiles->count(); // get total media count
        insertFiles(fileList);
        ui->listWidgetMediaFiles->setCurrentRow(mcount); // select first in list of just added and play it
    }
}

void MediaWidget::dropEvent(QDropEvent *e)
{
    if (e->mimeData()->hasUrls() && e->proposedAction() != Qt::LinkAction) {
        e->acceptProposedAction();
        handleDrop(e);
    } else {
        e->ignore();
    }
}

void MediaWidget::dragEnterEvent(QDragEnterEvent *e)
{
    dragMoveEvent(e);
}

void MediaWidget::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        if (e->proposedAction() == Qt::CopyAction || e->proposedAction() == Qt::MoveAction){
            e->acceptProposedAction();
        }
    }
}

void MediaWidget::playPause()
{
    if (mediaPlayer.state() == Phonon::PlayingState)
        mediaPlayer.pause();
    else
    {
        if (mediaPlayer.currentTime() == mediaPlayer.totalTime())
            mediaPlayer.seek(0);
        mediaPlayer.play();
    }
}

void MediaWidget::playFile(QString filePath)
{
    mediaPlayer.stop();
    mediaPlayer.setCurrentSource(Phonon::MediaSource(filePath));
    mediaPlayer.play();
}

void MediaWidget::updateInfo()
{
    int maxLength = 50;
    QString font = "<font color=#49fff9>";

    QString fileName = mediaPlayer.currentSource().fileName();
    QFileInfo f(fileName);
    QString fName = f.fileName();

    QMap <QString, QString> metaData = mediaPlayer.metaData();
    QString tAlbum = metaData.value("ALBUM");
    QString tTitle = metaData.value("TITLE");
    QString tArtist = metaData.value("ARTIST");
    int tBitrate = metaData.value("BITRATE").toInt();

    if (fName.length() > maxLength)
        fName = fName.left(maxLength) + "...";

    if (tAlbum.length() > maxLength)
        tAlbum = tAlbum.left(maxLength) + "...";

    if (tTitle.length() > maxLength)
        tTitle = tTitle.left(maxLength) + "...";

    if (tArtist.length() > maxLength)
        tArtist = tArtist.left(maxLength) + "...";

    QString file;
    if(!fName.isEmpty())
        file = "File: " + font + fName + "<br></font>";

    QString album;
    if(!tAlbum.isEmpty())
        album = "Album: " + font + tAlbum + "<br></font>";

    QString title;
    if(!tTitle.isEmpty())
    {
        title = "Title: " + font + tTitle + "<br></font>";
        file.clear();
    }

    QString artist;
    if (!tArtist.isEmpty())
        artist = "Artist: " + font + tArtist + "<br></font>";

    QString bitrate;
    if (tBitrate != 0)
        bitrate = "Bitrate: " + font + QString::number(tBitrate/1000) + "kbit</font>";

    ui->labelInfo->setText(file + album + title + artist + bitrate);
}

void MediaWidget::insertFiles(QStringList &files)
{
    // Insert filenames and path into appropriate lists and database
    QSqlQuery sq;
    foreach(const QString &file, files)
    {
        QFileInfo f(file);
        mediaFileNames.append(f.fileName());
        mediaFilePaths.append(file);
        sq.exec(QString("INSERT INTO Media (long_Path, short_path) VALUES('%1', '%2')").arg(file).arg(f.fileName()));
        ui->listWidgetMediaFiles->addItem(f.fileName());
    }
}

void MediaWidget::updateTime()
{
    long len = mediaPlayer.totalTime();
    long pos = mediaPlayer.currentTime();
    QString timeString;
    if (pos || len)
    {
        int sec = pos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = pos;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = len / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = len;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";
        timeString = playTime.toString(timeFormat);
        if (len)
            timeString += " / " + stopTime.toString(timeFormat);
    }
    ui->labelTime->setText(timeString);
}

void MediaWidget::finished()
{

}

void MediaWidget::hasVideoChanged(bool bHasVideo)
{
    if(!bHasVideo && videoWidget->isFullScreen())
        videoWidget->setFullScreen(false);
    ui->labelInfo->setVisible(!bHasVideo);
    ui->pushButtonGoLive->setEnabled(bHasVideo);
    videoWidget->setVisible(bHasVideo);
    ui->comboBoxAspectRatio->setEnabled(bHasVideo);
}

void MediaWidget::prepareForProjection()
{
    mediaPlayer.pause();
    QFileInfo fn( mediaPlayer.currentSource().fileName());
    VideoInfo v;
    v.aspectRatio = ui->comboBoxAspectRatio->currentIndex();
    v.fileName = fn.fileName();
    v.filePath = fn.filePath();
//    qDebug()<<fn.filePath();
    emit toProjector(v);
}

void MediaWidget::on_pushButtonOpen_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Open Music/Video File"),".",tr("Media Files (%1);;Audio Files (%2);;Video Files (%3)")
                                                .arg(audioExt + " " + videoExt) // media files
                                                .arg(audioExt) // audio files
                                                .arg(videoExt)); // video files
    if(!file.isEmpty())
    {
        playFile(file);
        ui->listWidgetMediaFiles->clearSelection();
    }
}

void MediaWidget::on_pushButtonPlayPause_clicked()
{
    playPause();
}

void MediaWidget::on_pushButtonGoLive_clicked()
{
    prepareForProjection();
}

void MediaWidget::on_pushButtonAdd_clicked()
{
    QStringList mfp = QFileDialog::getOpenFileNames(this,tr("Select Music/Video Files to Open"),".",tr("Media Files (%1);;Audio Files (%2);;Video Files (%3)")
                                                    .arg(audioExt + " " + videoExt) // media files
                                                    .arg(audioExt) // audio files
                                                    .arg(videoExt)); // video files

    if(mfp.count()>0)
        insertFiles(mfp);
}

void MediaWidget::on_pushButtonRemove_clicked()
{
    int cm = ui->listWidgetMediaFiles->currentIndex().row();
    if(cm>=0)
    {
        QSqlQuery sq;
        sq.exec("DELETE FROM Media WHERE short_path = '" +mediaFileNames.at(cm)+ "'");
        mediaFilePaths.removeAt(cm);
        mediaFileNames.removeAt(cm);
        ui->listWidgetMediaFiles->clear();
        ui->listWidgetMediaFiles->addItems(mediaFileNames);
        mediaPlayer.stop();
        hasVideoChanged(false);
        ui->labelInfo->setText(tr("<center>No media</center>"));
    }
}

void MediaWidget::on_listWidgetMediaFiles_currentRowChanged(int currentRow)
{
    if(currentRow>=0)
        playFile(mediaFilePaths.at(currentRow));
}

void MediaWidget::on_listWidgetMediaFiles_doubleClicked(const QModelIndex &index)
{
    if(ui->pushButtonGoLive->isEnabled())
        prepareForProjection();
}

void MediaWidget::on_comboBoxAspectRatio_currentIndexChanged(int index)
{
//    if (index == 0)
//        videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatioAuto);
//    else if (index == 1) // aspect Ratio scaled to widget dimensions
//        videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatioWidget);
//    else if (index == 2) // 16 - 9 ratio scale
//        videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatio16_9);
//    else if (index == 3) // 4-3 ratio scale
//        videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatio4_3);
//    else
//        videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatioAuto);
    videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatio(index));
}
