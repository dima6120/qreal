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

	void setReservedVariablesConverter(
			const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter);

	void visit(const qrtext::lua::ast::Assignment &node) override;
	void visit(const qrtext::lua::ast::TableConstructor &node) override;
	void visit(const qrtext::lua::ast::Concatenation &node) override;
	void visit(const qrtext::lua::ast::String &node) override;
	void visit(const qrtext::lua::ast::FieldInitialization &node) override;
	void visit(const qrtext::lua::ast::IndexingExpression &node) override;
	void visit(const qrtext::lua::ast::Identifier &node) override;

	void visit(const qrtext::lua::ast::Not &node) override;
	void visit(const qrtext::lua::ast::UnaryMinus &node) override;
	void visit(const qrtext::lua::ast::BitwiseNegation &node) override;
	void visit(const qrtext::lua::ast::Length &node) override;

	void visit(const qrtext::lua::ast::LogicalAnd &node) override;
	void visit(const qrtext::lua::ast::LogicalOr &node) override;
	void visit(const qrtext::lua::ast::Addition &node) override;
	void visit(const qrtext::lua::ast::Subtraction &node) override;
	void visit(const qrtext::lua::ast::Multiplication &node) override;
	void visit(const qrtext::lua::ast::Division &node) override;
	void visit(const qrtext::lua::ast::IntegerDivision &node) override;
	void visit(const qrtext::lua::ast::Modulo &node) override;
	void visit(const qrtext::lua::ast::Exponentiation &node) override;
	void visit(const qrtext::lua::ast::BitwiseAnd &node) override;
	void visit(const qrtext::lua::ast::BitwiseOr &node) override;
	void visit(const qrtext::lua::ast::BitwiseXor &node) override;
	void visit(const qrtext::lua::ast::BitwiseLeftShift &node) override;
	void visit(const qrtext::lua::ast::BitwiseRightShift &node) override;
	void visit(const qrtext::lua::ast::Equality &node) override;
	void visit(const qrtext::lua::ast::LessThan &node) override;
	void visit(const qrtext::lua::ast::LessOrEqual &node) override;
	void visit(const qrtext::lua::ast::Inequality &node) override;
	void visit(const qrtext::lua::ast::GreaterThan &node) override;
	void visit(const qrtext::lua::ast::GreaterOrEqual &node) override;

	void visit(const qrtext::lua::ast::FloatNumber &node) override;

private:
	int concatenationNumber(qrtext::lua::ast::Node *node);
	bool allowedTable(const QString &name);
	bool stringAssignment(const qrtext::lua::ast::Assignment &node);
	bool isDeadNodeUnary(const qrtext::core::ast::UnaryOperator &node);
	bool isDeadNodeBinary(const qrtext::core::ast::BinaryOperator &node);
	void popChildren(qrtext::lua::ast::Node *node);

	int mTempStringNumber;
	QSet<QString> mDeadVariables;
	QSet<const qrtext::lua::ast::Node *> mDeadNodes;

	PromelaGeneratorCustomizer &mCustomizer;
};

}
}
}

