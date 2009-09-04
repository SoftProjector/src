#ifndef ADDSBORNIKDIALOG_H
#define ADDSBORNIKDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class AddSbornikDialog;
}

class AddSbornikDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(AddSbornikDialog)
public:
    QString code;
    QString title;
    QString info;
    explicit AddSbornikDialog(QWidget *parent = 0);
    virtual ~AddSbornikDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::AddSbornikDialog *ui;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // ADDSBORNIKDIALOG_H
