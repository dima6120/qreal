#include "promelaGeneratorFactory.h"

#include <generatorBase/lua/luaProcessor.h>
#include <qrtext/core/types/typeExpression.h>
#include <generatorBase/parts/variables.h>
#include <generatorBase/generatorCustomizer.h>
#include <qrtext/lua/types/string.h>

#include "simpleGenerators/sendMessageThreadsGenerator.h"
#include "simpleGenerators/receiveMessageThreadsGenerator.h"
#include "simpleGenerators/switchGenerator.h"
#include "simpleGenerators/variableInitGenerator.h"
#include "lua/promelaLuaProcessor.h"

using namespace trik::promela;
using namespace trik::promela::simple;
using namespace generatorBase;
using namespace generatorBase::simple;
using namespace qrtext::core::types;
using namespace generatorBase::parts;

PromelaGeneratorFactory::PromelaGeneratorFactory(const qrRepo::RepoApi &repo
		 , qReal::ErrorReporterInterface &errorReporter
		 , const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
		 , generatorBase::lua::LuaProcessor &luaProcessor
		 , const QString &generatorName)
	: TrikGeneratorFactory(repo, errorReporter, robotModelManager, luaProcessor, generatorName)
	, mSwitchCounter(0)
	, mCaseCounter(0)
	, mStringSwitch(false)
	, mStrings(new parts::Strings(pathToTemplates()))
{
	dynamic_cast<lua::PromelaLuaProcessor *>(&luaProcessor)->setStrings(mStrings);
}

trik::promela::parts::Strings &PromelaGeneratorFactory::strings()
{
	return *mStrings;
}

AbstractSimpleGenerator *PromelaGeneratorFactory::simpleGenerator(const qReal::Id &id
		, GeneratorCustomizer &customizer)
{
	const QString elementType = id.element();

	if (elementType == "SendMessageThreads") {
		return new SendMessageThreadsGenerator(mRepo, customizer, id, this);
	} else if (elementType == "ReceiveMessageThreads") {
		return new ReceiveMessageThreadsGenerator(mRepo, customizer, id, this);
	} else if (elementType == "VariableInit") {
		return new VariableInitGenerator(mRepo, customizer, id, this);
	}

	return TrikGeneratorFactory::simpleGenerator(id, customizer);
}

AbstractSimpleGenerator *PromelaGeneratorFactory::switchHeadGenerator(const qReal::Id &id
		, GeneratorCustomizer &customizer, const QStringList &values)
{
	QString const expression = mRepo.property(id, "Expression").toString();

	if (!customizer.factory()->variables()
			->expressionType(expression).dynamicCast<qrtext::lua::types::String>().isNull()) {
		mCaseCounter = 0;
		mStringSwitch = true;
		return new SwitchGenerator(mRepo, customizer, id, "head", mSwitchCounter++, mCaseCounter++, this);
	}

	return GeneratorFactoryBase::switchHeadGenerator(id, customizer, values);
}

AbstractSimpleGenerator *PromelaGeneratorFactory::switchMiddleGenerator(const qReal::Id &id
		, GeneratorCustomizer &customizer, const QStringList &values)
{
	if (mStringSwitch) {
		return new SwitchGenerator(mRepo, customizer, id, "middle", mSwitchCounter - 1, mCaseCounter++, this);
	}

	return GeneratorFactoryBase::switchMiddleGenerator(id, customizer, values);
}
