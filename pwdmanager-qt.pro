CONFIG -= debug_and_release
CONFIG += release


win32:VERSION = 2.2.0.0
else:VERSION = 2.2.0

win32 {
    DESTDIR = bin/win32
    OBJECTS_DIR = bin/win32
    MOC_DIR = bin/win32
    RCC_DIR = bin/win32
    UI_DIR = bin/win32

    RC_ICONS = src/resources/pwdmanager-qt.ico

    languagefiles.path = bin/win32/translations
    languagefiles.files = src/translations/*.qm

    help.path = bin/win32/help
    help.files += src/help/*

    INSTALLS += languagefiles \
                help

    QMAKE_POST_LINK = windeployqt bin/win32/pwdmanager-qt.exe
}

unix:!macx {
    DESTDIR = bin/linux
    OBJECTS_DIR = bin/linux/obj/
    MOC_DIR = bin/linux/moc/
    RCC_DIR = bin/linux/rcc/
    UI_DIR = bin/linux/ui/

    languagefiles.path = /usr/local/share/pwdmanager-qt/translations
    languagefiles.files = src/translations/*.qm

    target.path = /usr/local/bin

    icon.path = /usr/share/pixmaps
    icon.files = src/resources/pwdmanager-qt.png

    help.path = /usr/local/share/pwdmanager-qt/help
    help.files = src/help/*/*.html

    LIBS += -lpam

    INSTALLS += target \
                languagefiles \
                help
}

QMAKE_CLEAN += object_script.* Makefile .qmake.stash

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
    src/auth.cpp

TRANSLATIONS += \
    src/translations/pwdmanager-qt_fr.ts

RESOURCES += \
    src/resources.qrc
