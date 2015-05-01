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

TextSettingsBase::TextSettingsBase()
{
    id = "-1";
    themeId = -1;
    textFont.fromString("Arial,16,-1,5,50,0,0,0,0,0");
    textColor = QColor(Qt::white);
    textShadowColor = QColor(Qt::black);
    textAlingmentV = 0;
    textAlingmentH = 0;
    backgroundType = 0;
    backgroundColor = QColor(Qt::black);
    backgroundName = "";
    backgroundPix = QPixmap(1,1);
    backgroundVideoPath = "";
    screenUse = 100;
    screenPosition = 0;
    transitionType = 0;
    effectsType = 1;
    useSameForDisp2 = true;
    setBaseChangeHandles();
}

TextSettings::TextSettings()
{
    isNotCommonFont = false;
    isNotCommonColor = false;
    isNotCommonLayout = false;
    transitionType = -1;
    effectsType = -1;
    backgroundType = -1;
    setMainChangeHandles();
}

BibleSettings::BibleSettings()
{
    captionFont.fromString("Arial,15,-1,5,50,0,0,0,0,0");
    isNotSameFont = true;
    captionColor = QColor(Qt::white);
    isNotSameColor = false;
    captionShadowColor = QColor(Qt::black);
    captionAlingment = 2;
    captionPosition = 1;
    useAbbriviation = false;
    setChangeHandes();
}

SongSettings::SongSettings()
{
    textAlingmentV = 1;
    textAlingmentH = 1;
    showStanzaTitle = false;
    showSongKey = false;
    showSongNumber = false;
    showSongEnding = true;
    isNotSameInfoFont = false;
    isNotSameInfoColor = false;
    infoColor = QColor(Qt::white);
    infoShadowColor = QColor(Qt::black);
    infoFont.fromString("Arial,15,-1,5,50,0,0,0,0,0");
    infoAling = 0;
    isNotSameEndingFont = false;
    isNotSameEndingColor = false;
    endingColor = QColor(Qt::white);
    endingShadowColor = QColor(Qt::black);
    endingFont.fromString("Arial,16,-1,5,50,0,0,0,0,0");
    endingType = 0;
    endingPosition = 0;
    setChangeHandes();
}

void saveIndividualSettings(QSqlQuery &sq, QString sId, int tId, QString name, const QVariant &value)
{
    sq.addBindValue(sId);
    if(tId!=-2)
        sq.addBindValue(tId);
    sq.addBindValue(name);
    sq.addBindValue(value);
    sq.exec();
    qDebug()<<"save:"<<name<<sId<<tId<<value;
}

void updateIndividualSettings(QSqlQuery &sq, QString sId, int tId, QString name, const QVariant &value)
{
    sq.addBindValue(value);
    sq.addBindValue(sId);
    if(tId!=-2)
    {
        sq.addBindValue(tId);
        qDebug()<<"TID";
    }
    sq.addBindValue(name);
    sq.exec();
    qDebug()<<"update:"<<name<<sId<<tId<<value;
}

//******************************************************************/
//********************* TextSettingsBase ***************************/
//******************************************************************/
void TextSettingsBase::setBaseChangeHandles()
{
    isChangedTextFont = isChangedTextColor = isChangedTextShadowColor =
            isChangedAlingV = isChangedAlingH = isChangesTranType =
            isChangedEffectType = isChangedBackType = isChangedBackColor =
            isChangedBackPix = isChangedBackVid = isChangedScreenUse =
            isChangedScreenPos = isChangedSameDisp2 = true;
}

void TextSettingsBase::resetBaseChangeHandles()
{
    isChangedTextFont = isChangedTextColor = isChangedTextShadowColor =
            isChangedAlingV = isChangedAlingH = isChangesTranType =
            isChangedEffectType = isChangedBackType = isChangedBackColor =
            isChangedBackPix = isChangedBackVid = isChangedScreenUse =
            isChangedScreenPos = isChangedSameDisp2 = false;
}

void TextSettingsBase::loadBase()
{
    QSqlQuery sq;
    loadBase(sq);
}

