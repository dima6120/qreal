TARGET = robots-trik-promela-generator

include(../../../../../global.pri)

QT += widgets network

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$DESTDIR/plugins/tools/kitPlugins/

includes(plugins/robots/generators/trik/trikGeneratorBase \
		plugins/robots/generators/generatorBase \
		plugins/robots/common/kitBase \
		plugins/robots/utils \
		qrtext \
)

links(qrkernel qslog qrutils qrrepo qscintilla2 robots-generator-base robots-trik-generator-base robots-utils)

HEADERS += \
	$$PWD/promelaGeneratorPlugin.h \
	$$PWD/promelaMasterGenerator.h \
	$$PWD/promelaGeneratorFactory.h \
	$$PWD/promelaGeneratorCustomizer.h \
	$$PWD/simpleGenerators/sendMessageThreadsGenerator.h \
	$$PWD/simpleGenerators/receiveMessageThreadsGenerator.h \

SOURCES += \
	$$PWD/promelaGeneratorPlugin.cpp \
	$$PWD/promelaMasterGenerator.cpp \
	$$PWD/promelaGeneratorFactory.cpp \
	$$PWD/promelaGeneratorCustomizer.cpp \
	$$PWD/simpleGenerators/sendMessageThreadsGenerator.cpp \
	$$PWD/simpleGenerators/receiveMessageThreadsGenerator.cpp \

RESOURCES = \
	$$PWD/templates.qrc \
