#include "spin.h"

#include <QtCore/QCoreApplication>

using namespace trik::promela;
using namespace qReal;

Spin::Spin(CodeBlockManagerInterface *codeBlockManager, gui::MainWindowInterpretersInterface *mainWindow
		, GraphicalModelAssistInterface &graphicalModelAssist)
	: mErrorReporter(mainWindow->errorReporter())
	, mCodeBlockManager(codeBlockManager)
	, mMainWindow(mainWindow)
	, mGraphicalModelAssist(graphicalModelAssist)
	, mTimer(new QTimer(this))
{
	QProcessEnvironment environment(QProcessEnvironment::systemEnvironment());
	environment.insert("GCC", "C:/Qt/Qt5.3.1/Tools/mingw482_32/bin");
	void (QProcess::*finished)(int, QProcess::ExitStatus) = &QProcess::finished;
	void (QProcess::*error)(QProcess::ProcessError) = &QProcess::error;

	mPromelaToCProcess.setProcessEnvironment(environment);
	connect(&mPromelaToCProcess, finished, this, &Spin::translationFinished);
	connect(&mPromelaToCProcess, error, this, &Spin::errorWhileTranslating);

	mCompileProcess.setProcessEnvironment(environment);
	connect(&mCompileProcess, finished, this, &Spin::compileFinished);
	connect(&mCompileProcess, error, this, &Spin::compileError);

	mVerificationProcess.setProcessEnvironment(environment);
	connect(&mVerificationProcess, finished, this, &Spin::verificationFinished);

	mCounterexampleProcess.setProcessEnvironment(environment);
	connect(&mCounterexampleProcess, finished, this, &Spin::counterexampleBuildingFinished);

	connect(mTimer, &QTimer::timeout, this, &Spin::onTimeout);
}

void Spin::run(const QFileInfo &fileInfo)
{
	mFile = fileInfo;
	mErrorReporter->addInformation(tr("Preparations for verification..."));
	QFile::remove(qApp->applicationDirPath() + "/spin/" + mFile.fileName() + ".trail");
	QFile::remove(qApp->applicationDirPath() + "/spin/counterexample");
	mPromelaToCProcess.setStandardErrorFile(qApp->applicationDirPath() + "/spin/mPromelaToCProcess.err");
	mPromelaToCProcess.setStandardOutputFile(qApp->applicationDirPath() + "/spin/mPromelaToCProcess.out");
	mPromelaToCProcess.setEnvironment(QProcess::systemEnvironment());
	mPromelaToCProcess.setWorkingDirectory(qApp->applicationDirPath() + "/spin/");
	mPromelaToCProcess.start(qApp->applicationDirPath() + "/spin/spin.exe", QStringList() << "-a" << fileInfo.absoluteFilePath());
}

void Spin::highlightCounterexample()
{
	if (!mCounterexample.isEmpty()) {
		if (mCurrentBlock == 0) {
			mMainWindow->highlight(graphicalId(mCounterexample[0]), false);
			mTimer->start(1500);
		} else {
			mTimer->stop();
			mMainWindow->dehighlight(graphicalId(mCounterexample[mCurrentBlock]));
			mCurrentBlock = 0;
		}
	}
}

void Spin::translationFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus)

	if (exitCode == 0) {
		correctCFile();
		mCompileProcess.setStandardErrorFile(qApp->applicationDirPath() + "/spin/mCompileProcess.err");
		mCompileProcess.setStandardOutputFile(qApp->applicationDirPath() + "/spin/mCompileProcess.out");
		mCompileProcess.setEnvironment(QProcess::systemEnvironment());
		mCompileProcess.setWorkingDirectory(qApp->applicationDirPath() + "/spin/");
		mCompileProcess.start("gcc", QStringList() << "-DVECTORSZ=1048576" << "-DBFS" << "-o" << "pan" << "pan.c");
	} else if (exitCode == 127) {
		mErrorReporter->addError(tr("spin.exe not found. Make sure it is present in QReal installation directory"));
	}
}