void TextSettingsBase::loadBase(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    QString n;
    sq.exec(QString("SELECT sname, svalue, sdata FROM ThemeData WHERE sid = '%1' AND theme_id = %2").arg(id).arg(themeId));
    while(sq.next())
    {
        n = sq.value(0).toString();
        if(n == "textFont")
            textFont.fromString(sq.value(1).toString());
        else if(n == "textColor")
            textColor = QColor(sq.value(1).toUInt());
        else if(n == "textShadowColor")
            textShadowColor = QColor(sq.value(1).toUInt());
        else if(n == "textAlingmentV")
            textAlingmentV = sq.value(1).toInt();
        else if(n == "textAlingmentH")
            textAlingmentH = sq.value(1).toInt();
        else if(n == "backgroundType")
            backgroundType = sq.value(1).toInt();
        else if(n == "backgroundColor")
            backgroundColor = QColor(sq.value(1).toUInt());
        else if(n == "backgroundPix")
        {
            backgroundName = sq.value(1).toString();
            backgroundPix.loadFromData(sq.value(2).toByteArray());
        }
        else if(n == "backgroundVideoPath")
            backgroundVideoPath = sq.value(1).toString();
        else if(n == "screenUse")
            screenUse = sq.value(1).toInt();
        else if(n == "screenPosition")
            screenPosition = sq.value(1).toInt();
        else if(n == "transitionType")
            transitionType = sq.value(1).toInt();
        else if(n == "effectsType")
            effectsType = sq.value(1).toInt();
        else if(n == "useSameForDisp2")
            useSameForDisp2 = sq.value(1).toBool();
    }
    resetBaseChangeHandles();
}

void TextSettingsBase::saveBase()
{
    QSqlQuery sq;
    saveBase(sq);
}

void TextSettingsBase::saveBase(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    sq.prepare("INSERT INTO ThemeData (sid, theme_id, sname, svalue) VALUES (?,?,?,?)");

    saveIndividualSettings(sq,id,themeId,"textFont",textFont.toString());
    saveIndividualSettings(sq,id,themeId,"textColor",textColor.rgb());
    saveIndividualSettings(sq,id,themeId,"textShadowColor",textShadowColor.rgb());
    saveIndividualSettings(sq,id,themeId,"textAlingmentV",textAlingmentV);
    saveIndividualSettings(sq,id,themeId,"textAlingmentH",textAlingmentH);
    saveIndividualSettings(sq,id,themeId,"transitionType",transitionType);
    saveIndividualSettings(sq,id,themeId,"effectsType",effectsType);
    saveIndividualSettings(sq,id,themeId,"backgroundType",backgroundType);
    saveIndividualSettings(sq,id,themeId,"backgroundColor",backgroundColor.rgb());
    saveIndividualSettings(sq,id,themeId,"backgroundVideoPath",backgroundVideoPath);
    saveIndividualSettings(sq,id,themeId,"screenUse",screenUse);
    saveIndividualSettings(sq,id,themeId,"screenPosition",screenPosition);
    saveIndividualSettings(sq,id,themeId,"useSameForDisp2",useSameForDisp2);

    sq.prepare("INSERT INTO ThemeData (sid, theme_id, sname, svalue, sdata) VALUES (?,?,?,?,?)");
    sq.addBindValue(id);
    sq.addBindValue(themeId);
    sq.addBindValue("backgroundPix");
    sq.addBindValue(backgroundName);
    sq.addBindValue(pixToByte(backgroundPix));
    sq.exec();

    resetBaseChangeHandles();
}

void TextSettingsBase::updateBase()
{
    QSqlQuery sq;
    updateBase(sq);
}

void TextSettingsBase::updateBase(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    sq.prepare("UPDATE ThemeData SET svalue = ? WHERE sid = ? AND theme_id = ? AND sname = ?");

    if(isChangedTextFont)
        updateIndividualSettings(sq,id,themeId,"textFont",textFont.toString());

    if(isChangedTextColor)
        updateIndividualSettings(sq,id,themeId,"textColor",textColor.rgb());

    if(isChangedTextShadowColor)
        updateIndividualSettings(sq,id,themeId,"textShadowColor",textShadowColor.rgb());

    if(isChangedAlingV)
        updateIndividualSettings(sq,id,themeId,"textAlingmentV",textAlingmentV);

    if(isChangedAlingH)
        updateIndividualSettings(sq,id,themeId,"textAlingmentH",textAlingmentH);

    if(isChangesTranType)
        updateIndividualSettings(sq,id,themeId,"transitionType",transitionType);

    if(isChangedEffectType)
        updateIndividualSettings(sq,id,themeId,"effectsType",effectsType);

    if(isChangedBackType)
        updateIndividualSettings(sq,id,themeId,"backgroundType",backgroundType);

    if(isChangedBackColor)
        updateIndividualSettings(sq,id,themeId,"backgroundColor",backgroundColor.rgb());

    if(isChangedBackVid)
        updateIndividualSettings(sq,id,themeId,"backgroundVideoPath",backgroundVideoPath);

    if(isChangedScreenUse)
        updateIndividualSettings(sq,id,themeId,"screenUse",screenUse);

    if(isChangedScreenPos)
        updateIndividualSettings(sq,id,themeId,"screenPosition",screenPosition);

    if(isChangedSameDisp2)
        updateIndividualSettings(sq,id,themeId,"useSameForDisp2",useSameForDisp2);

    if(isChangedBackPix)
    {
        sq.prepare("UPDATE ThemeData SET svalue = ?, sdata = ? WHERE sid = ? AND theme_id = ? AND sname = ?");
        sq.addBindValue(backgroundName);
        sq.addBindValue(pixToByte(backgroundPix));
        sq.addBindValue(id);
        sq.addBindValue(themeId);
        sq.addBindValue("backgroundPix");
        sq.exec();
        qDebug()<<"backgroundPix Updated"<<id;
    }

    resetBaseChangeHandles();
}

