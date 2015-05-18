#include <generatorBase/generatorCustomizer.h>
#include <qrtext/lua/types/string.h>
#include <qrtext/core/types/typeExpression.h>
#include <generatorBase/parts/variables.h>

#include "variableInitGenerator.h"


using namespace trik::promela::simple;
using namespace generatorBase;
using namespace generatorBase::simple;
using namespace qReal;

VariableInitGenerator::VariableInitGenerator(const qrRepo::RepoApi &repo
		, GeneratorCustomizer &customizer
		, const Id &id
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, "initVar.t"
			, { Binding::createStaticConverting("@@BODY@@"
					, expressionChoice(repo, customizer, id)
					, customizer.factory()->functionBlockConverter(id, "value")) }
			, parent)
{
}

QString VariableInitGenerator::expressionChoice(const qrRepo::RepoApi &repo,
		GeneratorCustomizer &customizer, const Id &id)
{
	QString const var = repo.property(id, "variable").toString();
	QString const value = repo.property(id, "value").toString();

	if (!customizer.factory()->variables()
				->expressionType(var).dynamicCast<qrtext::lua::types::String>().isNull()) {
		return readTemplate("luaPrinting/stringAssignment2.t").replace("@@VARIABLE@@", var).replace("@@VALUE@@", value);
	}

	return var + " = " + value + ";";
}
