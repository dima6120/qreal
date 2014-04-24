#include "morseMasterGenerator.h"
#include "morseGeneratorCustomizer.h"

#include <QtCore/QCoreApplication>

#include <qrkernel/settingsManager.h>

using namespace qReal::robots::generators::morse;

MorseMasterGenerator::MorseMasterGenerator(qrRepo::RepoApi const &repo
		, ErrorReporterInterface &errorReporter
		, Id const &diagramId)
	: MasterGeneratorBase(repo, errorReporter, diagramId)
{
}

qReal::robots::generators::GeneratorCustomizer *MorseMasterGenerator::createCustomizer()
{
	return new MorseGeneratorCustomizer(mRepo, mErrorReporter);
}

QString MorseMasterGenerator::targetPath()
{
	return QString("%1/%2.py").arg(mProjectDir, "exe_script");
}

bool MorseMasterGenerator::supportsGotoGeneration() const
{
	return false;
}

void MorseMasterGenerator::afterGeneration()
{
	generateBuildScriptAndExtraCode(mProjectDir);
	generateTempfile(mProjectDir);
}

void MorseMasterGenerator::generateTempfile(QString const &projectDir)
{
	QString tempfile = readTemplate("tempfile.t");
	tempfile.replace("@@BUILDERSCRIPT@@", projectDir + "/build_script.py");
	tempfile.replace("@@MORSEROOT@@", qApp->applicationDirPath() + "/morse");
	tempfile.replace("@@PYTHONDIR@@", qApp->applicationDirPath() + "/python");
	outputCode(projectDir + "/tempfile.py", tempfile);
}

void MorseMasterGenerator::generateBuildScriptAndExtraCode(QString const &projectDir)
{
	QString init_sensors = "";
	QString read_functions = "";
	QString get_sensors = "";
	QFile file(projectDir + "/exe_script.py");
	QTextStream *inStream = 0;
	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			inStream = new QTextStream(&file);
	}

	for(int port = 1; port <= 4; ++port) {
		QString const portString = QString::number(port);
		int const portValue = SettingsManager::value("port" + portString + "SensorType").toInt();
		qReal::interpreters::robots::enums::sensorType::SensorTypeEnum const sensorType
				= static_cast<qReal::interpreters::robots::enums::sensorType::SensorTypeEnum>(portValue);
		QString ist_path;
		switch (sensorType) {
			case qReal::interpreters::robots::enums::sensorType::colorFull:
			case qReal::interpreters::robots::enums::sensorType::colorRed:
			case qReal::interpreters::robots::enums::sensorType::colorGreen:
			case qReal::interpreters::robots::enums::sensorType::colorBlue:
			case qReal::interpreters::robots::enums::sensorType::colorNone:
			case qReal::interpreters::robots::enums::sensorType::light:
				ist_path = "initSensor/initColorLightSensor.t";
				break;
			case qReal::interpreters::robots::enums::sensorType::sonar:
				ist_path = "initSensor/initSonarSensor.t";
				break;
			case qReal::interpreters::robots::enums::sensorType::sound:
				continue;
			case qReal::interpreters::robots::enums::sensorType::gyroscope:
				continue;
			case qReal::interpreters::robots::enums::sensorType::touchBoolean:
			case qReal::interpreters::robots::enums::sensorType::touchRaw:
				ist_path = "initSensor/initTouchSensor.t";
				break;
			default:
				continue;
		}

		get_sensors += readTemplate("getSensor.t").replace("@@PORT@@", "port" + portString) + "\n";
		init_sensors += readTemplate(ist_path).replace("@@PORT@@", "port" + portString) + "\n";
	}
	// TODO: set chousen environment
	QString const build_code = readTemplate("build.t").replace("@@INITSENSORS@@", init_sensors).replace(
				"@@ENVIRONMENT@@", qApp->applicationDirPath() + "/morse/share/morse/data/environments/sandbox.blend");
	outputCode(projectDir + "/build_script.py", build_code);

	if (inStream) {
		QString const code = inStream->readAll().replace("@@READFUNCTIONS@@", read_functions).replace(
					"@@GETSENSORS@@", get_sensors);
		outputCode(projectDir + "/exe_script.py", code);
	}
}
