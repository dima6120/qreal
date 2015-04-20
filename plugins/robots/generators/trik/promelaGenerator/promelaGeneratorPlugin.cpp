#include "promelaGeneratorPlugin.h"

#include <QtWidgets/QApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

#include <utils/tcpRobotCommunicator.h>

#include "promelaMasterGenerator.h"

using namespace trik::promela;
using namespace qReal;

PromelaGeneratorPlugin::PromelaGeneratorPlugin()
	: TrikGeneratorPluginBase("PromelaGeneratorRobotModel", tr("Generation (Promela)"), 7 /* Last order */)
	, mGenerateCodeAction(new QAction(nullptr))
{
}

PromelaGeneratorPlugin::~PromelaGeneratorPlugin()
{
}

void PromelaGeneratorPlugin::init(kitBase::KitPluginConfigurator const &configurator)
{
	RobotsGeneratorPluginBase::init(configurator);
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

	return {generateCodeActionInfo, separatorInfo};
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