//******************************************************************/
//********************* TextSettings *******************************/
//******************************************************************/
void TextSettings::setMainChangeHandles()
{
    setBaseChangeHandles();
    isChangedNotFont = isChangedNotColor = isChangedNotLayout = true;
}

void TextSettings::resetMainChangeHandles()
{
    resetBaseChangeHandles();
    isChangedNotFont = isChangedNotColor = isChangedNotLayout = false;
}

void TextSettings::loadMain()
{
    QSqlQuery sq;
    loadMain(sq);
}

void TextSettings::loadMain(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    loadBase(sq);

    QString n;
    sq.exec(QString("SELECT sname, svalue, sdata FROM ThemeData WHERE sid = '%1' AND theme_id = %2").arg(id).arg(themeId));
    while(sq.next())
    {
        n = sq.value(0).toString();
        if(n == "isNotCommonFont")
            isNotCommonFont = sq.value(1).toBool();
        else if(n == "isNotCommonColor")
            isNotCommonColor = sq.value(1).toBool();
        else if(n == "isNotCommonLayout")
            isNotCommonLayout = sq.value(1).toBool();
    }
    resetMainChangeHandles();
}

void TextSettings::saveMain()
{
    QSqlQuery sq;
    saveMain(sq);
}

void TextSettings::saveMain(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    saveBase(sq);

    sq.prepare("INSERT INTO ThemeData (sid, theme_id, sname, svalue) VALUES (?,?,?,?)");

    saveIndividualSettings(sq,id,themeId,"isNotCommonFont",isNotCommonFont);
    saveIndividualSettings(sq,id,themeId,"isNotCommonColor",isNotCommonColor);
    saveIndividualSettings(sq,id,themeId,"isNotCommonLayout",isNotCommonLayout);
}

void TextSettings::updateMain()
{
    QSqlQuery sq;
    updateMain(sq);
}

void TextSettings::updateMain(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    updateBase(sq);

    sq.prepare("UPDATE ThemeData SET svalue = ? WHERE sid = ? AND theme_id = ? AND sname = ?");

    if(isChangedNotFont)
        updateIndividualSettings(sq,id,themeId,"isNotCommonFont",isNotCommonFont);

    if(isChangedNotColor)
        updateIndividualSettings(sq,id,themeId,"isNotCommonColor",isNotCommonColor);

    if(isChangedNotLayout)
        updateIndividualSettings(sq,id,themeId,"isNotCommonLayout",isNotCommonLayout);
}

//******************************************************************/
//********************* BibleSettings ******************************/
//******************************************************************/
void BibleSettings::setChangeHandes()
{
    setMainChangeHandles();
    isChangedCapFont = isChangedCapColor = isChangedCapShadColor =
            isChangedNotSameFont = isChangedNotSameColor = isChangedCapAlign =
            isChangedCapPos = isChangedUseAbbriv = true;
}

void BibleSettings::resetChangeHandles()
{
    resetMainChangeHandles();
    isChangedCapFont = isChangedCapColor = isChangedCapShadColor =
            isChangedNotSameFont = isChangedNotSameColor = isChangedCapAlign =
            isChangedCapPos = isChangedUseAbbriv = false;
}

void BibleSettings::load()
{
    QSqlQuery sq;
    load(sq);
}

