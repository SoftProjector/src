#include "theme.h"

PassiveSettings::PassiveSettings()
{
    useBackground = false;
    backgroundPath = "";
    useDisp2settings = false;
}

BibleSettings::BibleSettings()
{
    // Apply Bible Defaults
    primaryBible = "none";
    secondaryBible = "none";
    trinaryBible = "none";
    operatorBible = "same";
    useShadow = true;
    useFading = true;
    useBlurShadow = false;
    useBackground = false;
    backgroundPath = "";
    textFont.fromString("Arial,28,-1,5,50,0,0,0,0,0");
    textColor = QColor(Qt::white);
    textAlingmentV = 0;
    textAlingmentH = 0;
    useAbbriviations = false;
    maxScreen = 100;
    maxScreenFrom = "bottom";
    useDisp2settings = false;
}

SongSettings::SongSettings()
{
    // Apply song defauls
    showStanzaTitle = false;
    showSongKey = false;
    showSongNumber = false;
    showSongEnding = true;
    songEndingType = 0;
    useShadow = true;
    useFading = true;
    useBlurShadow = false;
    useBackground = false;
    backgroundPath = "";
    textFont.fromString("Arial,28,-1,5,50,0,0,0,0,0");
    textColor = QColor(Qt::white);
    textAlingmentV = 1;
    textAlingmentH = 1;
    useDisp2settings = false;
}

AnnounceSettings::AnnounceSettings()
{
    // Apply annouce defaults
    useShadow = true;
    useFading = true;
    useBlurShadow = false;
    useBackground = false;
    backgroundPath = "";
    textFont.fromString("Arial,28,-1,5,50,0,0,0,0,0");
    textColor = QColor(Qt::white);
    textAlingmentV = 0;
    textAlingmentH = 1;
    useDisp2settings = false;
}

ThemeInfo::ThemeInfo()
{

}

Theme::Theme()
{
    themeId = "0";
}

void Theme::saveTheme()
{
    QString pset,pset2,bset,bset2,sset,sset2,aset,aset2;//passive,bible,song,annouce

    // **** prepare bible, song and announce settings ************************
    pset = passiveSettingToString(passive); // prepare primary passive screen settings
    pset2 = passiveSettingToString(passive2); // prepare secondary passive screen settings

    bset = bibleSettingToString(bible); // prepare primary bible screen settings
    bset2 = bibleSettingToString(bible2); // prepare secondary bible screen settings

    sset = songSettingToString(song); // prepare primary song screen settings
    sset2 = songSettingToString(song2); // prepare secondary song screen settings

    aset = announceSettingToString(announce); // prepare primary announce screen settings
    aset2 = announceSettingToString(announce2); // prepare secondary announce screen settings

    QSqlQuery sq;
    sq.exec(QString("UPDATE ThemeData SET sets = '%1' WHERE type = 'passive' AND theme_id = '%2'").arg(pset).arg(themeId));
    sq.exec(QString("UPDATE ThemeData SET sets = '%1' WHERE type = 'passive2' AND theme_id = '%2'").arg(pset2).arg(themeId));
    sq.exec(QString("UPDATE ThemeData SET sets = '%1' WHERE type = 'bible' AND theme_id = '%2'").arg(bset).arg(themeId));
    sq.exec(QString("UPDATE ThemeData SET sets = '%1' WHERE type = 'bible2' AND theme_id = '%2'").arg(bset2).arg(themeId));
    sq.exec(QString("UPDATE ThemeData SET sets = '%1' WHERE type = 'song' AND theme_id = '%2'").arg(sset).arg(themeId));
    sq.exec(QString("UPDATE ThemeData SET sets = '%1' WHERE type = 'song2' AND theme_id = '%2'").arg(sset2).arg(themeId));
    sq.exec(QString("UPDATE ThemeData SET sets = '%1' WHERE type = 'announce' AND theme_id = '%2'").arg(aset).arg(themeId));
    sq.exec(QString("UPDATE ThemeData SET sets = '%1' WHERE type = 'announce2' AND theme_id = '%2'").arg(aset2).arg(themeId));
}

void Theme::saveNewTheme()
{
    QSqlQuery sq;
    sq.exec("INSERT INTO Themes (name, comment) VALUES ('Default', 'Default theme settings')");

    // get new theme id
    sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Themes'");
    sq.first();
    QString nId = sq.value(0).toString();

    saveNewTheme(nId);
}

