;NSIS Modern User Interface
;SoftProjector Installer
;Written by Vladislav Kobzar

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "SoftProjector 1.06"
  OutFile "SoftProjector-1.06.exe"
  
  XPStyle on

  ;Default installation folder
  InstallDir "$PROGRAMFILES\SoftProjector"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\SoftProjector" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\SoftProjector" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\SoftProjector" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"


  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  !insertmacro MUI_PAGE_INSTFILES

  !define MUI_FINISHPAGE_RUN softProjector.exe
  !define MUI_FINISHPAGE_SHOWREADME Readme.txt
  !insertmacro MUI_PAGE_FINISH
  
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Romanian"
  !insertmacro MUI_LANGUAGE "Russian"
  !insertmacro MUI_LANGUAGE "Ukrainian"

  
  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
;Installer Sections

Section "Dummy Section" SecDummy

  SetOutPath "$INSTDIR\Backgrounds"
  
  ;ADD YOUR OWN FILES HERE...
  File Backgrounds\Back1Active.jpeg
  File Backgrounds\Back2Active.jpeg
  File Backgrounds\Back3Active.jpeg
  File Backgrounds\Back4Active.jpeg
  File Backgrounds\Back5Active.jpeg
  File Backgrounds\Back6Active.jpeg
  File Backgrounds\Back7Active.jpeg
  File Backgrounds\Back8Active.jpeg
  File Backgrounds\Back9Active.jpeg
  File Backgrounds\Back10Active.jpeg
  File Backgrounds\Back11Active.jpeg
  File Backgrounds\Back12Active.jpeg
  File Backgrounds\Back13Active.jpeg
  File Backgrounds\Back14Active.jpeg
  File Backgrounds\Back15Active.jpeg
  File Backgrounds\Back16Active.jpeg
  File Backgrounds\Back17Active.jpeg
  File Backgrounds\Back18Active.jpeg
  File Backgrounds\Back19Active.jpeg
  File Backgrounds\Back20Active.jpeg
  
  SetOutPath "$INSTDIR\imageformats"
  File imageformats\qgif4.dll
  File imageformats\qico4.dll
  File imageformats\qjpeg4.dll
  File imageformats\qmng4.dll
  File imageformats\qsvg4.dll
  File imageformats\qtiff4.dll
  
  SetOutPath "$INSTDIR\sqldrivers"
  File sqldrivers\qsqlite4.dll
  File sqldrivers\qsqlodbc4.dll
  
  SetOutPath "$INSTDIR\"
  File libgcc_s_dw2-1.dll
  File License.txt
  File Readme.txt
  File mingwm10.dll
  File QtCore4.dll
  File QtGui4.dll
  File QtSql4.dll
  File sqlite3u.dll
  File softProjector.exe
  File spData.sqlite


  
  ;Store installation folder
  WriteRegStr HKCU "Software\SoftProjector" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\softProjector.lnk" "$INSTDIR\softProjector.exe"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

;--------------------------------
;Installer Functions

Function .onInit

    InitPluginsDir
	File /oname=$PLUGINSDIR\splash.bmp "install\splash.bmp"
	advsplash::show 1200 300 300 0 $PLUGINSDIR\splash

	!insertmacro MUI_LANGDLL_DISPLAY

FunctionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDummy ${LANG_ENGLISH} "A test section."
  LangString DESC_SecDummy ${LANG_GERMAN} "A test section."  
  LangString DESC_SecDummy ${LANG_ROMANIAN} "A test section."
  LangString DESC_SecDummy ${LANG_RUSSIAN} "A test section."
  LangString DESC_SecDummy ${LANG_UKRAINIAN} "A test section."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...
  Delete "$INSTDIR\Backgrounds\Back1Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back1Passive.jpeg"
  Delete "$INSTDIR\Backgrounds\Back2Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back2Passive.jpeg"
  Delete "$INSTDIR\Backgrounds\Back3Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back3Passive.jpeg"
  Delete "$INSTDIR\Backgrounds\Back4Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back4Passive.jpeg"
  Delete "$INSTDIR\Backgrounds\Back5Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back5Passive.jpeg"
  Delete "$INSTDIR\Backgrounds\Back6Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back6Passive.jpeg"
  Delete "$INSTDIR\Backgrounds\Back7Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back7Passive.jpeg"
  Delete "$INSTDIR\Backgrounds\Back8Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back8PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back8PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back9Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back9PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back9PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back10Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back10PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back10PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back11Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back11Empty.jpeg"
  Delete "$INSTDIR\Backgrounds\Back11PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back11PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back12Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back12Empty.jpeg"
  Delete "$INSTDIR\Backgrounds\Back12PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back12PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back12PassiveUa.jpeg"
  Delete "$INSTDIR\Backgrounds\Back13Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back13Empty.jpeg"
  Delete "$INSTDIR\Backgrounds\Back13PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back13PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back13PassiveUa.jpeg"
  Delete "$INSTDIR\Backgrounds\Back14Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back14PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back14PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back14PassiveUa.jpeg"
  Delete "$INSTDIR\Backgrounds\Back15Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back15Empty.jpeg"
  Delete "$INSTDIR\Backgrounds\Back15PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back15PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back15PassiveUa.jpeg"
  Delete "$INSTDIR\Backgrounds\Back16Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back16Blank.jpeg"
  Delete "$INSTDIR\Backgrounds\Back16PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back16PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back16PassiveUa.jpeg"
  Delete "$INSTDIR\Backgrounds\Back17Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back17Empty.jpeg"
  Delete "$INSTDIR\Backgrounds\Back17PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back17PassiveUa.jpeg"
  Delete "$INSTDIR\Backgrounds\Back18Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back18Empty.jpeg"
  Delete "$INSTDIR\Backgrounds\Back18PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back18PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back18PassiveUa.jpeg"
  Delete "$INSTDIR\Backgrounds\Back19Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back19Empty.jpeg"
  Delete "$INSTDIR\Backgrounds\Back19PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back19PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back19PassiveUa.jpeg"
  Delete "$INSTDIR\Backgrounds\Back20Active.jpeg"
  Delete "$INSTDIR\Backgrounds\Back20Empty.jpeg"
  Delete "$INSTDIR\Backgrounds\Back20PassiveEn.jpeg"
  Delete "$INSTDIR\Backgrounds\Back20PassiveRu.jpeg"
  Delete "$INSTDIR\Backgrounds\Back20PassiveUa.jpeg"
  Delete "$INSTDIR\imageformats\qgif4.dll"
  Delete "$INSTDIR\imageformats\qico4.dll"
  Delete "$INSTDIR\imageformats\qjpeg4.dll"
  Delete "$INSTDIR\imageformats\qmng4.dll"
  Delete "$INSTDIR\imageformats\qsvg4.dll"
  Delete "$INSTDIR\imageformats\qtiff4.dll"
  Delete "$INSTDIR\sqldrivers\qsqlite4.dll"
  Delete "$INSTDIR\sqldrivers\qsqlodbc4.dll"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\License.txt"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\sqlite3u.dll"
  Delete "$INSTDIR\softProjector.exe"
  Delete "$INSTDIR\spData.sqlite"


  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR\Backgrounds"
  RMDir "$INSTDIR\imageformats"
  RMDir "$INSTDIR\sqldrivers"
  RMDir "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  Delete "$SMPROGRAMS\$StartMenuFolder\softProjector.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  
  DeleteRegKey /ifempty HKCU "Software\SoftProjector"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit

  !insertmacro MUI_UNGETLANGUAGE
  
FunctionEnd