#include "editannouncementdialog.h"
#include "ui_editannouncementdialog.h"

EditAnnouncementDialog::EditAnnouncementDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditAnnouncementDialog)
{
    ui->setupUi(this);
    highlight = new HighlightAnnounce(ui->textEditAnnouncement->document());
    ui->groupBoxPrivateSettings->setVisible(false);
    ui->checkBoxUsePrivateSettings->setVisible(false);
}

EditAnnouncementDialog::~EditAnnouncementDialog()
{
    delete highlight;
    delete ui;
}

void EditAnnouncementDialog::on_checkBoxUsePrivateSettings_stateChanged(int arg1)
{
    ui->groupBoxPrivateSettings->setVisible(arg1 == 2);
}

void EditAnnouncementDialog::on_checkBoxTimedSlides_stateChanged(int arg1)
{
    ui->spinBoxTimeOut->setEnabled(arg1 == 2);
    ui->checkBoxLoop->setEnabled(arg1 == 2);
}

void EditAnnouncementDialog::setNewAnnouce()
{
//    ui->checkBoxLoop->setChecked(false);
//    ui->checkBoxUsePrivateSettings->setChecked(false);
//    ui->groupBoxPrivateSettings->setVisible(false);
    resetUiItems();
    isNew = true;
}

void EditAnnouncementDialog::setEditAnnouce(Announcement &editAnnoucement)
{
    editAnnounce = editAnnoucement;
    setUiItems();
    isNew = false;
}

void EditAnnouncementDialog::setCopyAnnouce(Announcement &copyAnnoucement)
{
    editAnnounce = copyAnnoucement;
    editAnnounce.idNum = 0;
    setUiItems();
    isNew = true;
}

void EditAnnouncementDialog::setUiItems()
{
    ui->lineEditTitle->setText(editAnnounce.title);
    ui->labelIdNum->setText(QString::number(editAnnounce.idNum));
    ui->textEditAnnouncement->setPlainText(editAnnounce.text);
    ui->checkBoxUsePrivateSettings->setChecked(editAnnounce.usePrivateSettings);
    ui->checkBoxTimedSlides->setChecked(editAnnounce.useAutoNext);
    ui->spinBoxTimeOut->setValue(editAnnounce.slideTimer);
    ui->checkBoxLoop->setChecked(editAnnounce.loop);
}

void EditAnnouncementDialog::resetUiItems()
{
    Announcement a;
    a.text = tr("Announce\n - Text of the announcement goes here\n\n"
                "Slide\n - Text of the announcement goes here\n"
                "You can have both Annouce or Slide as announcement block titles.");
    ui->lineEditTitle->setText(a.title);
    ui->labelIdNum->setText(QString::number(a.idNum));
    ui->textEditAnnouncement->setPlainText(a.text);
    ui->checkBoxUsePrivateSettings->setChecked(a.usePrivateSettings);
    ui->checkBoxTimedSlides->setChecked(a.useAutoNext);
    ui->spinBoxTimeOut->setValue(a.slideTimer);
    ui->checkBoxLoop->setChecked(a.loop);
}

bool EditAnnouncementDialog::setSave()
{
    QString t = ui->lineEditTitle->text();
    t = t.simplified();
    if(!t.isEmpty())
    {
        editAnnounce.title = t;
        editAnnounce.text = ui->textEditAnnouncement->toPlainText();
        editAnnounce.usePrivateSettings = ui->checkBoxUsePrivateSettings->isChecked();
        editAnnounce.useAutoNext = ui->checkBoxTimedSlides->isChecked();
        editAnnounce.slideTimer = ui->spinBoxTimeOut->value();
        editAnnounce.loop = ui->checkBoxLoop->isChecked();
        return true;
    }
    else
    {
        QMessageBox mb(this);
        mb.setText(tr("Announcement title cannot be left empty.\nPlease enter announcement title."));
        mb.setWindowTitle(tr("Announcement title is missing"));
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
        ui->lineEditTitle->setFocus();
        return false;
    }
}

//void EditAnnouncementDialog::on_buttonBox_accepted()
//{
//    if(!setSave())
//        return;

//    if(isNew)
//    {
//        editAnnounce.saveNew();
//        emit announceToAdd(editAnnounce);
//    }
//    else
//        editAnnounce.saveUpdate();
//}

void EditAnnouncementDialog::on_pushButtonSave_clicked()
{
    if(!setSave())
        return;

    if(isNew)
    {
        editAnnounce.saveNew();
        emit announceToAdd(editAnnounce);
    }
    else
    {
        editAnnounce.saveUpdate();
        emit announceToUpdate();
    }

    close();
}

void EditAnnouncementDialog::on_pushButtonCancel_clicked()
{
    close();
}
