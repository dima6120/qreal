#include "morseGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QFileInfo>
#include <QtCore/QStandardPaths>

#include "morseMasterGenerator.h"

using namespace qReal;
using namespace qReal::robots::generators;
using namespace morse;

MorseGeneratorPlugin::MorseGeneratorPlugin()
	: mGenerateCodeAction(nullptr)
	, mMorseAndBlenderPresent(false)
{
	mAppTranslator.load(":/morseGenerator_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);
	checkMorseAndBlender();
}

MorseGeneratorPlugin::~MorseGeneratorPlugin()
{
}

QList<ActionInfo> MorseGeneratorPlugin::actions()
{
	ActionInfo generateCodeActionInfo(&mGenerateCodeAction, "generators", "tools");
	connect(&mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(run3Dmodel()));

	return { generateCodeActionInfo };
}

MasterGeneratorBase *MorseGeneratorPlugin::masterGenerator()
{
	return new MorseMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, mMainWindowInterface->activeDiagram());
}

void MorseGeneratorPlugin::regenerateExtraFiles(QFileInfo const &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}

QString MorseGeneratorPlugin::defaultFilePath(QString const &projectName) const
{
	return QString("morse/%1/%2.py").arg(projectName, "exe_script");
}

QString MorseGeneratorPlugin::extension() const
{
	return "py";
}

QString MorseGeneratorPlugin::extDescrition() const
{
	return tr("Morse Source File");
}

QString MorseGeneratorPlugin::generatorName() const
{
	return "Morse";
}

bool MorseGeneratorPlugin::run3Dmodel()
{
	generateCode(false);
	return true;
}

void MorseGeneratorPlugin::checkMorseAndBlender()
{
	bool morsePresent, blenderPresent;
	QDir dir(qApp->applicationDirPath());
	if (!QDir().exists(dir.absolutePath() + "/morse")) {
		morsePresent = false;
	} else {
		dir.cd(dir.absolutePath() + "/morse");
		QDir lib(dir.absolutePath() + "/Lib");
		QDir share(dir.absolutePath() + "/share");
		morsePresent = lib.exists() && share.exists();
	}
	//TODO: take path to blender.exe from reg
	//QSettings blenderSettings("HKEY_LOCAL_MACHINE\\SOFTWARE\\BlenderFoundation", QSettings::NativeFormat);
	QString const blenderLocation = "C:/Program Files (x86)/Blender Foundation/Blender"; //blenderSettings.value("Install_Dir").toString();
	//qDebug() <<  blenderLocation;
	if (!blenderLocation.isEmpty()) {
		QString pythonPath = "@@PYTHONPATH@@\DLLs;@@PYTHONPATH@@\lib;@@PYTHONPATH@@;@@PYTHONPATH@@\lib\site-packages;@@MORSEPATH@@\Lib\site-packages";
		QProcessEnvironment environment(QProcessEnvironment::systemEnvironment());
		environment.insert("MORSE_BLENDER", blenderLocation + "/blender.exe");
		environment.insert("MORSE_NODE", "QReal");
		environment.insert("PYHTONPATH", pythonPath.replace("@@PYTHONPATH@@", "C:/python33").replace(
							   "@@MORSEPATH@@", qApp->applicationDirPath() + "/morse"));
		blenderPresent = true;
	} else {
		blenderPresent = false;
	}

	mMorseAndBlenderPresent = morsePresent && blenderPresent;
}
