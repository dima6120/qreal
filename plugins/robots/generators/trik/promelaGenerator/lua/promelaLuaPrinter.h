#pragma once

#include <generatorBase/lua/luaPrinter.h>
#include <generatorBase/lua/precedenceConverterInterface.h>

#include "promelaGeneratorCustomizer.h"
#include "parts/strings.h"

namespace trik {
namespace promela {
namespace lua {

class PromelaLuaPrinter : public generatorBase::lua::LuaPrinter
{
public:
	PromelaLuaPrinter(const QString &pathToTemplates
			, const qrtext::LanguageToolboxInterface &textLanguage
			, generatorBase::lua::PrecedenceConverterInterface &precedeceTable
			, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter
			, PromelaGeneratorCustomizer &customizer);

	void visit(const qrtext::lua::ast::Assignment &node) override;
	void visit(const qrtext::lua::ast::TableConstructor &node) override;
	void visit(const qrtext::lua::ast::Concatenation &node) override;
	void visit(const qrtext::lua::ast::String &node) override;
	void visit(const qrtext::lua::ast::FieldInitialization &node) override;
	void visit(const qrtext::lua::ast::IndexingExpression &node) override;

private:
	int concatenationNumber(qrtext::lua::ast::Node *node);

	int mTempStringNumber;

	PromelaGeneratorCustomizer &mCustomizer;
};

}
}
}

