**CODE CHANGES**

- All: Removed all the C-style code (for real this time), replaced by equivalent C++ code (this actually saves around 200kb of RAM usage)
- All: Removed all Apple checks. I don't think I will ever port this software to MacOS. Feel free to do it in my stead, if you wish.
- All: Added a check when starting up the program asking the user's password. This is to prevent computer jacking being a security risk.

**FEATURES**

- All: Merged import & convert functions (having both separated was redundant)
- All: Removed Edit menu after getting feedback that no one used it (to be fair, neither did I)
- All: New Help menu that looks really retro
- Linux: PAM authentication now works as intended