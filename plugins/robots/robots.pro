TEMPLATE = subdirs

SUBDIRS = \
	editor \
	robotsInterpreter \
	robotsGeneratorBase \
	nxtGenerator \
	trikGenerator \
	morseGenerator \
	russianCGenerator \
	qextserialport \

qextserialport.file = thirdparty/qextserialport/qextserialport.pro
robotsInterpreter.depends = qextserialport
nxtGenerator.depends = robotsGeneratorBase
morseGenerator.depends = robotsGeneratorBase
trikGenerator.depends = robotsGeneratorBase
russianCGenerator.depends = robotsGeneratorBase
