This document contains instructions on how to build Qt with a unicode patched version of sqlite

This document has been written for Qt 4.7.0 and SQlite 3.7.4
The Build environment is mingw as shipped with Qt 4.7.0

1. Compile SQlite sources into a library by running this command in the sqlite source directory:

	gcc -shared -o sqlite3u.dll -O2 sqlite3.c sqlite3_unicode.c -D SQLITE_CORE -D SQLITE_ENABLE_UNICODE -lpthread Wl,--out-implib,libsqlite3u.a

2. Configure Qt to use the new version of SQlite by running this commands (replace $QTDIR with the actual Qt path ex: C:\QT\2010.05\qt and $SQLITE with the actual path of sqlite sources)

	cd $QTDIR/src/plugins/sqldrivers/sqlite
 	qmake "INCLUDEPATH+=$SQLITE" "LIBS+=-L$SQLITE -lsqlite3u"
 	mingw32-make

3. Be sure to include sqlite3u.dll and the new version of qsqlite4.dll when distibuting the program. 


More information can be found here: documentation found here:

http://sig9.com/node/35
http://doc.qt.nokia.com/4.7/sql-driver.html#qsqlite


This documnt was written by Ilya Spivakov