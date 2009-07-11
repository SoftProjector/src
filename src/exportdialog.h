#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class ExportDialog;
}

class ExportDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ExportDialog)
public:
    explicit ExportDialog(QWidget *parent = 0);
    virtual ~ExportDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ExportDialog *ui;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void preload();
    void exportUser();

};

#endif // EXPORTDIALOG_H
