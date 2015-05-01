#include "promelaLuaPrinter.h"

#include <generatorBase/parts/variables.h>
#include <qrtext/lua/ast/node.h>
#include <qrtext/lua/ast/assignment.h>
#include <qrtext/lua/ast/tableConstructor.h>
#include <qrtext/lua/ast/fieldInitialization.h>
#include <qrtext/lua/ast/concatenation.h>
#include <qrtext/lua/ast/string.h>
#include <qrtext/lua/ast/indexingExpression.h>
#include <qrtext/lua/ast/identifier.h>
#include <qrtext/lua/ast/number.h>
#include <qrtext/lua/ast/floatNumber.h>
#include <qrtext/lua/ast/integerNumber.h>
#include <qrtext/lua/ast/logicalAnd.h>
#include <qrtext/lua/ast/logicalOr.h>
#include <qrtext/lua/ast/addition.h>
#include <qrtext/lua/ast/subtraction.h>
#include <qrtext/lua/ast/multiplication.h>
#include <qrtext/lua/ast/division.h>
#include <qrtext/lua/ast/integerDivision.h>
#include <qrtext/lua/ast/modulo.h>
#include <qrtext/lua/ast/exponentiation.h>
#include <qrtext/lua/ast/bitwiseAnd.h>
#include <qrtext/lua/ast/bitwiseOr.h>
#include <qrtext/lua/ast/bitwiseXor.h>
#include <qrtext/lua/ast/bitwiseLeftShift.h>
#include <qrtext/lua/ast/bitwiseRightShift.h>
#include <qrtext/lua/ast/equality.h>
#include <qrtext/lua/ast/lessThan.h>
#include <qrtext/lua/ast/lessOrEqual.h>
#include <qrtext/lua/ast/inequality.h>
#include <qrtext/lua/ast/greaterThan.h>
#include <qrtext/lua/ast/greaterOrEqual.h>
#include <qrtext/lua/ast/expression.h>
#include <qrtext/lua/ast/unaryMinus.h>
#include <qrtext/lua/ast/not.h>
#include <qrtext/lua/ast/bitwiseNegation.h>
#include <qrtext/lua/ast/length.h>
#include <qrtext/lua/types/table.h>
#include <qrtext/lua/types/string.h>
#include <qrtext/lua/types/integer.h>
#include <qrtext/core/types/typeExpression.h>
#include <qrtext/core/ast/unaryOperator.h>
#include <qrtext/core/ast/binaryOperator.h>

using namespace trik::promela::lua;
using namespace qrtext::lua;
using namespace qrtext::lua::ast;
using namespace qrtext;
using namespace kitBase::robotModel;

PromelaLuaPrinter::PromelaLuaPrinter(const QString &pathToTemplates
		, const qrtext::LanguageToolboxInterface &textLanguage
		, generatorBase::lua::PrecedenceConverterInterface &precedeceTable
		, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter
		, PromelaGeneratorCustomizer &customizer)
	: LuaPrinter(pathToTemplates, textLanguage, precedeceTable, reservedVariablesConverter)
	, mTempStringNumber(0)
	, mCustomizer(customizer)
{
}

void PromelaLuaPrinter::setReservedVariablesConverter(
		const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	mReservedVariablesConverter = reservedVariablesConverter;
}

void PromelaLuaPrinter::visit(const Assignment &node)
{
	if (!node.value().dynamicCast<TableConstructor>().isNull()) {
		if (mDeadNodes.contains(node.value().data())) {
			Identifier *id = dynamic_cast<Identifier *>(node.variable().data());
			mDeadVariables.insert(id->name());
			processTemplate(node, "skip.t", { {"@@VALUE@@", node.value()}, {"@@VARIABLE@@", node.variable()} });
		} else {
			processTemplate(node, "arrayAssignment.t", { {"@@INITIALIZERS@@", node.value()}
					, {"@@VARIABLE@@", node.variable()} });
			pushResult(node, popResult(node).replace("@@SIZE@@"
					, QString::number(dynamic_cast<TableConstructor *>(node.value().data())->initializers().size())));
		}
	} else if (!node.value().dynamicCast<Concatenation>().isNull()){
		//todo: dead node
		mTempStringNumber = 0;
		processTemplate(node, "stringAssignment1.t", { {"@@CONCATENATIONS@@", node.value()}
				, {"@@VARIABLE@@", node.variable()} });
		pushResult(node, popResult(node).replace("@@NUMBER@@"
				, QString::number(concatenationNumber(node.value().data()) - 1) ));
	} else if (stringAssignment(node)) {
		//todo: dead node
		processTemplate(node, "stringAssignment2.t", { {"@@VALUE@@", node.value()}
				, {"@@VARIABLE@@", node.variable()} });
	} else {
		if (mDeadNodes.contains(node.value().data())) {
			QString var;
			if (!node.variable().dynamicCast<Identifier>().isNull()) {
				var = dynamic_cast<Identifier *>(node.variable().data())->name();
			} else {
				var = dynamic_cast<Identifier *>(
						dynamic_cast<IndexingExpression *>(node.variable().data())->table().data())->name();
			}
			mDeadVariables.insert(var);
			processTemplate(node, "skip.t", { {"@@VALUE@@", node.value()}, {"@@VARIABLE@@", node.variable()} });
		} else {
			LuaPrinter::visit(node);
		}
	}
}

