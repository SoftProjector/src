#ifndef BIBLEINFORMATIONDIALOG_H
#define BIBLEINFORMATIONDIALOG_H

#include <QDialog>

namespace Ui {
    class BibleInformationDialog;
}

class BibleInformationDialog : public QDialog
{
    Q_OBJECT

public:
    QString title;
    QString abbr;
    QString info;
    bool isRtoL;
    explicit BibleInformationDialog(QWidget *parent = 0);
    ~BibleInformationDialog();

public slots:
    void setBibleIformation(QString title, QString abbreviation, QString info, bool is_rtol);

private:
    Ui::BibleInformationDialog *ui;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // BIBLEINFORMATIONDIALOG_H
