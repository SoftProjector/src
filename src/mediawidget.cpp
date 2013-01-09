#include "mediawidget.h"
#include "ui_mediawidget.h"
#include "ui_mpsettings.h"

#define SLIDER_RANGE 8

MediaWidget::MediaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaWidget),
     nextEffect(0), settingsDialog(0), uis(0),
                m_AudioOutput(Phonon::VideoCategory)
{
    ui->setupUi(this);
    m_videoWidget = new VideoPlayerWidget(ui->widget);
    ui->widget->layout()->addWidget(m_videoWidget);
    ui->widget->setVisible(false);
    ui->pushButtonGoLive->setEnabled(false);


    setContextMenuPolicy(Qt::CustomContextMenu);
        m_videoWidget->setContextMenuPolicy(Qt::CustomContextMenu);

        QSize buttonSize(34, 28);

        ui->pushButtonOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
        QPalette bpal;
        QColor arrowcolor = bpal.buttonText().color();
        if (arrowcolor == Qt::black)
            arrowcolor = QColor(80, 80, 80);
        bpal.setBrush(QPalette::ButtonText, arrowcolor);
        ui->pushButtonOpen->setPalette(bpal);

        ui->pushButtonPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

        ui->pushButtonNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
        ui->pushButtonNext->setEnabled(false);

        playIcon = style()->standardIcon(QStyle::SP_MediaPlay);
        pauseIcon = style()->standardIcon(QStyle::SP_MediaPause);
        ui->pushButtonPlayPause->setIcon(playIcon);

        slider = new Phonon::SeekSlider(this);

        slider->setMediaObject(&m_MediaObject);
        volume = new Phonon::VolumeSlider(&m_AudioOutput);


        QPalette palette;
        palette.setBrush(QPalette::WindowText, Qt::white);
    #ifndef Q_WS_MAC
        ui->pushButtonOpen->setMinimumSize(54, buttonSize.height());
        ui->pushButtonPrev->setMinimumSize(buttonSize);
        ui->pushButtonNext->setMinimumSize(buttonSize);
        ui->pushButtonPlayPause->setMinimumSize(buttonSize);
    #endif
        ui->labelInfo->setStyleSheet("border-image:url(:icons/icons/screen.png) ; border-width:3px");
        ui->labelInfo->setPalette(palette);
        ui->labelInfo->setText(tr("<center>No media</center>"));

        volume->setFixedWidth(120);

        ui->horizontalLayoutControls->addWidget(volume);

        ui->horizontalLayoutTime->insertWidget(0,slider);

    #ifdef Q_OS_MAC

        // QStyle *flatButtonStyle = new QWindowsStyle;
        ui->pushButtonOpen->setFocusPolicy(Qt::NoFocus);
        // openButton->setStyle(flatButtonStyle);
        // playButton->setStyle(flatButtonStyle);
        // rewindButton->setStyle(flatButtonStyle);
        // forwardButton->setStyle(flatButtonStyle);
     #endif


        // Create menu bar:
        fileMenu = new QMenu(this);

//        fileMenu->addSeparator();
        QMenu *aspectMenu = fileMenu->addMenu(tr("&Aspect ratio"));
        QActionGroup *aspectGroup = new QActionGroup(aspectMenu);
        connect(aspectGroup, SIGNAL(triggered(QAction*)), this, SLOT(aspectChanged(QAction*)));
        aspectGroup->setExclusive(true);
        QAction *aspectActionAuto = aspectMenu->addAction(tr("Auto"));
        aspectActionAuto->setCheckable(true);
        aspectActionAuto->setChecked(true);
        aspectGroup->addAction(aspectActionAuto);
        QAction *aspectActionScale = aspectMenu->addAction(tr("Scale"));
        aspectActionScale->setCheckable(true);
        aspectGroup->addAction(aspectActionScale);
        QAction *aspectAction16_9 = aspectMenu->addAction(tr("16/9"));
        aspectAction16_9->setCheckable(true);
        aspectGroup->addAction(aspectAction16_9);
        QAction *aspectAction4_3 = aspectMenu->addAction(tr("4/3"));
        aspectAction4_3->setCheckable(true);
        aspectGroup->addAction(aspectAction4_3);

        m_fullScreenAction = fileMenu->addAction(tr("Full screen video"));
        m_fullScreenAction->setCheckable(true);
        m_fullScreenAction->setEnabled(false); // enabled by hasVideoChanged
        bool b = connect(m_fullScreenAction, SIGNAL(toggled(bool)), m_videoWidget, SLOT(setFullScreen(bool)));
        Q_ASSERT(b);
        b = connect(m_videoWidget, SIGNAL(fullScreenChanged(bool)), m_fullScreenAction, SLOT(setChecked(bool)));
        Q_ASSERT(b);

        fileMenu->addSeparator();
        QAction *settingsAction = fileMenu->addAction(tr("&Settings..."));

        // Setup signal connections:

        connect(settingsAction, SIGNAL(triggered(bool)), this, SLOT(showSettingsDialog()));


        connect(m_videoWidget, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
        connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), SLOT(showContextMenu(const QPoint &)));
        connect(&m_MediaObject, SIGNAL(metaDataChanged()), this, SLOT(updateInfo()));
        connect(&m_MediaObject, SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTime()));
        connect(&m_MediaObject, SIGNAL(tick(qint64)), this, SLOT(updateTime()));
        connect(&m_MediaObject, SIGNAL(finished()), this, SLOT(finished()));
        connect(&m_MediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
        connect(&m_MediaObject, SIGNAL(bufferStatus(int)), this, SLOT(bufferStatus(int)));
        connect(&m_MediaObject, SIGNAL(hasVideoChanged(bool)), this, SLOT(hasVideoChanged(bool)));

        ui->pushButtonPrev->setEnabled(false);
        ui->pushButtonPlayPause->setEnabled(false);
        setAcceptDrops(true);

        m_audioOutputPath = Phonon::createPath(&m_MediaObject, &m_AudioOutput);
        Phonon::createPath(&m_MediaObject, m_videoWidget);

        resize(minimumSizeHint());
}

