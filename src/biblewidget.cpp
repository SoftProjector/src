#include "biblewidget.h"
#include "ui_biblewidget.h"

BibleWidget::BibleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BibleWidget)
{
    ui->setupUi(this);
}

BibleWidget::~BibleWidget()
{
    delete ui;
}

void BibleWidget::changeEvent(QEvent *e)
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

void BibleWidget::loadBible()
{
    ui->listBook->clear();
    ui->listBook->addItems(bible.getBooks(biblePrimary));
    ui->listBook->setCurrentRow(0);
}

QString BibleWidget::getPrimary()
{
    return biblePrimary;
}

QString BibleWidget::getSecondary()
{
    return bibleSecondary;
}


void BibleWidget::setPrimary(QString bibleId)
{
    biblePrimary = bibleId;
    loadBible();
}

void BibleWidget::setSecondary(QString bibleId)
{
    bibleSecondary = bibleId;
    loadBible();
}


void BibleWidget::on_listBook_currentTextChanged(QString currentText)
{
    int s = ui->listBook->currentRow();
    if( s != -1 )
    {
        int maxVerse = bible.maxChapters(currentText, biblePrimary);
        ui->listChapterNum->clear();
        for(int i=0; i<maxVerse; ++i)
            ui->listChapterNum->addItem(QString::number(i+1));
        ui->spinChapter->setMaximum(maxVerse);
        ui->listChapterNum->setCurrentRow(0);
    }
}

void BibleWidget::on_listChapterNum_currentTextChanged(QString currentText)
{
    int s = ui->listChapterNum->currentRow();
    if( s != -1 )
    {
        ui->listChapter->clear();
        ui->listChapter->addItems(bible.getChapter(biblePrimary, ui->listBook->currentItem()->text(), currentText.toInt()));
        ui->spinChapter->setValue(currentText.toInt());
        ui->spinVerse->setMaximum(ui->listChapter->count());
        ui->listChapter->setCurrentRow(0);
    }
}

void BibleWidget::on_listChapter_currentRowChanged(int currentRow)
{
    ui->spinVerse->setValue(currentRow+1);
}

void BibleWidget::on_spinChapter_valueChanged(int value)
{
    ui->listChapterNum->setCurrentRow(value-1);
}

void BibleWidget::on_spinVerse_valueChanged(int value)
{
    ui->listChapter->setCurrentRow(value-1);
}

void BibleWidget::on_listChapter_doubleClicked(QModelIndex index)
{
    // Called when a chapter or verse is double clicked
    emit goLive(bible, bibleSecondary, index.row());
}

void BibleWidget::on_lineEditBook_textChanged(QString text)
{
    // Called when the bible book filter field is modified.
    bool emt = ui->listBook->findItems(text, Qt::MatchStartsWith).isEmpty();
    if (!emt)
        ui->listBook->setCurrentItem(ui->listBook->findItems(text, Qt::MatchStartsWith).takeFirst());

    /* // We do not need this code anymore, since we have chapter and verse entry fields now.
    QStringList search = text.split(" ");
    if (search.count()>=1)
    {
        bool emt = ui->listBook->findItems(search[0],Qt::MatchStartsWith).isEmpty();
        if (!emt) 
	    ui->listBook->setCurrentItem(ui->listBook->findItems(search[0],Qt::MatchStartsWith).takeFirst());
    }
    if (search.count()>=2)
    {
        QString ch = search[1];
        int i = ch.toInt();
        ui->spinChapter->setValue(i);
    }
    if (search.count()==3)
    {
        QString ch = search[2];
        int i = ch.toInt();
        ui->spinVerse->setValue(i);
    }
    */
}

void BibleWidget::on_btnLive_clicked()
{
    emit goLive(bible, bibleSecondary, ui->spinVerse->value() - 1);
}

