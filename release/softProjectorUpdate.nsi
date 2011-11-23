;NSIS Modern User Interface
;SoftProjector Installer
;Written by Vladislav Kobzar

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "SoftProjector 1.05"
  OutFile "SoftProjector-1.05-upgrade.exe"
  
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
 
  ;ADD YOUR OWN FILES HERE...

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