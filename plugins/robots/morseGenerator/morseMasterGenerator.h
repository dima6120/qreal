#pragma once

#include <masterGeneratorBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace morse {

class MorseMasterGenerator : public MasterGeneratorBase
{
public:
	MorseMasterGenerator(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, Id const &diagramId);

protected:
	GeneratorCustomizer *createCustomizer() override;
	QString targetPath() override;
	bool supportsGotoGeneration() const override;
	void afterGeneration() override;

private:
	void generateBuildScriptAndExtraCode(QString const &projectDir);
	void generateTempfile(QString const &projectDir);
};

}
}
}
}
