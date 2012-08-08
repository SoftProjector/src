#include "settings.h"

/************************************
          MAIN SETTING BLOCK
************************************/
//Settings::Settings()
//{
//    loadSettings();

//}

//void Settings::loadSettings()
//{
//    QString settings_file_path = QApplication::instance()->applicationDirPath() + QDir::separator() + "spSettings.xml";
//    QFile file(settings_file_path);
//    file.open(QIODevice::ReadOnly);
//    QXmlStreamReader xml(&file);

//    while(!xml.atEnd())
//    {
//        xml.readNext();
//        if(xml.StartElement && xml.name() == "spSettings")
//        {
//            QString s_version = xml.attributes().value("version").toString().simplified().trimmed();
//            qDebug()<<s_version +"fd";
//            if(s_version=="1.0")
//            {
//                xml.readNext();
//                while(xml.tokenString() != "EndElement" && xml.name() != "spSettings")
//                {
//                    xml.readNext();
//                    if(xml.StartElement && xml.name() == "GeneralSettings")
//                    {
//                        // Read General settings from file
//                        while(xml.tokenString() != "EndElement")
//                        {
//                            xml.readNext();
//                            if(xml.StartElement)
//                                readGeneralSettings(xml.name().toString(),xml.readElementText());
//                        }
//                    }
//                }

//            }
//            else
//            {
//                //User friednly box for incorrect file version
//                QMessageBox mb;
//                mb.setWindowTitle(tr("Incorrect setting file format"));
//                mb.setText(tr("ERROR: Wrong Setting File Format"));
//                mb.setIcon(QMessageBox::Information);
//                mb.exec();

//                // remove any file paths

//                return;
//            }
//        }
//    }
//    file.close();
//}

//void Settings::readGeneralSettings(QString name, QString value)
//{
//    if(name == "displayIsOnTop")
//        gsettings.displayIsOnTop = (value == "true");
//    else if (name == "useShadow")
//        gsettings.useShadow = (value == "true");
//    else if (name == "useFading")
//        gsettings.useFading = (value == "true");
//    else if (name == "useBlurShadow")
//        gsettings.useBlurShadow = (value == "true");
//    else if (name == "useBackground")
//        gsettings.useBackground = (value == "true");
//    else if (name == "backgroundPath")
//        gsettings.backgroundPath == value;
//}

//void Settings::setSave()
//{
//    saveSettings();
//}

//void Settings::saveSettings()
//{
//    QString settings_file_path = QApplication::instance()->applicationDirPath() + QDir::separator() + "spSettings.xml";
//    QFile file(settings_file_path);
//    file.open(QIODevice::WriteOnly);
//    QXmlStreamWriter xml(&file);
//    xml.setAutoFormatting(true);
//    xml.setCodec("UTF8");

//    xml.writeStartDocument();
//    xml.writeStartElement("spSettings");
//    xml.writeAttribute("version","1.0");

//    // Write General Settings
//    xml.writeStartElement("GeneralSettings");
//    if(gsettings.displayIsOnTop)
//        xml.writeTextElement("displayIsOnTop","true");
//    else
//        xml.writeTextElement("displayIsOnTop","fale");
//    if(gsettings.useShadow)
//        xml.writeTextElement("useShadow","true");
//    else
//        xml.writeTextElement("useShadow","fale");
//    if(gsettings.useFading)
//        xml.writeTextElement("useFading","true");
//    else
//        xml.writeTextElement("useFading","fale");
//    if(gsettings.useBlurShadow)
//        xml.writeTextElement("useBlurShadow","true");
//    else
//        xml.writeTextElement("useBlurShadow","fale");
//    if(gsettings.useBackground)
//        xml.writeTextElement("useBackground","true");
//    else
//        xml.writeTextElement("useBackground","fale");
//    xml.writeTextElement("backgroundPath",gsettings.backgroundPath);
//    xml.writeEndElement();

//    xml.writeEndElement();// softProjector Settings
//    xml.writeEndDocument();

//    file.close();
//}

//GeneralSettings Settings::getGeneralSettings()
//{
//    return gsettings;
//}

//void Settings::setGeneralSettings(GeneralSettings g)
//{
//    gsettings = g;
//}

/************************************
          GENERAL SETTING BLOCK
************************************/


/************************************
          BIBLE SETTING BLOCK
************************************/
//BibleSettings::BibleSettings(QObject *parent) :
//    QObject(parent)
//{
//}

//void BibleSettings::setPrimaryBible(QString bible)
//{
//    qDebug()<<bible;
//    primaryBible = bible;
//}

//void BibleSettings::setSecondaryBible(QString bible)
//{
//    secondaryBible = bible;
//}

//void BibleSettings::setTrinatyBible(QString bible)
//{
//    trinaryBible = bible;
//}

//void BibleSettings::setControlBible(QString bible)
//{
//    controlBible = bible;
//}

//void BibleSettings::setBackground(QString path)
//{
//    backgroundPath = path;
//}

//void BibleSettings::setFont(QFont font)
//{
//    textFont = font;
//}

//void BibleSettings::setFontColor(QColor color)
//{
//    textColor = color;
//}

//void BibleSettings::setFlags(int flags)
//{
//    textFlags = flags;
//}

//QString BibleSettings::getPrimaryBible()
//{
//    return primaryBible;
//}

//QString BibleSettings::getSecondaryBible()
//{
//    return secondaryBible;
//}

//QString BibleSettings::getTrinatyBible()
//{
//    return trinaryBible;
//}

//QString BibleSettings::getControlBible()
//{
//    return controlBible;
//}

//QString BibleSettings::getBackground()
//{
//    return backgroundPath;
//}

//QFont BibleSettings::getFont()
//{
//    return textFont;
//}

//QColor BibleSettings::getColor()
//{
//    return textColor;
//}

//int BibleSettings::getFlags()
//{
//    return textFlags;
//}

/************************************
          SONG SETTING BLOCK
************************************/


/************************************
          ANNOUNCEMENT SETTING BLOCK
************************************/