void BibleSettings::load(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    loadMain(sq);

    QString n;
    sq.exec(QString("SELECT sname, svalue, sdata FROM ThemeData WHERE sid = '%1' AND theme_id = %2").arg(id).arg(themeId));
    while(sq.next())
    {
        n = sq.value(0).toString();
        if(n == "captionFont")
            captionFont.fromString(sq.value(1).toString());
        else if(n == "isNotSameFont")
            isNotSameFont = sq.value(1).toBool();
        else if(n == "captionColor")
            captionColor = QColor(sq.value(1).toUInt());
        else if(n == "isNotSameColor")
            isNotSameColor = sq.value(1).toBool();
        else if(n == "captionShadowColor")
            captionShadowColor = QColor(sq.value(1).toUInt());
        else if(n == "captionAlingment")
            captionAlingment = sq.value(1).toInt();
        else if(n == "captionPosition")
            captionPosition = sq.value(1).toInt();
        else if(n == "useAbbriviation")
            useAbbriviation = sq.value(1).toBool();
    }
    resetChangeHandles();
}

void BibleSettings::save()
{
    QSqlQuery sq;
    save(sq);
}

void BibleSettings::save(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    saveMain(sq);

    sq.prepare("INSERT INTO ThemeData (sid, theme_id, sname, svalue) VALUES (?,?,?,?)");

    saveIndividualSettings(sq,id,themeId,"captionFont",captionFont.toString());
    saveIndividualSettings(sq,id,themeId,"isNotSameFont",isNotSameFont);
    saveIndividualSettings(sq,id,themeId,"captionColor",captionColor.rgb());
    saveIndividualSettings(sq,id,themeId,"isNotSameColor",isNotSameColor);
    saveIndividualSettings(sq,id,themeId,"captionShadowColor",captionShadowColor.rgb());
    saveIndividualSettings(sq,id,themeId,"captionAlingment",captionAlingment);
    saveIndividualSettings(sq,id,themeId,"captionPosition",captionPosition);
    saveIndividualSettings(sq,id,themeId,"useAbbriviation",useAbbriviation);
}

void BibleSettings::update()
{
    QSqlQuery sq;
    update(sq);
}

void BibleSettings::update(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    updateMain(sq);

    sq.prepare("UPDATE ThemeData SET svalue = ? WHERE sid = ? AND theme_id = ? AND sname = ?");

    if(isChangedCapFont)
        updateIndividualSettings(sq,id,themeId,"captionFont",captionFont.toString());

    if(isChangedNotSameFont)
        updateIndividualSettings(sq,id,themeId,"isNotSameFont",isNotSameFont);

    if(isChangedCapColor)
        updateIndividualSettings(sq,id,themeId,"captionColor",captionColor.rgb());

    if(isChangedNotSameColor)
        updateIndividualSettings(sq,id,themeId,"isNotSameColor",isNotSameColor);

    if(isChangedCapShadColor)
        updateIndividualSettings(sq,id,themeId,"captionShadowColor",captionShadowColor.rgb());

    if(isChangedCapAlign)
        updateIndividualSettings(sq,id,themeId,"captionAlingment",captionAlingment);

    if(isChangedCapPos)
        updateIndividualSettings(sq,id,themeId,"captionPosition",captionPosition);

    if(isChangedUseAbbriv)
        updateIndividualSettings(sq,id,themeId,"useAbbriviation",useAbbriviation);
}

//******************************************************************/
//********************* SongSettings *******************************/
//******************************************************************/
void SongSettings::setChangeHandes()
{
    setMainChangeHandles();
    isChangedShowTitle = isChangedShowKey = isChangedShowNum = isChangedShowEnding =
            isChangedInfoFont = isChangedInfoColor = isChangedInfoShadColor =
            isChangedNotSameInfoFont = isChangedNotSameInfoColor = isChangedInfoAlign =
            isChangedEndingFont = isChangedEndingColor = isChangedEndingShadColor =
            isChangedNotSameEndingFont = isChangedNotSameEndingColor =
            isChangedEndingType = isChangedEndingPosition = true;
}

void SongSettings::resetChangeHandles()
{
    resetMainChangeHandles();
    isChangedShowTitle = isChangedShowKey = isChangedShowNum = isChangedShowEnding =
            isChangedInfoFont = isChangedInfoColor = isChangedInfoShadColor =
            isChangedNotSameInfoFont = isChangedNotSameInfoColor = isChangedInfoAlign =
            isChangedEndingFont = isChangedEndingColor = isChangedEndingShadColor =
            isChangedNotSameEndingFont = isChangedNotSameEndingColor =
            isChangedEndingType = isChangedEndingPosition = false;
}

