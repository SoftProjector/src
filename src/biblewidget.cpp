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


QString BibleWidget::getPrimary()
{
    return bible.primaryId;
}

QString BibleWidget::getSecondary()
{
    return bible.secondaryId;
}


void BibleWidget::loadBibles(QString primaryId, QString secondaryId)
{
    // secondaryId may be "none"
    bible.primaryId = primaryId;
    bible.secondaryId = secondaryId;
    ui->listBook->clear();
    ui->listBook->addItems(bible.getBooks());
    ui->listBook->setCurrentRow(0);
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
        QStringList chapters = bible.getChapter(getCurrentBook(), currentText.toInt());
        ui->listChapter->clear();
        ui->listChapter->addItems(chapters);
        ui->spinChapter->setValue(currentText.toInt());
        ui->spinVerse->setMaximum(ui->listChapter->count());
        ui->listChapter->setCurrentRow(0);
    }
}

QString BibleWidget::getCurrentBook(void)
{
    return ui->listBook->currentItem()->text();
}
int BibleWidget::getCurrentChapter(void)
{
    return ui->listChapterNum->currentItem()->text().toInt();
}

QString BibleWidget::getCaption()
{
    // Get the caption string to show above the show list (right-most list)
    QString id = bible.idList.at(0);
    QStringList temp = bible.getVerseAndCaption(id, bible.primaryId);
    QStringList temp1;
    temp1 = temp[1].split(":");
    return temp1[0];
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
    sendToProjector(index.row());
}

void BibleWidget::sendToProjector(int verse)
{
    if( bible.primaryId != bible.secondaryId && bible.by_chapter )
        bible.loadSecondaryData();

    emit goLive(bible.verseList, verse, getCaption());
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
    sendToProjector(ui->spinVerse->value() - 1);
}

