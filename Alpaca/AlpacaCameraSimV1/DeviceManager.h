#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H

#include "CamServer.h"
#include "CameraV1.h"
#include "CameraStateMachine.h"


struct DeviceData
{
	std::string    deviceName;
	std::string    deviceType;
	int            deviceNum;
};
typedef std::list<DeviceData> DeviceDataList;


class DeviceManager
{
	SM::DeviceSMManagerPtr                        m_pDeviceSMMaster;
	DevServer::DeviceServerPtr                    m_pServer;
	std::map<uint32_t, SM::DeviceStateMachinePtr> m_devices;

	
public:
	DeviceManager(DevServer::DeviceServerPtr);
	~DeviceManager();

	void initDevices();

	void createDevice(DeviceData&);
	void removeDevice(unsigned int);
	void clearDevices();

	std::map<uint32_t, SM::DeviceStateMachinePtr> getDeviceData() const { return m_devices; }

private:
	void saveDeviceList();
};
typedef std::shared_ptr<DeviceManager> DeviceManagerPtr;

#endif // !DEVICEMANAGER_H