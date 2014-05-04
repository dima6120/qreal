#include "morseGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QProcessEnvironment>
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <QtCore/QFileInfo>
#include <QtCore/QStandardPaths>

#include "morseMasterGenerator.h"
#include <qrkernel/settingsManager.h>

using namespace qReal;
using namespace qReal::robots::generators;
using namespace morse;

MorseGeneratorPlugin::MorseGeneratorPlugin()
	: mRunBlenderAction(nullptr)
	, mRunSimulationAction(nullptr)
	, mStopSimulationAction(nullptr)
	, mMorseAndBlenderPresent(false)
	, mBlenderIsRunning(false)
	, mScriptIsRunning(false)
	, mBlenderLocation("")
{
	mAppTranslator.load(":/morseGenerator_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);;
	if (checkMorse()) {
		readBlenderVersionRestriction();
		if (checkBlender()) {
			setProcessEnviroment();
			mMorseAndBlenderPresent = true;
		}
	}
	connect(&mBlender, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(blenderFinished(int, QProcess::ExitStatus)));
}

MorseGeneratorPlugin::~MorseGeneratorPlugin()
{
}

QList<ActionInfo> MorseGeneratorPlugin::actions()
{
	mRunBlenderAction.setText(tr("Run 3D model"));
	ActionInfo runBlenderActionInfo(&mRunBlenderAction, "generators", "tools");
	connect(&mRunBlenderAction, SIGNAL(triggered()), this, SLOT(runBlender()));

	mRunSimulationAction.setText(tr("Start simulation"));
	ActionInfo runSimulationActionInfo(&mRunSimulationAction, "generators", "tools");
	connect(&mRunSimulationAction, SIGNAL(triggered()), this, SLOT(runSimulation()));

	mStopSimulationAction.setText(tr("Stop simulation"));
	ActionInfo stopSimulationActionInfo(&mStopSimulationAction, "generators", "tools");
	connect(&mStopSimulationAction, SIGNAL(triggered()), this, SLOT(stopSimulation()));


	return QList<ActionInfo>() << runBlenderActionInfo << runSimulationActionInfo << stopSimulationActionInfo;
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
	Q_UNUSED(projectName)
	return QString("morse/3Dmodel/scripts/%1.py").arg("3Dmodel_client");
}

QString MorseGeneratorPlugin::extension() const
{
	return "py";
}

QString MorseGeneratorPlugin::extDescrition() const
{
	return "";
}

QString MorseGeneratorPlugin::generatorName() const
{
	return "Morse";
}

void MorseGeneratorPlugin::runBlender()
{
	if (!mMorseAndBlenderPresent) {
		checkMorseAndBlender();
		if (!mMorseAndBlenderPresent) {
			return;
		}
	}
	if (mBlenderIsRunning) {
		mMainWindowInterface->errorReporter()->addInformation(tr("Blender is already being launched"));
		return;
	}

	MorseMasterGenerator * const generator = new MorseMasterGenerator(*mRepo
																  , *mMainWindowInterface->errorReporter()
																  , mMainWindowInterface->activeDiagram());
	generator->initialize();
	generator->generateBuildScript();
	delete generator;

	mBlenderIsRunning = true;
	mBlender.setWorkingDirectory(qApp->applicationDirPath() + "/morse/");
	mBlender.start(mBlenderLocation + "/blender.exe", QStringList() << qApp->applicationDirPath() + "/morse/share/morse/data/morse_default_autorun.blend"
				   << "-P" << qApp->applicationDirPath() + "/morse/tempfile.py");
}

void MorseGeneratorPlugin::runSimulation()
{
	if (!mMorseAndBlenderPresent) {
		checkMorseAndBlender();
		if (!mMorseAndBlenderPresent) {
			return;
		}
	}

	if (!mBlenderIsRunning) {
		mMainWindowInterface->errorReporter()->addInformation(tr("First run 3D model"));
		return;
	}

	if (mScriptIsRunning) {
		mMainWindowInterface->errorReporter()->addInformation(tr("Simulation is already being run"));
		return;
	}

	if (generateCode(false)) {
		mMainWindowInterface->errorReporter()->addInformation(tr("Simulation is running"));
		mScript.setWorkingDirectory(qApp->applicationDirPath() + "/morse/");
		mScript.start(qApp->applicationDirPath() + "/python/python.exe", QStringList()
					  << qApp->applicationDirPath() + "/morse/3Dmodel/scripts/3Dmodel_client.py");
		mScriptIsRunning = true;
	}
}

void MorseGeneratorPlugin::stopSimulation()
{
	if (!mMorseAndBlenderPresent) {
		return;
	}
	mScript.kill();
	mScriptIsRunning = false;
	mMainWindowInterface->errorReporter()->addInformation(tr("Simulation is stopped"));
}

bool MorseGeneratorPlugin::checkMorse()
{
	QDir dir(qApp->applicationDirPath());

	if (QDir().exists(dir.absolutePath() + "/morse")) {
		dir.cd(dir.absolutePath() + "/morse");
		QDir lib(dir.absolutePath() + "/Lib");
		QDir share(dir.absolutePath() + "/share");
		QDir bin(dir.absolutePath() + "/bin");
		return lib.exists() && share.exists() && bin.exists();
	}

	return false;
}

bool MorseGeneratorPlugin::checkBlender()
{
	QString const blenderLocation = SettingsManager::value("BlenderPath").toString();
	if (!blenderLocation.isEmpty()) {
		mBlenderLocation = blenderLocation;
		return checkBlenderVersion();
	}
	return false;
}

void MorseGeneratorPlugin::checkMorseAndBlender()
{
	mMorseAndBlenderPresent = false;
	if (checkMorse()) {
		readBlenderVersionRestriction();
		if (checkBlender()) {
			mMorseAndBlenderPresent = true;
			setProcessEnviroment();
		} else {
			mMainWindowInterface->errorReporter()->addError(tr("Could not find correct Blender executable!"));
		}
	} else {
		mMainWindowInterface->errorReporter()->addError(tr("Could not find Morse library!"));
	}
}

bool MorseGeneratorPlugin::checkBlenderVersion()
{
	if (!mBlenderLocation.isEmpty()) {
		QProcess blender;
		blender.setWorkingDirectory(qApp->applicationDirPath() + "/morse/");
		blender.setStandardOutputFile(qApp->applicationDirPath() + "/morse/blender_version.txt");
		blender.start(mBlenderLocation + "/blender.exe", QStringList() << "--version");
		if (blender.state() == QProcess::NotRunning) {
			return false;
		}
		while (!blender.waitForFinished()) {
		}

		QFile file(qApp->applicationDirPath() + "/morse/blender_version.txt");
		QTextStream *inStream = 0;

		if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				inStream = new QTextStream(&file);
		} else {
			return false;
		}

		while(!inStream->atEnd()) {
			QString const line = inStream->readLine();
			if (line.startsWith("Blender")) {
				QString const version = line.split(" ")[1];
				file.remove();
				return version >= mMinBlenderVersion && version < mStrictMaxBlenderVersion;
			}
		}
		file.remove();
	}
	return false;
}

