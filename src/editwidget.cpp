#include "editwidget.h"
#include "ui_editwidget.h"

EditWidget::EditWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::EditWidget)
{
    song_database = SongDatabase();
    ui->setupUi(this);
    highlight = new Highlight(ui->textEditSong->document());

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

void EditWidget::on_btnDelete_clicked()
{
    QMessageBox ms;

    ms.setWindowTitle("Delete Song?");
    ms.setText("Are you sure that you want to delete a song?");
    ms.setInformativeText("This action will permanentrly delete this song");
    ms.setIcon(QMessageBox::Question);
    ms.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    ms.setDefaultButton(QMessageBox::Yes);
    int ret = ms.exec();

    switch (ret) {
    case QMessageBox::Yes:
        // Delete a song
        close();
        song_database.deleteSong(editSong.songID);
        break;
    case QMessageBox::No:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }

}

void EditWidget::on_btnSave_clicked()
{
    setSave();
    if (is_new)
        newSong.saveNew();
    else
        newSong.saveUpdate();
    resetUiItems();

    close();
}

void EditWidget::on_btnCancel_clicked()
{
    resetUiItems();
    close();
}

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

void EditWidget::resetUiItems()
{
    ui->lineEditTitle->clear();
    ui->lineEditMusicBy->clear();
    ui->lineEditWordsBy->clear();
    ui->lineEditTune->clear();
    ui->comboBoxCategory->setCurrentIndex(0);
    ui->textEditSong->clear();
    //ui->lblID->setText("");
    ui->font_textbox->clear();
    ui->wall_textbox->clear();
    ui->left_radioButton->setChecked(true);
    ui->sbornik_table->clearContents();
    ui->sbornik_table->setRowCount(0);
}

void EditWidget::setUiItems()
{
    ui->lineEditTitle->setText(editSong.title);
    ui->lineEditMusicBy->setText(editSong.musicBy);
    ui->lineEditWordsBy->setText(editSong.wordsBy);
    ui->lineEditTune->setText(editSong.tune);
    ui->comboBoxCategory->setCurrentIndex(editSong.category);
    setSbornikTable(editSong.songID);
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
    newSong.songID = editSong.songID;
    newSong.num = ui->sbornik_table->item(0,1)->text().toInt();
    newSong.sbornik_id = ui->sbornik_table->item(0,0)->text();
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

void EditWidget::setSbornikTable(int id)
{
    QSqlQuery sq;
    QStringList sbornik, song_num;

    sq.exec("SELECT sbornik_id, song_number FROM SongLink where song_id = "+QString::number(id));
    int count(0);

    while (sq.next())
    {
        ++count;
        sbornik << sq.value(0).toString();
        song_num << sq.value(1).toString();
    }
    ui->sbornik_table->setRowCount(count);

    QTableWidgetItem *item0 = new QTableWidgetItem();
    QTableWidgetItem *item1 = new QTableWidgetItem();
    for (int i(0); i < count; ++i)
    {
        item0->setText(sbornik.at(i));
        item1->setText(song_num.at(i));
        ui->sbornik_table->setItem(i,0,item0->clone());
        ui->sbornik_table->setItem(i,1,item1->clone());
    }


}

void EditWidget::setEdit(Song sEdit)
{
    editSong = sEdit;
    editSong = song_database.getSong(sEdit.songID);
    setUiItems();
    is_new = false;
}

void EditWidget::setNew()
{
    resetUiItems();
    ui->textEditSong->setPlainText(QString::fromUtf8("Куплет 1\n - слова куплета сдесь\n\nПрипев\n - слова припева сдесь\n\nКуплет 2\n - слова куплета сдесь"));
    is_new = true;
    bool ok;

    if (song_database.hasUserSbornik())
    {
        // Select Sbornik to add a new song into
        QStringList sbornik_list;
        sbornik_list << "Add a new Sbornik";
        sbornik_list.append(song_database.getUserSborniks());
        QString sb = QInputDialog::getItem(this,"Select Sbornik","Select Sbornik in which you want to add a song",
                                           sbornik_list,0,false,&ok);
        if (ok && !sb.isEmpty())
        {
            if (sb =="Add a new Sbornik")
            {
                // Add a Sbornik to add a new song into
                addSbornik();
            }
            else
            {
                QStringList sl = sb.split(" - ");
                sb = sl.at(0);
                int last = song_database.lastUser(sb);

                QTableWidgetItem *item0 = new QTableWidgetItem();
                QTableWidgetItem *item1 = new QTableWidgetItem();
                item0->setText(sb);
                item1->setText(QString::number(last));
                ui->sbornik_table->setRowCount(1);
                ui->sbornik_table->setItem(0,0,item0->clone());
                ui->sbornik_table->setItem(0,1,item1->clone());
            }
        }
        else
        {
            close();
        }
    }
    else
    {
        // Add a Sbornik to add a new song into
        addSbornik();
    }
}

void EditWidget::addSbornik()
{
    bool ok = false;
    AddSbornikDialog add_sbor;
    add_sbor.setWindowTitle("Add a Sbornik;");

    while (!ok)
    {
        int ret = add_sbor.exec();
        switch(ret)
        {
        case AddSbornikDialog::Accepted:
            ok = song_database.addSbornik(add_sbor.code,add_sbor.title,add_sbor.info);

            if (!ok)
            {
                QMessageBox ms;
                ms.setWindowTitle("Error");
                ms.setText("The Sbornik code that you have entered already exists.\nPlease enter a diffirent unique Sbornik code.");
                ms.exec();
            }
            else
            {
                int last = song_database.lastUser(add_sbor.code);

                QTableWidgetItem *item0 = new QTableWidgetItem();
                QTableWidgetItem *item1 = new QTableWidgetItem();
                item0->setText(add_sbor.code);
                item1->setText(QString::number(last));
                ui->sbornik_table->setRowCount(1);
                ui->sbornik_table->setItem(0,0,item0->clone());
                ui->sbornik_table->setItem(0,1,item1->clone());
            }
            break;
        case AddSbornikDialog::Rejected:
            ok=true;
            close();
            break;
        }
    }

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

