#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class ImportDialog;
}

class ImportDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ImportDialog)
public:
    explicit ImportDialog(QWidget *parent = 0);
    virtual ~ImportDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ImportDialog *ui;
};

#endif // IMPORTDIALOG_H
