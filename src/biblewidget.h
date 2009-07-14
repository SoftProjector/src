#ifndef BIBLEWIDGET_H
#define BIBLEWIDGET_H

#include <QtGui/QWidget>
#include "bible.h"

namespace Ui {
    class BibleWidget;
}

class BibleWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(BibleWidget)
public:
    explicit BibleWidget(QWidget *parent = 0);
    virtual ~BibleWidget();

public slots:
    int getPrimary();
    int getSecondary();
    void setPrimary(int i);
    void setSecondary(int i);

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void goLive(Bible bible, QString bible2, int row);

private slots:
    void on_btnLive_clicked();
    void on_lineEditBook_textChanged(QString );
    void on_listChapter_doubleClicked(QModelIndex index);
    void on_spinVerse_valueChanged(int value);
    void on_spinChapter_valueChanged(int value);
    void on_listChapter_currentRowChanged(int currentRow);
    void on_listChapterNum_currentTextChanged(QString currentText);
//    void on_comboBoxBible_currentIndexChanged(int index);
    void on_listBook_currentTextChanged(QString currentText);
    void loadBible();

private:
    Ui::BibleWidget *ui;
    Bible bible;
    QString biblePrimary, bibleSecondary;

};

#endif // BIBLEWIDGET_H
