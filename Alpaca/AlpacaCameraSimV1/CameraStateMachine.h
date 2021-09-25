#ifndef CAMERASM_H
#define CAMERASM_H

#include "CameraV1.h"
#include "SMBase.h"
#include "SMBaseManager.h"

namespace SM {

	class DeviceSMManager : public CSMBaseMgr
	{
	public:
		DeviceSMManager()
			: CSMBaseMgr("Device_SM_Manager", 10)
		{}

	};
	typedef std::shared_ptr<DeviceSMManager> DeviceSMManagerPtr;

class DeviceStateMachine : public CSMBase
{

public:
	DeviceStateMachine(TSMContextPtr pDev, std::string name);
	virtual ~DeviceStateMachine();

	void initSM() override;
};

typedef std::shared_ptr<DeviceStateMachine> DeviceStateMachinePtr;

}
#endif // !CAMERASM_H