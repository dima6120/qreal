#pragma once

#include <generatorBase/lua/luaProcessor.h>

#include "promelaGeneratorCustomizer.h"
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

	void setCustomizer(PromelaGeneratorCustomizer &customizer);

	generatorBase::lua::LuaPrinter *createLuaPrinter(
			const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter) override;

private:
	PromelaGeneratorCustomizer *mCustomizer;
};

}
}
}
