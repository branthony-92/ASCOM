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
	, m_pDeviceSMMaster(std::make_shared<SM::DeviceSMManager>())
{
}

App::CameraSimApp::~CameraSimApp()
{
	if (arguments().contains("-dumpstates"))
	{
		auto statemachines = m_pDeviceSMMaster->getStateMachines();
		std::ofstream file;
		file.open("StateHistory.csv", std::fstream::trunc | std::fstream::out);
		if (!file.is_open()) return;
		for (auto& sm : statemachines)
		{
			auto devName = sm.first;
			auto pDev	 = sm.second;
			if (pDev)
			{
				file << devName << '\n';
				auto states = pDev->dumpStateHistory();
				file << states;
			}
		}
		
		file.close();
	}
	m_pMainFrame      = nullptr;
	m_pDeviceSMMaster = nullptr;
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

	m_pServer = std::make_shared<CamServer::CameraServer>();

	// TODO : this logic should probably go into a controller class...
	// open the config filepath
	JSON config = jsonUtils::loadFile(CamServer::c_workingConfigPath);

	std::vector<JSON> devices = jsonUtils::extractValue<std::vector<JSON>>(config, "Device_List", std::vector<JSON>());

	// initialize our devices using the config file
	// this should be configurable via the gui using an "add device" button
	for (auto& devInfo : devices)
	{
		DeviceData info;

		info.deviceName = jsonUtils::extractValue<std::string>(devInfo, "Device_Name", "");
		info.deviceType = jsonUtils::extractValue<std::string>(devInfo, "Device_Type", "");
		info.deviceNum = jsonUtils::extractValue<int>(devInfo, "Device_Num", -1);
		createDevice(info);
	}

	// Init the statemachine master
	m_pDeviceSMMaster->startStateMachines();

	// skip gui creation if we specify the -g (gui-less) flag
	if (m_parser.isSet("nogui"))
	{
		return initWithoutGui();
	}

	m_pMainFrame = std::make_shared<Gui::DlgMainFrame>(m_pServer);
	m_pMainFrame->show();

	return true;
}

bool App::CameraSimApp::initWithoutGui()
{
	std::string configPath = CamServer::c_workingConfigPath;
	if (m_parser.isSet("config"))
	{
		configPath = m_parser.value("config").toStdString();
		if (configPath.empty()) configPath = CamServer::c_workingConfigPath;
	}

	// open the config filepath
	ServerConfig info = loadConfigFromFile(configPath);

	return m_pServer->start(info);

	return false;
}

void App::CameraSimApp::createDevice(DeviceData& data)
{
	if (data.deviceName.empty()) return;
	if (data.deviceType.empty()) return;
	if (data.deviceNum < 0)      return;

	if (Alpaca::c_stringsToDeviceTypeID.find(data.deviceType) == Alpaca::c_stringsToDeviceTypeID.end()) return;
	auto deviceID = Alpaca::c_stringsToDeviceTypeID.at(data.deviceType);

	RESTCtxPtr pDevice   = nullptr;
	TSMPtr     pDeviceSM = nullptr;

	std::ostringstream oss;

	switch (deviceID)
	{
	case Alpaca::DeviceTypeID::Camera:
	{
		// init cam 1, register it to the server, and a new state machine, register this state machine to the manager
		auto pCam = std::make_shared<Camera::CameraV1>(data.deviceNum, data.deviceName);
		oss << data.deviceName << " SM ID: " << pCam->getDeviceID();
		pDeviceSM = std::make_shared<SM::CameraStateMachine>(pCam, oss.str());
		pDevice = pCam;
		break;
	}
	case Alpaca::DeviceTypeID::CoverCalibrator:
	case Alpaca::DeviceTypeID::Dome:
	case Alpaca::DeviceTypeID::FilterWheel:
	case Alpaca::DeviceTypeID::Focuser:
	case Alpaca::DeviceTypeID::ObservingConditions:
	case Alpaca::DeviceTypeID::Rotator:
	case Alpaca::DeviceTypeID::SafetyMonitor:
	case Alpaca::DeviceTypeID::Switch:
	case Alpaca::DeviceTypeID::Telescope:
	default:
		break;
	}

	if (!(pDevice && pDeviceSM)) return;
	
	m_pServer->registerContext(pDevice);
	m_pDeviceSMMaster->registerStateMachine(pDeviceSM);

	if (m_parser.isSet("logsmstates"))
	{
		pDeviceSM->enableStateHistory(true);
	}
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