void SongSettings::load()
{
    QSqlQuery sq;
    load(sq);
}

void SongSettings::load(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    loadMain(sq);

    QString n;
    sq.exec(QString("SELECT sname, svalue, sdata FROM ThemeData WHERE sid = '%1' AND theme_id = %2").arg(id).arg(themeId));
    while(sq.next())
    {
        n = sq.value(0).toString();
        if(n == "showStanzaTitle")
            showStanzaTitle = sq.value(1).toBool();
        else if(n == "showSongKey")
            showSongKey = sq.value(1).toBool();
        else if(n == "showSongNumber")
            showSongNumber = sq.value(1).toBool();
        else if(n == "showSongEnding")
            showSongEnding = sq.value(1).toBool();
        else if(n == "infoFont")
            infoFont.fromString(sq.value(1).toString());
        else if(n == "infoColor")
            infoColor = QColor(sq.value(1).toUInt());
        else if(n == "infoShadowColor")
            infoShadowColor = QColor(sq.value(1).toUInt());
        else if(n == "isNotSameInfoFont")
            isNotSameInfoFont = sq.value(1).toBool();
        else if(n == "isNotSameInfoColor")
            isNotSameInfoColor = sq.value(1).toBool();
        else if(n == "infoAling")
            infoAling = sq.value(1).toInt();
        else if(n == "endingFont")
            endingFont.fromString(sq.value(1).toString());
        else if(n == "endingColor")
            endingColor = QColor(sq.value(1).toUInt());
        else if(n == "endingShadowColor")
            endingShadowColor = QColor(sq.value(1).toUInt());
        else if(n == "isNotSameEndingFont")
            isNotSameEndingFont = sq.value(1).toBool();
        else if(n == "isNotSameEndingColor")
            isNotSameEndingColor = sq.value(1).toBool();
        else if(n == "endingType")
            endingType = sq.value(1).toInt();
        else if(n == "endingPosition")
            endingPosition = sq.value(1).toInt();
    }
    resetChangeHandles();
}

void SongSettings::save()
{
    QSqlQuery sq;
    save(sq);
}

void SongSettings::save(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    saveMain(sq);

    sq.prepare("INSERT INTO ThemeData (sid, theme_id, sname, svalue) VALUES (?,?,?,?)");

    saveIndividualSettings(sq,id,themeId,"showStanzaTitle",showStanzaTitle);
    saveIndividualSettings(sq,id,themeId,"showSongKey",showSongKey);
    saveIndividualSettings(sq,id,themeId,"showSongNumber",showSongNumber);
    saveIndividualSettings(sq,id,themeId,"showSongEnding",showSongEnding);
    saveIndividualSettings(sq,id,themeId,"infoFont",infoFont.toString());
    saveIndividualSettings(sq,id,themeId,"infoColor",infoColor.rgb());
    saveIndividualSettings(sq,id,themeId,"infoShadowColor",infoShadowColor.rgb());
    saveIndividualSettings(sq,id,themeId,"isNotSameInfoFont",isNotSameInfoFont);
    saveIndividualSettings(sq,id,themeId,"isNotSameInfoColor",isNotSameInfoColor);
    saveIndividualSettings(sq,id,themeId,"infoAling",infoAling);
    saveIndividualSettings(sq,id,themeId,"endingFont",endingFont.toString());
    saveIndividualSettings(sq,id,themeId,"endingColor",endingColor.rgb());
    saveIndividualSettings(sq,id,themeId,"endingShadowColor",endingShadowColor.rgb());
    saveIndividualSettings(sq,id,themeId,"isNotSameEndingFont",isNotSameEndingFont);
    saveIndividualSettings(sq,id,themeId,"isNotSameEndingColor",isNotSameEndingColor);
    saveIndividualSettings(sq,id,themeId,"endingType",endingType);
    saveIndividualSettings(sq,id,themeId,"endingPosition",endingPosition);
}

void SongSettings::update()
{
    QSqlQuery sq;
    update(sq);
}