MediaWidget::~MediaWidget()
{
    delete ui;
}


void MediaWidget::stateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    Q_UNUSED(oldstate);

//    if (oldstate == Phonon::LoadingState) {
////        QRect videoHintRect = QRect(QPoint(0, 0), m_videoWindow.sizeHint());
////        QRect newVideoRect = QApplication::desktop()->screenGeometry().intersected(videoHintRect);
//        if (!m_smallScreen) {
//            if (m_MediaObject.hasVideo()) {
//                // Flush event que so that sizeHint takes the
//                // recently shown/hidden m_videoWindow into account:
////                qApp->processEvents();
////                resize(sizeHint());
//                qDebug()<<"resixe hint";
//            } else
//                qDebug()<<"resixe ninimum";
////                resize(minimumSize());
//        }
//    }

    switch (newstate) {
        case Phonon::ErrorState:
            if (m_MediaObject.errorType() == Phonon::FatalError) {
                ui->pushButtonPlayPause->setEnabled(false);
                ui->pushButtonPrev->setEnabled(false);
            } else {
                m_MediaObject.pause();
            }
            QMessageBox::warning(this, "Phonon Mediaplayer", m_MediaObject.errorString(), QMessageBox::Close);
            break;

        case Phonon::StoppedState:
//            m_videoWidget->setFullScreen(false);
            // Fall through
        case Phonon::PausedState:
            ui->pushButtonPlayPause->setIcon(playIcon);
            if (m_MediaObject.currentSource().type() != Phonon::MediaSource::Invalid){
                ui->pushButtonPlayPause->setEnabled(true);
                ui->pushButtonPrev->setEnabled(true);
            } else {
                ui->pushButtonPlayPause->setEnabled(false);
                ui->pushButtonPrev->setEnabled(false);
            }
            break;
        case Phonon::PlayingState:
            ui->pushButtonPlayPause->setEnabled(true);
            ui->pushButtonPlayPause->setIcon(pauseIcon);

            // Fall through
        case Phonon::BufferingState:
            ui->pushButtonPrev->setEnabled(true);
            break;
        case Phonon::LoadingState:
            ui->pushButtonPrev->setEnabled(false);
            m_AudioOutput.setMuted(true);
            break;
    }

}

