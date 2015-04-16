#include "promelaMasterGenerator.h"

using namespace trik::promela;

PromelaMasterGenerator::PromelaMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName)
	: TrikMasterGeneratorBase(repo, errorReporter, parserErrorReporter, robotModelManager, textLanguage
			, diagramId, generatorName)
{
}

QString PromelaMasterGenerator::targetPath()
{
	return QString("%1/%2.pml").arg(mProjectDir, mProjectName);
}

bool PromelaMasterGenerator::supportsGotoGeneration() const
{
	return false;
}
