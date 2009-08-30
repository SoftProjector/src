#include "editwidget.h"
#include "ui_editwidget.h"

EditWidget::EditWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::EditWidget)
{
    song_database = SongDatabase();
    ui->setupUi(this);

    // Hide font setting options until program will be compatable
    ui->font_button->hide();
    ui->font_default_button->hide();
    ui->font_textbox->hide();
    ui->font_label->hide();
    // Hide background setting options until program will be compatable
    ui->wall_button->hide();
    ui->wall_default_button->hide();
    ui->wall_textbox->hide();
    ui->wall_label->hide();
    // Hide alingment setting options until program will be compatable
    ui->aling_groupBox->hide();

//    s/*b*/ornik=QString("ALL");
//    loadTitles(sbornik);
//    ui->comboBoxSbornik->setCurrentIndex(-1);
    //ui->spinBoxSongNum->setEnabled(false);
//    ui->checkBoxAny->hide();
//    ui->lblEditProg->hide();
//    ui->label_19->hide();
//    ui->comboBoxLanguage->hide();
//    ui->checkBoxFavorite->hide();
    disablePV();
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

void EditWidget::disablePV()
{
//    ui->spinBoxpv3300->setEnabled(false);
//    ui->spinBoxpv2500->setEnabled(false);
//    ui->spinBoxpv2001->setEnabled(false);
//    ui->spinBoxpv2000a->setEnabled(false);
//    ui->spinBoxpv2000b->setEnabled(false);
//    ui->spinBoxpv1730->setEnabled(false);
//    ui->spinBoxpvCt->setEnabled(false);
//    ui->spinBoxuaPsalm->setEnabled(false);
//    ui->spinBoxuaEpisni->setEnabled(false);
}

//void EditWidget::on_btnEdit_clicked()
//{
//    ui->btnEdit->setEnabled(false);
//    ui->lblEditProg->setText("EDDITING A SONG");
//    ui->lblEditProg->show();
//}

//void EditWidget::on_btnNew_clicked()
//{
//    if (ui->btnEdit->isEnabled()){
//        ui->btnEdit->setEnabled(false);
//        ui->btnNew->setEnabled(false);
//        resetUiItems();
//        ui->lblEditProg->setText("ADDING A SONG");
//        ui->lblEditProg->show();
//        ui->spinBoxlpvUser->setValue(song_database.lastUser());
//        ui->textEditSong->setPlainText(QString::fromUtf8("Куплет 1\n - слова куплета сдесь\n\nПрипев\n - слова припева сдесь\n\nКуплет 2\n - слова куплета сдесь"));
//    }
//}

//void EditWidget::on_bntDelete_clicked()
//{
//    QMessageBox ms;
//    if(song_database.isUserOnly(editSong.songID)) {
//        song_database.deleteSong(editSong.songID);
////        loadTitles(sbornik);
//        resetUiItems();
//    }
//    else {
//        ms.setText("Cannot Delete this song because it is part of some Sbornik\nYou are allowed to delete only song from User ONLY");
//        ms.setWindowTitle("Cannot Delete Song");
//        ms.setIcon(QMessageBox::Warning);
//        ms.exec();
//    }
//}

void EditWidget::on_btnSave_clicked()
{
//    if (!ui->btnEdit->isEnabled()){
//        QMessageBox ms;
////        if ((song_database.hasTitle(ui->lineEditTitle->text())) && (!(ui->btnNew->isEnabled()))){
//            ms.setText("A song with this exact title already exist.\nPlease modify the title.\nAll non Alphanumeric characters will be removes");
//            ms.setWindowTitle("Title Duplicate");
//            ms.setIcon(QMessageBox::Critical);
//            ms.exec();
//        }
//        else {
            setSave();
            if (is_new)
                newSong.saveNew();
            else
                newSong.saveUpdate();
            resetUiItems();
//            loadTitles(sbornik);
//            ui->btnEdit->setEnabled(true);
//            ui->btnNew->setEnabled(true);
//            ui->lblEditProg->hide();
//        }
//    }
            close();
}

void EditWidget::on_btnCancel_clicked()
{
//    ui->btnEdit->setEnabled(true);
//    ui->btnNew->setEnabled(true);
//    ui->lblEditProg->hide();
    resetUiItems();
    close();
}

//void EditWidget::on_btnClose_clicked()
//{
//    close();
//}

//void EditWidget::on_btnSortTitles_clicked()
//{
//    titleType=0;
//    loadTitles("ALL");
//    ui->comboBoxSbornik->setCurrentIndex(-1);
//    ui->spinBoxSongNum->setEnabled(false);
//    //    ui->spinBoxSongNum->setEnabled(false);
//    sbornik="ALL";
//    //    }
//    //    bool empty = ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).isEmpty();
//    //    if (!empty) ui->listTitles->setCurrentItem(ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).takeFirst());
//    //    ui->listTitles->scrollTo(ui->listTitles->currentIndex(),QAbstractItemView::PositionAtCenter);
//}

//void EditWidget::on_spinBoxSongNum_valueChanged(int value)
//{
//
//    bool empty = ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).isEmpty();
//    if (!empty) ui->listTitles->setCurrentItem(ui->listTitles->findItems(QString::number(value),Qt::MatchStartsWith).takeFirst());
//    ui->listTitles->scrollTo(ui->listTitles->currentIndex(),QAbstractItemView::PositionAtCenter);
//}

//void EditWidget::loadTitles(QString tSbornik)
//{
//    QList<Song> songs;
//
//    //song_database = SongDatabase();
//    songs = song_database.getSongs(tSbornik);
//
//    QStringList titles;
//    for (int i = 0; i < songs.size(); i++) {
//        Song song = songs.at(i);
//        titles.append(song.title);
//    }
//
//    ui->listTitles->clear();
//    ui->listTitles->addItems(titles);
//}

QString EditWidget::setSongText(QString song)
{
    QString text, text2, verselist;
    QStringList split, editlist;
    int i(0),j(0),k(0);

    editlist = song.split("@$");// split the text into verses seperated by @$

    while (i <editlist.size()){
        text = editlist[i];
        //text.chop(1);
        split = text.split("@%"); // split the text into rythmic line seperated by @%
        k=split.size();
        text=""; // clear text
        j=0;
        text2=split[0];
        while (j<k){
            if (j==k-1)
                text += split[j];
            else
                text += split[j] + "\n";
            ++j;
        }
        verselist += text.trimmed() + "\n\n";
        ++i;
    }
    return verselist;
}


//
//void EditWidget::on_comboBoxSbornik_currentIndexChanged(int index)
//{
//    if (index>=0){
//                ui->spinBoxSongNum->setEnabled(true);
//        if (index == 0) sbornik = "pv3300";
//        else if (index==1) sbornik = "pv2500";
//        else if (index==2) sbornik = "pv2001";
//        else if (index==3) sbornik = "pv2000a";
//        else if (index==4) sbornik = "pv2000b";
//        else if (index==5) sbornik = "pv1730";
//        else if (index==6) sbornik = "pvCt";
//        else if (index==7) sbornik = "uaPsalm";
//        else if (index==8) sbornik = "uaEpisni";
//        else if (index==9) sbornik = "pvUser";
//
//        titleType=1;
//
//        QList<Song> songs = song_database.getSongs(sbornik);
//        QStringList titles;
//        for (int i = 0; i < songs.size(); i++) {
//            Song song = songs.at(i);
//            titles.append(song.title);
//        }
//
//        ui->listTitles->clear();
//        ui->listTitles->addItems(titles);
//    }
//}

void EditWidget::resetUiItems()
{
    ui->lineEditTitle->clear();
    ui->lineEditMusicBy->clear();
    ui->lineEditWordsBy->clear();
    ui->lineEditTune->clear();
    ui->comboBoxCategory->setCurrentIndex(0);
    ui->textEditSong->clear();
    ui->lblID->setText("");
    ui->font_textbox->clear();
    ui->wall_textbox->clear();
    ui->left_radioButton->setChecked(true);
}

void EditWidget::setUiItems()
{
    ui->lineEditTitle->setText(editSong.title);
    ui->lineEditMusicBy->setText(editSong.musicBy);
    ui->lineEditWordsBy->setText(editSong.wordsBy);
    ui->lineEditTune->setText(editSong.tune);
    ui->comboBoxCategory->setCurrentIndex(editSong.category);
    ui->lblID->setText(QString::number(editSong.songID));
    ui->font_textbox->setText(editSong.font);
    ui->wall_textbox->setText(editSong.background);
    ui->textEditSong->setPlainText(setSongText(editSong.songText));
    QString aling = editSong.alingment;
    if(aling=="right")
        ui->rihgt_radioButton->setChecked(true);
    else if (aling=="center")
        ui->center_radioButton->setChecked(true);
    else if (aling=="left")
        ui->left_radioButton->setChecked(true);
    else
        ui->left_radioButton->setChecked(true);
}

void EditWidget::setSave(){
    newSong.songID = ui->lblID->text().toInt();
    newSong.title = ui->lineEditTitle->text();
    newSong.category = ui->comboBoxCategory->currentIndex();
    newSong.tune = ui->lineEditTune->text();
    newSong.wordsBy = ui->lineEditWordsBy->text();
    newSong.musicBy = ui->lineEditMusicBy->text();
    newSong.songText = resetLyric(ui->textEditSong->toPlainText());
    newSong.font = ui->font_textbox->text();
    newSong.background = ui->wall_textbox->text();
    if (ui->rihgt_radioButton->isChecked())
        newSong.alingment = "right";
    else if (ui->center_radioButton->isChecked())
        newSong.alingment = "center";
    else if (ui->left_radioButton->isCheckable())
        newSong.alingment = "left";
}

QString EditWidget::resetLyric(QString lyric)
{
    QString fSong;
    QStringList lSong = lyric.split("\n");
    int i(1);
    lyric = lSong[0];
    while (i<lSong.size()){
        fSong = lSong[i];
        if (fSong.startsWith(QString::fromUtf8("Куплет")) || fSong.startsWith(QString::fromUtf8("Припев"))){
            lyric += "@$" + lSong[i];
        }else if ((fSong == "") || (fSong == " "));
        else  {
            lyric += "@%" + lSong[i];
        }
        ++i;
    }
    return lyric;
}

//void EditWidget::on_listTitles_currentTextChanged(QString currentText)
//{
//    QStringList list = currentText.split(" - ");
//    if (list.size()==1)
//        editSong = song_database.getSong(currentText);
//    else
//        editSong = song_database.getSong(list[1]);
//    if (ui->btnEdit->isEnabled())setUiItems();
//}
//
//void EditWidget::on_txtSearch_textChanged(QString text)
//{
//    if (ui->checkBoxAny->isChecked())
//    {
//        titleType =2;
//        ui->listTitles->clear();
//        QStringList tlist = text.split(" ");
//        QStringList tlist2;
//        if(tlist.count()==1){
//            QString tx = tlist[0];
////            qDebug() << "tx1 = " + tx;
//            tlist2 = allTitles.filter(tx.trimmed(),Qt::CaseInsensitive);
//            ui->listTitles->addItems(tlist2);
//        }
//        if(tlist.count()==2)
//        {
//            QString tx = tlist[0];
////            qDebug() << "tx1 = " + tx;
//            tlist2 = allTitles.filter(tx.trimmed(),Qt::CaseInsensitive);
//            tx = tlist[1];
////            qDebug() << "tx2 = " + tx;
//            ui->listTitles->addItems(tlist2.filter(tx,Qt::CaseInsensitive));
//        }
//    }
//    else
//    {
//        if (!titleType==0)
//        {
//            titleType =0;
//            ui->listTitles->clear();
//            ui->listTitles->addItems(allTitles);
//            ui->comboBoxSbornik->setCurrentIndex(-1);
//            ui->spinBoxSongNum->setEnabled(false);
//        }
//        bool empty = ui->listTitles->findItems(text,Qt::MatchStartsWith).isEmpty();
//        if (!empty) ui->listTitles->setCurrentItem(ui->listTitles->findItems(text,Qt::MatchStartsWith).takeFirst());
//        ui->listTitles->scrollTo(ui->listTitles->currentIndex(),QAbstractItemView::PositionAtTop);
//    }
//}

void EditWidget::setEdit(Song sEdit)
{
    editSong = sEdit;
    editSong = song_database.getSong(sEdit.songID);
    setUiItems();
    is_new = false;
//    on_btnEdit_clicked();
}

void EditWidget::setNew()
{
    resetUiItems();
    ui->textEditSong->setPlainText(QString::fromUtf8("Куплет 1\n - слова куплета сдесь\n\nПрипев\n - слова припева сдесь\n\nКуплет 2\n - слова куплета сдесь"));
    is_new = true;
//    on_btnNew_clicked();
}

void EditWidget::on_font_button_clicked()
{
    QFont old_font;
    old_font.fromString(ui->font_textbox->text());
    bool ok;
    QFont font = QFontDialog::getFont(&ok, old_font, this);
    if (ok)
        ui->font_textbox->setText(font.toString());
}

void EditWidget::on_font_default_button_clicked()
{
    ui->font_textbox->clear();
}

void EditWidget::on_wall_button_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Select a picture for the wallpaper",
                                                    ".", "Images (*.png *.jpg *.jpeg)");

    if( !filename.isNull() )
        ui->wall_textbox->setText(filename);
}

void EditWidget::on_wall_default_button_clicked()
{
    ui->wall_textbox->clear();
}