void MediaWidget::initSettingsDialog()
{
    settingsDialog = new QDialog(this);
    uis = new Ui_settings();
    uis->setupUi(settingsDialog);

    connect(uis->brightnessSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)));
    connect(uis->hueSlider, SIGNAL(valueChanged(int)), this, SLOT(setHue(int)));
    connect(uis->saturationSlider, SIGNAL(valueChanged(int)), this, SLOT(setSaturation(int)));
    connect(uis->contrastSlider , SIGNAL(valueChanged(int)), this, SLOT(setContrast(int)));
    connect(uis->aspectCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setAspect(int)));
    connect(uis->scalemodeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(setScale(int)));

    uis->brightnessSlider->setValue(int(m_videoWidget->brightness() * SLIDER_RANGE));
    uis->hueSlider->setValue(int(m_videoWidget->hue() * SLIDER_RANGE));
    uis->saturationSlider->setValue(int(m_videoWidget->saturation() * SLIDER_RANGE));
    uis->contrastSlider->setValue(int(m_videoWidget->contrast() * SLIDER_RANGE));
    uis->aspectCombo->setCurrentIndex(m_videoWidget->aspectRatio());
    uis->scalemodeCombo->setCurrentIndex(m_videoWidget->scaleMode());
    connect(uis->effectButton, SIGNAL(clicked()), this, SLOT(configureEffect()));

#ifdef Q_WS_X11
    //Cross fading is not currently implemented in the GStreamer backend
    uis->crossFadeSlider->setVisible(false);
    uis->crossFadeLabel->setVisible(false);
    uis->crossFadeLabel1->setVisible(false);
    uis->crossFadeLabel2->setVisible(false);
    uis->crossFadeLabel3->setVisible(false);
#endif
    uis->crossFadeSlider->setValue((int)(2 * m_MediaObject.transitionTime() / 1000.0f));

    // Insert audio devices:
    QList<Phonon::AudioOutputDevice> devices = Phonon::BackendCapabilities::availableAudioOutputDevices();
    for (int i=0; i<devices.size(); i++){
        QString itemText = devices[i].name();
        if (!devices[i].description().isEmpty()) {
            itemText += QString::fromLatin1(" (%1)").arg(devices[i].description());
        }
        uis->deviceCombo->addItem(itemText);
        if (devices[i] == m_AudioOutput.outputDevice())
            uis->deviceCombo->setCurrentIndex(i);
    }

    // Insert audio effects:
    uis->audioEffectsCombo->addItem(tr("<no effect>"));
    QList<Phonon::Effect *> currEffects = m_audioOutputPath.effects();
    Phonon::Effect *currEffect = currEffects.size() ? currEffects[0] : 0;
    QList<Phonon::EffectDescription> availableEffects = Phonon::BackendCapabilities::availableAudioEffects();
    for (int i=0; i<availableEffects.size(); i++){
        uis->audioEffectsCombo->addItem(availableEffects[i].name());
        if (currEffect && availableEffects[i] == currEffect->description())
            uis->audioEffectsCombo->setCurrentIndex(i+1);
    }
    connect(uis->audioEffectsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(effectChanged()));

}

void MediaWidget::setVolume(qreal volume)
{
    m_AudioOutput.setVolume(volume);
}

void MediaWidget::setSmallScreen(bool smallScreen)
{
    m_smallScreen = smallScreen;
}

void MediaWidget::effectChanged()
{
    int currentIndex = uis->audioEffectsCombo->currentIndex();
    if (currentIndex) {
        QList<Phonon::EffectDescription> availableEffects = Phonon::BackendCapabilities::availableAudioEffects();
        Phonon::EffectDescription chosenEffect = availableEffects[currentIndex - 1];

        QList<Phonon::Effect *> currEffects = m_audioOutputPath.effects();
        Phonon::Effect *currentEffect = currEffects.size() ? currEffects[0] : 0;

        // Deleting the running effect will stop playback, it is deleted when removed from path
        if (nextEffect && !(currentEffect && (currentEffect->description().name() == nextEffect->description().name())))
            delete nextEffect;

        nextEffect = new Phonon::Effect(chosenEffect);
    }
    uis->effectButton->setEnabled(currentIndex);
}

