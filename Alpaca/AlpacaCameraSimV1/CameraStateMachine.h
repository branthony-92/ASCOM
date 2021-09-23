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

class CameraStateMachine : public CSMBase
{

	DEVICEPROP(Camera::CamPtr, Camera, pCamera)
public:
	CameraStateMachine(std::shared_ptr<Camera::CameraV1> pCam, std::string name);
	virtual ~CameraStateMachine();

	void initSM() override;
};

typedef std::shared_ptr<CameraStateMachine> CameraSMPtr;

}
#endif // !CAMERASM_H