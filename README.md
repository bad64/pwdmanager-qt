# Password Manager, now with Qt !

A large(ish) rework of my console-only multiplatform password manager that you can find on this GitHub (right [here](https://github.com/bad64/pwdmanager)).

Made with Qt 5.10. It has a couple more functions than its console counterpart, like editing database rows, or cross-platform copy to clipboard.

# Latest update: v2.1, 30/08/18

**CODE CHANGES**

- All: Removed (almost) all the C-style code, replaced by equivalent C++ code (this actually saves around 200kb of RAM usage)
- All: Moved some functions around for better file organization. Also function definitions roughly match the order used in mainwindow.h.
- All: Added sanity checks when importing a database file.
- All: Added a launch option to set language, independently of OS language settings. Since the code is only translated in French at the moment, it only works with French.

**FEATURES**

- All: New file format. A tool to convert the old database to the new format has been included.
- All: Move rows up or down using Ctrl+up/down
- All: Added shortcut (Ctrl + H) to hide the table view. On Windows, it prompts for user password on unhide. On Linux, it doesn't (see comments in askuserpassword.cpp)
- All: Added an option to restore db from backup

**KNOWN ISSUES**

- Linux: Does not ask for password on unhide (see comments in askuserpassword.cpp for why it will probably not get fixed)

**TO DO**

- Maybe an icon ?

# Download links

**Not available yet. Those are for 2.0**

**WINDOWS**  
[Windows Installer](https://github.com/bad64/pwdmanager-qt/releases/download/v2.1/pwdmanager-qt-v2.1-windows-setup.exe)  
[Windows ZIP](https://github.com/bad64/pwdmanager-qt/releases/download/v2.1/pwdmanager-qt-v2.1-windows.zip)

**LINUX**  
Linux version depends on Qt5. Installer script itself requires dialog and sudo privileges.  
[Linux ZIP](https://github.com/bad64/pwdmanager-qt/releases/download/v2.1/pwdmanager-qt-v2.1-linux.zip)