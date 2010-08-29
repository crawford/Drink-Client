TEMPLATE = app
TARGET = DrinkClient
QT += core \
	gui \
	network
CONFIG += qt \
	release
DESTDIR = release
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
HEADERS += src/FrmClient.h \
	src/FrmLogin.h \
	src/ItemButton.h \
	src/TrayIcon.h
SOURCES += src/FrmClient.cpp \
	src/FrmLogin.cpp \
	src/ItemButton.cpp \
	src/TrayIcon.cpp \
	src/main.cpp
FORMS += ui/FrmClient.ui \
	ui/FrmLogin.ui
win32 {
	RESOURCES += ui/logos.qrc
}
macx {
	RESOURCES += ui/macLogos.qrc
}
RC_FILE +=   ui/Icon.rc
