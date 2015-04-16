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

#include "theme.h"

ThemeInfo::ThemeInfo()
{
    themeId = 0;
    name = "Default";
    comments  = "Default SoftProjector Theme";
}

Theme::Theme()
{
    common.id = "c1";
    common2.id = "c2";
    passive.id = "p1";
    passive2.id = "p2";
    bible.id = "b1";
    bible2.id = "b2";
    song.id = "s1";
    song2.id = "s2";
    announce.id = "a1";
    announce2.id = "a2";
}

void Theme::saveThemeNew()
{
    QSqlQuery sq;
    sq.prepare("INSERT INTO Themes (name, comment) VALUES (?,?)");
    sq.addBindValue(m_info.name);
    sq.addBindValue(m_info.comments);
    sq.exec();

    // get new theme id
    sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Themes'");
    sq.first();
    m_info.themeId = sq.value(0).toInt();

    common.themeId = common2.themeId = passive.themeId = passive2.themeId =
            bible.themeId = bible2.themeId = song.themeId = song2.themeId =
            announce.themeId = announce2.themeId = m_info.themeId;
    common.saveBase();
    common2.saveBase();
    passive.saveMain();
    passive2.saveMain();
    bible.save();
    bible2.save();
    song.save();
    song2.save();
    announce.saveMain();
    announce2.saveMain();
}

void Theme::saveThemeUpdate()
{
    QSqlQuery sq;
    sq.prepare("UPDATE Themes SET name = ?, comments = ? WHERE id = ?");
    sq.addBindValue(m_info.name);
    sq.addBindValue(m_info.comments);
    sq.addBindValue(m_info.themeId);
    sq.exec();

    common.themeId = common2.themeId = passive.themeId = passive2.themeId =
            bible.themeId = bible2.themeId = song.themeId = song2.themeId =
            announce.themeId = announce2.themeId = m_info.themeId;
    common.updateBase();
    common2.updateBase();
    passive.updateMain();
    passive2.updateMain();
    bible.update();
    bible2.update();
    song.update();
    song2.update();
    announce.updateMain();
    announce2.updateMain();
}

void Theme::loadTheme()
{
    QSqlQuery sq;
    bool ok, allok;
    allok = false;

    sq.exec(QString("SELECT name, comment FROM Themes WHERE id = %1").arg(m_info.themeId));
    ok = sq.first();
    if(ok)
    {
        m_info.name = sq.value(0).toString();
        m_info.comments = sq.value(1).toString();
        allok = true;
    }
    else
    {
        sq.exec("SELECT id, name, comment FROM Themes");
        ok = sq.first();

        // Check at least one theme exitst
        if (ok) // If exist, then load it
        {
            m_info.themeId = sq.value(0).toInt();
            m_info.name = sq.value(1).toString();
            m_info.comments = sq.value(2).toString();
            allok = true;
        }
        else // No themes exist, creat one and exit
        {
            saveThemeNew();
            allok = false;
        }
    }

    if(allok)
    {
        common.themeId = common2.themeId = passive.themeId = passive2.themeId =
                bible.themeId = bible2.themeId = song.themeId = song2.themeId =
                announce.themeId = announce2.themeId = m_info.themeId;
        common.loadBase();
        common2.loadBase();
        passive.loadMain();
        passive2.loadMain();
        bible.load();
        bible2.load();
        song.load();
        song2.load();
        announce.loadMain();
        announce2.loadMain();
    }
}

void Theme::setThemeInfo(ThemeInfo info)
{
    m_info = info;
}

ThemeInfo Theme::getThemeInfo()
{
    return m_info;
}
