CONFIG -= debug_and_release
CONFIG += release

win32 {
    DESTDIR = bin/win32
    OBJECTS_DIR = bin/win32/obj/
    MOC_DIR = bin/win32/moc/
    RCC_DIR = bin/win32/rcc/
    UI_DIR = bin/win32/ui/
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