void Spin::errorWhileTranslating(QProcess::ProcessError error)
{
	Q_UNUSED(error)
}

void Spin::correctCFile()
{
	QByteArray fileData;
	QFile file(qApp->applicationDirPath() + "/spin/pan.c");
	file.open(QIODevice::ReadOnly);
	fileData = file.readAll();
	QString text(fileData);
	file.close();
	text.replace("random(", "rand(");
	file.open(QIODevice::WriteOnly);
	file.write(text.toUtf8());
	file.close();
}

void Spin::onTimeout()
{
	mMainWindow->dehighlight(graphicalId(mCounterexample[mCurrentBlock]));
	mCurrentBlock++;

	if (mCurrentBlock == mCounterexample.size()) {
		mTimer->stop();
		mCurrentBlock = 0;
	} else {
		mMainWindow->highlight(graphicalId(mCounterexample[mCurrentBlock]));
	}
}

Id Spin::graphicalId(Id logicalId)
{
	const IdList graphicalIds = mGraphicalModelAssist.graphicalIdsByLogicalId(logicalId);

	if (!graphicalIds.isEmpty()) {
		return graphicalIds.at(0);
	}
	return Id();
}

void Spin::compileFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus)

	if (exitCode == 0) {
		mErrorReporter->addInformation(tr("Verification..."));
		mVerificationProcess.setStandardErrorFile(qApp->applicationDirPath() + "/spin/mVerificationProcess.err");
		mVerificationProcess.setStandardOutputFile(qApp->applicationDirPath() + "/spin/mVerificationProcess.out");
		mVerificationProcess.setEnvironment(QProcess::systemEnvironment());
		mVerificationProcess.setWorkingDirectory(qApp->applicationDirPath() + "/spin/");
		mVerificationProcess.start(qApp->applicationDirPath() + "/spin/pan.exe", QStringList());
	}
}

void Spin::compileError(QProcess::ProcessError error)
{
	Q_UNUSED(error)
}

void Spin::verificationFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitCode)
	Q_UNUSED(exitStatus)

	QString const trailFileName = qApp->applicationDirPath() + "/spin/" + mFile.fileName() + ".trail";

	QFileInfo trailFile(trailFileName);
	if (trailFile.exists()) {
		mCounterexampleProcess.setStandardOutputFile(qApp->applicationDirPath() + "/spin/counterexample");
		mCounterexampleProcess.setEnvironment(QProcess::systemEnvironment());
		mCounterexampleProcess.setWorkingDirectory(qApp->applicationDirPath() + "/spin/");
		mCounterexampleProcess.start(qApp->applicationDirPath() + "/spin/pan.exe", QStringList() << "-g"
				<< trailFileName);
		mErrorReporter->addError(tr("Verification completed. Violations are found. See counterexample."));
	} else {
		mErrorReporter->addInformation(tr("Verification completed. Violations not found"));
	}
}

void Spin::counterexampleBuildingFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	Q_UNUSED(exitStatus)
	Q_UNUSED(exitCode)

	QString const trailFileName = qApp->applicationDirPath() + "/spin/counterexample";
	QString const pattern = "(:\\d+)";
	QRegExp re(pattern);
	QFile trail(trailFileName);
	int lastLineNumber = 0;

	mCounterexample.clear();
	mCurrentBlock = 0;

	if (trail.open(QIODevice::ReadOnly))
	{
	   QTextStream in(&trail);
		while (!in.atEnd()) {
			QString line = in.readLine();
			int pos = re.indexIn(line);

			if (pos > -1) {
				int lineNumber = re.cap().remove(":").toInt();
				if (lineNumber != lastLineNumber) {
					lastLineNumber = lineNumber;
					QList<Id> blocks = mCodeBlockManager->IdsByLineNumber(mFile.absoluteFilePath(), lineNumber);
					if (!blocks.isEmpty()) {
						mCounterexample.append(blocks.last());
					}
				}
			} else {
				break;
			}
		}

		trail.close();
	}
}
