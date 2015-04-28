#pragma once

#include "generatorBase/simpleGenerators/bindingGenerator.h"

namespace trik {
namespace promela {
namespace simple {

/// Generator for the first case of the 'Switch' block.
class SwitchGenerator : public generatorBase::simple::BindingGenerator
{
public:
	SwitchGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, const QString &part
			, int switchNumber
			, int index
			, QObject *parent = 0);
};

}
}
}
