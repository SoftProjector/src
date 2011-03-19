/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2011  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

#include "editwidget.h"
#include "ui_editwidget.h"
#include "song.h"

EditWidget::EditWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::EditWidget)
{
    song_database = SongDatabase();
    ui->setupUi(this);
    highlight = new Highlight(ui->textEditSong->document());

    // Allow only positive values for the song number:
    song_num_validator = new QIntValidator(1,10000000,ui->song_number_lineEdit);
    ui->song_number_lineEdit->setValidator(song_num_validator);

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

void EditWidget::on_btnSave_clicked()
{
    /* This will never happen, because song number is automatically
       set when a songbook is chosen:
    if( ui->song_number_lineEdit->text().isEmpty() )
    {

        QMessageBox mb;
        mb.setText(tr("Please specify a song number to use"));
        mb.setWindowTitle(tr("Song number is missing"));
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
        return;
    }
    */

    // Check if song title exists. A song title MUST exits
    QString song_title = ui->lineEditTitle->text();
    song_title = song_title.simplified(); // make sure that its not all empty spaces

    if(song_title.isEmpty())
    {
        QMessageBox mb;
        mb.setText(tr("Song title cannot be left empty.\nPlease enter song title."));
        mb.setWindowTitle(tr("Song title is missing"));
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
        ui->lineEditTitle->setFocus();
        return;
    }
    setSave();

    setWaitCursor();
    if (is_new)
    {
        newSong.saveNew();
        emit addedNew();
    }
    else
    {
        newSong.saveUpdate();
        emit updateSongFromDatabase(newSong.songID);
    }
    setArrowCursor();
    
    resetUiItems();
    close();
}

void EditWidget::setArrowCursor()
{
    this->setCursor(Qt::ArrowCursor);
}

void EditWidget::setWaitCursor()
{
    this->setCursor(Qt::WaitCursor);
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
    ui->lineEditKey->clear();
    ui->comboBoxCategory->setCurrentIndex(0);
    ui->textEditSong->clear();
    ui->font_textbox->clear();
    ui->wall_textbox->clear();
    ui->left_radioButton->setChecked(true);
    ui->song_number_lineEdit->setText("");
}

void EditWidget::setUiItems()
{
    ui->lineEditTitle->setText(editSong.title);
    ui->lineEditMusicBy->setText(editSong.musicBy);
    ui->lineEditWordsBy->setText(editSong.wordsBy);
    ui->lineEditKey->setText(editSong.tune);
    ui->comboBoxCategory->setCurrentIndex(editSong.category);
    setSongbook(editSong.songID);
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
    newSong.num = ui->song_number_lineEdit->text().toInt();
    newSong.songbook_id = song_database.getSongbookIdStringFromName(ui->songbook_label->text());
    newSong.title = ui->lineEditTitle->text();
    newSong.category = ui->comboBoxCategory->currentIndex();
    newSong.tune = ui->lineEditKey->text();
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
        if(fSong.startsWith("Verse") || fSong.startsWith(QString::fromUtf8("Куплет"))
            || fSong.startsWith("&Verse") || fSong.startsWith(QString::fromUtf8("&Куплет"))
            || fSong.startsWith("Chorus") || fSong.startsWith(QString::fromUtf8("Припев"))
            || fSong.startsWith("&Chorus") || fSong.startsWith(QString::fromUtf8("&Припев"))
            || fSong.startsWith("Refrain") || fSong.startsWith("&Refrain") || fSong.startsWith("Slide")
            || fSong.startsWith("Insert") || fSong.startsWith(QString::fromUtf8("Вставка"))
            || fSong.startsWith("Intro") || fSong.startsWith(QString::fromUtf8("Вступление"))
            || fSong.startsWith("Ending") || fSong.startsWith(QString::fromUtf8("Окончание")))
        {
            lyric += "@$" + lSong[i];
        }
        else if ((fSong == "") || (fSong == " "))
            ;
        else
        {
            lyric += "@%" + lSong[i];
        }
        ++i;
    }
    return lyric;
}

