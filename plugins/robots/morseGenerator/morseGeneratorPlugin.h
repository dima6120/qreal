#pragma once

#include <robotsGeneratorPluginBase.h>

namespace qReal {
namespace robots {
namespace generators {
namespace morse {

class MorseGeneratorPlugin : public RobotsGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "qReal.robots.MorseGenerator.MorseGeneratorPlugin")

public:
	MorseGeneratorPlugin();

	~MorseGeneratorPlugin() override;

	QList<qReal::ActionInfo> actions() override;

private slots:
	bool run3Dmodel();

private:
	MasterGeneratorBase *masterGenerator() override;
	void regenerateExtraFiles(QFileInfo const &newFileInfo) override;
	QString defaultFilePath(QString const &projectName) const override;
	QString extension() const override;
	QString extDescrition() const override;
	QString generatorName() const override;
	void checkMorseAndBlender();

	/// Action that launches code generator
	QAction mGenerateCodeAction;

	/// When true, Morse and Blender required version are found by QReal and using 3D model is possible
	bool mMorseAndBlenderPresent;

	/// Translator object for this plugin
	QTranslator mAppTranslator;
};

}
}
}
}
