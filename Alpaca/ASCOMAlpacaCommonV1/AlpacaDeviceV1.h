#ifndef ALPACADEVICEV1_H
#define ALPACADEVICEV1_H

#include "RESTServerContext.h"
#include "AlpacaUtils.h"

namespace Alpaca {

	enum class DeviceTypeID
	{
		// these are essentially to be used as device ID masks 
		// on top of the device num so long as device num is between 0-255
		Camera              = 0x0100,
		CoverCalibrator     = 0x0200,
		Dome                = 0x0300,
		FilterWheel		    = 0x0400,
		Focuser			    = 0x0500,
		ObservingConditions = 0x0600,
		Rotator				= 0x0700,
		SafetyMonitor		= 0x0800,
		Switch				= 0x0900,
		Telescope			= 0x0A00,
	};
	const std::map<DeviceTypeID, std::string> c_deviceTypeIDToStrings = {
		{ DeviceTypeID::Camera,				 "camera" },
		{ DeviceTypeID::CoverCalibrator,	 "covercalibrator"},
		{ DeviceTypeID::Dome,				 "dome"},
		{ DeviceTypeID::FilterWheel,		 "filterwheel"},
		{ DeviceTypeID::Focuser,			 "focuser"},
		{ DeviceTypeID::ObservingConditions, "observingconditions"},
		{ DeviceTypeID::Rotator,			 "rotator"},
		{ DeviceTypeID::SafetyMonitor,		 "safetymonitor" },
		{ DeviceTypeID::Switch,				 "switch" },
		{ DeviceTypeID::Telescope,			 "telescope" }
	};

	const std::map<std::string, DeviceTypeID> c_stringsToDeviceTypeID = {
	{ "camera",				    DeviceTypeID::Camera              },
	{ "coverCalibrator",	    DeviceTypeID::CoverCalibrator     },
	{ "dome",				    DeviceTypeID::Dome                },
	{ "filterWheel",		    DeviceTypeID::FilterWheel         },
	{ "focuser",			    DeviceTypeID::Focuser             },
	{ "observingconditions",    DeviceTypeID::ObservingConditions },
	{ "rotator",			    DeviceTypeID::Rotator             },
	{ "safetymonitor",		    DeviceTypeID::SafetyMonitor       },
	{ "switch",				    DeviceTypeID::Switch              },
	{ "telescope",			    DeviceTypeID::Telescope           }
	};

class AlpacaDeviceV1 : public RESTServerContext
{
public:
	using StringList = std::vector<std::string>;
	using IntList    = std::vector<int>;
	// Common Alpaca Device Interface info
	DEVICEPROP(std::string,    Name ,            name)
	DEVICEPROP(std::string,    Description,      description)
	DEVICEPROP(std::string,    DriverInfo,       driverInfo)
	DEVICEPROP(bool,           Connected,		 connected)
	DEVICEPROP(unsigned int,   InterfaceVersion, interfaceVersion)
	DEVICEPROP(std::string,    DriverVersion,    driverVersion)
	DEVICEPROP(StringList,     SupportedActions, supportedActions)

	DEVICEPROP(std::string,  RootEPName,   rootEPName)
	DEVICEPROP(DeviceTypeID, DeviceType,   deviceType)
	DEVICEPROP(unsigned int, DeviceNumber, deviceNumber)
	DEVICEPROP(unsigned int, DeviceID,     deviceID)

	DEVICEPROP(bool, DeviceBusy, deviceBusy)

public:
	AlpacaDeviceV1(DeviceTypeID type, unsigned int deviceNum, std::string deviceName);
	virtual ~AlpacaDeviceV1();

	virtual std::string action(std::string action, std::string parameters) = 0;

	virtual void commandBlind(std::string cmd,  std::string raw) = 0;
	virtual bool commandBool(std::string cmd,   std::string raw) = 0;
	virtual std::string commandString(std::string cmd, std::string raw) = 0;

	virtual void connect() = 0;
	virtual void disconnect() = 0;

private:
	void initCommonEndpoints();
};
typedef std::list<std::shared_ptr<AlpacaDeviceV1>>  DeviceList;

		
}
#endif // !ALPACADEVICEV1_H