void MediaWidget::showSettingsDialog()
{
    const bool hasPausedForDialog = playPauseForDialog();

    if (!settingsDialog)
        initSettingsDialog();

    float oldBrightness = m_videoWidget->brightness();
    float oldHue = m_videoWidget->hue();
    float oldSaturation = m_videoWidget->saturation();
    float oldContrast = m_videoWidget->contrast();
    Phonon::VideoWidget::AspectRatio oldAspect = m_videoWidget->aspectRatio();
    Phonon::VideoWidget::ScaleMode oldScale = m_videoWidget->scaleMode();
    int currentEffect = uis->audioEffectsCombo->currentIndex();
    settingsDialog->exec();

    if (settingsDialog->result() == QDialog::Accepted){
        m_MediaObject.setTransitionTime((int)(1000 * float(uis->crossFadeSlider->value()) / 2.0f));
        QList<Phonon::AudioOutputDevice> devices = Phonon::BackendCapabilities::availableAudioOutputDevices();
        m_AudioOutput.setOutputDevice(devices[uis->deviceCombo->currentIndex()]);
        QList<Phonon::Effect *> currEffects = m_audioOutputPath.effects();
        QList<Phonon::EffectDescription> availableEffects = Phonon::BackendCapabilities::availableAudioEffects();

        if (uis->audioEffectsCombo->currentIndex() > 0){
            Phonon::Effect *currentEffect = currEffects.size() ? currEffects[0] : 0;
            if (!currentEffect || currentEffect->description() != nextEffect->description()){
                foreach(Phonon::Effect *effect, currEffects) {
                    m_audioOutputPath.removeEffect(effect);
                    delete effect;
                }
                m_audioOutputPath.insertEffect(nextEffect);
            }
        } else {
            foreach(Phonon::Effect *effect, currEffects) {
                m_audioOutputPath.removeEffect(effect);
                delete effect;
                nextEffect = 0;
            }
        }
    } else {
        // Restore previous settings
        m_videoWidget->setBrightness(oldBrightness);
        m_videoWidget->setSaturation(oldSaturation);
        m_videoWidget->setHue(oldHue);
        m_videoWidget->setContrast(oldContrast);
        m_videoWidget->setAspectRatio(oldAspect);
        m_videoWidget->setScaleMode(oldScale);
        uis->audioEffectsCombo->setCurrentIndex(currentEffect);
    }

    if (hasPausedForDialog)
        m_MediaObject.play();
}

void MediaWidget::configureEffect()
{
    if (!nextEffect)
        return;


    QList<Phonon::Effect *> currEffects = m_audioOutputPath.effects();
    const QList<Phonon::EffectDescription> availableEffects = Phonon::BackendCapabilities::availableAudioEffects();
    if (uis->audioEffectsCombo->currentIndex() > 0) {
        Phonon::EffectDescription chosenEffect = availableEffects[uis->audioEffectsCombo->currentIndex() - 1];

        QDialog effectDialog;
        effectDialog.setWindowTitle(tr("Configure effect"));
        QVBoxLayout *topLayout = new QVBoxLayout(&effectDialog);

        QLabel *description = new QLabel("<b>Description:</b><br>" + chosenEffect.description(), &effectDialog);
        description->setWordWrap(true);
        topLayout->addWidget(description);

        QScrollArea *scrollArea = new QScrollArea(&effectDialog);
        topLayout->addWidget(scrollArea);

        QVariantList savedParamValues;
        foreach(Phonon::EffectParameter param, nextEffect->parameters()) {
            savedParamValues << nextEffect->parameterValue(param);
        }

        QWidget *scrollWidget = new Phonon::EffectWidget(nextEffect);
        scrollWidget->setMinimumWidth(320);
        scrollWidget->setContentsMargins(10, 10, 10,10);
        scrollArea->setWidget(scrollWidget);

        QDialogButtonBox *bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &effectDialog);
        connect(bbox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), &effectDialog, SLOT(accept()));
        connect(bbox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), &effectDialog, SLOT(reject()));
        topLayout->addWidget(bbox);

        effectDialog.exec();

        if (effectDialog.result() != QDialog::Accepted) {
            //we need to restore the parameters values
            int currentIndex = 0;
            foreach(Phonon::EffectParameter param, nextEffect->parameters()) {
                nextEffect->setParameterValue(param, savedParamValues.at(currentIndex++));
            }

        }
    }
}

