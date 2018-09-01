win32 {
    Release:DESTDIR = release-win32
    Release:OBJECTS_DIR = release-win32/obj/.obj
    Release:MOC_DIR = release-win32/moc/.moc
    Release:RCC_DIR = release-win32/rcc/.rcc
    Release:UI_DIR = release-win32/ui/.ui

    Debug:DESTDIR = debug-win32
    Debug:OBJECTS_DIR = debug-win32/obj/.obj
    Debug:MOC_DIR = debug-win32/moc/.moc
    Debug:RCC_DIR = debug-win32/rcc/.rcc
    Debug:UI_DIR = debug-win32/ui/.ui
}

unix:!macx {
    Release:DESTDIR = release-win32
    Release:OBJECTS_DIR = release-win32/obj/.obj
    Release:MOC_DIR = release-win32/moc/.moc
    Release:RCC_DIR = release-win32/rcc/.rcc
    Release:UI_DIR = release-win32/ui/.ui

    Debug:DESTDIR = debug-win32
    Debug:OBJECTS_DIR = debug-win32/obj/.obj
    Debug:MOC_DIR = debug-win32/moc/.moc
    Debug:RCC_DIR = debug-win32/rcc/.rcc
    Debug:UI_DIR = debug-win32/ui/.ui

    languagefiles.path = /usr/local/share/pwdmanager-qt/translations
    languagefiles.files = src/translations/*

    target.path = /usr/local/bin
}

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

unix:!macx {
    INSTALLS += target \
                languagefiles
}
