#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace promela {
namespace simple {

/// Generator for 'Send message' block.
class SendMessageThreadsGenerator : public generatorBase::simple::BindingGenerator
{
public:
	SendMessageThreadsGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent);
};

}
}
}
