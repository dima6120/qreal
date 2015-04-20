#include "promelaMasterGenerator.h"
#include "generatorBase/parts/threads.h"
#include "promelaGeneratorCustomizer.h"

using namespace trik::promela;
using namespace generatorBase::parts;

PromelaMasterGenerator::PromelaMasterGenerator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const utils::ParserErrorReporter &parserErrorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, qrtext::LanguageToolboxInterface &textLanguage
		, const qReal::Id &diagramId
		, const QString &generatorName)
	: MasterGeneratorBase(repo, errorReporter, robotModelManager, textLanguage, parserErrorReporter, diagramId)
	, mGeneratorName(generatorName)
{
}

QString PromelaMasterGenerator::targetPath()
{
	return QString("%1/%2.pml").arg(mProjectDir, mProjectName);
}

void PromelaMasterGenerator::processGeneratedCode(QString &generatedCode)
{

}

generatorBase::GeneratorCustomizer *PromelaMasterGenerator::createCustomizer()
{
	return new PromelaGeneratorCustomizer(mRepo, mErrorReporter
			, mRobotModelManager, *createLuaProcessor(), mGeneratorName);
}

QString PromelaMasterGenerator::generateThreadsChannels()
{
	QString defines = "";
	QString channels = "";
	int number = 1;

	for (QString const &name : mCustomizer->factory()->threads().threadNames()) {
		defines += readTemplate("defineProcessId.t").replace("@@NAME@@", name).replace("@@NUMBER@@"
				, QString::number(number)) + "\n";
		channels += readTemplate("defineProcessChannels.t").replace("@@NAME@@", name) + "\n";
		number++;
	}

	return defines + "\n" + channels;
}

bool PromelaMasterGenerator::supportsGotoGeneration() const
{
	return false;
}
