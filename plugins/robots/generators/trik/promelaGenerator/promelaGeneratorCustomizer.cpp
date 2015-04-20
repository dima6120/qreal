#include "promelaGeneratorCustomizer.h"

using namespace trik;
using namespace promela;

PromelaGeneratorCustomizer::PromelaGeneratorCustomizer(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		, generatorBase::lua::LuaProcessor &luaProcessor
		, const QString &generatorName)
	: mFactory(repo, errorReporter, robotModelManager, luaProcessor, generatorName)
{
}

generatorBase::GeneratorFactoryBase *PromelaGeneratorCustomizer::factory()
{
	return &mFactory;
}
