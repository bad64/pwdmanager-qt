# Password Manager, now with Qt !

A large(ish) rework of my console-only multiplatform password manager that you can find on this GitHub (right [here](https://github.com/bad64/pwdmanager)).

Made with Qt 5.10. It has a couple more functions than its console counterpart, like editing database rows, or cross-platform copy to clipboard.

# Latest update: v1.21, 17/08/18

**CHANGES FROM v1.0**

- All: Changed database file extension to .db (trying to harmonize filenames between Windows and Linux)
- All: Changed all malloc() calls to calloc() to improve stability (at the cost of slightly higher memory usage)
- All: Help and About windows changed from QMessageBox::information to QMessageBox::about
- All: Status report describing the results of last operation implemented in the bottom right corner
- All: Generally cleaned up the grid layout, resulting in a nicer-looking UI
- Windows: Changed the storage destination to %AppData%\passwordmanager

**NEW FEATURES**

- Now accepts zero-length input (replaced by the string "\<none\>") for all three fields when adding a new entry
- Search bar has been improved, now allows for partial pattern matching ("test" finally matches both "testing" and "latest") and exact matching
- Added French Localization

**BUGFIXES**

- Fixed zero-length input in either the username or password fields corrupting the database
- "Fixed" special characters sometimes shortening a generated password and displaying as gibberish (actually replaced them by other non-alphanumeric characters)

**TO DO**

- Maybe an icon ?

# Download links

[Windows Installer](https://github.com/bad64/pwdmanager-qt/releases/download/1.21/pwdmanager-qt-v1.21-windows-setup.exe)  
[Windows ZIP](https://github.com/bad64/pwdmanager-qt/releases/download/1.21/pwdmanager-qt-v1.21-windows.zip)  
[Linux ZIP](https://github.com/bad64/pwdmanager-qt/releases/download/1.21/pwdmanager-qt-v1.21-linux.zip) (comes with an installation script that requires dialog)