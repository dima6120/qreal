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
	, mLuaPrinter(nullptr)
{
}

PromelaLuaProcessor::~PromelaLuaProcessor()
{
	if (mLuaPrinter) {
		delete mLuaPrinter;
	}
}

void PromelaLuaProcessor::setCustomizer(PromelaGeneratorCustomizer &customizer)
{
	mCustomizer = &customizer;
}

LuaPrinter *PromelaLuaProcessor::createLuaPrinter(
		const simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	if (mLuaPrinter == nullptr) {
		mLuaPrinter = new PromelaLuaPrinter(pathToRoot(), mTextLanguage
				, precedenceConverter(), nullptr, *mCustomizer);
	}

	mLuaPrinter->setReservedVariablesConverter(reservedVariablesConverter);
	return mLuaPrinter;
}
