TEMPLATE = subdirs

SUBDIRS = \
	generatorBase \
	trikGeneratorBase \
	trikQtsGenerator \
	trikFSharpGenerator \
	trikRuntimeUploaderPlugin \
	nxtGeneratorBase \
	nxtOsekCGenerator \
	nxtRussianCGenerator \
	promelaGenerator \

promelaGenerator.subdir = $$PWD/trik/promelaGenerator
trikGeneratorBase.subdir = $$PWD/trik/trikGeneratorBase
trikQtsGenerator.subdir = $$PWD/trik/trikQtsGenerator
trikFSharpGenerator.subdir = $$PWD/trik/trikFSharpGenerator
trikRuntimeUploaderPlugin.subdir = $$PWD/trik/trikRuntimeUploaderPlugin
nxtGeneratorBase.subdir = $$PWD/nxt/nxtGeneratorBase
nxtOsekCGenerator.subdir = $$PWD/nxt/nxtOsekCGenerator
nxtRussianCGenerator.subdir = $$PWD/nxt/nxtRussianCGenerator

promelaGenerator.depends = trikGeneratorBase
trikGeneratorBase.depends = generatorBase
trikQtsGenerator.depends = trikGeneratorBase
trikFSharpGenerator.depends = trikGeneratorBase
trikRuntimeUploaderPlugin.depends = trikGeneratorBase
nxtGeneratorBase.depends = generatorBase
nxtOsekCGenerator.depends = nxtGeneratorBase
nxtRussianCGenerator.depends = nxtGeneratorBase