void SongSettings::update(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    updateMain(sq);

    sq.prepare("UPDATE ThemeData SET svalue = ? WHERE sid = ? AND theme_id = ? AND sname = ?");

    if(isChangedShowTitle)
        updateIndividualSettings(sq,id,themeId,"showStanzaTitle",showStanzaTitle);

    if(isChangedShowKey)
        updateIndividualSettings(sq,id,themeId,"showSongKey",showSongKey);

    if(isChangedShowNum)
        updateIndividualSettings(sq,id,themeId,"showSongNumber",showSongNumber);

    if(isChangedShowEnding)
        updateIndividualSettings(sq,id,themeId,"showSongEnding",showSongEnding);

    if(isChangedInfoFont)
        updateIndividualSettings(sq,id,themeId,"infoFont",infoFont.toString());

    if(isChangedInfoColor)
        updateIndividualSettings(sq,id,themeId,"infoColor",infoColor.rgb());

    if(isChangedInfoShadColor)
        updateIndividualSettings(sq,id,themeId,"infoShadowColor",infoShadowColor.rgb());

    if(isChangedNotSameInfoFont)
        updateIndividualSettings(sq,id,themeId,"isNotSameInfoFont",isNotSameInfoFont);

    if(isChangedNotSameInfoColor)
        updateIndividualSettings(sq,id,themeId,"isNotSameInfoColor",isNotSameInfoColor);

    if(isChangedInfoAlign)
        updateIndividualSettings(sq,id,themeId,"infoAling",infoAling);

    if(isChangedEndingFont)
        updateIndividualSettings(sq,id,themeId,"endingFont",endingFont.toString());

    if(isChangedEndingColor)
        updateIndividualSettings(sq,id,themeId,"endingColor",endingColor.rgb());

    if(isChangedEndingShadColor)
        updateIndividualSettings(sq,id,themeId,"endingShadowColor",endingShadowColor.rgb());

    if(isChangedNotSameEndingFont)
        updateIndividualSettings(sq,id,themeId,"isNotSameEndingFont",isNotSameEndingFont);

    if(isChangedNotSameEndingColor)
        updateIndividualSettings(sq,id,themeId,"isNotSameEndingColor",isNotSameEndingColor);

    if(isChangedEndingType)
        updateIndividualSettings(sq,id,themeId,"endingType",endingType);

    if(isChangedEndingPosition)
        updateIndividualSettings(sq,id,themeId,"endingPosition",endingPosition);
}

/*  **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
*/
GeneralSettings::GeneralSettings()
{
    // Apply General Defauls
    displayIsOnTop = false;
    displayScreen = 0;
    displayScreen2 = -1; // interger "-1" means "None" or not to display
    currentThemeId = 0;
    displayOnStartUp = false;
    settingsChangedAll = false;
    settingsChangedMulti = false;
    settingsChangedSingle = false;
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
    settingsChanged = false;
}

SlideShowSettings::SlideShowSettings()
{
    expandSmall = true;
    fitType = 0;
    resize = true;
    boundType = 2;
    boundWidth = 1280;
    settingsChanged = false;
    transitionType = 0;
}

Settings::Settings()
{
    isSpClosing = false;
}

