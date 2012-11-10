This document contains instructions on how to build Qt with a unicode patched version of sqlite

This document has been written for Qt 4.8.1 and SQlite 3.7.4
The Build environment is mingw as shipped with Qt 4.8.1
Should work on later versions of Qt

1. Compile SQlite sources into a library by running this command in the sqlite source directory:

	gcc -shared -o sqlite3u.dll -O2 sqlite3.c sqlite3_unicode.c -D SQLITE_CORE -D SQLITE_ENABLE_UNICODE -lpthread -Wl,--out-implib,libsqlite3u.a

2. Configure Qt to use the new version of SQlite by running this commands (replace $QTDIR with the actual Qt path ex: C:\QT\2010.05\qt and $SQLITE with the actual path of sqlite sources)

	cd $QTDIR/src/plugins/sqldrivers/sqlite
	C:\Users\lo304d\qsr\softprojector\sqlite
 	qmake "INCLUDEPATH+=$SQLITE" "LIBS+=-L$SQLITE -lsqlite3u"
	
	// NOTE: On  Windows with MinGW use mingw32-make other wise use appropriate complilers make 
	// to make a debug version
 	mingw32-make 
	
	// to make release version - use this for distributions
	mingw32-make release

3. Install files into current libraries - not necessary for distribution but needed for development use 
	
	// to install a debug version
 	mingw32-make install
	
	// to install release version
	mingw32-make release install
	
4. Be sure to include sqlite3u.dll and the new version of qsqlite4.dll when distibuting the program. 



More information can be found here: documentation found here:

http://sig9.com/node/35
http://doc.qt.nokia.com/4.7/sql-driver.html#qsqlite


This documnt was written by Ilya Spivakov
On November 9, 2012 - Modified my Vladislav Kobzar