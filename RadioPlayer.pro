QT += core widgets multimedia
CONFIG += c++17

TARGET = FurRadio
TEMPLATE = app

SOURCES += main.cpp

macx {
    ICON = FurRadio.icns
}

win32 {
    RC_ICONS = icon.ico
}

unix:!macx {
    desktop.files = FurRadio.desktop
    desktop.path = /usr/share/applications
    INSTALLS += desktop
}
