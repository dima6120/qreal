TARGET = robots-trik-promela-generator

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base robots-utils)

INCLUDEPATH += \
	$$PWD/../trikGeneratorBase/include/ \
	$$PWD/../../generatorBase/include/ \
	$$PWD/../../../interpreters/interpreterBase/include \
	$$PWD/../../../utils/include/ \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui \
	$$PWD/../../../../../qrtext/include \

HEADERS += \
	$$PWD/promelaGeneratorPlugin.h \
	$$PWD/promelaMasterGenerator.h \

SOURCES += \
	$$PWD/promelaGeneratorPlugin.cpp \
	$$PWD/promelaMasterGenerator.cpp \

RESOURCES = \
	$$PWD/templates.qrc \
