#ifndef ALPACADEVICEV1_H
#define ALPACADEVICEV1_H

#include "RESTServerContext.h"
#include "AlpacaUtils.h"


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

public:
	AlpacaDeviceV1(const DeviceTypeID type, const unsigned int deviceNum,  const std::string deviceName);
	virtual ~AlpacaDeviceV1();

	virtual std::string action(std::string action, std::string parameters) = 0;

	virtual void commandBlind(std::string cmd,  std::string raw) = 0;
	virtual bool commandBool(std::string cmd,   std::string raw) = 0;
	virtual std::string commandString(std::string cmd, std::string raw) = 0;

private:
	void initCommonEndpoints();
};


#endif // !ALPACADEVICEV1_H