void Theme::saveNewTheme(QString newId)
{
    themeId = newId;
    QSqlQuery sq;
    sq.exec("DELETE FROM ThemeData WHERE theme_id = '"+themeId+"' ");
    sq.exec("INSERT INTO ThemeData (theme_id, type, sets) VALUES ('"+themeId+"', 'passive', 'n=v')");
    sq.exec("INSERT INTO ThemeData (theme_id, type, sets) VALUES ('"+themeId+"', 'passive2', 'n=v')");
    sq.exec("INSERT INTO ThemeData (theme_id, type, sets) VALUES ('"+themeId+"', 'bible', 'n=v')");
    sq.exec("INSERT INTO ThemeData (theme_id, type, sets) VALUES ('"+themeId+"', 'bible2', 'n=v')");
    sq.exec("INSERT INTO ThemeData (theme_id, type, sets) VALUES ('"+themeId+"', 'song', 'n=v')");
    sq.exec("INSERT INTO ThemeData (theme_id, type, sets) VALUES ('"+themeId+"', 'song2', 'n=v')");
    sq.exec("INSERT INTO ThemeData (theme_id, type, sets) VALUES ('"+themeId+"', 'announce', 'n=v')");
    sq.exec("INSERT INTO ThemeData (theme_id, type, sets) VALUES ('"+themeId+"', 'announce2', 'n=v')");

    saveTheme();
}

void Theme::loadTheme()
{
    QString t,sets; // type, name, value, userValues
    QSqlQuery sq;

    // If theme Id is '0' then, get first theme in the list
    if(themeId == "0")
    {
        sq.exec("SELECT id FROM Themes");
        bool ok = sq.first();

        // Check at least one theme exitst
        if (ok) // If exist, then load it
            themeId = sq.value(0).toString();
        else // No themes exist, creat one and exit
        {
            saveNewTheme();
            return;
        }
    }

    sq.exec(QString("SELECT type, sets FROM ThemeData WHERE theme_id = %1").arg(themeId));
    bool p,p2,b,b2,s,s2,a,a2;
    p=p2=b=b2=s=s2=a=a2=false;
    while (sq.next())
    {
        t = sq.value(0).toString();
        sets = sq.value(1).toString();

        if(t == "passive")
        {
            passiveSettingFromString(sets,passive);
            p = true;
        }
        else if(t == "passive2")
        {
            passiveSettingFromString(sets,passive2);
            p2 = true;
        }
        else if(t == "bible")
        {
            bibleSettingFromString(sets,bible);
            b = true;
        }
        else if(t == "bible2")
        {
            bibleSettingFromString(sets,bible2);
            b2 = true;
        }
        else if(t == "song")
        {
            songSettingFromString(sets,song);
            s = true;
        }
        else if(t == "song2")
        {
            songSettingFromString(sets,song2);
            s2 = true;
        }
        else if(t == "announce")
        {
            announceSettingFromString(sets,announce);
            a = true;
        }
        else if(t == "announce2")
        {
            announceSettingFromString(sets,announce2);
            a2 = true;
        }
    }

    // check if database has all the items
    // If not then create or re create all items.
    if(!p || !p2 || !b || !b2 || !s || !s2 || !a || !a2)
        saveNewTheme(themeId);
}

void Theme::passiveSettingFromString(QString &sets, PassiveSettings &settings)
{
    QString s,n,v;
    QStringList values = sets.split("\n");
    for(int i(0);i<values.count();++i)
    {
        s = values.at(i);
        QStringList set = s.split("=");
        n = set.at(0).trimmed();
        v = set.at(1).trimmed();
        if (n == "useDisp2settings")
            settings.useDisp2settings = (v=="true");
        else if(n=="useBackground")
            settings.useBackground = (v=="true");
        else if(n=="backgroundPath")
            settings.backgroundPath = v;
    }
}

