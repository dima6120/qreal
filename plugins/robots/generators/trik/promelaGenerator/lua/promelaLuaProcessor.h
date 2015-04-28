#pragma once

#include <generatorBase/lua/luaProcessor.h>

#include "parts/strings.h"

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

	void setStrings(parts::Strings *strings);

	generatorBase::lua::LuaPrinter *createLuaPrinter(
			const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter) override;

private:
	parts::Strings *mStrings;
};

}
}
}
