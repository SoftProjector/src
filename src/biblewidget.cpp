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
    ui->listBook->addItems(bible.getBooks());
    ui->listBook->setCurrentRow(0);
}

QString BibleWidget::getPrimary()
{
    return bible.primaryId;
}

QString BibleWidget::getSecondary()
{
    return bible.secondaryId;
}


void BibleWidget::setBibles(QString primaryId, QString secondaryId)
{
    // secondaryId may be "none"
    bible.primaryId = primaryId;
    bible.secondaryId = secondaryId;
    loadBible();
}


void BibleWidget::on_listBook_currentTextChanged(QString currentText)
{
    int s = ui->listBook->currentRow();
    if( s != -1 )
    {
        int maxVerse = bible.maxChapters(currentText, bible.primaryId);
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
        QStringList chapters = bible.getChapter(bible.primaryId, ui->listBook->currentItem()->text(), currentText.toInt());
        ui->listChapter->clear();
        ui->listChapter->addItems(chapters);
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
    emit goLive(bible, index.row());
}

void BibleWidget::on_lineEditBook_textChanged(QString text)
{
    // Called when the bible book filter field is modified.
    if( text.isEmpty() )
    {
        ui->listBook->clear();
        ui->listBook->addItems(bible.getBooks());
        ui->listBook->setCurrentRow(0);
    }
    else
    {
        ui->listBook->clear();
        ui->listBook->addItems(bible.getBooks().filter(text, Qt::CaseInsensitive));
        if( ui->listBook->count() > 0 )
            ui->listBook->setCurrentRow(0);
    }
}

void BibleWidget::on_btnLive_clicked()
{
    emit goLive(bible, ui->spinVerse->value() - 1);
}

