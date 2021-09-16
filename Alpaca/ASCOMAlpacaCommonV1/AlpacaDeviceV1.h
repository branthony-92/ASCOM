#ifndef ALPACADEVICEV1_H
#define ALPACADEVICEV1_H

#include "RESTServerContext.h"
#include "AlpacaUtils.h"

namespace Alpaca {

class AlpacaDeviceV1 : public RESTServerContext
{
public:
	enum class DeviceTypeID
	{
		Camera,
		CoverCalibrator,
		Dome, 
		FilterWheel,
		Focuser, 
		ObservingConditions,
		Rotator,
		SafetyMonitor,
		Switch,
		Telescope,

		NumDeviceTypes,
	};
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