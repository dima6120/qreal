#include "promelaGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include <utils/tcpRobotCommunicator.h>
#include <qrgui/textEditor/qscintillaTextEdit.h>

#include "promelaMasterGenerator.h"

using namespace trik::promela;
using namespace qReal;

PromelaGeneratorPlugin::PromelaGeneratorPlugin()
	: TrikGeneratorPluginBase("PromelaGeneratorRobotModel", tr("Generation (Promela)"), 7 /* Last order */)
	, mGenerateCodeAction(new QAction(nullptr))
	, mRunVerifierAction(new QAction(nullptr))
{
}

PromelaGeneratorPlugin::~PromelaGeneratorPlugin()
{
}

void PromelaGeneratorPlugin::init(kitBase::KitPluginConfigurator const &configurator)
{
	RobotsGeneratorPluginBase::init(configurator);

	mSpin = new Spin(mMainWindowInterface->errorReporter());
}

QList<ActionInfo> PromelaGeneratorPlugin::customActions()
{
	QAction *separator = new QAction(this);
	separator->setSeparator(true);
	qReal::ActionInfo separatorInfo(separator, "generators", "tools");

	mGenerateCodeAction->setText(tr("Generate Promela code"));
	//mGenerateCodeAction->setIcon(QIcon(":/images/generateQtsCode.svg"));
	ActionInfo generateCodeActionInfo(mGenerateCodeAction, "generators", "tools");
	connect(mGenerateCodeAction, SIGNAL(triggered()), this, SLOT(generateCode()), Qt::UniqueConnection);

	mRunVerifierAction->setText(tr("Run Verifier"));
	//mGenerateCodeAction->setIcon(QIcon(":/images/generateQtsCode.svg"));
	ActionInfo runVerifierActionInfo(mRunVerifierAction, "generators", "tools");
	connect(mRunVerifierAction, &QAction::triggered
			, this, &PromelaGeneratorPlugin::runVerifier, Qt::UniqueConnection);

	return {generateCodeActionInfo, runVerifierActionInfo, separatorInfo};
}

QList<HotKeyActionInfo> PromelaGeneratorPlugin::hotKeyActions()
{
	mGenerateCodeAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));

	HotKeyActionInfo generateCodeInfo("Generator.GeneratePromela", tr("Generate Promela Code"), mGenerateCodeAction);

	return { generateCodeInfo };
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

void PromelaGeneratorPlugin::runVerifier(bool checked)
{
	Q_UNUSED(checked)

	text::QScintillaTextEdit *area = dynamic_cast<text::QScintillaTextEdit *>(mMainWindowInterface->currentTab());

	if (area) {
		QString const filePath = mTextManager->path(area);
		if (mTextManager->generatorName(filePath) == generatorName()) {
			mSpin->run(QFileInfo(filePath));
			return;
		}
	}

	mMainWindowInterface->errorReporter()->addError(tr("First open tab with Promela code"));
}