void Theme::bibleSettingFromString(QString &sets, BibleSettings &settings)
{
    QString s,n,v;
    QStringList values = sets.split("\n");
    for(int i(0);i<values.count();++i)
    {
        s = values.at(i);
        QStringList set = s.split("=");
        n = set.at(0).trimmed();
        v = set.at(1).trimmed();

        if (n == "useDisp2settings")
            settings.useDisp2settings = (v=="true");
        else if(n=="primaryBible")
            settings.primaryBible = v;
        else if(n=="secondaryBible")
            settings.secondaryBible = v;
        else if(n=="trinaryBible")
            settings.trinaryBible = v;
        else if(n=="operatorBible")
            settings.operatorBible = v;
        else if (n == "useShadow")
            settings.useShadow = (v=="true");
        else if (n == "useFading")
            settings.useFading = (v=="true");
        else if (n == "useBlurShadow")
            settings.useBlurShadow = (v=="true");
        else if(n=="useBackground")
            settings.useBackground = (v=="true");
        else if(n=="backgroundPath")
            settings.backgroundPath = v;
        else if(n=="textAlingment")
        {
            QStringList alinglist = v.split(",");
            settings.textAlingmentV = alinglist.at(0).toInt();
            settings.textAlingmentH = alinglist.at(1).toInt();
        }
        else if(n=="textColor")
            settings.textColor = QColor::fromRgb(v.toUInt());
        else if(n=="textFont")
            settings.textFont.fromString(v);
        else if(n=="useAbbriviations")
            settings.useAbbriviations = (v=="true");
        else if(n=="maxScreen")
            settings.maxScreen = v.toInt();
        else if(n=="maxScreenFrom")
            settings.maxScreenFrom = v;
    }
}

void Theme::songSettingFromString(QString &sets, SongSettings &settings)
{
    QString s,n,v;
    QStringList values = sets.split("\n");
    for(int i(0);i<values.count();++i)
    {
        s = values.at(i);
        QStringList set = s.split("=");
        n = set.at(0).trimmed();
        v = set.at(1).trimmed();
        if(n=="useDisp2settings")
            settings.useDisp2settings = (v=="true");
        else if(n=="showStanzaTitle")
            settings.showStanzaTitle = (v=="true");
        else if(n=="showSongKey")
            settings.showSongKey = (v=="true");
        else if(n=="showSongNumber")
            settings.showSongNumber = (v=="true");
        else if(n=="showSongEnding")
            settings.showSongEnding = (v=="true");
        else if(n=="songEndingType")
            settings.songEndingType = v.toInt();
        else if (n == "useShadow")
            settings.useShadow = (v=="true");
        else if (n == "useFading")
            settings.useFading = (v=="true");
        else if (n == "useBlurShadow")
            settings.useBlurShadow = (v=="true");
        else if(n=="useBackground")
            settings.useBackground = (v=="true");
        else if(n=="backgroundPath")
            settings.backgroundPath = v;
        else if(n=="textAlingment")
        {
            QStringList alinglist = v.split(",");
            settings.textAlingmentV = alinglist.at(0).toInt();
            settings.textAlingmentH = alinglist.at(1).toInt();
        }
        else if(n=="textColor")
            settings.textColor = QColor::fromRgb(v.toUInt());
        else if(n=="textFont")
            settings.textFont.fromString(v);
    }
}

void Theme::announceSettingFromString(QString &sets, AnnounceSettings &settings)
{
    QString s,n,v;
    QStringList values = sets.split("\n");
    for(int i(0);i<values.count();++i)
    {
        s = values.at(i);
        QStringList set = s.split("=");
        n = set.at(0).trimmed();
        v = set.at(1).trimmed();
        if (n == "useDisp2settings")
            settings.useDisp2settings = (v=="true");
        else if (n == "useShadow")
            settings.useShadow = (v=="true");
        else if (n == "useFading")
            settings.useFading = (v=="true");
        else if (n == "useBlurShadow")
            settings.useBlurShadow = (v=="true");
        else if(n=="useBackground")
            settings.useBackground = (v=="true");
        else if(n=="backgroundPath")
            settings.backgroundPath = v;
        else if(n=="textAlingment")
        {
            QStringList alinglist = v.split(",");
            settings.textAlingmentV = alinglist.at(0).toInt();
            settings.textAlingmentH = alinglist.at(1).toInt();
        }
        else if(n=="textColor")
            settings.textColor = QColor::fromRgb(v.toUInt());
        else if(n=="textFont")
            settings.textFont.fromString(v);
    }
}

QString Theme::passiveSettingToString(PassiveSettings &settings)
{
    QString rString;

    // **** prepare passive settings ************************************
    if(settings.useDisp2settings)
        rString = "useDisp2settings = true";
    else
        rString = "useDisp2settings = false";

    // background
    (settings.useBackground)? rString += "\nuseBackground = true" : rString += "\nuseBackground = false";
    rString += "\nbackgroundPath = " + settings.backgroundPath;

    return rString;
}

