#include "stdafx.h"
#include "AlpacaDeviceV1.h"
#include "AlpacaCommonEndpointsV1.h"
#include <map>
#include <sstream>

using namespace Alpaca;

const std::map<AlpacaDeviceV1::DeviceTypeID, std::string> c_deviceTypeIDStrings = {
		{ AlpacaDeviceV1::DeviceTypeID::Camera,				 "camera" },
		{ AlpacaDeviceV1::DeviceTypeID::CoverCalibrator,	 "covercalibrator"},
		{ AlpacaDeviceV1::DeviceTypeID::Dome,				 "dome"},
		{ AlpacaDeviceV1::DeviceTypeID::FilterWheel,		 "filterwheel"},
		{ AlpacaDeviceV1::DeviceTypeID::Focuser,			 "focuser"},
		{ AlpacaDeviceV1::DeviceTypeID::ObservingConditions, "observingconditions"},
		{ AlpacaDeviceV1::DeviceTypeID::Rotator,			 "rotator"},
		{ AlpacaDeviceV1::DeviceTypeID::SafetyMonitor,		 "safetymonitor" },
		{ AlpacaDeviceV1::DeviceTypeID::Switch,				 "switch" },
		{ AlpacaDeviceV1::DeviceTypeID::Telescope,			 "telescope" }
};

AlpacaDeviceV1::AlpacaDeviceV1(DeviceTypeID type, unsigned int deviceNum, std::string deviceName)
	: RESTServerContext(deviceName)
	, m_name(deviceName)
	, m_description("")
	, m_driverInfo("")
	, m_connected(false)
	, m_interfaceVersion(1)
	, m_driverVersion("1.0")
	, m_supportedActions()
	, m_rootEPName("/api/v1")
	, m_deviceType(type)
	, m_deviceNumber(deviceNum)
	, m_deviceBusy(false)
	, m_deviceID(-1)
{
	// each device id will exist in an ID space by taking the binary and of the device num and the device type
	// eg: a cam with device num 1 will be 0 | 0x1000 and such will have an id of 0x1001 where as a telescope with a device of
	// 0 will have a an id of 1 | 0x9000 which will be 0x9001 allowing devices to be enumerated via the device space of the id 
	m_deviceID = static_cast<unsigned int>(m_deviceType) | deviceNum;
	initCommonEndpoints();
}

AlpacaDeviceV1::~AlpacaDeviceV1()
{
}

void AlpacaDeviceV1::initCommonEndpoints()
{
	try
	{
		std::shared_ptr<HTTPRequestHandler> pHandler = nullptr;

		auto listStart = static_cast<unsigned int>(Alpaca::Common::CommonEndpointID::epFirst);
		auto listEnd   = static_cast<unsigned int>(Alpaca::Common::CommonEndpointID::epLast);

		std::string deviceTypeString = c_deviceTypeIDStrings.at(m_deviceType);

		std::ostringstream oss;
		oss << m_rootEPName << "/" << deviceTypeString << "/" << m_deviceNumber;

		for (auto i = listStart; i < listEnd; i++)
		{
			auto id = static_cast<Alpaca::Common::CommonEndpointID>(i);
			pHandler = Alpaca::Common::createHandler(id, oss.str());

			if (pHandler)
			{
				registerHandler(pHandler->getName(), pHandler);
			}
		}
	}
	catch (std::exception& err)
	{
		throw std::runtime_error("Failed to initialize common endpoints");
	}
}
