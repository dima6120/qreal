#include "MorseGeneratorFactory.h"

using namespace qReal::robots::generators;
using namespace morse;

MorseGeneratorFactory::MorseGeneratorFactory(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter)
	: GeneratorFactoryBase(repo, errorReporter)
{
}

MorseGeneratorFactory::~MorseGeneratorFactory()
{
}

QString MorseGeneratorFactory::pathToTemplates() const
{
	return ":/morse/templates";
}
