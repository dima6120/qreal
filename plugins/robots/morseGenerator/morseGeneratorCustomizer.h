#pragma once

#include <generatorCustomizer.h>
#include "morseGeneratorFactory.h"

namespace qReal {
namespace robots {
namespace generators {
namespace morse {

class MorseGeneratorCustomizer : public GeneratorCustomizer
{
public:
	MorseGeneratorCustomizer(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);

	GeneratorFactoryBase *factory() override;

private:
	MorseGeneratorFactory mFactory;
};

}
}
}
}
