#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace promela {
namespace simple {

/// Generator for conditional construnctions
class IfGenerator : public generatorBase::simple::BindingGenerator
{
public:
	IfGenerator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, const qReal::Id &id
		, bool elseIsEmpty
		, bool needInverting
		, QObject *parent = 0);
};

}
}
}