void MorseGeneratorPlugin::readBlenderVersionRestriction()
{
	QFile file(qApp->applicationDirPath() + "/morse/blender_version_restr.txt");
	QTextStream *inStream = 0;
	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			inStream = new QTextStream(&file);
	} else {
		return;
	}

	mMinBlenderVersion = inStream->readLine().split(" ")[2].remove('"');
	mStrictMaxBlenderVersion = inStream->readLine().split(" ")[2].remove('"');

	file.close();
}

void MorseGeneratorPlugin::setProcessEnviroment()
{
	QProcessEnvironment environment(QProcessEnvironment::systemEnvironment());
	QString pythonPath = "@PYTHONPATH@/DLLs;@PYTHONPATH@/lib;@PYTHONPATH@;@PYTHONPATH@/lib/site-packages;@MORSEPATH@/Lib/site-packages";
	environment.insert("MORSE_BLENDER", mBlenderLocation + "/blender.exe");
	environment.insert("MORSE_NODE", "QReal");
	environment.insert("PYHTONPATH", pythonPath.replace("@PYTHONPATH@", qApp->applicationDirPath() + "/python").replace(
							   "@MORSEPATH@", qApp->applicationDirPath() + "/morse"));
	mBlender.setProcessEnvironment(environment);
	mScript.setProcessEnvironment(environment);
	createMorseEnvironment();
}

void MorseGeneratorPlugin::createMorseEnvironment()
{
	QFile tempfile(qApp->applicationDirPath() + "/morse/tempfile.py");
	if (tempfile.exists()) {
		tempfile.remove();
	}
	mScript.setWorkingDirectory(qApp->applicationDirPath() + "/morse/");
	mScript.start(qApp->applicationDirPath() + "/python/python.exe", QStringList() << qApp->applicationDirPath() + "/morse/bin/morserun.py"
				  << "create" << "-f" << "3Dmodel");
	if (mScript.state() == QProcess::NotRunning) {
		mMorseAndBlenderPresent = false;
		mMainWindowInterface->errorReporter()->addError(tr("Problems with Morse library!"));
		return;
	}
	while(!mScript.waitForFinished()) {
	}
}

void MorseGeneratorPlugin::blenderFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus)
	Q_UNUSED(exitCode)
	mBlenderIsRunning = false;
	if (mScriptIsRunning) {
		mScriptIsRunning = false;
		mScript.kill();
		mMainWindowInterface->errorReporter()->addInformation(tr("Simulation is stopped"));
	}
}
