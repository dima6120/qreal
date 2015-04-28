#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace promela {
namespace simple {

/// Generator for 'VariableInit' block
class VariableInitGenerator : public generatorBase::simple::BindingGenerator
{
public:
	VariableInitGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);

private:
	QString expressionChoice(const qrRepo::RepoApi &repo,
			generatorBase::GeneratorCustomizer &customizer, const qReal::Id &id);
};

}
}
}
