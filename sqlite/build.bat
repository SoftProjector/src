@echo off
echo ---------------------------------------
echo       Sqlite with unicode support 
echo  build script provided by Ilya Spivakov
echo     This script comes witout warranty, 
echo it should work, but use at your own risk
echo ----------------------------------------
set sqlite_dir=%cd%
set sqlite_dir_unix=%sqlite_dir:\=/%

echo build sqlite3u.dll and sqlite3u.a for linking
gcc -shared -o sqlite3u.dll -O2 sqlite3.c sqlite3_unicode.c -D SQLITE_CORE -D SQLITE_ENABLE_UNICODE -lpthread -Wl,--out-implib,libsqlite3u.a
echo done.


echo build qtsqlite
echo on
cd /D %QTDIR%\src\plugins\sqldrivers\sqlite
qmake "INCLUDEPATH+=%sqlite_dir_unix%" "LIBS+=-L%sqlite_dir_unix% -lsqlite3u"
mingw32-make clean && mingw32-make && mingw32-make install
cd /d %sqlite_dir%
echo All done :)