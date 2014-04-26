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
	: mRunBlenderAction(nullptr)
	, mRunSimulationAction(nullptr)
	, mStopSimulationAction(nullptr)
	, mMorseAndBlenderPresent(false)
	, mBlenderIsRunning(false)
	, mBlenderLocation("")
{
	mAppTranslator.load(":/morseGenerator_" + QLocale::system().name());
	QApplication::installTranslator(&mAppTranslator);;

	if (checkMorseAndBlender()) {
		QProcessEnvironment environment(QProcessEnvironment::systemEnvironment());
		QString pythonPath = "@PYTHONPATH@/DLLs;@PYTHONPATH@/lib;@PYTHONPATH@;@PYTHONPATH@/lib/site-packages;@MORSEPATH@/Lib/site-packages";
		environment.insert("MORSE_BLENDER", mBlenderLocation + "/blender.exe");
		environment.insert("MORSE_NODE", "QReal");
		environment.insert("PYHTONPATH", pythonPath.replace("@PYTHONPATH@", qApp->applicationDirPath() + "/python").replace(
								   "@MORSEPATH@", qApp->applicationDirPath() + "/morse"));
		mBlender.setProcessEnvironment(environment);
		mScript.setProcessEnvironment(environment);
		connect(&mBlender, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(blenderFinished(int, QProcess::ExitStatus)));
		connect(&mScript, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(scriptFinished(int, QProcess::ExitStatus)));
		createEnvironment();
	}
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
		mMainWindowInterface->errorReporter()->addInformation(tr("Could not find morse library or correct Blender executable"));
		return;
	}
	if (mBlenderIsRunning) {
		mMainWindowInterface->errorReporter()->addInformation(tr("Blender is already being launched"));
		return;
	}
	if (generateCode(false)) {
		mBlenderIsRunning = true;
		mBlender.setWorkingDirectory(qApp->applicationDirPath() + "/morse/");
		mBlender.start(mBlenderLocation + "/blender.exe", QStringList() << qApp->applicationDirPath() + "/morse/share/morse/data/morse_default_autorun.blend"
					   << "-P" << qApp->applicationDirPath() + "/morse/tempfile.py");
	}
}

void MorseGeneratorPlugin::runSimulation()
{
	if (!mBlenderIsRunning) {
		mMainWindowInterface->errorReporter()->addInformation(tr("First run 3D model"));
		return;
	}
	mMainWindowInterface->errorReporter()->addInformation(tr("Simulation is running"));
	QFileInfo const fileInfo = srcPath();
	mScript.setWorkingDirectory(qApp->applicationDirPath() + "/morse/");
	mScript.setStandardErrorFile(fileInfo.absolutePath() + "/error.txt");
	mScript.setStandardOutputFile(fileInfo.absolutePath() + "/out.txt");
	mScript.start(qApp->applicationDirPath() + "/python/python.exe", QStringList()
				  << qApp->applicationDirPath() + "/morse/3Dmodel/scripts/3Dmodel_client.py");
}

void MorseGeneratorPlugin::stopSimulation()
{
	mScript.kill();
	mMainWindowInterface->errorReporter()->addInformation(tr("Simulation is stopped"));
}

bool MorseGeneratorPlugin::checkMorseAndBlender()
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
	//QSettings blenderSettings("HKEY_CLASSES_ROOT\\blendfile\\DefaultIcon", QSettings::NativeFormat);
	QString const blenderLocation = "C:/Program Files/Blender Foundation/Blender";
	//qDebug() <<  blenderLocation;
	if (!blenderLocation.isEmpty()) {
		mBlenderLocation = blenderLocation;
		blenderPresent = true;
	} else {
		blenderPresent = false;
	}

	mMorseAndBlenderPresent = morsePresent && blenderPresent;

	return mMorseAndBlenderPresent;
}

void MorseGeneratorPlugin::createEnvironment()
{
	QFile tempfile(qApp->applicationDirPath() + "/morse/tempfile.py");
	if (tempfile.exists()) {
		tempfile.remove();
	}
	mScript.setWorkingDirectory(qApp->applicationDirPath() + "/morse/");
	mScript.setStandardErrorFile(qApp->applicationDirPath() + "/morse/error.txt");
	mScript.setStandardOutputFile(qApp->applicationDirPath() + "/morse/out.txt");
	mScript.start(qApp->applicationDirPath() + "/python/python.exe", QStringList() << qApp->applicationDirPath() + "/morse/morserun.py"
				  << "create" << "-f" << "3Dmodel");
}

void MorseGeneratorPlugin::blenderFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus)
	Q_UNUSED(exitCode)
	mBlenderIsRunning = false;
	mScript.kill();
}

void MorseGeneratorPlugin::scriptFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus)
	qDebug() << exitCode;
}
