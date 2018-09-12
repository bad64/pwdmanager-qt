# Password Manager, now with Qt !

A large(ish) rework of my console-only multiplatform password manager that you can find on this GitHub (right [here](https://github.com/bad64/pwdmanager)).

Made with Qt 5.7. It has a couple more functions than its console counterpart, like editing database rows, or cross-platform copy to clipboard.

# Latest update: v2.1.4, 11/09/18

**CODE CHANGES**

- All: Removed all the C-style code (for real this time), replaced by equivalent C++ code (this actually saves around 200kb of RAM usage)

**FEATURES**

- All: Merged import & convert functions (having both separated was redundant)
- All: Removed Edit menu after getting feedback that no one used it (to be fair, neither did I)
- All: New Help menu that looks really retro

**The memory leak appears to be fixed, though that might require more thorough testing**

# Download links

**BUILDING FROM SOURCES**

*Please note: You will need the Qt framework to build this software.*  
Start off by cloning the repo. Navigate into the created folder through your console of choice, then;

- To build on Windows: path\to\qmake.exe && mingw32-make && mingw32-make clean (or use Qt Creator. Your call)  
- To build on Linux: qmake && make && sudo make install && make clean

**PRECOMPILED BINARIES**

Not gonna release precompiled binaries on experimental branch.