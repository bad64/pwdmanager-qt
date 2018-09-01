QT += widgets

HEADERS += \
    src/includes.h \
    src/mainwindow.h \
    src/createuser.h \
    src/minigeneratebox.h \
    src/credentialsfile.h

SOURCES += \
    src/main.cpp \
    src/database.cpp \
    src/password.cpp \
    src/seed.cpp \
    src/mainwindow.cpp \
    src/misc.cpp \
    src/createuser.cpp \
    src/minigeneratebox.cpp \
    src/credentialsfile.cpp \
    src/askuserpassword.cpp

TRANSLATIONS += \
    translations/pwdmanager-qt_fr.ts

win32 {
    Release:DESTDIR = bin/release-win32
    Release:OBJECTS_DIR = bin/release-win32/obj/.obj
    Release:MOC_DIR = bin/release-win32/moc/.moc
    Release:RCC_DIR = bin/release-win32/rcc/.rcc
    Release:UI_DIR = bin/release-win32/ui/.ui

    Debug:DESTDIR = bin/debug-win32
    Debug:OBJECTS_DIR = bin/debug-win32/obj/.obj
    Debug:MOC_DIR = bin/debug-win32/moc/.moc
    Debug:RCC_DIR = bin/debug-win32/rcc/.rcc
    Debug:UI_DIR = bin/debug-win32/ui/.ui
}

unix:!macx {
    Release:DESTDIR = bin/release-win32
    Release:OBJECTS_DIR = bin/release-win32/obj/.obj
    Release:MOC_DIR = bin/release-win32/moc/.moc
    Release:RCC_DIR = bin/release-win32/rcc/.rcc
    Release:UI_DIR = bin/release-win32/ui/.ui

    Debug:DESTDIR = bin/debug-win32
    Debug:OBJECTS_DIR = bin/debug-win32/obj/.obj
    Debug:MOC_DIR = bin/debug-win32/moc/.moc
    Debug:RCC_DIR = bin/debug-win32/rcc/.rcc
    Debug:UI_DIR = bin/debug-win32/ui/.ui
}
