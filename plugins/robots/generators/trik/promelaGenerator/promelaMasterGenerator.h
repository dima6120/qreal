#pragma once

#include <generatorBase/masterGeneratorBase.h>

namespace trik {
namespace promela {

/// Master generator implementation for generator into QtScript for TRIK platform
class PromelaMasterGenerator : public generatorBase::MasterGeneratorBase
{
public:
	PromelaMasterGenerator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, const utils::ParserErrorReporter &parserErrorReporter
			, const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage
			, const qReal::Id &diagramId
			, const QString &generatorName);

protected:
	QString targetPath() override;
	bool supportsGotoGeneration() const override;
	void processGeneratedCode(QString &generatedCode) override;
	generatorBase::GeneratorCustomizer *createCustomizer() override;

private:
	QString generateThreadsChannels();
	const QString mGeneratorName;
};

}
}
