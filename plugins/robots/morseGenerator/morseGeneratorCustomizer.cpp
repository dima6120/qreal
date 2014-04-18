#include "morseGeneratorCustomizer.h"

using namespace qReal::robots::generators::morse;

MorseGeneratorCustomizer::MorseGeneratorCustomizer(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter)
	: mFactory(repo, errorReporter)
{
}

qReal::robots::generators::GeneratorFactoryBase *MorseGeneratorCustomizer::factory()
{
	return &mFactory;
}
