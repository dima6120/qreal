#pragma once

#include <trikGeneratorBase/trikGeneratorFactory.h>

namespace trik {
namespace promela {

class PromelaGeneratorFactory : public TrikGeneratorFactory
{
public:
	PromelaGeneratorFactory(const qrRepo::RepoApi &repo
			 , qReal::ErrorReporterInterface &errorReporter
			 , const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			 , generatorBase::lua::LuaProcessor &luaProcessor
			 , const QString &generatorName);

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;
};

}
}
