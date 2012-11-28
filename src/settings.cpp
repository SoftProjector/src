#include "settings.h"

GeneralSettings::GeneralSettings()
{
    // Apply General Defauls
    displayIsOnTop = false;
    useShadow = true;
    useFading = true;
    useBlurShadow = false;
    useBackground = false;
    backgroundPath = "";
    displayScreen = 0;
}

BibleSettings::BibleSettings()
{
    // Apply Bible Defaults
    primaryBible = "none";
    secondaryBible = "none";
    trinaryBible = "none";
    operatorBible = "same";
    useBackground = false;
    backgroundPath = "";
    textFont.fromString("Arial,28,-1,5,50,0,0,0,0,0");
    textColor = QColor(Qt::white);
    textAlingmentV = 0;
    textAlingmentH = 0;
    useAbbriviations = false;
    maxScreen = 100;
    maxScreenFrom = "bottom";
}

SongSettings::SongSettings()
{
    // Apply song defauls
    showStanzaTitle = false;
    showSongKey = false;
    showSongNumber = false;
    showSongEnding = true;
    songEndingType = 0;
    useBackground = false;
    backgroundPath = "";
    textFont.fromString("Arial,28,-1,5,50,0,0,0,0,0");
    textColor = QColor(Qt::white);
    textAlingmentV = 1;
    textAlingmentH = 1;
}

AnnounceSettings::AnnounceSettings()
{
    // Apply annouce defaults
    useBackground = false;
    backgroundPath = "";
    textFont.fromString("Arial,28,-1,5,50,0,0,0,0,0");
    textColor = QColor(Qt::white);
    textAlingmentV = 0;
    textAlingmentH = 1;
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
                else if (n == "useShadow")
                    general.useShadow = (v=="true");
                else if (n == "useFading")
                    general.useFading = (v=="true");
                else if (n == "useBlurShadow")
                    general.useBlurShadow = (v=="true");
                else if (n == "useBackground")
                    general.useBackground = (v=="true");
                else if (n == "backgroundPath")
                    general.backgroundPath = v;
                else if (n == "displayScreen")
                    general.displayScreen = v.toInt();
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
        {
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();

                if(n=="primaryBible")
                    bible.primaryBible = v;
                else if(n=="secondaryBible")
                    bible.secondaryBible = v;
                else if(n=="trinaryBible")
                    bible.trinaryBible = v;
                else if(n=="operatorBible")
                    bible.operatorBible = v;
                else if(n=="useBackground")
                    bible.useBackground = (v=="true");
                else if(n=="backgroundPath")
                    bible.backgroundPath = v;
                else if(n=="textAlingment")
                {
                    QStringList alinglist = v.split(",");
                    bible.textAlingmentV = alinglist.at(0).toInt();
                    bible.textAlingmentH = alinglist.at(1).toInt();
                }
                else if(n=="textColor")
                    bible.textColor = QColor::fromRgb(v.toUInt());
                else if(n=="textFont")
                    bible.textFont.fromString(v);
                else if(n=="useAbbriviations")
                    bible.useAbbriviations = (v=="true");
                else if(n=="maxScreen")
                    bible.maxScreen = v.toInt();
                else if(n=="maxScreenFrom")
                    bible.maxScreenFrom = v;
            }
        }
        else if(t == "song")
        {
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n=="showStanzaTitle")
                    song.showStanzaTitle = (v=="true");
                else if(n=="showSongKey")
                    song.showSongKey = (v=="true");
                else if(n=="showSongNumber")
                    song.showSongNumber = (v=="true");
                else if(n=="showSongEnding")
                    song.showSongEnding = (v=="true");
                else if(n=="songEndingType")
                    song.songEndingType = v.toInt();
                else if(n=="useBackground")
                    song.useBackground = (v=="true");
                else if(n=="backgroundPath")
                    song.backgroundPath = v;
                else if(n=="textAlingment")
                {
                    QStringList alinglist = v.split(",");
                    song.textAlingmentV = alinglist.at(0).toInt();
                    song.textAlingmentH = alinglist.at(1).toInt();
                }
                else if(n=="textColor")
                    song.textColor = QColor::fromRgb(v.toUInt());
                else if(n=="textFont")
                    song.textFont.fromString(v);

            }
        }
        else if(t == "announce")
        {
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n=="useBackground")
                    announce.useBackground = (v=="true");
                else if(n=="backgroundPath")
                    announce.backgroundPath = v;
                else if(n=="textAlingment")
                {
                    QStringList alinglist = v.split(",");
                    announce.textAlingmentV = alinglist.at(0).toInt();
                    announce.textAlingmentH = alinglist.at(1).toInt();
                }
                else if(n=="textColor")
                    announce.textColor = QColor::fromRgb(v.toUInt());
                else if(n=="textFont")
                    announce.textFont.fromString(v);
            }
        }
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