void Settings::loadSettings()
{
    QString t,n; // type, name
    bool dataGenOk,dataSpOk,dataB1Ok,dataB2Ok, dataPixOk;
    dataGenOk = dataSpOk = dataB1Ok = dataB2Ok = dataPixOk = false;
    QSqlQuery sq;
    sq.exec(QString("SELECT type, sname, svalue FROM Settings "));
    while (sq.next())
    {
        t = sq.value(0).toString();
        n = sq.value(1).toString();

        if(t == "general")
        {
            dataGenOk = true;
            if(n == "displayIsOnTop")
                general.displayIsOnTop = sq.value(2).toBool();
            else if(n == "currentThemeId")
                general.currentThemeId = sq.value(2).toInt();
            else if (n == "displayScreen")
                general.displayScreen = sq.value(2).toInt();
            else if (n == "displayScreen2")
                general.displayScreen2 = sq.value(2).toInt();
            else if (n == "displayOnStartUp")
                general.displayOnStartUp = sq.value(2).toBool();
            else if (n == "dcIconSize")
                general.displayControls.buttonSize = sq.value(2).toInt();
            else if (n == "dcAlignmentV")
                general.displayControls.alignmentV = sq.value(2).toInt();
            else if (n == "dcAlignmentH")
                general.displayControls.alignmentH = sq.value(2).toInt();
            else if (n == "dcOpacity")
                general.displayControls.opacity = sq.value(2).toDouble();
        }
        else if(t == "spMain")
        {
            dataSpOk = true;
            if(n == "spSplitter")
                spMain.spSplitter = sq.value(2).toByteArray();
            else if(n == "bibleHiddenSplitter")
                spMain.bibleHiddenSplitter = sq.value(2).toByteArray();
            else if(n == "bibleShowSplitter")
                spMain.bibleShowSplitter = sq.value(2).toByteArray();
            else if(n == "songSplitter")
                spMain.songSplitter = sq.value(2).toByteArray();
            else if(n == "uiTranslation")
                spMain.uiTranslation = sq.value(2).toString();
            else if(n == "isWindowMaximized")
                spMain.isWindowMaximized = sq.value(2).toBool();
        }
        else if(t == "bible1")
        {
            dataB1Ok = true;
            if(n == "primaryBible")
                bibleSets.primaryBible = sq.value(2).toString();
            else if(n == "secondaryBible")
                bibleSets.secondaryBible = sq.value(2).toString();
            else if (n == "trinaryBible")
                bibleSets.trinaryBible = sq.value(2).toString();
            else if (n == "operatorBible")
                bibleSets.operatorBible = sq.value(2).toString();
        }
        else if(t == "bible2")
        {
            dataB2Ok = true;
            if(n == "primaryBible")
                bibleSets2.primaryBible = sq.value(2).toString();
            else if(n == "secondaryBible")
                bibleSets2.secondaryBible = sq.value(2).toString();
            else if (n == "trinaryBible")
                bibleSets2.trinaryBible = sq.value(2).toString();
        }
        else if(t == "pix")
        {
            dataPixOk = true;
            if(n == "expandSmall")
                slideSets.expandSmall = sq.value(2).toBool();
            else if(n == "fitType")
                slideSets.fitType = sq.value(2).toInt();
            else if (n == "resize")
                slideSets.resize = sq.value(2).toBool();
            else if (n == "boundType")
                slideSets.boundType = sq.value(2).toInt();
            else if (n == "boundWidth")
                slideSets.boundWidth = sq.value(2).toInt();
            else if (n == "transitionType")
                slideSets.transitionType = sq.value(2).toInt();
        }
    }

    // if no data exist, then create
    if(!dataGenOk || !dataSpOk || !dataB1Ok || !dataB2Ok || !dataPixOk)
        saveNewSettings();
}

void Settings::saveSettings()
{
    QSqlQuery sq;
    sq.prepare("UPDATE Settings SET svalue = ? WHERE type = ? AND sname = ?");

    if(general.settingsChangedAll)
    {
    updateIndividualSettings(sq,"general",-2,"displayIsOnTop",general.displayIsOnTop);
    updateIndividualSettings(sq,"general",-2,"currentThemeId",general.currentThemeId);
    }
    if(general.settingsChangedMulti)
    {
    updateIndividualSettings(sq,"general",-2,"displayScreen",general.displayScreen);
    updateIndividualSettings(sq,"general",-2,"displayScreen2",general.displayScreen2);
    updateIndividualSettings(sq,"general",-2,"displayOnStartUp",general.displayOnStartUp);
    }
    if(general.settingsChangedSingle)
    {
    updateIndividualSettings(sq,"general",-2,"dcIconSize",general.displayControls.buttonSize);
    updateIndividualSettings(sq,"general",-2,"dcAlignmentV",general.displayControls.alignmentV);
    updateIndividualSettings(sq,"general",-2,"dcAlignmentH",general.displayControls.alignmentH);
    updateIndividualSettings(sq,"general",-2,"dcOpacity",general.displayControls.opacity);
    }

    if(isSpClosing)
    {
    updateIndividualSettings(sq,"spMain",-2,"spSplitter",spMain.spSplitter);
    updateIndividualSettings(sq,"spMain",-2,"bibleHiddenSplitter",spMain.bibleHiddenSplitter);
    updateIndividualSettings(sq,"spMain",-2,"bibleShowSplitter",spMain.bibleShowSplitter);
    updateIndividualSettings(sq,"spMain",-2,"songSplitter",spMain.songSplitter);
    updateIndividualSettings(sq,"spMain",-2,"uiTranslation",spMain.uiTranslation);
    updateIndividualSettings(sq,"spMain",-2,"isWindowMaximized",spMain.isWindowMaximized);
    }

    if(bibleSets.settingsChanged)
    {
    updateIndividualSettings(sq,"bible1",-2,"primaryBible",bibleSets.primaryBible);
    updateIndividualSettings(sq,"bible1",-2,"secondaryBible",bibleSets.secondaryBible);
    updateIndividualSettings(sq,"bible1",-2,"trinaryBible",bibleSets.trinaryBible);
    updateIndividualSettings(sq,"bible1",-2,"operatorBible",bibleSets.operatorBible);
    }

    if(bibleSets2.settingsChanged)
    {
    updateIndividualSettings(sq,"bible2",-2,"primaryBible",bibleSets2.primaryBible);
    updateIndividualSettings(sq,"bible2",-2,"primaryBible",bibleSets2.secondaryBible);
    updateIndividualSettings(sq,"bible2",-2,"primaryBible",bibleSets2.trinaryBible);
    }

    if(slideSets.settingsChanged)
    {
    updateIndividualSettings(sq,"pix",-2,"expandSmall",slideSets.expandSmall);
    updateIndividualSettings(sq,"pix",-2,"fitType",slideSets.fitType);
    updateIndividualSettings(sq,"pix",-2,"resize",slideSets.resize);
    updateIndividualSettings(sq,"pix",-2,"boundType",slideSets.boundType);
    updateIndividualSettings(sq,"pix",-2,"boundWidth",slideSets.boundWidth);
    updateIndividualSettings(sq,"pix",-2,"transitionType",slideSets.transitionType);
    }

    isSpClosing = false;
    general.settingsChangedAll = false;
    general.settingsChangedMulti = false;
    general.settingsChangedSingle = false;
    bibleSets.settingsChanged = false;
    bibleSets2.settingsChanged = false;
    slideSets.settingsChanged = false;
}

