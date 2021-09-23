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
		// tshee can be useful in the even that some class has ownership of multiple devices and may 
		// want to identify by device num but a scope and a cam can both have a device num of 0 or something
		Camera                = 0x100,
		CoverCalibrator       = 0x200,
		Dome  				  = 0x300,
		FilterWheel 		  = 0x400,
		Focuser  			  = 0x500,
		ObservingConditions   = 0x600,
		Rotator 			  = 0x700,
		SafetyMonitor 		  = 0x800,
		Switch 				  = 0x900,
		Telescope			  = 0xA00,

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