void EditWidget::setSongbook(int id)
{
    QSqlQuery sq;
    QString songbook, song_num;

    sq.exec("SELECT songbook_id, song_number FROM SongLink WHERE song_id = "+QString::number(id));
    sq.first();
        songbook = sq.value(0).toString();
        song_num = sq.value(1).toString();
    sq.clear();

    sq.exec("SELECT name FROM Songbooks WHERE id = " + songbook);
    sq.first();
    songbook = sq.value(0).toString();
    sq.clear();

    ui->songbook_label->setText(songbook);
    ui->song_number_lineEdit->setText(song_num);
}

void EditWidget::setEdit(Song sEdit)
{
    editSong = sEdit;
//    editSong = song_database.getSong(sEdit.songID);
    setUiItems();
    is_new = false;
}

void EditWidget::setCopy(Song copy)
{
    addNewSong(copy,tr("Copy to a new Songbook"),tr("Select a Songbook to which you want to copy this song to"));
}

void EditWidget::setNew()
{
    Song new_song;
    new_song.songText = tr("Verse 1\n - words of verse go here\n\nRefrain\n - words of Chorus/Refrain\ngo here\n\nVerse 2\n - words of verse go here");
//    new_song.songText = QString::fromUtf8("Куплет 1\n - слова куплета сдесь\n\nПрипев\n - слова припева сдесь\n\nКуплет 2\n - слова куплета сдесь");
    addNewSong(new_song,tr("Add a new Songbook"),tr("Select a Songbook to which you want to add a song"));
}

void EditWidget::addNewSong(Song song, QString msgNewSongbook, QString msgCaption)
{
    editSong = song;
    if (song.songbook_id.isEmpty())
        ui->textEditSong->setPlainText(song.songText);
    else
        setUiItems();
    is_new = true;
    bool ok;

    QSqlQuery sq;
    QStringList songbook_list;
    songbook_list << msgNewSongbook;
    sq.exec("SELECT id, name FROM Songbooks");
    while (sq.next())
        songbook_list << sq.value(1).toString();

    int current_songbook(0);
    if (!add_to_songbook.isEmpty())
        current_songbook = songbook_list.indexOf(add_to_songbook);
    else
        current_songbook = 0;

    add_to_songbook = QInputDialog::getItem(this,tr("Select Songbook"),msgCaption,
                                       songbook_list,current_songbook,false,&ok);

    if (ok && !add_to_songbook.isEmpty())
    {
        if (add_to_songbook == msgNewSongbook)
        {
            // Add a Songbook to add a new song into
            addSongbook();
        }
        else
        {
            int last = song_database.lastUser(song_database.getSongbookIdStringFromName(add_to_songbook));
            ui->songbook_label->setText(add_to_songbook);
            ui->song_number_lineEdit->setText(QString::number(last));
        }
    }
    else
    {
        close();
    }
}

void EditWidget::addSongbook()
{
    AddSongbookDialog add_sbor;
    add_sbor.setWindowTitle(tr("Add a Songbook"));
    int last(0);
    int ret = add_sbor.exec();
    switch(ret)
    {
    case AddSongbookDialog::Accepted:
        song_database.addSongbook(add_sbor.title,add_sbor.info);
        last = song_database.lastUser(song_database.getSongbookIdStringFromName(add_sbor.title));
        ui->songbook_label->setText(add_sbor.title);
        ui->song_number_lineEdit->setText(QString::number(last));
        add_to_songbook = add_sbor.title;
        break;
    case AddSongbookDialog::Rejected:
        close();
        break;
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
                                                    tr("Select a picture for the wallpaper"),
                                                    ".", "Images (*.png *.jpg *.jpeg)");

    if( !filename.isNull() )
        ui->wall_textbox->setText(filename);
}

void EditWidget::on_wall_default_button_clicked()
{
    ui->wall_textbox->clear();
}

