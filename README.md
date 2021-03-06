[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

 # Password Manager, now with Qt !

A large(ish) rework of my console-only multiplatform password manager that you can find on this GitHub (right [here](https://github.com/bad64/pwdmanager)).

Made with Qt 5.7. It has a couple more functions than its console counterpart, like editing database rows, or cross-platform copy to clipboard.

# Latest update: v2.2.1, 26/09/18

**FEATURES**

- All: Added support for several command line args, which aren't 
documented yet (but a quick look at src/main.cpp should be enough)
- All: Added a function to merge two databases together, eliminating (presumed) duplicate entries (basically, if username && purpose && password all match, it's considered a duplicate, so it's pretty lenient)

# Download links

**BUILDING FROM SOURCES**

*Please note: You will need the Qt framework to build this software.*  
Start off by cloning the repo. Navigate into the created folder through your console of choice, then;

- To build on Linux: qmake && make && sudo make install && make clean
- To build on Windows:
	- Via command line: qmake.exe && mingw32-make && mingw32-make install && mingw32-make clean (this assumes both qmake, mingw32-make, and windeployqt are present in your path, which might not be the case)
	- Via Qt Creator: Import the pwdmanager-qt.pro file, set the config to Release, then compile it. Once you're done, you will need to run windeployqt. Drag-drop the pwdmanager-qt.exe file onto the windeployqt executable to do so. Then copy the help and translation folders in src/ to the executable folder. (Seriously it might be easier/faster to do it via cmd)

**PRECOMPILED BINARIES**

[WINDOWS INSTALLER](https://github.com/bad64/pwdmanager-qt/releases/download/v2.2.1/pwdmanager-qt-v2.2.1-windows-setup.exe)  
[WINDOWS ZIP](https://github.com/bad64/pwdmanager-qt/releases/download/v2.2.1/pwdmanager-qt-v2.2.1-windows.zip)  
[LINUX DEB](https://github.com/bad64/pwdmanager-qt/releases/download/v2.2.1/pwdmanager-qt_amd64.deb)
