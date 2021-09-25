#include "DeviceManager.h"

DeviceManager::DeviceManager(DevServer::DeviceServerPtr pServer)
	: m_pServer(pServer)
	, m_pDeviceSMMaster(std::make_shared<SM::DeviceSMManager>())
	, m_devices()
{
}

DeviceManager::~DeviceManager()
{
	saveDeviceList();
}

void DeviceManager::initDevices()
{
	JSON config = jsonUtils::loadFile(DevServer::c_workingConfigPath);

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
}


void DeviceManager::createDevice(DeviceData& data)
{
	if (data.deviceName.empty()) return;
	if (data.deviceType.empty()) return;
	if (data.deviceNum < 0)      return;

	if (Alpaca::c_stringsToDeviceTypeID.find(data.deviceType) == Alpaca::c_stringsToDeviceTypeID.end()) return;
	auto deviceType = Alpaca::c_stringsToDeviceTypeID.at(data.deviceType);

	RESTCtxPtr                    pDevice = nullptr;
	SM::DeviceStateMachinePtr     pDeviceSM = nullptr;
	unsigned int deviceID = 0;

	std::ostringstream oss;

	switch (deviceType)
	{
	case Alpaca::DeviceTypeID::Camera:
	{
		// init cam 1, register it to the server, and a new state machine, register this state machine to the manager
		auto pCam = std::make_shared<Camera::CameraV1>(data.deviceNum, data.deviceName);
		oss << data.deviceName << " SM ID: " << pCam->getDeviceID();
		pDevice = pCam;
		pDeviceSM = std::make_shared<SM::DeviceStateMachine>(pCam, oss.str());
		deviceID = pCam->getDeviceID();
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

	m_devices.insert_or_assign(deviceID, pDeviceSM);
	m_pServer->registerContext(pDevice);
	m_pDeviceSMMaster->registerStateMachine(pDeviceSM);
}

void DeviceManager::removeDevice(unsigned int ID)
{
	auto devIter = m_devices.find(ID);
	if (devIter == m_devices.end()) return;

	auto pSM = devIter->second;
	auto pDev = std::dynamic_pointer_cast<Alpaca::AlpacaDeviceV1>(pSM->getContext());
	if (!pDev) return;

	// unregister the statemachine from the manager and the device from the server
	m_pDeviceSMMaster->unregisterStateMachine(pSM->c_stateName);
	m_pServer->unregisterContext(pDev);
	m_devices.erase(devIter);
}

void DeviceManager::clearDevices()
{
	auto devs = m_devices;
	for (auto& dev : devs)
	{
		unsigned int ID = dev.first;
		removeDevice(ID);
	}
}

void DeviceManager::saveDeviceList()
{
	JSON config = jsonUtils::loadFile(DevServer::c_workingConfigPath);

	std::vector<JSON> devices = jsonUtils::extractValue<std::vector<JSON>>(config, "Device_List", std::vector<JSON>());

	devices.clear();

	for (auto& dev : m_devices)
	{
		auto pDev = std::dynamic_pointer_cast<Alpaca::AlpacaDeviceV1>(dev.second->getContext());
		if (!pDev) continue;
		if (Alpaca::c_deviceTypeIDToStrings.find(pDev->getDeviceType()) == Alpaca::c_deviceTypeIDToStrings.end()) continue;

		JSON devObject = JSON::object();

		auto devTypeString = Alpaca::c_deviceTypeIDToStrings.at(pDev->getDeviceType());

		devObject["Device_Name"] = pDev->getName();
		devObject["Device_Type"] = devTypeString;
		devObject["Device_Num"]  = pDev->getDeviceNumber();

		devices.push_back(devObject);
	}

	config["Device_List"] = devices;

	jsonUtils::saveFile(DevServer::c_workingConfigPath, config);
}

