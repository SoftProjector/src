#include "editwidget.h"
#include "ui_editwidget.h"

EditWidget::EditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWidget)
{
    ui->setupUi(this);
    loadTitles("ALL");
}

EditWidget::~EditWidget()
{
    delete ui;
}

void EditWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void EditWidget::on_btnEdit_clicked()
{
    ui->btnEdit->setEnabled(false);
    ui->lblEditProg->show();
}

void EditWidget::on_btnNew_clicked()
{

}

void EditWidget::on_btnSave_clicked()
{

}

void EditWidget::on_btnCancel_clicked()
{
    ui->btnEdit->setEnabled(true);
    ui->btnNew->setEnabled(true);
    ui->lblEditProg->hide();
//    resetItems();
}

void EditWidget::on_btnClose_clicked()
{
    close();
}

void EditWidget::on_btnSortTitles_clicked()
{
//    if (!titleType==2){
    titleType=2;
    Song t;
    ui->listTitles->clear();
    ui->listTitles->addItems(t.getTitle());
    ui->comboBoxSbornik->setCurrentIndex(-1);
//    ui->spinBoxSongNum->setEnabled(false);
    sbornik=" ";
    //    }
//    bool empty = ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).isEmpty();
//    if (!empty) ui->listTitles->setCurrentItem(ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).takeFirst());
//    ui->listTitles->scrollTo(ui->listTitles->currentIndex(),QAbstractItemView::PositionAtCenter);
}

void EditWidget::on_spinBoxSongNum_valueChanged(int value)
{
    bool empty = ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).isEmpty();
    if (!empty) ui->listTitles->setCurrentItem(ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).takeFirst());
    ui->listTitles->scrollTo(ui->listTitles->currentIndex(),QAbstractItemView::PositionAtCenter);
}

void EditWidget::loadTitles(QString tSbornik)
{
    Song t;
    ui->listTitles->clear();
    if (tSbornik == "ALL") ui->listTitles->addItems(t.getTitle());
    else ui->listTitles->addItems(t.getTitle(tSbornik));
}

QString EditWidget::setSongText(QString song)
{
    QString text, text2, verselist;
    QStringList split, editlist;// verselist;
    int i(0),j(0),k(0);//chor(0);// i = carrier for file in while statement; j = carrier for split in while statement; k = size of split

        editlist = song.split("@$");

        while (i <editlist.size()){
            text = editlist[i];
            //text.chop(1);
            split = text.split("@%"); // split the text into rythmic line seperated by @%
            k=split.size();
            text=""; // clear text
            j=0;
            text2=split[0];
            while (j<k){
                if (j==k-1) text += split[j];
                else text += split[j] + "\n";
                ++j;
            }
            verselist += text.trimmed() + "\n\n";
            ++i;
        }
        return verselist;
}

void EditWidget::on_comboBoxSbornik_currentIndexChanged(int index)
{
    if (index>=0){
//        ui->spinBoxSongNum->setEnabled(true);
        if (index == 0) sbornik = "pv3300";
        else if (index==2) sbornik = "pv2001";
        else if (index==7) sbornik = "uaEpisni";
        else if (index==8) sbornik = "pvUser";

        titleType=1;
        Song t;
        ui->listTitles->clear();
        ui->listTitles->addItems(t.getTitle2(sbornik));
    }
}

void EditWidget::resetUiItems()
{
    ui->lineEditTitle->clear();
    ui->lineEditMusicBy->clear();
    ui->lineEditWordsBy->clear();
    ui->lineEditTune->clear();
    ui->comboBoxCategory->setCurrentIndex(0);
    ui->comboBoxLanguage->setCurrentIndex(0);
    ui->spinBoxlpvUser->setValue(1);
    ui->spinBoxpv1730->setValue(0);
    ui->spinBoxpv2000a->setValue(0);
    ui->spinBoxpv2000b->setValue(0);
    ui->spinBoxpv2001->setValue(0);
    ui->spinBoxpv2500->setValue(0);
    ui->spinBoxpv3300->setValue(0);
    ui->spinBoxpvCt->setValue(0);
    ui->spinBoxuaEpisni->setValue(0);
    ui->spinBoxuaPsalm->setValue(0);
    ui->textEditSong->clear();
    ui->lblID->setText("");
}

void EditWidget::setUiItems()
{
    ui->lineEditTitle->setText(editSong.title);
    ui->lineEditMusicBy->setText(editSong.musicBy);
    ui->lineEditWordsBy->setText(editSong.wordsBy);
    ui->lineEditTune->setText(editSong.tune);
    ui->comboBoxCategory->setCurrentIndex(editSong.category);
//
    ui->lblID->setText(QString::number(editSong.songID));
    ui->textEditSong->setPlainText(setSongText(editSong.songText));
    ui->spinBoxpv3300->setValue(editSong.pv3300);
    ui->spinBoxpv2500->setValue(editSong.pv2500);
    ui->spinBoxpv2001->setValue(editSong.pv2001);
    ui->spinBoxpv2000a->setValue(editSong.pv2000a);
    ui->spinBoxpv2000b->setValue(editSong.pv2000b);
    ui->spinBoxpv1730->setValue(editSong.pv1730);
    ui->spinBoxpvCt->setValue(editSong.pvCt);
    ui->spinBoxuaPsalm->setValue(editSong.uaPsalm);
    ui->spinBoxuaEpisni->setValue(editSong.uaEpisni);
    ui->spinBoxlpvUser->setValue(editSong.pvUser);
}

void EditWidget::on_listTitles_currentTextChanged(QString currentText)
{
    QStringList list = currentText.split(" - ");
    if (list.size()==1) editSong.getSong(currentText);
    else editSong.getSong(list[1]);
    setUiItems();
}

