#include "biblesettingwidget.h"
#include "ui_biblesettingwidget.h"

BibleSettingWidget::BibleSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BibleSettingWidget)
{
    ui->setupUi(this);
    setDefaults();
}

BibleSettingWidget::~BibleSettingWidget()
{
    delete ui;
}

void BibleSettingWidget::setDefaults()
{
    QSqlQuery sq;

    // Set defaults
    sq.exec("INSERT INTO Settings (type, name, default_value) VALUES ('bible', 'primaryBible', 'none')");
    sq.exec("INSERT INTO Settings (type, name, default_value) VALUES ('bible', 'secondaryBible', 'none')");
    sq.exec("INSERT INTO Settings (type, name, default_value) VALUES ('bible', 'trinaryBible', 'none')");
    sq.exec("INSERT INTO Settings (type, name, default_value) VALUES ('bible', 'operatorBible', 'none')");
    sq.exec("INSERT INTO Settings (type, name, default_value) VALUES ('bible', 'bibleBackground', 'none')");
    sq.exec("INSERT INTO Settings (type, name, default_value) VALUES ('bible', 'bibleTextColor', 'white')");
    sq.exec("INSERT INTO Settings (type, name, default_value) VALUES ('bible', 'bibleTextFont', 'none')");
    sq.exec("INSERT INTO Settings (type, name, default_value) VALUES ('bible', 'versionAbbriviation', '0')");
    sq.exec("INSERT INTO Settings (type, name, default_value) VALUES ('bible', 'bibleAlingment', '4129')");
}

BibleSettings BibleSettingWidget::getSettings()
{
//    return settings;
}

void BibleSettingWidget::loadSettings()
{
//    QString n,v,dv;
//    QSqlQuery sq;
//    sq.exec("SELECT name, value, default_value FROM Settings WHERE type = 'bible");
//    while (sq.next())
//    {
//        n = sq.value(0).toString();
//        v = sq.value(1).toString();
//        dv = sq.value(2).toString();

//        if (v.isNull() || v.isEmpty()) //check if value is null, if it is, use default value.
//            v = dv;

//        if(n == "primaryBible")
//            settings.primaryBible = v;
//        else if (n == "secondaryBible")
//            settings.secondaryBible = v;
//        else if (n == "trinaryBible")
//            settings.trinaryBible = v;
//        else if (n == "operatorBible")
//            settings.controlBible = v;
//        else if (n == "bibleBackground")
//            settings.backgroundPath = v;
//        else if (n == "bibleTextColor")
//            settings.textColor.setColorFromString(v);
//        else if (n == "bibleTextFont")
//            settings.textFont.fromString(v);
//        else if (n == "versionAbbriviation")
//            settings.abbriviations = v.toInt();
//        else if (n == "bibleAlingment")
//            settings.textFlags = v.toInt();
//    }
}
