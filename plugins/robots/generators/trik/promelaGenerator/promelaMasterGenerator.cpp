#include "promelaMasterGenerator.h"

#include <generatorBase/parts/threads.h>

#include "parts/strings.h"
#include "promelaGeneratorCustomizer.h"
#include "lua/promelaLuaProcessor.h"

using namespace trik::promela;
using namespace trik::promela::parts;
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
	generatedCode.replace("@@CHANNELS@@", generateThreadsChannels());
	dynamic_cast<PromelaGeneratorFactory *> (mCustomizer->factory())->strings().processCode(generatedCode);
}

generatorBase::GeneratorCustomizer *PromelaMasterGenerator::createCustomizer()
{
	return new PromelaGeneratorCustomizer(mRepo, mErrorReporter
			, mRobotModelManager, *createLuaProcessor(), mGeneratorName);
}

generatorBase::lua::LuaProcessor *PromelaMasterGenerator::createLuaProcessor()
{
	return new lua::PromelaLuaProcessor(mErrorReporter, mTextLanguage, mParserErrorReporter, this);
}

QString PromelaMasterGenerator::generateThreadsChannels()
{
	QString defines = "";
	QString channels = "";
	int number = 1;
	PromelaGeneratorFactory *factory = dynamic_cast<PromelaGeneratorFactory *>(mCustomizer->factory());

	for (QString const &name : mCustomizer->factory()->threads().threadIds()) {
		defines += readTemplate("defineProcessId.t").replace("@@NAME@@", name).replace("@@NUMBER@@"
				, QString::number(number)) + "\n";
		bool const stringChan = factory->strings().stringChannel(name);
		channels += readTemplate("defineProcessChannels.t").replace("@@NAME@@", name)
				.replace("@@STRING_BUFFER@@", stringChan
					? readTemplate("defineProcessStringBuffer.t").replace("@@NAME@@", name)
					: "")
				+ "\n";
		number++;
	}

	bool const stringChan = factory->strings().stringChannel("main");
	channels += (stringChan ? readTemplate("defineProcessStringBuffer.t").replace("@@NAME@@", "main") : "") + "\n";

	return defines + "\n" + channels;
}

bool PromelaMasterGenerator::supportsGotoGeneration() const
{
	return false;
}