void Settings::saveSettings(QString user)
{
    QSqlQuery sq;
    QString gset,bset,sset,aset,spset;//general,bible,song,annouce,spmain

    // **** Prepare general settings ***************************************
    if(general.displayIsOnTop)
        gset = "displayIsOnTop = true\n";
    else
        gset = "displayIsOnTop = false\n";
    if(general.useShadow)
        gset += "useShadow = true\n";
    else
        gset += "useShadow = false\n";
    if(general.useFading)
        gset += "useFading = true\n";
    else
        gset += "useFading = false\n";
    if(general.useBlurShadow)
        gset += "useBlurShadow = true\n";
    else
        gset += "useBlurShadow = false\n";
    if(general.useBackground)
        gset += "useBackground = true\n";
    else
        gset += "useBackground = fasle\n";
    gset += "backgroundPath = " + general.backgroundPath;
    gset += "\ndisplayScreen = " + QString::number(general.displayScreen);
    gset += "\ndcIconSize = " + QString::number(general.displayControls.buttonSize);
    gset += QString("\ndcAlignment = %1,%2").arg(general.displayControls.alignmentV).arg(general.displayControls.alignmentH);
    gset += "\ndcOpacity = " + QString::number(general.displayControls.opacity);

    // **** Prepare bible settings *********************************************************
    bset = "primaryBible = " + bible.primaryBible;
    bset += "\nsecondaryBible = " + bible.secondaryBible;
    bset += "\ntrinaryBible = " + bible.trinaryBible;
    bset += "\noperatorBible = " + bible.operatorBible;

    // Background
    if(bible.useBackground)
        bset += "\nuseBackground = true";
    else
        bset += "\nuseBackground = false";

    bset += "\nbackgroundPath = " + bible.backgroundPath;

    // Text alingment
    bset += "\ntextAlingment = " + QString::number(bible.textAlingmentV) +
            "," + QString::number(bible.textAlingmentH);

    // Text color
    unsigned int textColorInt = (unsigned int)(bible.textColor.rgb());
    bset += "\ntextColor = " + QString::number(textColorInt);

    // Text font
    bset += "\ntextFont = " + bible.textFont.toString();

    // Version abbriviations
    if(bible.useAbbriviations)
        bset += "\nuseAbbriviations = true";
    else
        bset += "\nuseAbbriviations = false";

    // Max screen
    bset += "\nmaxScreen = " + QString::number(bible.maxScreen);
    bset += "\nmaxScreenFrom = " + bible.maxScreenFrom;

    // **** Prepare song settings ************************************************
    if(song.showStanzaTitle)
        sset = "showStanzaTitle = true\n";
    else
        sset = "showStanzaTitle = false\n";
    if(song.showSongKey)
        sset += "showSongKey = true\n";
    else
        sset += "showSongKey = false\n";
    if(song.showSongNumber)
        sset += "showSongNumber = true\n";
    else
        sset += "showSongNumber = false\n";
    if(song.showSongEnding)
        sset += "showSongEnding = true\n";
    else
        sset += "showSongEnding = false\n";
    sset += "songEndingType = " + QString::number(song.songEndingType);

    if(song.useBackground)
        sset += "\nuseBackground = true";
    else
        sset += "\nuseBackground = false";

    sset += "\nbackgroundPath = " + song.backgroundPath;

    // Text alingment
    sset += "\ntextAlingment = " + QString::number(song.textAlingmentV) +
            "," + QString::number(song.textAlingmentH);

    // Text color
    textColorInt = (unsigned int)(song.textColor.rgb());
    sset += "\ntextColor = " + QString::number(textColorInt);

    // Text font
    sset += "\ntextFont = " + song.textFont.toString();

    // **** prepare announcement settings ************************************
    // background
    (announce.useBackground)? aset = "useBackground = true" : aset = "useBackground = false";
    aset += "\nbackgroundPath = " + announce.backgroundPath;

    // Text alingment
    aset += "\ntextAlingment = " + QString::number(announce.textAlingmentV) +
            "," + QString::number(announce.textAlingmentH);

    // Text color
    textColorInt = (unsigned int)(announce.textColor.rgb());
    aset += "\ntextColor = " + QString::number(textColorInt);

    // Text font
    aset += "\ntextFont = " + announce.textFont.toString();

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
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'song' AND user = '%2'").arg(sset).arg(user));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'announce' AND user = '%2'").arg(aset).arg(user));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'spmain' AND user = '%2'").arg(spset).arg(user));

}

QByteArray Settings::textToByt(QString text)
{
    QByteArray b;
    b.insert(0,text);
    b = b.fromHex(b);
    return b;
}