void MediaWidget::handleDrop(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if (e->proposedAction() == Qt::MoveAction){
        // Just add to the queue:
        for (int i=0; i<urls.size(); i++)
            m_MediaObject.enqueue(Phonon::MediaSource(urls[i].toLocalFile()));
    } else {
        // Create new queue:
        m_MediaObject.clearQueue();
        if (urls.size() > 0) {
            QString fileName = urls[0].toLocalFile();
            QDir dir(fileName);
            if (dir.exists()) {
                dir.setFilter(QDir::Files);
                QStringList entries = dir.entryList();
                if (entries.size() > 0) {
                    setFile(fileName + QDir::separator() +  entries[0]);
                    for (int i=1; i< entries.size(); ++i)
                        m_MediaObject.enqueue(fileName + QDir::separator() + entries[i]);
                }
            } else {
                setFile(fileName);
                for (int i=1; i<urls.size(); i++)
                    m_MediaObject.enqueue(Phonon::MediaSource(urls[i].toLocalFile()));
            }
        }
    }
    ui->pushButtonNext->setEnabled(m_MediaObject.queue().size() > 0);
    m_MediaObject.play();
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
    if (m_MediaObject.state() == Phonon::PlayingState)
        m_MediaObject.pause();
    else {
        if (m_MediaObject.currentTime() == m_MediaObject.totalTime())
            m_MediaObject.seek(0);
        m_MediaObject.play();
    }
}

void MediaWidget::setFile(const QString &fileName)
{
    setWindowTitle(fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1));
    m_MediaObject.setCurrentSource(Phonon::MediaSource(fileName));
    m_MediaObject.play();
}

void MediaWidget::setLocation(const QString& location)
{
    setWindowTitle(location.right(location.length() - location.lastIndexOf('/') - 1));
    m_MediaObject.setCurrentSource(Phonon::MediaSource(QUrl::fromEncoded(location.toUtf8())));
    m_MediaObject.play();
}

bool MediaWidget::playPauseForDialog()
{
    // If we're running on a small screen, we want to pause the video when
    // popping up dialogs. We neither want to tamper with the state if the
    // user has paused.
    if (m_smallScreen && m_MediaObject.hasVideo()) {
        if (Phonon::PlayingState == m_MediaObject.state()) {
            m_MediaObject.pause();
            return true;
        }
    }
    return false;
}

void MediaWidget::openFile()
{
    const bool hasPausedForDialog = playPauseForDialog();

    QStringList fileNames = QFileDialog::getOpenFileNames(this, QString(),
                                                          QDesktopServices::storageLocation(QDesktopServices::MusicLocation));

    if (hasPausedForDialog)
        m_MediaObject.play();

    m_MediaObject.clearQueue();
    if (fileNames.size() > 0) {
        QString fileName = fileNames[0];
        setFile(fileName);
        for (int i=1; i<fileNames.size(); i++)
            m_MediaObject.enqueue(Phonon::MediaSource(fileNames[i]));
    }
    ui->pushButtonNext->setEnabled(m_MediaObject.queue().size() > 0);
}

void MediaWidget::bufferStatus(int percent)
{
    if (percent == 100)
        ui->labelProg->setText(QString());
    else {
        QString str = QString::fromLatin1("(%1%)").arg(percent);
        ui->labelProg->setText(str);
    }
}

void MediaWidget::setSaturation(int val)
{
    m_videoWidget->setSaturation(val / qreal(SLIDER_RANGE));
}

void MediaWidget::setHue(int val)
{
    m_videoWidget->setHue(val / qreal(SLIDER_RANGE));
}

void MediaWidget::setAspect(int val)
{
    m_videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatio(val));
}

void MediaWidget::setScale(int val)
{
    m_videoWidget->setScaleMode(Phonon::VideoWidget::ScaleMode(val));
}

void MediaWidget::setBrightness(int val)
{
    m_videoWidget->setBrightness(val / qreal(SLIDER_RANGE));
}

void MediaWidget::setContrast(int val)
{
    m_videoWidget->setContrast(val / qreal(SLIDER_RANGE));
}

