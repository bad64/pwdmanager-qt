# Password Manager, now with Qt !

A large(ish) rework of my console-only multiplatform password manager that you can find on this GitHub (right [here](https://github.com/bad64/pwdmanager)).

Made with Qt 5.10. It has a couple more functions than its console counterpart, like editing database rows, or cross-platform copy to clipboard.

# Latest update: v2.0, 20/08/18

**CHANGES FROM v1.21**

- All: Downgraded Qt version to 5.7 for compatibility's sake (makes no change to the actual code)
- All: Function naming convention has been changed from UpperCamelCase to camelCase
- All: New subwindow facilities inheriting from QDialog have been implemented (CreateUser and MiniGenerateBox)

**NEW FEATURES**

- Changed creation process: now appears as one single window
- Added special edit dialog that allows for re-generation of a password
- Added a clear button for the search bar

**TO DO**

- Maybe an icon ?

# Download links

**WINDOWS**  
[Windows Installer](https://github.com/bad64/pwdmanager-qt/releases/download/v2.0/pwdmanager-qt-v2.0-windows-setup.exe)  
[Windows ZIP](https://github.com/bad64/pwdmanager-qt/releases/download/v2.0/pwdmanager-qt-v2.0-windows.zip)

**LINUX**  
Linux version depends on Qt5. Installer script itself requires dialog and sudo privileges.  
[Linux ZIP](https://github.com/bad64/pwdmanager-qt/releases/download/v2.0/pwdmanager-qt-v2.0-linux.zip)