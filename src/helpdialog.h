#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QtGui/QDialog>
#include <QtGui>

namespace Ui {
    class HelpDialog;
}

class HelpDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(HelpDialog)
public:
    explicit HelpDialog(QWidget *parent = 0);
    virtual ~HelpDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::HelpDialog *ui;

private slots:
    void on_close_pushButton_clicked();
};

#endif // HELPDIALOG_H
