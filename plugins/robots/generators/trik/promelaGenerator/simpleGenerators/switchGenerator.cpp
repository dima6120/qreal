#include "switchGenerator.h"

#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase::simple;
using namespace trik::promela::simple;

SwitchGenerator::SwitchGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, const QString &part
		, int switchNumber
		, int index
		, QObject *parent)
	: BindingGenerator(repo, customizer, id, QString("switch/%1.t").arg(part), {
			Binding::createStatic("@@EXPRESSION@@", "switch_array" + QString::number(switchNumber)
					+ "[" + QString::number(index) + "]")
			, Binding::createStatic("@@CONDITION@@", "== true) ->")
			}, parent)
{
}
