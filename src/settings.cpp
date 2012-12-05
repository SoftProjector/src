#include "settings.h"

GeneralSettings::GeneralSettings()
{
    // Apply General Defauls
    displayIsOnTop = false;
    useBackground = false;
    backgroundPath = "";
    displayScreen = 0;
    useDisplaySettings2 = false;
    useBackground2 = false;
    backgroundPath2 = "";
    displayScreen2 = -1; // interger "-1" mean "None" or not to display
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

DisplayControlsSettings::DisplayControlsSettings()
{
    buttonSize = 3; // 0=16,1=24,2=32,3=48,4=64,5=96
    alignmentV = 2;
    alignmentH = 1;
    opacity = .3;
}

SpSettings::SpSettings()
{
    // Apply main window defaults
    isWindowMaximized = false;
    uiTranslation = "en";
}

Settings::Settings()
{

}

void Settings::loadSettings(QString user)
{
    QString t,n,v,s,sets; // type, name, value, userValues
    QStringList set,values;
    QSqlQuery sq;
    sq.exec(QString("SELECT type, sets FROM Settings WHERE user = %1").arg(user));
    while (sq.next())
    {
        t = sq.value(0).toString();
        sets = sq.value(1).toString();

        if(t == "general") // set general setting values
        {
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n == "displayIsOnTop")
                    general.displayIsOnTop = (v=="true");
                else if (n == "useBackground")
                    general.useBackground = (v=="true");
                else if (n == "backgroundPath")
                    general.backgroundPath = v;
                else if (n == "displayScreen")
                    general.displayScreen = v.toInt();
                // Display Screen 2 settings
                else if (n == "useBackground2")
                    general.useBackground2 = (v=="true");
                else if (n == "backgroundPath2")
                    general.backgroundPath2 = v;
                else if (n == "useDisplaySettings2")
                    general.useDisplaySettings2 = (v=="true");
                else if (n == "displayScreen2")
                    general.displayScreen2 = v.toInt();
                // End display screen 2 settings
                else if (n == "dcIconSize")
                    general.displayControls.buttonSize = v.toInt();
                else if (n == "dcAlignment")
                {
                    QStringList alinglist = v.split(",");
                    general.displayControls.alignmentV = alinglist.at(0).toInt();
                    general.displayControls.alignmentH = alinglist.at(1).toInt();
                }
                else if (n == "dcOpacity")
                    general.displayControls.opacity = v.toDouble();
            }
        }
        else if(t == "bible")
            bibleSettingFromString(sets,bible);
        else if(t == "bible2")
            bibleSettingFromString(sets,bible2);
        else if(t == "song")
            songSettingFromString(sets,song);
        else if(t == "song2")
            songSettingFromString(sets,song2);
        else if(t == "announce")
            announceSettingFromString(sets,announce);
        else if(t == "announce2")
            announceSettingFromString(sets,announce2);
        else if(t == "spmain")
        {
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n=="spSplitter")
                    spmain.spSplitter = textToByt(v);
                else if(n=="bibleHiddenSplitter")
                    spmain.bibleHiddenSplitter = textToByt(v);
                else if(n=="bibleShowSplitter")
                    spmain.bibleShowSplitter = textToByt(v);
                else if(n=="songSplitter")
                    spmain.songSplitter = textToByt(v);
                else if(n=="uiTranslation")
                    spmain.uiTranslation = v;
                else if(n=="isWindowMaximized")
                    spmain.isWindowMaximized = (v=="true");
            }
        }
    }
}

void Settings::bibleSettingFromString(QString &sets, BibleSettings &settings)
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

void Settings::songSettingFromString(QString &sets, SongSettings &settings)
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

void Settings::announceSettingFromString(QString &sets, AnnounceSettings &settings)
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

void Settings::saveSettings(QString user)
{
    QSqlQuery sq;
    QString gset,bset,bset2,sset,sset2,aset,aset2,spset;//general,bible,song,annouce,spmain

    // **** Prepare general settings ***************************************
    if(general.displayIsOnTop)
        gset = "displayIsOnTop = true\n";
    else
        gset = "displayIsOnTop = false\n";
    if(general.useBackground)
        gset += "useBackground = true\n";
    else
        gset += "useBackground = false\n";
    gset += "backgroundPath = " + general.backgroundPath;
    gset += "\ndisplayScreen = " + QString::number(general.displayScreen);
    if(general.useDisplaySettings2)
        gset += "\nuseDisplaySettings2 = true";
    else
        gset += "\nuseDisplaySettings2 = false";
    if(general.useBackground2)
        gset += "\nuseBackground2 = true";
    else
        gset += "\nuseBackground2 = false";
    gset += "\nbackgroundPath2 = " + general.backgroundPath2;
    gset += "\ndisplayScreen2 = " + QString::number(general.displayScreen2);
    gset += "\ndcIconSize = " + QString::number(general.displayControls.buttonSize);
    gset += QString("\ndcAlignment = %1,%2").arg(general.displayControls.alignmentV).arg(general.displayControls.alignmentH);
    gset += "\ndcOpacity = " + QString::number(general.displayControls.opacity);

    // **** prepare bible, song and announce settings ************************
    bset = bibleSettingToString(bible); // prepare primary bible screen settings
    bset2 = bibleSettingToString(bible2); // prepare secondary bible screen settings

    sset = songSettingToString(song); // prepare primary song screen settings
    sset2 = songSettingToString(song2); // prepare secondary song screen settings

    aset = announceSettingToString(announce); // prepare primary announce screen settings
    aset2 = announceSettingToString(announce2); // prepare secondary announce screen settings

    // **** prepare SpMain settings *****************************************
    spset = "spSplitter = " + spmain.spSplitter.toHex();
    spset += "\nbibleHiddenSplitter = " + spmain.bibleHiddenSplitter.toHex();
    spset += "\nbibleShowSplitter = " + spmain.bibleShowSplitter.toHex();
    spset += "\nsongSplitter = " + spmain.songSplitter.toHex();
    spset += "\nuiTranslation = " + spmain.uiTranslation;
    if(spmain.isWindowMaximized)
        spset += "\nisWindowMaximized = true";
    else
        spset += "\nisWindowMaximized = false";

    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'general' AND user = '%2'").arg(gset).arg(user));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'bible' AND user = '%2'").arg(bset).arg(user));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'bible2' AND user = '%2'").arg(bset2).arg(user));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'song' AND user = '%2'").arg(sset).arg(user));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'song2' AND user = '%2'").arg(sset2).arg(user));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'announce' AND user = '%2'").arg(aset).arg(user));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'announce2' AND user = '%2'").arg(aset2).arg(user));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'spmain' AND user = '%2'").arg(spset).arg(user));

}

QString Settings::bibleSettingToString(BibleSettings &settings)
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

QString Settings::songSettingToString(SongSettings &settings)
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

QString Settings::announceSettingToString(AnnounceSettings &settings)
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

QByteArray Settings::textToByt(QString text)
{
    QByteArray b;
    b.insert(0,text);
    b = b.fromHex(b);
    return b;
}
