#include "promelaLuaPrinter.h"

#include <qrtext/lua/ast/node.h>
#include <qrtext/lua/ast/assignment.h>
#include <qrtext/lua/ast/tableConstructor.h>
#include <qrtext/lua/ast/fieldInitialization.h>

using namespace trik::promela::lua;
using namespace qrtext::lua::ast;

PromelaLuaPrinter::PromelaLuaPrinter(const QString &pathToTemplates
		 , const qrtext::LanguageToolboxInterface &textLanguage
		 , generatorBase::lua::PrecedenceConverterInterface &precedeceTable
		 , const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter)
	: LuaPrinter(pathToTemplates, textLanguage, precedeceTable, reservedVariablesConverter)
{
}

void PromelaLuaPrinter::visit(const Assignment &node)
{
	if (dynamic_cast<const TableConstructor * const>(&(*node.value())) != nullptr) {
		processTemplate(node, "arrayAssignment.t", { {"@@INITIALIZERS@@", node.value()}, {"@@VARIABLE@@", node.variable()} });
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
