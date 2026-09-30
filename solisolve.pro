TEMPLATE = app
TARGET = solisolve

QT += core gui widgets
CONFIG += c++17

# Le solveur utilise encore ncurses pour la visualisation console
INCLUDEPATH += /usr/include/ncursesw
LIBS += -lncurses

# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    CCoup.h \
    common.h \
    CPlateau.h \
    CPlateaux.h \
    CSolver.h \
    SPlateauCmp.h \
    mainwindow.h \
    wplateau.h

SOURCES += \
    CCoup.cpp \
    CPlateau.cpp \
    CPlateaux.cpp \
    CSolver.cpp \
    main.cpp \
    mainwindow.cpp \
    wplateau.cpp

FORMS += \
    mainwindow.ui

RESOURCES += \
    solisolve.qrc

# Icône de l'exécutable
win32: RC_ICONS = icons/solisolve.ico
macx: ICON = icons/solisolve.icns

# Linux : "make install" pose le binaire, le .desktop et les icônes hicolor
unix:!macx {
    isEmpty(PREFIX): PREFIX = /usr/local
    target.path = $$PREFIX/bin
    desktop.files = solisolve.desktop
    desktop.path = $$PREFIX/share/applications
    icon_svg.files = icons/solisolve.svg
    icon_svg.path = $$PREFIX/share/icons/hicolor/scalable/apps
    INSTALLS += target desktop icon_svg
    for(taille, $$list(16 24 32 48 64 128 256 512)) {
        icon_$${taille}.path = $$PREFIX/share/icons/hicolor/$${taille}x$${taille}/apps
        icon_$${taille}.extra = install -D -m 644 $$PWD/icons/png/solisolve-$${taille}.png $(INSTALL_ROOT)$$PREFIX/share/icons/hicolor/$${taille}x$${taille}/apps/solisolve.png
        INSTALLS += icon_$${taille}
    }
}
