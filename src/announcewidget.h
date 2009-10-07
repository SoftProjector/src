#ifndef ANNOUNCEWIDGET_H
#define ANNOUNCEWIDGET_H

#include <QtGui/QWidget>
//#include "softprojector.h"

namespace Ui {
    class AnnounceWidget;
}

class AnnounceWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(AnnounceWidget)
public:
    explicit AnnounceWidget(QWidget *parent = 0);
    virtual ~AnnounceWidget();
    QString getText();

protected:
    virtual void changeEvent(QEvent *e);

private slots:
    void on_btnLive_clicked();
    void sendToProjector();

signals:
    // To be used ONLY by SongWidget::sendToProjector():
    void sendText(QString text);

private:
    Ui::AnnounceWidget *ui;
public slots:
};

#endif // ANNOUNCEWIDGET_H
