#include "promelaGeneratorFactory.h"

#include <generatorBase/lua/luaProcessor.h>

#include "simpleGenerators/sendMessageThreadsGenerator.h"
#include "simpleGenerators/receiveMessageThreadsGenerator.h"

using namespace trik::promela;
using namespace trik::promela::simple;
using namespace generatorBase;
using namespace generatorBase::simple;

PromelaGeneratorFactory::PromelaGeneratorFactory(const qrRepo::RepoApi &repo
		 , qReal::ErrorReporterInterface &errorReporter
		 , const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		 , lua::LuaProcessor &luaProcessor
		 , const QString &generatorName)
	: TrikGeneratorFactory(repo, errorReporter, robotModelManager, luaProcessor, generatorName)
{
}

AbstractSimpleGenerator *PromelaGeneratorFactory::simpleGenerator(const qReal::Id &id
		, GeneratorCustomizer &customizer)
{
	const QString elementType = id.element();

	if (elementType == "SendMessageThreads") {
		return new SendMessageThreadsGenerator(mRepo, customizer, id, this);
	} else if (elementType == "ReceiveMessageThreads") {
		return new ReceiveMessageThreadsGenerator(mRepo, customizer, id, this);
	}

	return TrikGeneratorFactory::simpleGenerator(id, customizer);
}
