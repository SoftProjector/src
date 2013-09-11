#ifndef MODULEDOWNLOADDIALOG_H
#define MODULEDOWNLOADDIALOG_H

#include <QDialog>

namespace Ui {
class ModuleDownloadDialog;
}

class ModuleDownloadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ModuleDownloadDialog(QWidget *parent = 0);
    ~ModuleDownloadDialog();

public slots:
    void setList(QStringList &modList);
    QList<int> getSelected();
    
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButtonSelectAll_clicked();

    void on_pushButtonDeselectAll_clicked();

private:
    Ui::ModuleDownloadDialog *ui;
};

#endif // MODULEDOWNLOADDIALOG_H
