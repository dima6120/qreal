#pragma once

#include <generatorBase/lua/luaPrinter.h>
#include <generatorBase/lua/precedenceConverterInterface.h>

namespace trik {
namespace promela {
namespace lua {

class PromelaLuaPrinter : public generatorBase::lua::LuaPrinter
{
public:
	PromelaLuaPrinter(const QString &pathToTemplates
			, const qrtext::LanguageToolboxInterface &textLanguage
			, generatorBase::lua::PrecedenceConverterInterface &precedeceTable
			, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter);

	void visit(const qrtext::lua::ast::Assignment &node) override;
	void visit(const qrtext::lua::ast::TableConstructor &node) override;
};

}
}
}