QString Theme::bibleSettingToString(BibleSettings &settings)
{
    QString rString;
    // **** Prepare bible settings ************************************************
    if(settings.useDisp2settings)
        rString = "useDisp2settings = true";
    else
        rString = "useDisp2settings = false";
    rString += "\nprimaryBible = " + settings.primaryBible;
    rString += "\nsecondaryBible = " + settings.secondaryBible;
    rString += "\ntrinaryBible = " + settings.trinaryBible;
    rString += "\noperatorBible = " + settings.operatorBible;

    // Effects
    if(settings.useShadow)
        rString += "\nuseShadow = true";
    else
        rString += "\nuseShadow = false";
    if(settings.useFading)
        rString += "\nuseFading = true";
    else
        rString += "\nuseFading = false";
    if(settings.useBlurShadow)
        rString += "\nuseBlurShadow = true";
    else
        rString += "\nuseBlurShadow = false";

    // Background
    if(settings.useBackground)
        rString += "\nuseBackground = true";
    else
        rString += "\nuseBackground = false";

    rString += "\nbackgroundPath = " + settings.backgroundPath;

    // Text alingment
    rString += "\ntextAlingment = " + QString::number(settings.textAlingmentV) +
            "," + QString::number(settings.textAlingmentH);

    // Text color
    unsigned int textColorInt = (unsigned int)(settings.textColor.rgb());
    rString += "\ntextColor = " + QString::number(textColorInt);

    // Text font
    rString += "\ntextFont = " + settings.textFont.toString();

    // Version abbriviations
    if(settings.useAbbriviations)
        rString += "\nuseAbbriviations = true";
    else
        rString += "\nuseAbbriviations = false";

    // Max screen
    rString += "\nmaxScreen = " + QString::number(settings.maxScreen);
    rString += "\nmaxScreenFrom = " + settings.maxScreenFrom;

    return rString;
}

QString Theme::songSettingToString(SongSettings &settings)
{
    QString rString;

    // **** Prepare song settings ************************************************
    if(settings.useDisp2settings)
        rString = "useDisp2settings = true\n";
    else
        rString = "useDisp2settings = false\n";
    if(settings.showStanzaTitle)
        rString += "showStanzaTitle = true\n";
    else
        rString += "showStanzaTitle = false\n";
    if(settings.showSongKey)
        rString += "showSongKey = true\n";
    else
        rString += "showSongKey = false\n";
    if(settings.showSongNumber)
        rString += "showSongNumber = true\n";
    else
        rString += "showSongNumber = false\n";
    if(settings.showSongEnding)
        rString += "showSongEnding = true\n";
    else
        rString += "showSongEnding = false\n";
    rString += "songEndingType = " + QString::number(settings.songEndingType);

    // Effects
    if(settings.useShadow)
        rString += "\nuseShadow = true";
    else
        rString += "\nuseShadow = false";
    if(settings.useFading)
        rString += "\nuseFading = true";
    else
        rString += "\nuseFading = false";
    if(settings.useBlurShadow)
        rString += "\nuseBlurShadow = true";
    else
        rString += "\nuseBlurShadow = false";

    if(settings.useBackground)
        rString += "\nuseBackground = true";
    else
        rString += "\nuseBackground = false";

    rString += "\nbackgroundPath = " + settings.backgroundPath;

    // Text alingment
    rString += "\ntextAlingment = " + QString::number(settings.textAlingmentV) +
            "," + QString::number(settings.textAlingmentH);

    // Text color
    unsigned int textColorInt = (unsigned int)(settings.textColor.rgb());
    rString += "\ntextColor = " + QString::number(textColorInt);

    // Text font
    rString += "\ntextFont = " + settings.textFont.toString();

    return rString;
}

QString Theme::announceSettingToString(AnnounceSettings &settings)
{
    QString rString;

    // **** prepare announcement settings ************************************
    if(settings.useDisp2settings)
        rString = "useDisp2settings = true";
    else
        rString = "useDisp2settings = false";

    // Effects
    if(settings.useShadow)
        rString += "\nuseShadow = true";
    else
        rString += "\nuseShadow = false";
    if(settings.useFading)
        rString += "\nuseFading = true";
    else
        rString += "\nuseFading = false";
    if(settings.useBlurShadow)
        rString += "\nuseBlurShadow = true";
    else
        rString += "\nuseBlurShadow = false";

    // background
    (settings.useBackground)? rString += "\nuseBackground = true" : rString += "\nuseBackground = false";
    rString += "\nbackgroundPath = " + settings.backgroundPath;

    // Text alingment
    rString += "\ntextAlingment = " + QString::number(settings.textAlingmentV) +
            "," + QString::number(settings.textAlingmentH);

    // Text color
    unsigned int textColorInt = (unsigned int)(settings.textColor.rgb());
    rString += "\ntextColor = " + QString::number(textColorInt);

    // Text font
    rString += "\ntextFont = " + settings.textFont.toString();

    return rString;
}
