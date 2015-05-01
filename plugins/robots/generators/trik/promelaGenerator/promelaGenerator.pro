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
	$$PWD/lua/promelaLuaPrinter.h \
	$$PWD/lua/promelaLuaProcessor.h \
	$$PWD/simpleGenerators/switchGenerator.h \
	$$PWD/parts/strings.h \
	$$PWD/simpleGenerators/variableInitGenerator.h \
    simpleGenerators/ifGenerator.h

SOURCES += \
	$$PWD/promelaGeneratorPlugin.cpp \
	$$PWD/promelaMasterGenerator.cpp \
	$$PWD/promelaGeneratorFactory.cpp \
	$$PWD/promelaGeneratorCustomizer.cpp \
	$$PWD/simpleGenerators/sendMessageThreadsGenerator.cpp \
	$$PWD/simpleGenerators/receiveMessageThreadsGenerator.cpp \
	$$PWD/lua/promelaLuaPrinter.cpp \
	$$PWD/lua/promelaLuaProcessor.cpp \
	$$PWD/simpleGenerators/switchGenerator.cpp \
	$$PWD/parts/strings.cpp \
	$$PWD/simpleGenerators/variableInitGenerator.cpp \
    simpleGenerators/ifGenerator.cpp

RESOURCES = \
	$$PWD/templates.qrc \
