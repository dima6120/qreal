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
	return QString("%1/%2.py").arg(mProjectDir, "3Dmodel_client");
}

bool MorseMasterGenerator::supportsGotoGeneration() const
{
	return false;
}

void MorseMasterGenerator::afterGeneration()
{
	//generateBuildScriptAndExtraCode(mProjectDir);
	//generateTempfile(mProjectDir);
	generateExtraCode();
}

void MorseMasterGenerator::generateTempfile()
{
	QString tempfile = readTemplate("tempfile.t");
	tempfile.replace("@@BUILDERSCRIPT@@", qApp->applicationDirPath() + "/morse/3Dmodel/default.py");
	tempfile.replace("@@MORSEROOT@@", qApp->applicationDirPath() + "/morse");
	tempfile.replace("@@PYTHONDIR@@", qApp->applicationDirPath() + "/python");
	outputCode(qApp->applicationDirPath() + "/morse/tempfile.py", tempfile);
}

void MorseMasterGenerator::generateBuildScript()
{
	QString init_sensors = "";
	QString init_sensor = "";
	double const portCoord[4][3] = {{1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}
								   , {-1.0, 0.0, 1.0}, {0.0, -1.0, 1.0}};
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

		init_sensor = readTemplate(ist_path).replace("@@PORT@@", "port" + portString);
		init_sensor.replace("@@X@@", QString::number(portCoord[port - 1][0]));
		init_sensor.replace("@@Y@@", QString::number(portCoord[port - 1][1]));
		init_sensor.replace("@@Z@@", QString::number(portCoord[port - 1][2]));

		init_sensors += init_sensor + "\n";
	}

	// TODO: set chousen environment
	QString const build_code = readTemplate("build.t").replace("@@INITSENSORS@@", init_sensors).replace(
				"@@ENVIRONMENT@@", qApp->applicationDirPath() + "/morse/share/morse/data/environments/test_env.blend");
	outputCode(qApp->applicationDirPath() + "/morse/3Dmodel/default.py", build_code);
	generateTempfile();
}

void MorseMasterGenerator::generateExtraCode()
{
	QString const projectDir = qApp->applicationDirPath() + "/morse/3Dmodel/scripts";
	QString get_sensors = "";
	QFile file(projectDir + "/3Dmodel_client.py");
	QTextStream *inStream = 0;
	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			inStream = new QTextStream(&file);
	}
	for(int port = 1; port <= 4; ++port) {
		QString const portString = QString::number(port);
		int const portValue = SettingsManager::value("port" + portString + "SensorType").toInt();
		qReal::interpreters::robots::enums::sensorType::SensorTypeEnum const sensorType
				= static_cast<qReal::interpreters::robots::enums::sensorType::SensorTypeEnum>(portValue);
		switch (sensorType) {
			case qReal::interpreters::robots::enums::sensorType::colorFull:
			case qReal::interpreters::robots::enums::sensorType::colorRed:
			case qReal::interpreters::robots::enums::sensorType::colorGreen:
			case qReal::interpreters::robots::enums::sensorType::colorBlue:
			case qReal::interpreters::robots::enums::sensorType::colorNone:
			case qReal::interpreters::robots::enums::sensorType::light:
				break;
			case qReal::interpreters::robots::enums::sensorType::sonar:
				break;
			case qReal::interpreters::robots::enums::sensorType::sound:
				continue;
			case qReal::interpreters::robots::enums::sensorType::gyroscope:
				continue;
			case qReal::interpreters::robots::enums::sensorType::touchBoolean:
			case qReal::interpreters::robots::enums::sensorType::touchRaw:
				break;
			default:
				continue;
		}
		get_sensors += readTemplate("getSensor.t").replace("@@PORT@@", "port" + portString) + "\n";
	}

	if (inStream) {
		QString const code = inStream->readAll().replace("@@GETSENSORS@@", get_sensors)
				.replace("@@MORSEROOT@@", qApp->applicationDirPath() + "/morse");
		outputCode(projectDir + "/3Dmodel_client.py", code);
	}
	file.close();
}

/*void MorseMasterGenerator::generateBuildScriptAndExtraCode(QString const &projectDir)
{
	QString init_sensors = "";
	QString get_sensors = "";
	QString init_sensor = "";
	QFile file(projectDir + "/3Dmodel_client.py");
	QTextStream *inStream = 0;
	if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			inStream = new QTextStream(&file);
	}

	double const portCoord[4][3] = {{1.0, 0.0, 1.0}, {0.0, 1.0, 1.0}
								   , {-1.0, 0.0, 1.0}, {0.0, -1.0, 1.0}};

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

		init_sensor = readTemplate(ist_path).replace("@@PORT@@", "port" + portString);
		init_sensor.replace("@@X@@", QString::number(portCoord[port - 1][0]));
		init_sensor.replace("@@Y@@", QString::number(portCoord[port - 1][1]));
		init_sensor.replace("@@Z@@", QString::number(portCoord[port - 1][2]));

		init_sensors += init_sensor + "\n";
	}
	// TODO: set chousen environment
	QString const build_code = readTemplate("build.t").replace("@@INITSENSORS@@", init_sensors).replace(
				"@@ENVIRONMENT@@", qApp->applicationDirPath() + "/morse/share/morse/data/environments/test_env.blend");
	outputCode(qApp->applicationDirPath() + "/morse/3Dmodel/default.py", build_code);

	if (inStream) {
		QString const code = inStream->readAll().replace("@@GETSENSORS@@", get_sensors)
				.replace("@@MORSEROOT@@", qApp->applicationDirPath() + "/morse");
		outputCode(projectDir + "/3Dmodel_client.py", code);
	}
}*/
