#pragma once

#include <QtCore/QProcess>
#include <QtCore/QFileInfo>
#include <QtCore/QTimer>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/graphicalModelAssistInterface.h>
#include <qrgui/textEditor/codeBlockManagerInterface.h>
#include <qrrepo/repoApi.h>

namespace trik {
namespace promela {

class Spin : public QObject
{
	Q_OBJECT

public:
	Spin(qReal::CodeBlockManagerInterface *codeBlockManager
			, qReal::gui::MainWindowInterpretersInterface *mainWindow
			, qReal::GraphicalModelAssistInterface &graphicalModelAssist);

	void run(const QFileInfo &fileInfo);
	void highlightCounterexample();

private:
	void translationFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void errorWhileTranslating(QProcess::ProcessError error);

	void compileFinished(int exitCode, QProcess::ExitStatus exitStatus);
	void compileError(QProcess::ProcessError error);

	void verificationFinished(int exitCode, QProcess::ExitStatus exitStatus);

	void counterexampleBuildingFinished(int exitCode, QProcess::ExitStatus exitStatus);

	void correctCFile();
	void onTimeout();
	qReal::Id graphicalId(qReal::Id logicalId);

	QProcess mPromelaToCProcess;
	QProcess mCompileProcess;
	QProcess mVerificationProcess;
	QProcess mCounterexampleProcess;
	qReal::ErrorReporterInterface *mErrorReporter;
	qReal::CodeBlockManagerInterface *mCodeBlockManager;
	qReal::gui::MainWindowInterpretersInterface *mMainWindow;
	qReal::GraphicalModelAssistInterface &mGraphicalModelAssist;

	QFileInfo mFile;
	QList<qReal::Id> mCounterexample;
	int mCurrentBlock = 0;
	QTimer *mTimer;
};

}
}
