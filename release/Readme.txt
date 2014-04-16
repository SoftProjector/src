Please Note: That releases from now on will not include pre-build database. SoftProjector will create database if one does not exist. Also softProjector 2 will not work with older databases that came with 1.07 and earlier. 

SoftProjector 2.0 - Beta
New:
	Added "Tune" column to song view
Improved:
	Updated UI translations
	Changed shortcut for "Remove from schedule" from "F3" to "Ctrl+Del"
	Removed "@$" and "@%" charechters from song text
	Removed auto play from media
	Modified "Close Display Screen" to Turn display screen On/Off
Bug Fixes:
	Improper deleting when "Delete" action pressed when schedule item was viewed.
	Code improvement for Unix/Linux phonon libraries
	Initial screen geometry on Linux
	Sending bible to schedule with no bible loaded
	Printing Marings with UI tranlations

SoftProjector 2.0 - Alpha
New: Service Schedule, includes Bible verses, songs, sideshows, media and announcements. (replacement for project)
Removed: song playlist (now part of schedule)
Improved: Song ending alignment in settings
New: Settings for Bible: font, color, alignment for caption
New: Settings for Songs: font, color, alignment for song info(key, verse, song number). 
New: Settings for Songs: fort, color, type for song ending.
Improved: Render logic
Rework: themes from xml to sqlite format
Update: songbook export format
Improved: background images are saved in settings instead of its origin location
New: Picture slide show settings
Bug Fix: Picture preview window
Improved/Bug fix: For installed on windows, database file will be saved in shared folder for all user
New: Download and import from online Bibles, Songs, Themes. (Need to solve server/hosting issue)
Bug Fix: last Song/Picture/Announcement delete in the list
Improved: open all supported image formats for backgrounds
Bug Fix: media player crash
New: Display screen control buttons now include close screen/show no text(on active background)
NOTE: Not all additions/improvements/bug fixes may be included in the list above

SoftProjector 2.0 - Development Build 5 (2db5) 
Improved: Faster Bible text search with more search options
Improved: Faster Bible chapter lookup
Improved: Added ability to save Announcement in database
Improved: Multi Slide announcements
Bug Fix: Language translation and Help Dialog loading
New: Picture Slide Show
New: Video/Music Player **
Bug Fix: Bible search double click issue (bug ticket #6)

** Multimedia playback will depend on the back-end codecs installed on the computer. 

SoftProjector 2.0 - Development Build 4 (2db4) 
New: Themes
Improved: Song search to handle properly non Alphanumeric characters
Improved: Can handle non Alphanumeric characters in song titles
Improved: Presentation contol shortcuts w/wo usb-presentors*
Bug Fix: SongCounter number sorting
Bug Fix/New: Check for proper database version before it starts the softProjector
Bug Fix: Printing of Song copyright info

* Support for Usb-Presentors works partially. It may or may not work depending on the usb presenter. Following button from should work, next/previous slide and stop(pause/hide) button.

SoftProjector 2.0 - Development Build 3 (2db3) 
Improved full song text search

SoftProjector 2.0 - Development Build 2 (2db2) 
Support for two separate display screens

SoftProjector 2.0 - Development Build 1 
New:
	New SongBook format
	Added private song settings.
	Full-text song search
	Printing:
		Current preview song & edited song
		Current preview Bible chapter
		Announcement list
		Project (Bible histories, Song playlist, Announcement list)
	Display Screen controls 
	Few shortcuts to control next/previous slides(PageUp/PageDown/Enter/Return/Arrow(Up/Down/Right/Left))
		
Improvements:
	Faster load/import/export/delete of songbooks
	User Friendly projection when only one monitor is available
Bug Fix:
	When new/copy/edit song, Is_song_being_edited validation

-----
NOTE:
This is development release. May have serious bug. If any found, please report on bug tracker including build version.
This release is for testing only.
