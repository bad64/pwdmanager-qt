# Password Manager, now with Qt !

A large(ish) rework of my console-only multiplatform password manager that you can find on this GitHub (right [here](https://github.com/bad64/pwdmanager)).

Made with Qt 5.10. It has a couple more functions than its console counterpart, like editing database rows, or cross-platform copy to clipboard.

# Latest update: v1.10, 06/08/18

Lots.

**CHANGES**

- All: Changed database file extension to .db (trying to harmonize filenames between Windows and Linux)
- All: Changed all malloc() calls to calloc() to improve stability (at the cost of slightly higher memory usage)
- All: Help and About windows changed from QMessageBox::information to QMessageBox::about
- Windows: Changed the storage destination to %AppData%\passwordmanager

**NEW FEATURES**

- Now accepts zero-length input (replaced by the string "<none>") for all three fields when adding a new entry
- Search bar has been improved, now allows for partial pattern matching ("test" finally matches both "testing" and "latest")

**BUGFIXES**

- Fixed zero-length input in either the username or password fields corrupting the database

**KNOWN ISSUES**

- Generation using non-alphanumeric characters sometimes generates passwords that are smaller than the requested length (has actually been there since the first release, but this had never been reported to me before)