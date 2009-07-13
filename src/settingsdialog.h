
#include <QtGui/QDialog>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SettingsDialog)
public:
    explicit SettingsDialog(QWidget *parent = 0);
    virtual ~SettingsDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::SettingsDialog *ui;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

};

