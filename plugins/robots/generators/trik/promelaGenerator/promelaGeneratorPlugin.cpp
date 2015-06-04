#include "promelaGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include <qrutils/inFile.h>
#include <qrutils/outFile.h>
#include <qrgui/textEditor/qscintillaTextEdit.h>

#include "promelaMasterGenerator.h"

using namespace trik::promela;
using namespace qReal;

PromelaGeneratorPlugin::PromelaGeneratorPlugin()
	: TrikGeneratorPluginBase("PromelaGeneratorRobotModel", tr("Generation (Promela)"), 7 /* Last order */)
	, mHighlightCounterexampleAction(new QAction(nullptr))
	, mLTLEditorAction(new QAction(nullptr))
	, mStepByStepAction(new QAction(nullptr))
	, mStopAction(new QAction(nullptr))
{
}

PromelaGeneratorPlugin::~PromelaGeneratorPlugin()
{
}

void PromelaGeneratorPlugin::init(kitBase::KitPluginConfigurator const &configurator)
{
	RobotsGeneratorPluginBase::init(configurator);

	mSpin = new Spin(mTextManager->codeBlockManager(), mMainWindowInterface
			, configurator.qRealConfigurator().graphicalModelApi());

	connect(mSpin, &Spin::formulaEntered, this, &PromelaGeneratorPlugin::runVerifier);
}

QList<ActionInfo> PromelaGeneratorPlugin::customActions()
{
	QAction *separator = new QAction(this);
	separator->setSeparator(true);
	qReal::ActionInfo separatorInfo(separator, "generators", "tools");

	mHighlightCounterexampleAction->setText(tr("Show counterexample"));
	//mGenerateCodeAction->setIcon(QIcon(":/images/generateQtsCode.svg"));
	ActionInfo highlightCounterexampleActionInfo(mHighlightCounterexampleAction, "generators", "tools");
	connect(mHighlightCounterexampleAction, &QAction::triggered
			, this, &PromelaGeneratorPlugin::showCounterexample, Qt::UniqueConnection);

	mLTLEditorAction->setText(tr("Enter checked property"));
	//mGenerateCodeAction->setIcon(QIcon(":/images/generateQtsCode.svg"));
	ActionInfo ltlEditorActionInfo(mLTLEditorAction, "generators", "tools");
	connect(mLTLEditorAction, &QAction::triggered
			, this, &PromelaGeneratorPlugin::showLTLDialog, Qt::UniqueConnection);

	mStepByStepAction->setText(tr("Next Step"));
	//mGenerateCodeAction->setIcon(QIcon(":/images/generateQtsCode.svg"));
	ActionInfo stepByStepActionInfo(mStepByStepAction, "generators", "tools");
	connect(mStepByStepAction, &QAction::triggered, this
			, &PromelaGeneratorPlugin::nextBlock, Qt::UniqueConnection);

	mStopAction->setText(tr("Next Step"));
	//mGenerateCodeAction->setIcon(QIcon(":/images/generateQtsCode.svg"));
	ActionInfo stopActionInfo(mStopAction, "generators", "tools");
	connect(mStopAction, &QAction::triggered, this
			, &PromelaGeneratorPlugin::stopShowingCounterexample, Qt::UniqueConnection);

	return {ltlEditorActionInfo, separatorInfo, highlightCounterexampleActionInfo, stepByStepActionInfo
			, stopActionInfo, separatorInfo};
}

QList<HotKeyActionInfo> PromelaGeneratorPlugin::hotKeyActions()
{
	return {};
}

QIcon PromelaGeneratorPlugin::iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const
{
	Q_UNUSED(robotModel)
	return QIcon(":/trik/qts/images/switch-to-trik-qts.svg");
}

generatorBase::MasterGeneratorBase *PromelaGeneratorPlugin::masterGenerator()
{
	return new PromelaMasterGenerator(*mRepo
			, *mMainWindowInterface->errorReporter()
			, *mParserErrorReporter
			, *mRobotModelManager
			, *mTextLanguage
			, mMainWindowInterface->activeDiagram()
			, generatorName());
}

QString PromelaGeneratorPlugin::defaultFilePath(const QString &projectName) const
{
	return QString("spin/%1/%1.pml").arg(projectName);
}

text::LanguageInfo PromelaGeneratorPlugin::language() const
{
	return qReal::text::Languages::c();
}

QString PromelaGeneratorPlugin::generatorName() const
{
	return "trikPromela";
}

void PromelaGeneratorPlugin::showCounterexample(bool checked)
{
	Q_UNUSED(checked)

	mSpin->highlightCounterexample();
}

void PromelaGeneratorPlugin::showLTLDialog(bool checked)
{
	Q_UNUSED(checked)

	mSpin->showLTLDialog();
}

void PromelaGeneratorPlugin::nextBlock(bool checked)
{
	Q_UNUSED(checked)

	mSpin->highlightNextBlock();
}

void PromelaGeneratorPlugin::stopShowingCounterexample(bool checked)
{
	Q_UNUSED(checked)

	mSpin->stop();
}

void PromelaGeneratorPlugin::runVerifier(const QString &formula)
{
	QString srcPath = generateCode(false);

	if (!srcPath.isEmpty()) {
		QString const pattern =
				"[/][*][@][@](\\bLTL_BEGIN\\b)[@][@][*][/](.*)[/][*][@][@](\\bLTL_END\\b)[@][@][*][/]";
		QString generatedCode = utils::InFile::readAll(srcPath);
		generatedCode.replace(QRegExp(pattern),
				"/*@@LTL_BEGIN@@*/\nltl l1 {" + formula +"}\n/*@@LTL_END@@*/");
		QFile::remove(srcPath);
		utils::OutFile out(srcPath);
		out() << generatedCode;
		mSpin->run(QFileInfo(srcPath));
	}
}