bool PromelaLuaPrinter::stringAssignment(const Assignment &node)
{
	if (!node.value().dynamicCast<String>().isNull()) {
		return true;
	} else if (!node.value().dynamicCast<Identifier>().isNull()) {
		QString const id = dynamic_cast<Identifier *>(node.variable().data())->name();

		return !mCustomizer.factory()->variables()->expressionType(id).dynamicCast<types::String>().isNull();
	} else if (!node.value().dynamicCast<IndexingExpression>().isNull()) {
		QString const tableId = dynamic_cast<Identifier *>(
				dynamic_cast<IndexingExpression *>(node.value().data())->table().data())->name();

		core::types::TypeExpression *type = mCustomizer.factory()->variables()->expressionType(tableId).data();

		return !dynamic_cast<types::Table *>(type)->elementType().dynamicCast<types::String>().isNull();
	}

	return false;
}

void PromelaLuaPrinter::visit(const TableConstructor &node)
{
	QStringList initializers;
	int index = 0;
	bool dead = false;

	for (const QSharedPointer<FieldInitialization> &field : node.initializers()) {
		if (mDeadNodes.contains(field.data())) {
			if (!dead) {
				dead = true;
			}
		} else {
			initializers << popResult(*(qrtext::as<qrtext::lua::ast::Node>(field)))
					.replace("@@INDEX@@", QString::number(index++));
		}
	}

	if (dead || !node.initializers().size()) {
		mDeadNodes.insert(&node);
		pushResult(node, "");
		return;
	}

	pushResult(node, readTemplate("tableConstructor.t")
			.replace("@@COUNT@@", QString::number(initializers.count()))
			.replace("@@INITIALIZERS@@", initializers.join(readTemplate("fieldInitializersSeparator.t"))));
}


void PromelaLuaPrinter::visit(const Concatenation &node)
{
	bool const rightConcat = !node.rightOperand().dynamicCast<Concatenation>().isNull();
	bool const leftConcat = !node.leftOperand().dynamicCast<Concatenation>().isNull();

	pushResult(node, readTemplate("oneConcatenation.t")
			.replace("@@LEFT@@"
					, !leftConcat
							? toString(node.leftOperand())
							 : "t" + QString::number(mTempStringNumber - 1))
			.replace("@@RIGHT@@"
					 , !rightConcat
							? toString(node.rightOperand())
							: "t" + QString::number(mTempStringNumber - 1))
			.replace("@@NUMBER@@", QString::number(mTempStringNumber))
			.replace("@@CONCATENATION@@"
					 , !rightConcat
							? (!leftConcat ? "" : toString(node.leftOperand()))
							: toString(node.rightOperand())));
	mTempStringNumber++;
}

void PromelaLuaPrinter::visit(const String &node)
{
	pushResult(node, readTemplate("string.t").replace("@@VALUE@@"
			, mCustomizer.promelaFactory()->strings().addString(node.string())));
}

void PromelaLuaPrinter::visit(const FieldInitialization &node)
{
	const QString templatePath = node.value().dynamicCast<String>().isNull()
			? "implicitKeyIntFieldInitialization.t"
			: "implicitKeyStringFieldInitialization.t";
	if (mDeadNodes.contains(node.value().data())) {
		mDeadNodes.insert(&node);
		popChildren(node.value().data());
	} else {
		processTemplate(node, templatePath, { {"@@VALUE@@", node.value()} });
	}
}

