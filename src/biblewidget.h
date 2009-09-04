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
    Bible bible;

public slots:
    QString getPrimary();
    QString getSecondary();
    void loadBibles(QString primaryId, QString secondaryId);

protected:
    virtual void changeEvent(QEvent *e);

signals:
    void goLive(int row);

private slots:
    void on_btnLive_clicked();
    void on_lineEditBook_textChanged(QString );
    void on_listChapter_doubleClicked(QModelIndex index);
    void on_spinVerse_valueChanged(int value);
    void on_spinChapter_valueChanged(int value);
    void on_listChapter_currentRowChanged(int currentRow);
    void on_listChapterNum_currentTextChanged(QString currentText);
    void on_listBook_currentTextChanged(QString currentText);


private:
    Ui::BibleWidget *ui;

};

#endif // BIBLEWIDGET_H
