#ifndef CAMERASM_H
#define CAMERASM_H

#include "CameraV1.h"
#include "SMBase.h"

namespace SM {


class CameraStateMachine : public CSMBase
{

	DEVICEPROP(Camera::CamPtr, Camera, pCamera)
public:
	CameraStateMachine(std::shared_ptr<Camera::CameraV1> pCam);
	virtual ~CameraStateMachine();

	void initSM();
};

typedef std::shared_ptr<CameraStateMachine> CameraSMPtr;

}
#endif // !CAMERASM_H