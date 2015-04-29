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
#include <qrtext/lua/types/table.h>
#include <qrtext/core/types/typeExpression.h>

using namespace trik::promela::lua;
using namespace qrtext::lua::types;
using namespace qrtext::lua::ast;
using namespace qrtext::core;

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

void PromelaLuaPrinter::visit(const Assignment &node)
{
	if (!node.value().dynamicCast<TableConstructor>().isNull()) {
		processTemplate(node, "arrayAssignment.t", { {"@@INITIALIZERS@@", node.value()}
				, {"@@VARIABLE@@", node.variable()} });
		pushResult(node, popResult(node).replace("@@SIZE@@"
				, QString::number(dynamic_cast<TableConstructor *>(node.value().data())->initializers().size())));
	} else if (!node.value().dynamicCast<Concatenation>().isNull()){
		processTemplate(node, "stringAssignment1.t", { {"@@CONCATENATIONS@@", node.value()}
				, {"@@VARIABLE@@", node.variable()} });
		pushResult(node, popResult(node).replace("@@NUMBER@@"
				, QString::number(concatenationNumber(node.value().data()) - 1) ));
	} else if (!node.value().dynamicCast<String>().isNull()) {
		processTemplate(node, "stringAssignment2.t", { {"@@VALUE@@", node.value()}
				, {"@@VARIABLE@@", node.variable()} });
	} else {
		LuaPrinter::visit(node);
	}
}

void PromelaLuaPrinter::visit(const TableConstructor &node)
{
	QStringList initializers;
	int index = 0;
	for (const QSharedPointer<FieldInitialization> &field : node.initializers()) {
		initializers << popResult(*(qrtext::as<qrtext::lua::ast::Node>(field)))
				.replace("@@INDEX@@", QString::number(index++));
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
	processTemplate(node, templatePath, { {"@@VALUE@@", node.value()} });
}

void PromelaLuaPrinter::visit(const IndexingExpression &node)
{
	QString const name = dynamic_cast<Identifier *>(node.table().data())->name();
	types::TypeExpression *type = mCustomizer.factory()->variables()->expressionType(name).data();
	Table *table = dynamic_cast<Table *>(type);

	if (table != nullptr) {
		//todo: what about float?
		processTemplate(node, !table->elementType().dynamicCast<String>().isNull()
				? "indexingExpressionStringType.t"
				: "indexingExpressionIntType.t"
				, { {"@@TABLE@@", node.table()}, {"@@INDEXER@@", node.indexer()} });
	} else {
		visit(node);
	}
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
