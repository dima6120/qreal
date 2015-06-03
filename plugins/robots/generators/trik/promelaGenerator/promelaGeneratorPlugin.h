#pragma once

#include <trikGeneratorBase/trikGeneratorPluginBase.h>

#include "spin.h"

namespace trik {
namespace promela {

class PromelaGeneratorPlugin : public TrikGeneratorPluginBase
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID "trik.PromelaGeneratorPlugin")

public:
	PromelaGeneratorPlugin();
	~PromelaGeneratorPlugin() override;

	QList<qReal::ActionInfo> customActions() override;
	QList<qReal::HotKeyActionInfo> hotKeyActions() override;

	void init(const kitBase::KitPluginConfigurator &configurator) override;
	QIcon iconForFastSelector(const kitBase::robotModel::RobotModelInterface &robotModel) const override;

protected:
	generatorBase::MasterGeneratorBase *masterGenerator() override;
	QString defaultFilePath(const QString &projectName) const override;
	qReal::text::LanguageInfo language() const override;
	QString generatorName() const override;

private:
	void showCounterexample(bool checked);
	void showLTLDialog(bool checked);
	void runVerifier(QString const &formula);

	QAction *mHighlightCounterexampleAction;
	QAction *mLTLEditorAction;
	Spin *mSpin = nullptr;
};

}
}
