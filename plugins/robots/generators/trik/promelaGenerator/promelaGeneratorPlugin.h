#pragma once

#include <trikGeneratorBase/trikGeneratorPluginBase.h>

namespace utils {
class TcpRobotCommunicator;
}

namespace trik {
namespace promela {

/// Generation of QtScript program for TRIK, uploading and execution of a program.
/// Uses setting "tcpServer" from RobotsInterpreter.
class PromelaGeneratorPlugin : public TrikGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "trik.PromelaGeneratorPlugin")

public:
	PromelaGeneratorPlugin();
	~PromelaGeneratorPlugin() override;

	QList<qReal::ActionInfo> actions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

	void init(const qReal::PluginConfigurator &configurator
			, const interpreterBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::LanguageToolboxInterface &textLanguage) override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	QString defaultFilePath(const QString &projectName) const override;
	qReal::text::LanguageInfo language() const override;
	QString generatorName() const override;

private:
	/// Action that launches code generator
	QAction *mGenerateCodeAction;  // Doesn't have ownership; may be disposed by GUI.
};

}
}
