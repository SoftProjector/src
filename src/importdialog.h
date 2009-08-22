#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QFileDialog>

namespace Ui {
    class ImportDialog;
}

class ImportDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(ImportDialog)
public:
    explicit ImportDialog(QWidget *parent = 0);
    virtual ~ImportDialog();
    void load();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::ImportDialog *ui;
    QString file_path;
    void load_file(QString path);
    bool has_info;


private slots:
    void on_laod_file_button_clicked();
    void on_buttonBox_accepted();
};

#endif // IMPORTDIALOG_H
