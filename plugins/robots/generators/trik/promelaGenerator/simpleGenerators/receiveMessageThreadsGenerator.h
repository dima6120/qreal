#pragma once

#include <generatorBase/simpleGenerators/bindingGenerator.h>

namespace trik {
namespace promela {
namespace simple {

/// Generates receiving a message sent to this thread by another one.
class ReceiveMessageThreadsGenerator : public generatorBase::simple::BindingGenerator
{
public:
	ReceiveMessageThreadsGenerator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, const qReal::Id &id
			, QObject *parent = 0);
};

}
}
}
