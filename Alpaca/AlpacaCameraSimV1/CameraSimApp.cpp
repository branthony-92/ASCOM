#include "stdafx.h"
#include "CameraSimApp.h"
#include <fstream>>

using namespace App;


#ifdef _WIN32 || _WIN64
	bool isWindows = true;
#else
	bool isWindows = false;
#endif


const QList<QCommandLineOption> c_supportedOptions = {
	QCommandLineOption("logsmstates", "Save the state machine states to a CSV file"),
	QCommandLineOption("nogui", "Run the Application without the GUI enabled"),
	QCommandLineOption("config", "Absolute path the to server configuration file", "Config")
};


App::CameraSimApp::CameraSimApp(int argc, char* argv[])
	: QApplication(argc, argv)
	, m_pServer(nullptr)
	, m_pMainFrame(nullptr)
	, m_parser()
	, m_pDeviceManager(nullptr)
{
	m_pServer = std::make_shared<DevServer::DeviceServer>();
	m_pDeviceManager = std::make_shared<DeviceManager>(m_pServer);
}

App::CameraSimApp::~CameraSimApp()
{
	m_pMainFrame      = nullptr;
	m_pDeviceManager  = nullptr;
	m_pServer		  = nullptr;
}

bool CameraSimApp::init()
{
	m_parser.clearPositionalArguments();
	// check for command line args here
	m_parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

	// add the options to the command line parser
	m_parser.addOptions(c_supportedOptions);
	auto helpOpt = m_parser.addHelpOption();

	if (!m_parser.parse(arguments())) {

		fputs(qPrintable(m_parser.errorText()), stderr);
		fputs("\n\n", stderr);
		fputs(qPrintable(m_parser.helpText()), stderr);
		exit();
	}

	if (m_parser.isSet(helpOpt))
	{
		m_parser.showHelp();
		exit();
	}

	m_pDeviceManager->initDevices();

	// skip gui creation if we specify the -g (gui-less) flag
	if (m_parser.isSet("nogui"))
	{
		return initWithoutGui();
	}

	m_pMainFrame = std::make_shared<Gui::DlgMainFrame>(m_pDeviceManager, m_pServer);
	m_pMainFrame->show();

	return true;
}

bool App::CameraSimApp::initWithoutGui()
{
	std::string configPath = DevServer::c_workingConfigPath;
	if (m_parser.isSet("config"))
	{
		configPath = m_parser.value("config").toStdString();
		if (configPath.empty()) configPath = DevServer::c_workingConfigPath;
	}

	// open the config filepath
	ServerConfig info = loadConfigFromFile(configPath);

	return m_pServer->start(info);

	return false;
}

ServerConfig App::CameraSimApp::loadConfigFromFile(std::string filePath)
{
	ServerConfig info;

	std::ifstream configFile;
	configFile.open(filePath);
	if (!configFile.is_open()) return info;

	JSON configJSON = JSON::object();

	configFile >> configJSON;
	configFile.close();

	if (configJSON.empty()) return info;

	auto serverConfig = jsonUtils::extractValue<JSON>(configJSON, "Server_Config", JSON::object());

	info.configFilePath = filePath;
	info.port = jsonUtils::extractValue<unsigned int>(serverConfig, "Port", 0);
	info.IPv4Address = jsonUtils::extractValue<std::string>(serverConfig, "IPv4", "");
	info.certificateFilePath = jsonUtils::extractValue<std::string>(serverConfig, "Certificate_Path", "");
	info.privKeyFilePath = jsonUtils::extractValue<std::string>(serverConfig, "Priv_Key_Path", "");
	info.diffieHellmanParamFilePath = jsonUtils::extractValue<std::string>(serverConfig, "DH_Params_Path", "");
	info.useHTTPS = jsonUtils::extractValue<bool>(serverConfig, "Enable_HTTPS", false);
	info.serverThreadCount = jsonUtils::extractValue<unsigned int>(serverConfig, "Server_Thread_Count", 1);

	return info;
}
