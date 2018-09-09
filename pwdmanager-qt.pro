CONFIG -= debug_and_release
CONFIG += qt debug release

CONFIG(debug)
{
    QMAKE_CXXFLAGS_DEBUG += /Zi
    QMAKE_CXXFLAGS_DEBUG += /Od
    QMAKE_LFLAGS_DEBUG += /DEBUG
}

win32:VERSION = 2.1.2.4
else:VERSION = 2.1.2

win32 {
    DESTDIR = bin/win32
    OBJECTS_DIR = bin/win32/obj/
    MOC_DIR = bin/win32/moc/
    RCC_DIR = bin/win32/rcc/
    UI_DIR = bin/win32/ui/

    RC_ICONS = src/resources/pwdmanager-qt.ico
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

    INSTALLS += target \
                languagefiles
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
    src/askuserpassword.cpp

TRANSLATIONS += \
    src/translations/pwdmanager-qt_fr.ts

RESOURCES += \
    src/resources.qrc
