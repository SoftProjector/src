#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

#include <QtGui>
#include <QtSql>
#include <phonon>
#include "videoplayerwidget.h"
#include "videoinfo.h"

namespace Ui {
class MediaWidget;
}

class MediaWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MediaWidget(QWidget *parent = 0);
    ~MediaWidget();
protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);

signals:
//    void toProjector(QString vidPath, int aspectRatio);
    void toProjector(VideoInfo &vid);

private slots:
    void playFile(QString filePath);
    void updateInfo();
    void updateTime();
    void finished();
    void playPause();


    void handleDrop(QDropEvent *e);
    void loadMediaLibrary();
    void stateChanged(Phonon::State newstate, Phonon::State oldstate);

    void hasVideoChanged(bool);
    void insertFiles(QStringList &files);
    void prepareForProjection();

    void on_pushButtonOpen_clicked();
    void on_pushButtonPlayPause_clicked();
    void on_pushButtonGoLive_clicked();
    void on_pushButtonAdd_clicked();
    void on_pushButtonRemove_clicked();
    void on_listWidgetMediaFiles_currentRowChanged(int currentRow);
    void on_listWidgetMediaFiles_doubleClicked(const QModelIndex &index);
    void on_comboBoxAspectRatio_currentIndexChanged(int index);

private:
    Ui::MediaWidget *ui;

    QIcon playIcon;
    QIcon pauseIcon;

    Phonon::SeekSlider *timeSlider;

    Phonon::VolumeSlider *volumeSlider;

    Phonon::MediaObject mediaPlayer;
    Phonon::AudioOutput m_AudioOutput;

    VideoPlayerWidget *videoWidget;
//    Phonon::Path m_audioOutputPath;

    QString audioExt;
    QString videoExt;

    QStringList mediaFilePaths;
    QStringList mediaFileNames;

};

#endif // MEDIAWIDGET_H
