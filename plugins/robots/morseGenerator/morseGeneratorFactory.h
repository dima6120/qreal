#pragma once

#include <generatorFactoryBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace morse {

class MorseGeneratorFactory : public GeneratorFactoryBase
{
public:
	MorseGeneratorFactory(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter);
	virtual ~MorseGeneratorFactory();

	virtual QString pathToTemplates() const;
};

}
}
}
}

