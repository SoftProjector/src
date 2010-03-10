#ifndef ADDSONGBOOKDIALOG_H
#define ADDSONGBOOKDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class AddSongbookDialog;
}

class AddSongbookDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(AddSongbookDialog)
public:
    QString title;
    QString info;
    explicit AddSongbookDialog(QWidget *parent = 0);
    virtual ~AddSongbookDialog();

public slots:
    void setSongbook(QString title, QString info);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::AddSongbookDialog *ui;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // ADDSONGBOOKDIALOG_H