void Settings::saveNewSettings()
{
    QSqlQuery sq;
    sq.prepare("INSERT INTO Settings (type, sname, svalue) VALUES (?,?,?)");

    saveIndividualSettings(sq,"general",-2,"displayIsOnTop",general.displayIsOnTop);
    saveIndividualSettings(sq,"general",-2,"currentThemeId",general.currentThemeId);
    saveIndividualSettings(sq,"general",-2,"displayScreen",general.displayScreen);
    saveIndividualSettings(sq,"general",-2,"displayScreen2",general.displayScreen2);
    saveIndividualSettings(sq,"general",-2,"displayOnStartUp",general.displayOnStartUp);
    saveIndividualSettings(sq,"general",-2,"dcIconSize",general.displayControls.buttonSize);
    saveIndividualSettings(sq,"general",-2,"dcAlignmentV",general.displayControls.alignmentV);
    saveIndividualSettings(sq,"general",-2,"dcAlignmentH",general.displayControls.alignmentH);
    saveIndividualSettings(sq,"general",-2,"dcOpacity",general.displayControls.opacity);

    saveIndividualSettings(sq,"spMain",-2,"spSplitter",spMain.spSplitter);
    saveIndividualSettings(sq,"spMain",-2,"bibleHiddenSplitter",spMain.bibleHiddenSplitter);
    saveIndividualSettings(sq,"spMain",-2,"bibleShowSplitter",spMain.bibleShowSplitter);
    saveIndividualSettings(sq,"spMain",-2,"songSplitter",spMain.songSplitter);
    saveIndividualSettings(sq,"spMain",-2,"uiTranslation",spMain.uiTranslation);
    saveIndividualSettings(sq,"spMain",-2,"isWindowMaximized",spMain.isWindowMaximized);

    saveIndividualSettings(sq,"bible1",-2,"primaryBible",bibleSets.primaryBible);
    saveIndividualSettings(sq,"bible1",-2,"secondaryBible",bibleSets.secondaryBible);
    saveIndividualSettings(sq,"bible1",-2,"trinaryBible",bibleSets.trinaryBible);
    saveIndividualSettings(sq,"bible1",-2,"operatorBible",bibleSets.operatorBible);

    saveIndividualSettings(sq,"bible2",-2,"primaryBible",bibleSets2.primaryBible);
    saveIndividualSettings(sq,"bible2",-2,"secondaryBible",bibleSets2.secondaryBible);
    saveIndividualSettings(sq,"bible2",-2,"trinaryBible",bibleSets2.trinaryBible);

    saveIndividualSettings(sq,"pix",-2,"expandSmall",slideSets.expandSmall);
    saveIndividualSettings(sq,"pix",-2,"fitType",slideSets.fitType);
    saveIndividualSettings(sq,"pix",-2,"resize",slideSets.resize);
    saveIndividualSettings(sq,"pix",-2,"boundType",slideSets.boundType);
    saveIndividualSettings(sq,"pix",-2,"boundWidth",slideSets.boundWidth);
    saveIndividualSettings(sq,"pix",-2,"transitionType",slideSets.transitionType);
}
