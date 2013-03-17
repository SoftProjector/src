#ifndef EDITANNOUNCEMENTDIALOG_H
#define EDITANNOUNCEMENTDIALOG_H

#include <QDialog>
#include <QDebug>
#include "announcement.h"
#include "highlight.h"

namespace Ui {
class EditAnnouncementDialog;
}

class EditAnnouncementDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditAnnouncementDialog(QWidget *parent = 0);
    ~EditAnnouncementDialog();

public slots:
    void setNewAnnouce();
    void setEditAnnouce(Announcement &editAnnoucement);
    void setCopyAnnouce(Announcement &copyAnnoucement);

signals:
    void announceToAdd(Announcement announce);

    
private slots:
    void on_checkBoxUsePrivateSettings_stateChanged(int arg1);
    void on_checkBoxTimedSlides_stateChanged(int arg1);

    void resetUiItems();
    void setUiItems();
    void setSave();


    void on_buttonBox_accepted();

private:
    Ui::EditAnnouncementDialog *ui;
    Announcement editAnnounce;
    bool isNew;
    HighlightAnnounce * highlight;
};

#endif // EDITANNOUNCEMENTDIALOG_H
