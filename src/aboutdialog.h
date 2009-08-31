#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui/QDialog>
#include "softprojector.h"

namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(AboutDialog)
public:
    explicit AboutDialog(QWidget *parent = 0);
    virtual ~AboutDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::AboutDialog *ui;
    SoftProjector *softProjector;

private slots:
    void on_pushButton_clicked();
};

#endif // ABOUTDIALOG_H
