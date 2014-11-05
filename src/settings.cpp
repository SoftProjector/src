/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2014  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

#include "settings.h"

TextSettings::TextSettings()
{
    textFont.fromString("Arial,15,-1,5,50,0,0,0,0,0");
    commonFont = true;
    textColor = QColor(Qt::white);
    textShadowColor = QColor(Qt::black);
    commonColor = true;
    textAlingmentV = 0;
    textAlingmentH = 0;
    useBackground = false;
    backgroundType = 0;
    backgroundColor = QColor(Qt::black);
    backgroundName = "";
    background = QPixmap(1,1);
    backgroundVideoPath = "";
    screenUse = 100;
    screenPosition = 1;
    commonLayout = true;
    transitionType = 0;
    useFading = true;
    effectsType = 1;
    useShadow = true;
    useBluredShadow = false;
    useSameForDisp2 = true;
    useDisp2Settings = false;
}

BibleSettings::BibleSettings()//:TextSettings()
{
    transitionType = -1;
    effectsType = -1;
    backgroundType = -1;
    captionFont.fromString("Arial,36,-1,5,50,0,0,0,0,0");
    captionColor = QColor(Qt::white);
    captionAlingment = 2;
    captionPosition = 1;
    useAbbriviation = false;
}

SongSettings::SongSettings()//:TextSettings()
{
    textAlingmentV = 1;
    textAlingmentH = 1;
    transitionType = -1;
    effectsType = -1;
    backgroundType = -1;
    showStanzaTitle = false;
    showSongKey = false;
    showSongNumber = false;
    showSongEnding = true;
    infoColor = QColor(Qt::white);
    infoFont.fromString("Arial,36,-1,5,50,0,0,0,0,0");
    infoAling = 0;
    endingColor = QColor(Qt::white);
    endingFont.fromString("Arial,48,-1,5,50,0,0,0,0,0");
    endingType = 0;
    endingPosition = 1;
}

GeneralSettings::GeneralSettings()
{
    // Apply General Defauls
    displayIsOnTop = false;
    displayScreen = 0;
    displayScreen2 = -1; // interger "-1" mean "None" or not to display
    currentThemeId = 0;
}

DisplayControlsSettings::DisplayControlsSettings()
{
    buttonSize = 3; // 0=16,1=24,2=32,3=48,4=64,5=96
    alignmentV = 2;
    alignmentH = 1;
    opacity = 0.3;
}

SpSettings::SpSettings()
{
    // Apply main window defaults
    isWindowMaximized = false;
    uiTranslation = "en";
}

BibleVersionSettings::BibleVersionSettings()
{
    primaryBible = "none";
    secondaryBible = "none";
    trinaryBible = "none";
    operatorBible = "same";
}

SlideShowSettings::SlideShowSettings()
{
    expandSmall = true;
    fitType = 0;
    resize = true;
    boundType = 2;
    boundWidth = 1280;
}

Settings::Settings()
{

}

void Settings::loadSettings()
{
    QString t,n,v,s,sets; // type, name, value, userValues
    QStringList set,values;
    bool dataGenOk,dataSpOk,dataB1Ok,dataB2Ok, dataPixOk;
    dataGenOk = dataSpOk = dataB1Ok = dataB2Ok = dataPixOk = false;
    QSqlQuery sq;
    sq.exec(QString("SELECT type, sets FROM Settings "));
    while (sq.next())
    {
        t = sq.value(0).toString();
        sets = sq.value(1).toString();

        if(t == "general") // set general setting values
        {
            dataGenOk = true;
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n == "displayIsOnTop")
                    general.displayIsOnTop = (v=="true");
                else if(n == "currentThemeId")
                    general.currentThemeId = v.toInt();
                else if (n == "displayScreen")
                    general.displayScreen = v.toInt();
                // Display Screen 2 settings
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
        else if(t == "spMain")
        {
            dataSpOk = true;
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();

                if(n=="spSplitter")
                    spMain.spSplitter = textToByt(v);
                else if(n=="bibleHiddenSplitter")
                    spMain.bibleHiddenSplitter = textToByt(v);
                else if(n=="bibleShowSplitter")
                    spMain.bibleShowSplitter = textToByt(v);
                else if(n=="songSplitter")
                    spMain.songSplitter = textToByt(v);
                else if(n=="uiTranslation")
                    spMain.uiTranslation = v;
                else if(n=="isWindowMaximized")
                    spMain.isWindowMaximized = (v=="true");
            }
        }
        else if(t == "bible1")
        {
            dataB1Ok = true;
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n == "primary")
                    bibleSets.primaryBible = v;
                else if(n == "secondary")
                    bibleSets.secondaryBible = v;
                else if (n == "trinary")
                    bibleSets.trinaryBible = v;
                else if (n == "operator")
                    bibleSets.operatorBible = v;
            }
        }
        else if(t == "bible2")
        {
            dataB2Ok = true;
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n == "primary")
                    bibleSets2.primaryBible = v;
                else if(n == "secondary")
                    bibleSets2.secondaryBible = v;
                else if (n == "trinary")
                    bibleSets2.trinaryBible = v;
            }
        }
        else if(t == "pix")
        {
            dataPixOk = true;
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n == "expandSmall")
                    slideSets.expandSmall = (v == "true");
                else if(n == "fitType")
                    slideSets.fitType = v.toInt();
                else if (n == "resize")
                    slideSets.resize = (v == "true");
                else if (n == "boundType")
                    slideSets.boundType = v.toInt();
                else if (n == "boundWidth")
                    slideSets.boundWidth = v.toInt();
            }
        }
    }

    // if no data exist, then create
    if(!dataGenOk || !dataSpOk || !dataB1Ok || !dataB2Ok || !dataPixOk)
        saveNewSettings();
}

