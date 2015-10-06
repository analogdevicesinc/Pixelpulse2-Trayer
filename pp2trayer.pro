TEMPLATE = app

QT += qml quick widgets
CONFIG += c++11

win32 {
	CONFIG += release
}
unix {
	CONFIG += release
}

QMAKE_CFLAGS_DEBUG += -ggdb
QMAKE_CXXFLAGS_DEBUG += -ggdb

CFLAGS += -v -static -static-libgcc -static-libstdc++ -g

DEFINES += GIT_VERSION='"\\\"$(shell git -C $$PWD describe --always --tags --abbrev)\\\""'
DEFINES += BUILD_DATE='"\\\"$(shell date +%F)\\\""'

SOURCES += main.cpp window.cpp

HEADERS += window.h pixelpulse2_wrap.h

RESOURCES += pp2trayer.qrc

# Additional import path used to resolve QML modules in the Qt Creator code model
QML_IMPORT_PATH =

win32:debug {
	CONFIG += console
	LIBS += -limagehlp -ldbghelp
}


osx {
	ICON = icons/pp2.icns
	LIBS += -lobjc -framework IOKit -framework CoreFoundation
	INCLUDEPATH += /usr/local/opt/qt5/include
}

win32 {
	RC_ICONS = icons/pp2.ico
	LIBS += "C:\libusb\MinGW32\static\libusb-1.0.a"
	INCLUDEPATH += "C:\libusb\include\libusb-1.0"
	INCLUDEPATH += "C:\mingw32\include"
}

unix {
	CONFIG += link_pkgconfig
# if we do not have a locally compiled static version of libusb-1.0 installed, use pkg-config
	!exists(/usr/local/lib/libusb-1.0.a) {
		PKGCONFIG += libusb-1.0
	}
# if we do have a locally compiled static version of libusb-1.0 installed, use it
	exists(/usr/local/lib/libusb-1.0.a) {
		LIBS += /usr/local/lib/libusb-1.0.a
		INCLUDEPATH += "/usr/local/include/libusb-1.0"
	}
}

unix:!osx {
	PKGCONFIG += libudev
	INSTALLS+=target
	isEmpty(PREFIX) {
		PREFIX = /usr
	}
	BINDIR = $$PREFIX/bin
	target.path=$$BINDIR
	QMAKE_CFLAGS_DEBUG += -rdynamic
	QMAKE_CXXFLAGS_DEBUG += -rdynamic
}
