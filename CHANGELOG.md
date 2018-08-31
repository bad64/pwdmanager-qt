**Code changes**

- All: Removed (almost) all the C-style code, replaced by equivalent C++ code (this actually saves around 200kb of RAM usage)
- All: Moved some functions around for better file organization. Also function definitions roughly match the order used in mainwindow.h.
- All: Added sanity checks when importing a database file.
- All: Added a launch option to set language, independently of OS language settings. Since the code is only translated in French at the moment, it only works with French.
- Linux: Moved translations to /usr/share (from /usr/local/share)

**New features**

- All: New file format. A tool to convert the old database to the new format has been included.
- All: Move rows up or down using Ctrl+up/down
- All: Added shortcut (Ctrl + H) to hide the table view. On Windows, it prompts for user password on unhide. On Linux, it doesn't (see comments in askuserpassword.cpp)
- All: Added an option to restore db from backup

**Bugfixes**

- All: Fixed a rare case where both the database and the backup can get corrupted if the user cancels a password edit MiniGenerateBox

**Known issues**

- Linux: Does not ask for password on unhide (see comments in askuserpassword.cpp for why it will probably not get fixed)