#pragma once

#include <QtCore/QProcess>
#include <QtCore/QFileInfo>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace trik {
namespace promela {

class Spin : public QObject
{
	Q_OBJECT

public:
	explicit Spin(qReal::ErrorReporterInterface *errorReporter);

	void run(const QFileInfo &fileInfo);

private:
	void translationFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void errorWhileTranslating(QProcess::ProcessError error);

	void compileFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void compileError(QProcess::ProcessError error);

	void verificationFinished(int exitCode, QProcess::ExitStatus exitStatus);

	void counterexampleBuildingFinished(int exitCode, QProcess::ExitStatus exitStatus);

	void correctCFile();

	QProcess mPromelaToCProcess;
	QProcess mCompileProcess;
	QProcess mVerificationProcess;
	QProcess mCounterexampleProcess;
	qReal::ErrorReporterInterface *mErrorReporter;
	QFileInfo mFile;
};

}
}
