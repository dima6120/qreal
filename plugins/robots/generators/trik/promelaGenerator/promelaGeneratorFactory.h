#pragma once

#include <trikGeneratorBase/trikGeneratorFactory.h>

#include "parts/strings.h"

namespace trik {
namespace promela {

class PromelaGeneratorFactory : public TrikGeneratorFactory
{
public:
	PromelaGeneratorFactory(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, generatorBase::lua::LuaProcessor &luaProcessor
			, const QString &generatorName
			, generatorBase::GeneratorCustomizer *customizer);

	parts::Strings &strings();

	generatorBase::simple::AbstractSimpleGenerator *simpleGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer) override;
	generatorBase::simple::AbstractSimpleGenerator *switchHeadGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer, const QStringList &values) override;
	generatorBase::simple::AbstractSimpleGenerator *switchMiddleGenerator(const qReal::Id &id
			, generatorBase::GeneratorCustomizer &customizer, const QStringList &values) override;

private:
	//QString stringComparisons(QString const &expression, generatorBase::GeneratorCustomizer &customizer);

	int mSwitchCounter;
	int mCaseCounter;
	bool mStringSwitch;

	parts::Strings *mStrings;
};

}
}
