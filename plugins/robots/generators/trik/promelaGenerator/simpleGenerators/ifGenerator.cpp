#include "ifGenerator.h"
#include <generatorBase/generatorCustomizer.h>

using namespace generatorBase::simple;
using namespace trik::promela::simple;

IfGenerator::IfGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, bool elseIsEmpty
		, bool needInverting
		, QObject *parent)
	: BindingGenerator(repo, customizer, id
			, elseIsEmpty ? "conditional/deadIf.t" : "conditional/deadIfElse.t"
			, QList<Binding *>()
			, parent)
{
}
