#include "promelaLuaProcessor.h"

#include "promelaLuaPrinter.h"

using namespace trik::promela::lua;
using namespace generatorBase::lua;
using namespace generatorBase;

PromelaLuaProcessor::PromelaLuaProcessor(qReal::ErrorReporterInterface &errorReporter
		 , qrtext::LanguageToolboxInterface &textLanguage
		 , const utils::ParserErrorReporter &parserErrorReporter
		 , QObject *parent)
	: LuaProcessor(errorReporter, textLanguage, parserErrorReporter, parent)
{
}

LuaPrinter *PromelaLuaProcessor::createLuaPrinter(
		const simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	return new PromelaLuaPrinter(pathToRoot(), mTextLanguage
			, precedenceConverter(), reservedVariablesConverter);
}
