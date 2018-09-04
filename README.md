# Password Manager, now with Qt !

A large(ish) rework of my console-only multiplatform password manager that you can find on this GitHub (right [here](https://github.com/bad64/pwdmanager)).

Made with Qt 5.10. It has a couple more functions than its console counterpart, like editing database rows, or cross-platform copy to clipboard.

# Latest update: v2.1, 31/08/18 (not ready for release yet)

**CODE CHANGES**

- All: Removed (almost) all the C-style code, replaced by equivalent C++ code (this actually saves around 200kb of RAM usage)
- All: Moved some functions around for better file organization. Also function definitions roughly match the order used in mainwindow.h.
- All: Added sanity checks when importing a database file.
- All: Added a launch option to set language, independently of OS language settings. Since the code is only translated in French at the moment, it only works with French.

**NEW FEATURES**

- All: New file format. A tool to convert the old database to the new format has been included.
- All: Move rows up or down using Ctrl+up/down
- All: Added shortcut (Ctrl + H) to hide the table view. On Windows, it prompts for user password on unhide. On Linux, it doesn't (see comments in askuserpassword.cpp)
- All: Added an option to restore db from backup

**KNOWN ISSUES**

- Linux: Does not ask for password on unhide (see comments in askuserpassword.cpp for why it will probably not get fixed)

**BUGFIXES**

- All: Fixed a rare case where both the database and the backup can get corrupted if the user cancels a password edit MiniGenerateBox

**TO DO**

- Maybe an icon ?

# Download links

**BUILDING FROM SOURCES**

*Please note: You will need the Qt framework to build this software.*  
Start off by cloning the repo. Navigate into the created folder through your console of choice, then;

- To build on Windows: path\to\qmake.exe && mingw32-make && mingw32-make clean (or use Qt Creator. Your call)  
- To build on Linux: qmake && make && sudo make install && make clean

**PRECOMPILED BINARIES**

[WINDOWS ZIP](https://github.com/bad64/pwdmanager-qt/releases/download/v2.1/pwdmanager-qt-v2.1.2-windows.zip)  
[WINDOWS INSTALLER](https://github.com/bad64/pwdmanager-qt/releases/download/v2.1/pwdmanager-qt-v2.1.2-windows-setup.exe)

*Please note: This package depends on libqt5core5a, libqt5gui5, and libqt5widgets5.*  
[LINUX DEB](https://github.com/bad64/pwdmanager-qt/releases/download/v2.1/pwdmanager-qt_amd64.deb)