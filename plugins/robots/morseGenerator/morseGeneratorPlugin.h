#pragma once

#include <robotsGeneratorPluginBase.h>
#include <QtCore/QProcess>
#include <QtCore/QProcessEnvironment>

#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace qReal {
namespace robots {
namespace generators {
namespace morse {

class MorseGeneratorPlugin : public RobotsGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "qReal.robots.MorseGenerator.MorseGeneratorPlugin")

public:
	MorseGeneratorPlugin();

	~MorseGeneratorPlugin() override;

	QList<qReal::ActionInfo> actions() override;

private slots:
	void runBlender();
	void runSimulation();
	void blenderFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void scriptFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
	MasterGeneratorBase *masterGenerator() override;
	void regenerateExtraFiles(QFileInfo const &newFileInfo) override;
	QString defaultFilePath(QString const &projectName) const override;
	QString extension() const override;
	QString extDescrition() const override;
	QString generatorName() const override;
	bool checkMorseAndBlender();
	void createEnvironment();

	QAction mRunBlenderAction;
	QAction mRunSimulationAction;

	//qReal::ErrorReporterInterface *mErrorReporter;

	/// When true, Morse and Blender required version are found by QReal and using 3D model is possible
	bool mMorseAndBlenderPresent;
	bool mBlenderIsRunning;

	QString mBlenderLocation;

	/// Translator object for this plugin
	QTranslator mAppTranslator;

	QProcess mBlender;
	QProcess mScript;
};

}
}
}
}
