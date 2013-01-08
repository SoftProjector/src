#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

#include <QtGui>
#include <phonon>
#include "videoplayerwidget.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
class QSlider;
class QTextEdit;
class QMenu;
class Ui_settings;
QT_END_NAMESPACE

namespace Ui {
class MediaWidget;
}

class MediaWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MediaWidget(QWidget *parent = 0);
    ~MediaWidget();

    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
    void handleDrop(QDropEvent *e);
    void setFile(const QString &text);
    void setLocation(const QString &location);
//    void initVideoWindow();
    void initSettingsDialog();
    void setVolume(qreal volume);
    void setSmallScreen(bool smallScreen);

public slots:
    void openFile();
    void rewind();
    void forward();
    void updateInfo();
    void updateTime();
    void finished();
    void playPause();
    void scaleChanged(QAction *);
    void aspectChanged(QAction *);

    void on_pushButtonOpen_clicked();
    void on_pushButtonPrev_clicked();
    void on_pushButtonPlayPause_clicked();
    void on_pushButtonNext_clicked();

signals:
    void toProjector(QString vidPath);

private slots:
    void setAspect(int);
    void setScale(int);
    void setSaturation(int);
    void setContrast(int);
    void setHue(int);
    void setBrightness(int);
    void stateChanged(Phonon::State newstate, Phonon::State oldstate);
    void effectChanged();
    void showSettingsDialog();
    void showContextMenu(const QPoint& point);
    void bufferStatus(int percent);
    void openUrl();
    void openRamFile();
    void configureEffect();
    void hasVideoChanged(bool);
    
    void on_pushButtonGoLive_clicked();

private:
    Ui::MediaWidget *ui;

    bool playPauseForDialog();
    QIcon playIcon;
    QIcon pauseIcon;
    QMenu *fileMenu;

    Phonon::SeekSlider *slider;

    Phonon::VolumeSlider *volume;
    QSlider *m_hueSlider;
    QSlider *m_satSlider;
    QSlider *m_contSlider;
    QLabel *info;
    Phonon::Effect *nextEffect;
    QDialog *settingsDialog;
    Ui_settings *uis;
    QAction *m_fullScreenAction;

//    QWidget m_videoWindow;
    Phonon::MediaObject m_MediaObject;
    Phonon::AudioOutput m_AudioOutput;
//    MediaVideoWidget *m_videoWidget;
    VideoPlayerWidget *m_videoWidget;
    Phonon::Path m_audioOutputPath;
    bool m_smallScreen;

};

#endif // MEDIAWIDGET_H