void MediaWidget::updateInfo()
{
    int maxLength = 30;
    QString font = "<font color=#ffeeaa>";
    QString fontmono = "<font family=\"monospace,courier new\" color=#ffeeaa>";

    QMap <QString, QString> metaData = m_MediaObject.metaData();
    QString trackArtist = metaData.value("ARTIST");
    if (trackArtist.length() > maxLength)
        trackArtist = trackArtist.left(maxLength) + "...";

    QString trackTitle = metaData.value("TITLE");
    int trackBitrate = metaData.value("BITRATE").toInt();

    QString fileName;
    if (m_MediaObject.currentSource().type() == Phonon::MediaSource::Url) {
        fileName = m_MediaObject.currentSource().url().toString();
    } else {
        fileName = m_MediaObject.currentSource().fileName();
        fileName = fileName.right(fileName.length() - fileName.lastIndexOf('/') - 1);
        if (fileName.length() > maxLength)
            fileName = fileName.left(maxLength) + "...";
    }

    QString title;
    if (!trackTitle.isEmpty()) {
        if (trackTitle.length() > maxLength)
            trackTitle = trackTitle.left(maxLength) + "...";
        title = "Title: " + font + trackTitle + "<br></font>";
    } else if (!fileName.isEmpty()) {
        if (fileName.length() > maxLength)
            fileName = fileName.left(maxLength) + "...";
        title = font + fileName + "</font>";
        if (m_MediaObject.currentSource().type() == Phonon::MediaSource::Url) {
            title.prepend("Url: ");
        } else {
            title.prepend("File: ");
        }
    }

    QString artist;
    if (!trackArtist.isEmpty())
        artist = "Artist:  " + font + trackArtist + "</font>";

    QString bitrate;
    if (trackBitrate != 0)
        bitrate = "<br>Bitrate:  " + font + QString::number(trackBitrate/1000) + "kbit</font>";

    ui->labelInfo->setText(title + artist + bitrate);
}

void MediaWidget::updateTime()
{
    long len = m_MediaObject.totalTime();
    long pos = m_MediaObject.currentTime();
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

void MediaWidget::rewind()
{
    m_MediaObject.seek(0);
}

void MediaWidget::forward()
{
    QList<Phonon::MediaSource> queue = m_MediaObject.queue();
    if (queue.size() > 0) {
        m_MediaObject.setCurrentSource(queue[0]);
        ui->pushButtonNext->setEnabled(queue.size() > 1);
        m_MediaObject.play();
    }
}

void MediaWidget::openUrl()
{

}

void MediaWidget::openRamFile()
{


}

void MediaWidget::finished()
{
    if(ui->checkBoxLoop->isChecked())
    {
//        m_MediaObject.seek(0);
//        m_MediaObject.play();
//        qDebug()<<"playPause()";
//        playPause();
//        playPause();
    }
}

void MediaWidget::showContextMenu(const QPoint &p)
{
    fileMenu->popup(m_videoWidget->isFullScreen() ? p : mapToGlobal(p));
}

void MediaWidget::scaleChanged(QAction *act)
{
    if (act->text() == tr("Scale and crop"))
        m_videoWidget->setScaleMode(Phonon::VideoWidget::ScaleAndCrop);
    else
        m_videoWidget->setScaleMode(Phonon::VideoWidget::FitInView);
}

void MediaWidget::aspectChanged(QAction *act)
{
    if (act->text() == tr("16/9"))
        m_videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatio16_9);
    else if (act->text() == tr("Scale"))
        m_videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatioWidget);
    else if (act->text() == tr("4/3"))
        m_videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatio4_3);
    else
        m_videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatioAuto);
}

void MediaWidget::hasVideoChanged(bool bHasVideo)
{
    ui->labelInfo->setVisible(!bHasVideo);
    ui->pushButtonGoLive->setEnabled(bHasVideo);
//    m_videoWindow.setVisible(bHasVideo);
//    m_videoWidget->setVisible(bHasVideo);
    ui->widget->setVisible(bHasVideo);
    qDebug()<<"bHasVideo:"<<bHasVideo;
//    m_fullScreenAction->setEnabled(bHasVideo);
}


void MediaWidget::on_pushButtonOpen_clicked()
{
    openFile();
}

void MediaWidget::on_pushButtonPrev_clicked()
{
    rewind();
}

void MediaWidget::on_pushButtonPlayPause_clicked()
{
    playPause();
}

void MediaWidget::on_pushButtonNext_clicked()
{
    forward();
}

void MediaWidget::on_pushButtonGoLive_clicked()
{
    m_MediaObject.pause();
    QString f = m_MediaObject.currentSource().fileName();
    emit toProjector(f);
}