void PromelaLuaPrinter::visit(const IndexingExpression &node)
{
	QString const name = dynamic_cast<Identifier *>(node.table().data())->name();

	if (mDeadVariables.contains(name) || mDeadNodes.contains(node.indexer().data())) {
		mDeadNodes.insert(&node);
		pushResult(node, "");
		popResult(*node.table().data());
		popChildren(node.indexer().data());
		return;
	}

	core::types::TypeExpression *type = mCustomizer.factory()->variables()->expressionType(name).data();
	types::Table *table = dynamic_cast<types::Table *>(type);

	if (table != nullptr) {
		//todo: what about float?
		processTemplate(node, !table->elementType().dynamicCast<types::String>().isNull()
				? "indexingExpressionStringType.t"
				: "indexingExpressionIntType.t"
				, { {"@@TABLE@@", node.table()}, {"@@INDEXER@@", node.indexer()} });
	} else {
		LuaPrinter::visit(node);
	}
}

bool PromelaLuaPrinter::allowedTable(const QString &name)
{
	types::Table *table = dynamic_cast<types::Table *>(mCustomizer.factory()->variables()->expressionType(name).data());

	if (table != nullptr) {
		bool const stringType = !table->elementType().dynamicCast<types::String>().isNull();
		bool const intType = !table->elementType().dynamicCast<types::Integer>().isNull();

		return stringType || intType;
	}

	return true;
}

void PromelaLuaPrinter::visit(const Identifier &node)
{
	QString const variable = mReservedVariablesConverter->convert(node.name());

	if (variable != node.name() || mDeadVariables.contains(variable) || !allowedTable(node.name())) {
		mDeadNodes.insert(&node);
		pushResult(node, "");
		mDeadVariables.insert(node.name());
	} else {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const Not &node)
{
	if (!isDeadNodeUnary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const UnaryMinus &node)
{
	if (!isDeadNodeUnary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const Length &node)
{
	if (!isDeadNodeUnary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const BitwiseNegation &node)
{
	if (!isDeadNodeUnary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::LogicalAnd &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::LogicalOr &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::Addition &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::Subtraction &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::Multiplication &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::Division &node)
{
	mDeadNodes.insert(&node);
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::IntegerDivision &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::Modulo &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::Exponentiation &node)
{
	mDeadNodes.insert(&node);
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::BitwiseAnd &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::BitwiseOr &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::BitwiseXor &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::BitwiseLeftShift &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::BitwiseRightShift &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::Equality &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::LessThan &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::LessOrEqual &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::Inequality &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::GreaterThan &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const qrtext::lua::ast::GreaterOrEqual &node)
{
	if (!isDeadNodeBinary(node)) {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const FloatNumber &node)
{
	mDeadNodes.insert(&node);
	pushResult(node, "");
}

int PromelaLuaPrinter::concatenationNumber(Node *node)
{
	Concatenation *c = dynamic_cast<Concatenation *>(node);

	if (c == nullptr) {
		return 0;
	} else {
		return 1 + concatenationNumber(c->leftOperand().data())
				+ concatenationNumber(c->rightOperand().data());
	}
}

bool PromelaLuaPrinter::isDeadNodeUnary(const ast::UnaryOperator &node)
{
	if (mDeadNodes.contains(node.operand().data())) {
		mDeadNodes.insert(&node);
		pushResult(node, "");
		popResult(*node.operand().data());
		return true;
	}
	return false;
}

bool PromelaLuaPrinter::isDeadNodeBinary(const ast::BinaryOperator &node)
{
	bool const deadRight = mDeadNodes.contains(node.rightOperand().data());
	bool const deadLeft = mDeadNodes.contains(node.leftOperand().data());

	if (deadRight || deadLeft) {
		mDeadNodes.insert(&node);
		if (deadRight && deadLeft) {
			popResult(*node.rightOperand());
			popResult(*node.leftOperand());
		} else {
			popChildren(deadRight ? node.leftOperand().data() : node.rightOperand().data());
			popResult(deadLeft ? *node.leftOperand().data() : *node.rightOperand().data());
		}
		pushResult(node, "");
		return true;
	}
	return false;
}

void PromelaLuaPrinter::popChildren(Node *node)
{
	if (node == nullptr) {
		return;
	}

	for (QSharedPointer<Node> child : node->children()) {
		popChildren(child.data());
	}

	popResult(*node);
}