void Settings::saveSettings()
{
    QSqlQuery sq;
    QString gset,spset,b1set,b2set,pset;//general,bible,song,annouce,spmain

    // **** Prepare general settings ***************************************
    if(general.displayIsOnTop)
        gset = "displayIsOnTop = true";
    else
        gset = "displayIsOnTop = false";
    gset += "\ncurrentThemeId = " + QString::number(general.currentThemeId);
    gset += "\ndisplayScreen = " + QString::number(general.displayScreen);
    gset += "\ndisplayScreen2 = " + QString::number(general.displayScreen2);
    gset += "\ndcIconSize = " + QString::number(general.displayControls.buttonSize);
    gset += QString("\ndcAlignment = %1,%2").arg(general.displayControls.alignmentV).arg(general.displayControls.alignmentH);
    gset += "\ndcOpacity = " + QString::number(general.displayControls.opacity);

    // **** prepare softProjector main settings
    spset += "spSplitter = " + spMain.spSplitter.toHex();
    spset += "\nbibleHiddenSplitter = " + spMain.bibleHiddenSplitter.toHex();
    spset += "\nbibleShowSplitter = " + spMain.bibleShowSplitter.toHex();
    spset += "\nsongSplitter = " + spMain.songSplitter.toHex();
    spset += "\nuiTranslation = " + spMain.uiTranslation;
    if(spMain.isWindowMaximized)
        spset += "\nisWindowMaximized = true";
    else
        spset += "\nisWindowMaximized = false";

    // **** prepare screen 1 bible versions
    b1set = "primary = " + bibleSets.primaryBible;
    b1set += "\nsecondary = " + bibleSets.secondaryBible;
    b1set += "\ntrinary = " + bibleSets.trinaryBible;
    b1set += "\noperator = " + bibleSets.operatorBible;

    // **** prepare screen 1 bible versions
    b2set = "primary = " + bibleSets2.primaryBible;
    b2set += "\nsecondary = " + bibleSets2.secondaryBible;
    b2set += "\ntrinary = " + bibleSets2.trinaryBible;

    // **** prepare pix settings
    if(slideSets.expandSmall)
        pset = "expandSmall = true";
    else
        pset = "expandSmall = false";
    pset += "\nfitType = " + QString::number(slideSets.fitType);
    if(slideSets.resize)
        pset += "\nresize = true";
    else
        pset += "\nresize = false";
    pset += "\nboundType = " + QString::number(slideSets.boundType);
    pset += "\nboundWidth = " + QString::number(slideSets.boundWidth);

    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'general'").arg(gset));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'spMain'").arg(spset));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'bible1'").arg(b1set));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'bible2'").arg(b2set));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'pix'").arg(pset));
}

void Settings::saveNewSettings()
{
    QSqlQuery sq;
    sq.exec("INSERT OR REPLACE INTO Settings (type, sets) VALUES ('general', 'n=v')");
    sq.exec("INSERT OR REPLACE INTO Settings (type, sets) VALUES ('spMain', 'n=v')");
    sq.exec("INSERT OR REPLACE INTO Settings (type, sets) VALUES ('bible1', 'n=v')");
    sq.exec("INSERT OR REPLACE INTO Settings (type, sets) VALUES ('bible2', 'n=v')");
    sq.exec("INSERT OR REPLACE INTO Settings (type, sets) VALUES ('pix', 'n=v')");

    saveSettings();
}

QByteArray Settings::textToByt(QString text)
{
    QByteArray b;
    b.insert(0,text);
    b = b.fromHex(b);
    return b;
}
