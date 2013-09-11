#ifndef MODULEPROGRESSDIALOG_H
#define MODULEPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class ModuleProgressDialog;
}

class ModuleProgressDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ModuleProgressDialog(QWidget *parent = 0);
    ~ModuleProgressDialog();

public slots:
    void setCurrentValue(int value);
    void setCurrentMax(int max);
    void setCurrentMin(int min);
    void setCurrent(int value, int max);
    void setTotalValue(int value);
    void setTotalMax(int max);
    void setTotal(int value, int max);
    void increaseTotal();
    void setSpeed(QString speed);
    void appendText(QString text);
    void clearAll();
    void enableCloseButton(bool enable);
    void setToMax();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::ModuleProgressDialog *ui;
};

#endif // MODULEPROGRESSDIALOG_H
