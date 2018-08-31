#Fairly standard setup
AC_INIT([pwdmanager-qt], [2.1], [bug-automake@gnu.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_PROG_CXX
AC_CONFIG_FILES([Makefile, src/Makefile])

#Check for Qt libs existence
PKG_CHECK_MODULES(Qt5Core, [libQt5Core >= 5.7], [], [AC_MSG_ERROR([You need to install Qt5 to compile this program.])])
PKG_CHECK_MODULES(Qt5Gui, [libQt5Gui >= 5.7], [], [AC_MSG_ERROR([You need to install Qt5 to compile this program.])])
PKG_CHECK_MODULES(Qt5Widgets, [libQt5Widgets >= 5.7], [], [AC_MSG_ERROR([You need to install Qt5 to compile this program.])])

#Write Makefile.in
AC_OUTPUT