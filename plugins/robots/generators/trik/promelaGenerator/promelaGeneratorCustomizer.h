#pragma once

#include <generatorBase/generatorCustomizer.h>

#include "promelaGeneratorFactory.h"

namespace trik {
namespace promela {

class PromelaGeneratorCustomizer : public generatorBase::GeneratorCustomizer
{
public:
	PromelaGeneratorCustomizer(const qrRepo::RepoApi &repo
								, qReal::ErrorReporterInterface &errorReporter
								, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
								, generatorBase::lua::LuaProcessor &luaProcessor
								, const QString &generatorName);

	generatorBase::GeneratorFactoryBase *factory() override;

private:
	PromelaGeneratorFactory mFactory;
};

}
}
