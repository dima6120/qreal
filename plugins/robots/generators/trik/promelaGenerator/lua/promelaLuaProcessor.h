#pragma once

#include <generatorBase/lua/luaProcessor.h>

namespace trik {
namespace promela {
namespace lua {

class PromelaLuaProcessor : public generatorBase::lua::LuaProcessor
{
public:
	PromelaLuaProcessor(qReal::ErrorReporterInterface &errorReporter
			, qrtext::LanguageToolboxInterface &textLanguage
			, const utils::ParserErrorReporter &parserErrorReporter
			, QObject *parent = 0);

	generatorBase::lua::LuaPrinter *createLuaPrinter(
			const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter) override;
};

}
}
}
