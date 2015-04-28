#include "promelaLuaPrinter.h"

#include <qrtext/lua/ast/node.h>
#include <qrtext/lua/ast/assignment.h>
#include <qrtext/lua/ast/tableConstructor.h>
#include <qrtext/lua/ast/fieldInitialization.h>
#include <qrtext/lua/ast/concatenation.h>
#include <qrtext/lua/ast/string.h>

using namespace trik::promela::lua;
using namespace qrtext::lua::ast;
using namespace qrtext::core;

PromelaLuaPrinter::PromelaLuaPrinter(const QString &pathToTemplates
		 , const qrtext::LanguageToolboxInterface &textLanguage
		 , generatorBase::lua::PrecedenceConverterInterface &precedeceTable
		 , const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter)
	: LuaPrinter(pathToTemplates, textLanguage, precedeceTable, reservedVariablesConverter)
	, mTempStringNumber(0)
{
}

void PromelaLuaPrinter::visit(const Assignment &node)
{
	if (!node.value().dynamicCast<TableConstructor>().isNull()) {
		processTemplate(node, "arrayAssignment.t", { {"@@INITIALIZERS@@", node.value()}
				, {"@@VARIABLE@@", node.variable()} });
	} else if (!node.value().dynamicCast<Concatenation>().isNull()){
		mTempStringNumber = 0;
		processTemplate(node, "stringAssignment.t", { {"@@CONCATENATIONS@@", node.value()}
				, {"@@VARIABLE@@", node.variable()} });
		pushResult(node, popResult(node).replace("@@NUMBER@@"
				, QString::number(concatenationNumber(node.value().data()) - 1) ));
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
	QString t = "oneConcatenation.t";

	pushResult(node, readTemplate(t)
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
	pushResult(node, readTemplate("string.t").replace("@@VALUE@@", node.string()));
}

int PromelaLuaPrinter::concatenationNumber(Node *node)
{
	QSharedPointer<Node> p(node);

	if (p.dynamicCast<Concatenation>().isNull()) {
		return 0;
	} else {
		QSharedPointer<Concatenation> c(p.dynamicCast<Concatenation>());
		return 1 + concatenationNumber(c.data()->leftOperand().data())
				+ concatenationNumber(c.data()->rightOperand().data());
	}
}
