#ifndef CAMERASM_H
#define CAMERASM_H

#include "CameraV1.h"
#include "SMBase.h"

class CameraStateMachine : public CSMBase
{
public:
	CameraStateMachine(std::shared_ptr<CameraV1> pCam);
	virtual ~CameraStateMachine();
};


#endif // !CAMERASM_H